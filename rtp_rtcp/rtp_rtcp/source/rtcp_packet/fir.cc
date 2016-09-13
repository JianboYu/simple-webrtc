/*
 *  Copyright (c) 2016 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rtp_rtcp/source/rtcp_packet/fir.h"

#include "system_wrappers/interface/checks.h"
#include "system_wrappers/interface/logging.h"
#include "rtp_rtcp/source/byte_io.h"
#include "rtp_rtcp/source/rtcp_packet/common_header.h"

namespace webrtc {
namespace rtcp {
constexpr uint8_t Fir::kFeedbackMessageType;
// RFC 4585: Feedback format.
// Common packet format:
//
//   0                   1                   2                   3
//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |V=2|P|   FMT   |       PT      |          length               |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                  SSRC of packet sender                        |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |             SSRC of media source (unused) = 0                 |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  :            Feedback Control Information (FCI)                 :
//  :                                                               :
// Full intra request (FIR) (RFC 5104).
// The Feedback Control Information (FCI) for the Full Intra Request
// consists of one or more FCI entries.
// FCI:
//   0                   1                   2                   3
//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                              SSRC                             |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  | Seq nr.       |    Reserved = 0                               |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
bool Fir::Parse(const CommonHeader& packet) {
  CHECK_EQ(packet.type(), kPacketType);
  CHECK_EQ(packet.fmt(), kFeedbackMessageType);

  // The FCI field MUST contain one or more FIR entries.
  if (packet.payload_size_bytes() < kCommonFeedbackLength + kFciLength) {
    LOG(LS_WARNING) << "Packet is too small to be a valid FIR packet.";
    return false;
  }

  if ((packet.payload_size_bytes() - kCommonFeedbackLength) % kFciLength != 0) {
    LOG(LS_WARNING) << "Invalid size for a valid FIR packet.";
    return false;
  }

  ParseCommonFeedback(packet.payload());

  size_t number_of_fci_items =
      (packet.payload_size_bytes() - kCommonFeedbackLength) / kFciLength;
  const uint8_t* next_fci = packet.payload() + kCommonFeedbackLength;
  items_.resize(number_of_fci_items);
  for (Request& request : items_) {
    request.ssrc = ByteReader<uint32_t>::ReadBigEndian(next_fci);
    request.seq_nr = ByteReader<uint8_t>::ReadBigEndian(next_fci + 4);
    next_fci += kFciLength;
  }
  return true;
}

bool Fir::Create(uint8_t* packet,
                 size_t* index,
                 size_t max_length,
                 RtcpPacket::PacketReadyCallback* callback) const {
  CHECK(!items_.empty());
  while (*index + BlockLength() > max_length) {
    if (!OnBufferFull(packet, index, callback))
      return false;
  }
  size_t index_end = *index + BlockLength();
  CreateHeader(kFeedbackMessageType, kPacketType, HeaderLength(), packet,
               index);
  CHECK_EQ(Psfb::media_ssrc(), 0u);
  CreateCommonFeedback(packet + *index);
  *index += kCommonFeedbackLength;

  constexpr uint32_t kReserved = 0;
  for (const Request& request : items_) {
    ByteWriter<uint32_t>::WriteBigEndian(packet + *index, request.ssrc);
    ByteWriter<uint8_t>::WriteBigEndian(packet + *index + 4, request.seq_nr);
    ByteWriter<uint32_t, 3>::WriteBigEndian(packet + *index + 5, kReserved);
    *index += kFciLength;
  }
  CHECK_EQ(*index, index_end);
  return true;
}
}  // namespace rtcp
}  // namespace webrtc
