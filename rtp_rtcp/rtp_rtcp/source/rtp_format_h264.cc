/*
 *  Copyright (c) 2014 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rtp_rtcp/source/rtp_format_h264.h"

#include <string.h>
#include <vector>

#include "system_wrappers/interface/checks.h"
#include "system_wrappers/interface/logging.h"
#include "system_wrappers/interface/module_common_types.h"
#include "rtp_rtcp/source/byte_io.h"
#include "common_video/h264/sps_vui_rewriter.h"
#include "common_video/h264/h264_common.h"
#include "common_video/h264/sps_parser.h"
#include "base/metrics.h"

namespace webrtc {
namespace {

static const size_t kNalHeaderSize = 1;
static const size_t kFuAHeaderSize = 2;
static const size_t kLengthFieldSize = 2;
static const size_t kStapAHeaderSize = kNalHeaderSize + kLengthFieldSize;

static const char* kSpsValidHistogramName = "WebRTC.Video.H264.SpsValid";
enum SpsValidEvent {
  kReceivedSpsPocOk = 0,
  kReceivedSpsVuiOk = 1,
  kReceivedSpsRewritten = 2,
  kReceivedSpsParseFailure = 3,
  kSentSpsPocOk = 4,
  kSentSpsVuiOk = 5,
  kSentSpsRewritten = 6,
  kSentSpsParseFailure = 7,
  kSpsRewrittenMax = 8
};

// Bit masks for FU (A and B) indicators.
enum NalDefs : uint8_t { kFBit = 0x80, kNriMask = 0x60, kTypeMask = 0x1F };

// Bit masks for FU (A and B) headers.
enum FuDefs : uint8_t { kSBit = 0x80, kEBit = 0x40, kRBit = 0x20 };

// TODO(pbos): Avoid parsing this here as well as inside the jitter buffer.
bool ParseStapAStartOffsets(const uint8_t* nalu_ptr,
                            size_t length_remaining,
                            std::vector<size_t>* offsets) {
  size_t offset = 0;
  while (length_remaining > 0) {
    // Buffer doesn't contain room for additional nalu length.
    if (length_remaining < sizeof(uint16_t))
      return false;
    uint16_t nalu_size = ByteReader<uint16_t>::ReadBigEndian(nalu_ptr);
    nalu_ptr += sizeof(uint16_t);
    length_remaining -= sizeof(uint16_t);
    if (nalu_size > length_remaining)
      return false;
    nalu_ptr += nalu_size;
    length_remaining -= nalu_size;

    offsets->push_back(offset + kStapAHeaderSize);
    offset += kLengthFieldSize + nalu_size;
  }
  return true;
}

}  // namespace

RtpPacketizerH264::RtpPacketizerH264(FrameType frame_type,
                                     size_t max_payload_len)
    : max_payload_len_(max_payload_len) {}

RtpPacketizerH264::~RtpPacketizerH264() {
}

RtpPacketizerH264::Fragment::Fragment(const uint8_t* buffer, size_t length)
    : buffer(buffer), length(length) {}
RtpPacketizerH264::Fragment::Fragment(const Fragment& fragment)
    : buffer(fragment.buffer), length(fragment.length) {}

void RtpPacketizerH264::SetPayloadData(
    const uint8_t* payload_data,
    size_t payload_size,
    const RTPFragmentationHeader* fragmentation) {
  CHECK(packets_.empty());
  CHECK(input_fragments_.empty());
  CHECK(fragmentation);
  for (int i = 0; i < fragmentation->fragmentationVectorSize; ++i) {
    const uint8_t* buffer =
        &payload_data[fragmentation->fragmentationOffset[i]];
    size_t length = fragmentation->fragmentationLength[i];

    bool updated_sps = false;
    H264::NaluType nalu_type = H264::ParseNaluType(buffer[0]);
    if (nalu_type == H264::NaluType::kSps) {
      // Check if stream uses picture order count type 0, and if so rewrite it
      // to enable faster decoding. Streams in that format incur additional
      // delay because it allows decode order to differ from render order.
      // The mechanism used is to rewrite (edit or add) the SPS's VUI to contain
      // restrictions on the maximum number of reordered pictures. This reduces
      // latency significantly, though it still adds about a frame of latency to
      // decoding.
      // Note that we do this rewriting both here (send side, in order to
      // protect legacy receive clients) and below in
      // RtpDepacketizerH264::ParseSingleNalu (receive side, in orderer to
      // protect us from unknown or legacy send clients).

      // Create temporary RBSP decoded buffer of the payload (exlcuding the
      // leading nalu type header byte (the SpsParser uses only the payload).
      std::unique_ptr<Buffer> rbsp_buffer = H264::ParseRbsp(
          buffer + H264::kNaluTypeSize, length - H264::kNaluTypeSize);
      Optional<SpsParser::SpsState> sps;

      std::unique_ptr<Buffer> output_buffer(new Buffer());
      // Add the type header to the output buffer first, so that the rewriter
      // can append modified payload on top of that.
      output_buffer->AppendData(buffer[0]);
      SpsVuiRewriter::ParseResult result = SpsVuiRewriter::ParseAndRewriteSps(
          rbsp_buffer->data(), rbsp_buffer->size(), &sps, output_buffer.get());

      switch (result) {
        case SpsVuiRewriter::ParseResult::kVuiRewritten:
          input_fragments_.push_back(
              Fragment(output_buffer->data(), output_buffer->size()));
          input_fragments_.rbegin()->tmp_buffer = std::move(output_buffer);
          updated_sps = true;
          RTC_HISTOGRAM_ENUMERATION(kSpsValidHistogramName,
                                    SpsValidEvent::kSentSpsRewritten,
                                    SpsValidEvent::kSpsRewrittenMax);
          break;
        case SpsVuiRewriter::ParseResult::kPocOk:
          RTC_HISTOGRAM_ENUMERATION(kSpsValidHistogramName,
                                    SpsValidEvent::kSentSpsPocOk,
                                    SpsValidEvent::kSpsRewrittenMax);
          break;
        case SpsVuiRewriter::ParseResult::kVuiOk:
          RTC_HISTOGRAM_ENUMERATION(kSpsValidHistogramName,
                                    SpsValidEvent::kSentSpsVuiOk,
                                    SpsValidEvent::kSpsRewrittenMax);
          break;
        case SpsVuiRewriter::ParseResult::kFailure:
          RTC_HISTOGRAM_ENUMERATION(kSpsValidHistogramName,
                                    SpsValidEvent::kSentSpsParseFailure,
                                    SpsValidEvent::kSpsRewrittenMax);
          break;
      }
    }

    if (!updated_sps)
      input_fragments_.push_back(Fragment(buffer, length));
  }
  GeneratePackets();
}

void RtpPacketizerH264::GeneratePackets() {
  for (size_t i = 0; i < input_fragments_.size();) {
    if (input_fragments_[i].length > max_payload_len_) {
      PacketizeFuA(i);
      ++i;
    } else {
      i = PacketizeStapA(i);
    }
  }
}

void RtpPacketizerH264::PacketizeFuA(size_t fragment_index) {
  // Fragment payload into packets (FU-A).
  // Strip out the original header and leave room for the FU-A header.
  const Fragment& fragment = input_fragments_[fragment_index];

  size_t fragment_length = fragment.length - kNalHeaderSize;
  size_t offset = kNalHeaderSize;
  size_t bytes_available = max_payload_len_ - kFuAHeaderSize;
  const size_t num_fragments =
      (fragment_length + (bytes_available - 1)) / bytes_available;

  const size_t avg_size = (fragment_length + num_fragments - 1) / num_fragments;
  while (fragment_length > 0) {
    size_t packet_length = avg_size;
    if (fragment_length < avg_size)
      packet_length = fragment_length;
    packets_.push(PacketUnit(Fragment(fragment.buffer + offset, packet_length),
                             offset - kNalHeaderSize == 0,
                             fragment_length == packet_length, false,
                             fragment.buffer[0]));
    offset += packet_length;
    fragment_length -= packet_length;
  }
  CHECK_EQ(0u, fragment_length);
}

size_t RtpPacketizerH264::PacketizeStapA(size_t fragment_index) {
  // Aggregate fragments into one packet (STAP-A).
  size_t payload_size_left = max_payload_len_;
  int aggregated_fragments = 0;
  size_t fragment_headers_length = 0;
  const Fragment* fragment = &input_fragments_[fragment_index];
  CHECK_GE(payload_size_left, fragment->length);
  while (payload_size_left >= fragment->length + fragment_headers_length) {
    CHECK_GT(fragment->length, 0u);
    packets_.push(PacketUnit(*fragment, aggregated_fragments == 0, false, true,
                             fragment->buffer[0]));
    payload_size_left -= fragment->length;
    payload_size_left -= fragment_headers_length;

    // Next fragment.
    ++fragment_index;
    if (fragment_index == input_fragments_.size())
      break;
    fragment = &input_fragments_[fragment_index];

    fragment_headers_length = kLengthFieldSize;
    // If we are going to try to aggregate more fragments into this packet
    // we need to add the STAP-A NALU header and a length field for the first
    // NALU of this packet.
    if (aggregated_fragments == 0)
      fragment_headers_length += kNalHeaderSize + kLengthFieldSize;
    ++aggregated_fragments;
  }
  packets_.back().last_fragment = true;
  return fragment_index;
}

bool RtpPacketizerH264::NextPacket(uint8_t* buffer,
                                   size_t* bytes_to_send,
                                   bool* last_packet) {
  *bytes_to_send = 0;
  if (packets_.empty()) {
    *bytes_to_send = 0;
    *last_packet = true;
    return false;
  }

  PacketUnit packet = packets_.front();

  if (packet.first_fragment && packet.last_fragment) {
    // Single NAL unit packet.
    *bytes_to_send = packet.source_fragment.length;
    memcpy(buffer, packet.source_fragment.buffer, *bytes_to_send);
    packets_.pop();
    input_fragments_.pop_front();
    CHECK_LE(*bytes_to_send, max_payload_len_);
  } else if (packet.aggregated) {
    NextAggregatePacket(buffer, bytes_to_send);
    CHECK_LE(*bytes_to_send, max_payload_len_);
  } else {
    NextFragmentPacket(buffer, bytes_to_send);
    CHECK_LE(*bytes_to_send, max_payload_len_);
  }
  *last_packet = packets_.empty();
  return true;
}

void RtpPacketizerH264::NextAggregatePacket(uint8_t* buffer,
                                            size_t* bytes_to_send) {
  PacketUnit* packet = &packets_.front();
  CHECK(packet->first_fragment);
  // STAP-A NALU header.
  buffer[0] = (packet->header & (kFBit | kNriMask)) | H264::NaluType::kStapA;
  int index = kNalHeaderSize;
  *bytes_to_send += kNalHeaderSize;
  while (packet->aggregated) {
    const Fragment& fragment = packet->source_fragment;
    // Add NAL unit length field.
    ByteWriter<uint16_t>::WriteBigEndian(&buffer[index], fragment.length);
    index += kLengthFieldSize;
    *bytes_to_send += kLengthFieldSize;
    // Add NAL unit.
    memcpy(&buffer[index], fragment.buffer, fragment.length);
    index += fragment.length;
    *bytes_to_send += fragment.length;
    packets_.pop();
    input_fragments_.pop_front();
    if (packet->last_fragment)
      break;
    packet = &packets_.front();
  }
  CHECK(packet->last_fragment);
}

void RtpPacketizerH264::NextFragmentPacket(uint8_t* buffer,
                                           size_t* bytes_to_send) {
  PacketUnit* packet = &packets_.front();
  // NAL unit fragmented over multiple packets (FU-A).
  // We do not send original NALU header, so it will be replaced by the
  // FU indicator header of the first packet.
  uint8_t fu_indicator =
      (packet->header & (kFBit | kNriMask)) | H264::NaluType::kFuA;
  uint8_t fu_header = 0;

  // S | E | R | 5 bit type.
  fu_header |= (packet->first_fragment ? kSBit : 0);
  fu_header |= (packet->last_fragment ? kEBit : 0);
  uint8_t type = packet->header & kTypeMask;
  fu_header |= type;
  buffer[0] = fu_indicator;
  buffer[1] = fu_header;

  const Fragment& fragment = packet->source_fragment;
  *bytes_to_send = fragment.length + kFuAHeaderSize;
  memcpy(buffer + kFuAHeaderSize, fragment.buffer, fragment.length);
  if (packet->last_fragment)
    input_fragments_.pop_front();
  packets_.pop();
}

ProtectionType RtpPacketizerH264::GetProtectionType() {
  return kProtectedPacket;
}

StorageType RtpPacketizerH264::GetStorageType(
    uint32_t retransmission_settings) {
  return kAllowRetransmission;
}

std::string RtpPacketizerH264::ToString() {
  return "RtpPacketizerH264";
}

RtpDepacketizerH264::RtpDepacketizerH264() : offset_(0), length_(0) {}
RtpDepacketizerH264::~RtpDepacketizerH264() {}

bool RtpDepacketizerH264::Parse(ParsedPayload* parsed_payload,
                                const uint8_t* payload_data,
                                size_t payload_data_length) {
  CHECK(parsed_payload != nullptr);
  if (payload_data_length == 0) {
    LOG(LS_ERROR) << "Empty payload.";
    return false;
  }

  offset_ = 0;
  length_ = payload_data_length;
  modified_buffer_.reset();

  uint8_t nal_type = payload_data[0] & kTypeMask;
  if (nal_type == H264::NaluType::kFuA) {
    // Fragmented NAL units (FU-A).
    if (!ParseFuaNalu(parsed_payload, payload_data))
      return false;
  } else {
    // We handle STAP-A and single NALU's the same way here. The jitter buffer
    // will depacketize the STAP-A into NAL units later.
    // TODO(sprang): Parse STAP-A offsets here and store in fragmentation vec.
    if (!ProcessStapAOrSingleNalu(parsed_payload, payload_data))
      return false;
  }

  const uint8_t* payload =
      modified_buffer_ ? modified_buffer_->data() : payload_data;

  parsed_payload->payload = payload + offset_;
  parsed_payload->payload_length = length_;
  return true;
}

bool RtpDepacketizerH264::ProcessStapAOrSingleNalu(
    ParsedPayload* parsed_payload,
    const uint8_t* payload_data) {
  parsed_payload->type.Video.width = 0;
  parsed_payload->type.Video.height = 0;
  parsed_payload->type.Video.codec = kRtpVideoH264;
  parsed_payload->type.Video.isFirstPacket = true;
  RTPVideoHeaderH264* h264_header =
      &parsed_payload->type.Video.codecHeader.H264;

  const uint8_t* nalu_start = payload_data + kNalHeaderSize;
  const size_t nalu_length = length_ - kNalHeaderSize;
  uint8_t nal_type = payload_data[0] & kTypeMask;
  std::vector<size_t> nalu_start_offsets;
  if (nal_type == H264::NaluType::kStapA) {
    // Skip the StapA header (StapA NAL type + length).
    if (length_ <= kStapAHeaderSize) {
      LOG(LS_ERROR) << "StapA header truncated.";
      return false;
    }

    if (!ParseStapAStartOffsets(nalu_start, nalu_length, &nalu_start_offsets)) {
      LOG(LS_ERROR) << "StapA packet with incorrect NALU packet lengths.";
      return false;
    }

    h264_header->packetization_type = kH264StapA;
    nal_type = payload_data[kStapAHeaderSize] & kTypeMask;
  } else {
    h264_header->packetization_type = kH264SingleNalu;
    nalu_start_offsets.push_back(0);
  }
  h264_header->nalu_type = nal_type;
  parsed_payload->frame_type = kVideoFrameDelta;

  nalu_start_offsets.push_back(length_ + kLengthFieldSize);  // End offset.
  for (size_t i = 0; i < nalu_start_offsets.size() - 1; ++i) {
    size_t start_offset = nalu_start_offsets[i];
    // End offset is actually start offset for next unit, excluding length field
    // so remove that from this units length.
    size_t end_offset = nalu_start_offsets[i + 1] - kLengthFieldSize;
    if (end_offset - start_offset < H264::kNaluTypeSize) {
      LOG(LS_ERROR) << "STAP-A packet too short";
      return false;
    }

    nal_type = payload_data[start_offset] & kTypeMask;
    start_offset += H264::kNaluTypeSize;

    if (nal_type == H264::NaluType::kSps) {
      // Check if VUI is present in SPS and if it needs to be modified to avoid
      // excessive decoder latency.

      // Copy any previous data first (likely just the first header).
      std::unique_ptr<Buffer> output_buffer(new Buffer());
      if (start_offset)
        output_buffer->AppendData(payload_data, start_offset);

      // RBSP decode of payload data.
      std::unique_ptr<Buffer> rbsp_buffer = H264::ParseRbsp(
          &payload_data[start_offset], end_offset - start_offset);
      Optional<SpsParser::SpsState> sps;

      SpsVuiRewriter::ParseResult result = SpsVuiRewriter::ParseAndRewriteSps(
          rbsp_buffer->data(), rbsp_buffer->size(), &sps, output_buffer.get());
      switch (result) {
        case SpsVuiRewriter::ParseResult::kVuiRewritten:
          if (modified_buffer_) {
            LOG(LS_WARNING) << "More than one H264 SPS NAL units needing "
                               "rewriting found within a single STAP-A packet. "
                               "Keeping the first and rewriting the last.";
          }

          // Rewrite length field to new SPS size.
          if (h264_header->packetization_type == kH264StapA) {
            size_t length_field_offset =
                start_offset - (H264::kNaluTypeSize + kLengthFieldSize);
            // Stap-A Length includes payload data and type header.
            size_t rewritten_size =
                output_buffer->size() - start_offset + H264::kNaluTypeSize;
            ByteWriter<uint16_t>::WriteBigEndian(
                &(*output_buffer)[length_field_offset], rewritten_size);
          }

          // Append rest of packet.
          output_buffer->AppendData(&payload_data[end_offset],
                                    nalu_length + kNalHeaderSize - end_offset);

          modified_buffer_ = std::move(output_buffer);
          length_ = modified_buffer_->size();

          RTC_HISTOGRAM_ENUMERATION(kSpsValidHistogramName,
                                    SpsValidEvent::kReceivedSpsRewritten,
                                    SpsValidEvent::kSpsRewrittenMax);
          break;
        case SpsVuiRewriter::ParseResult::kPocOk:
          RTC_HISTOGRAM_ENUMERATION(kSpsValidHistogramName,
                                    SpsValidEvent::kReceivedSpsPocOk,
                                    SpsValidEvent::kSpsRewrittenMax);
          break;
        case SpsVuiRewriter::ParseResult::kVuiOk:
          RTC_HISTOGRAM_ENUMERATION(kSpsValidHistogramName,
                                    SpsValidEvent::kReceivedSpsVuiOk,
                                    SpsValidEvent::kSpsRewrittenMax);
          break;
        case SpsVuiRewriter::ParseResult::kFailure:
          RTC_HISTOGRAM_ENUMERATION(kSpsValidHistogramName,
                                    SpsValidEvent::kReceivedSpsParseFailure,
                                    SpsValidEvent::kSpsRewrittenMax);
          break;
      }

      if (sps) {
        parsed_payload->type.Video.width = sps->width;
        parsed_payload->type.Video.height = sps->height;
      }
      parsed_payload->frame_type = kVideoFrameKey;
    } else if (nal_type == H264::NaluType::kPps ||
               nal_type == H264::NaluType::kSei ||
               nal_type == H264::NaluType::kIdr) {
      parsed_payload->frame_type = kVideoFrameKey;
    }
  }

  return true;
}

bool RtpDepacketizerH264::ParseFuaNalu(
    RtpDepacketizer::ParsedPayload* parsed_payload,
    const uint8_t* payload_data) {
  if (length_ < kFuAHeaderSize) {
    LOG(LS_ERROR) << "FU-A NAL units truncated.";
    return false;
  }
  uint8_t fnri = payload_data[0] & (kFBit | kNriMask);
  uint8_t original_nal_type = payload_data[1] & kTypeMask;
  bool first_fragment = (payload_data[1] & kSBit) > 0;

  if (first_fragment) {
    offset_ = 0;
    length_ -= kNalHeaderSize;
    uint8_t original_nal_header = fnri | original_nal_type;
    modified_buffer_.reset(new Buffer());
    modified_buffer_->AppendData(payload_data + kNalHeaderSize, length_);
    (*modified_buffer_)[0] = original_nal_header;
  } else {
    offset_ = kFuAHeaderSize;
    length_ -= kFuAHeaderSize;
  }

  if (original_nal_type == H264::NaluType::kIdr) {
    parsed_payload->frame_type = kVideoFrameKey;
  } else {
    parsed_payload->frame_type = kVideoFrameDelta;
  }
  parsed_payload->type.Video.width = 0;
  parsed_payload->type.Video.height = 0;
  parsed_payload->type.Video.codec = kRtpVideoH264;
  parsed_payload->type.Video.isFirstPacket = first_fragment;
  RTPVideoHeaderH264* h264_header =
      &parsed_payload->type.Video.codecHeader.H264;
  h264_header->packetization_type = kH264FuA;
  h264_header->nalu_type = original_nal_type;
  return true;
}

}  // namespace webrtc
