/*
 *  Copyright (c) 2016 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_BASE_ONETIMEEVENT_H_
#define WEBRTC_BASE_ONETIMEEVENT_H_

#include "system_wrappers/interface/critical_section_wrapper.h"
#include "system_wrappers/interface/scoped_ptr.h"
#include "system_wrappers/interface/typedefs.h"

namespace webrtc {
// Provides a simple way to perform an operation (such as logging) one
// time in a certain scope.
// Example:
//   OneTimeEvent firstFrame;
//   ...
//   if (firstFrame()) {
//     LOG(LS_INFO) << "This is the first frame".
//   }
class OneTimeEvent {
 public:
  OneTimeEvent() {
    critsect_.reset(CriticalSectionWrapper::CreateCriticalSection());
  }
  bool operator()() {
    CriticalSectionScoped cs(critsect_.get());
    if (happened_) {
      return false;
    }
    happened_ = true;
    return true;
  }

 private:
  bool happened_ = false;
  scoped_ptr<CriticalSectionWrapper> critsect_;
};

// A non-thread-safe, ligher-weight version of the OneTimeEvent class.
class ThreadUnsafeOneTimeEvent {
 public:
  ThreadUnsafeOneTimeEvent() {}
  bool operator()() {
    if (happened_) {
      return false;
    }
    happened_ = true;
    return true;
  }

 private:
  bool happened_ = false;
};

}  // namespace webrtc

#endif  // WEBRTC_BASE_ONETIMEEVENT_H_
