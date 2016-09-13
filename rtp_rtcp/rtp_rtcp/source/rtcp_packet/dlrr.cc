/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rtp_rtcp/source/rtcp_packet/dlrr.h"

#include "system_wrappers/interface/checks.h"
#include "system_wrappers/interface/logging.h"
#include "rtp_rtcp/source/byte_io.h"

namespace webrtc {
namespace rtcp {
// DLRR Report Block (RFC 3611).
//
//   0                   1                   2                   3
//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |     BT=5      |   reserved    |         block length          |
//  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//  |                 SSRC_1 (SSRC of first receiver)               | sub-
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
//  |                         last RR (LRR)                         |   1
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                   delay since last RR (DLRR)                  |
//  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//  |                 SSRC_2 (SSRC of second receiver)              | sub-
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
//  :                               ...                             :   2
bool Dlrr::Parse(const uint8_t* buffer, uint16_t block_length_32bits) {
  CHECK(buffer[0] == kBlockType);
  // kReserved = buffer[1];
  CHECK_EQ(block_length_32bits,
                ByteReader<uint16_t>::ReadBigEndian(&buffer[2]));
  if (block_length_32bits % 3 != 0) {
    LOG(LS_WARNING) << "Invalid size for dlrr block.";
    return false;
  }

  size_t blocks_count = block_length_32bits / 3;
  const uint8_t* read_at = buffer + kBlockHeaderLength;
  sub_blocks_.resize(blocks_count);
  for (ReceiveTimeInfo& sub_block : sub_blocks_) {
    sub_block.ssrc = ByteReader<uint32_t>::ReadBigEndian(&read_at[0]);
    sub_block.last_rr = ByteReader<uint32_t>::ReadBigEndian(&read_at[4]);
    sub_block.delay_since_last_rr =
        ByteReader<uint32_t>::ReadBigEndian(&read_at[8]);
    read_at += kSubBlockLength;
  }
  return true;
}

size_t Dlrr::BlockLength() const {
  if (sub_blocks_.empty())
    return 0;
  return kBlockHeaderLength + kSubBlockLength * sub_blocks_.size();
}

void Dlrr::Create(uint8_t* buffer) const {
  if (sub_blocks_.empty())  // No subblocks, no need to write header either.
    return;
  // Create block header.
  const uint8_t kReserved = 0;
  buffer[0] = kBlockType;
  buffer[1] = kReserved;
  ByteWriter<uint16_t>::WriteBigEndian(&buffer[2], 3 * sub_blocks_.size());
  // Create sub blocks.
  uint8_t* write_at = buffer + kBlockHeaderLength;
  for (const ReceiveTimeInfo& sub_block : sub_blocks_) {
    ByteWriter<uint32_t>::WriteBigEndian(&write_at[0], sub_block.ssrc);
    ByteWriter<uint32_t>::WriteBigEndian(&write_at[4], sub_block.last_rr);
    ByteWriter<uint32_t>::WriteBigEndian(&write_at[8],
                                         sub_block.delay_since_last_rr);
    write_at += kSubBlockLength;
  }
  CHECK_EQ(buffer + BlockLength(), write_at);
}

bool Dlrr::WithDlrrItem(const ReceiveTimeInfo& block) {
  if (sub_blocks_.size() >= kMaxNumberOfDlrrItems) {
    LOG(LS_WARNING) << "Max DLRR items reached.";
    return false;
  }
  sub_blocks_.push_back(block);
  return true;
}

bool Dlrr::WithDlrrItem(uint32_t ssrc,
                        uint32_t last_rr,
                        uint32_t delay_last_rr) {
  ReceiveTimeInfo block;
  block.ssrc = ssrc;
  block.last_rr = last_rr;
  block.delay_since_last_rr = delay_last_rr;
  return WithDlrrItem(block);
}
}  // namespace rtcp
}  // namespace webrtc
