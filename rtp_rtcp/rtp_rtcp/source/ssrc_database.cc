/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rtp_rtcp/source/ssrc_database.h"
#include "system_wrappers/interface/timeutils.h"
#include "system_wrappers/interface/checks.h"

namespace webrtc {

SSRCDatabase* SSRCDatabase::GetSSRCDatabase() {
  return GetStaticInstance<SSRCDatabase>(kAddRef);
}

void SSRCDatabase::ReturnSSRCDatabase() {
  GetStaticInstance<SSRCDatabase>(kRelease);
}

uint32_t SSRCDatabase::CreateSSRC() {
  CriticalSectionScoped cs(crit_.get());

  while (true) {  // Try until get a new ssrc.
    // 0 and 0xffffffff are invalid values for SSRC.
    uint32_t ssrc = random_.Rand(1u, 0xfffffffe);
    if (ssrcs_.insert(ssrc).second) {
      return ssrc;
    }
  }
}

void SSRCDatabase::RegisterSSRC(uint32_t ssrc) {
  CriticalSectionScoped cs(crit_.get());
  ssrcs_.insert(ssrc);
}

void SSRCDatabase::ReturnSSRC(uint32_t ssrc) {
  CriticalSectionScoped cs(crit_.get());
  ssrcs_.erase(ssrc);
}

SSRCDatabase::SSRCDatabase() : random_(TimeMicros()) {
  crit_.reset(CriticalSectionWrapper::CreateCriticalSection());
}

SSRCDatabase::~SSRCDatabase() {}

}  // namespace webrtc
