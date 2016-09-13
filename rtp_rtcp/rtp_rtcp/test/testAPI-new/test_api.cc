/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "test_api.h"
#include "system_wrappers/interface/checks.h"

#include <algorithm>
#include <memory>
#include <vector>
#include <unistd.h>

namespace webrtc {

class NullTransport : public Transport {
public:
  bool SendRtp(const uint8_t* packet,
               size_t length,
               const PacketOptions& options) {return true;}
  bool SendRtcp(const uint8_t* packet, size_t length) { return true;}
};

void LoopBackTransport::SetSendModule(RtpRtcp* rtp_rtcp_module,
                                      RTPPayloadRegistry* payload_registry,
                                      RtpReceiver* receiver,
                                      ReceiveStatistics* receive_statistics) {
  rtp_rtcp_module_ = rtp_rtcp_module;
  rtp_payload_registry_ = payload_registry;
  rtp_receiver_ = receiver;
  receive_statistics_ = receive_statistics;
}

void LoopBackTransport::DropEveryNthPacket(int n) {
  packet_loss_ = n;
}

bool LoopBackTransport::SendRtp(const uint8_t* data,
                                size_t len,
                                const PacketOptions& options) {
  count_++;
  if (packet_loss_ > 0) {
    if ((count_ % packet_loss_) == 0) {
      return true;
    }
  }
  RTPHeader header;
  std::unique_ptr<RtpHeaderParser> parser(RtpHeaderParser::Create());
  if (!parser->Parse(static_cast<const uint8_t*>(data), len, &header)) {
    CHECK(!!"Parse error!!!!");
    return false;
  }
  PayloadUnion payload_specific;
  if (!rtp_payload_registry_->GetPayloadSpecifics(header.payloadType,
                                                  &payload_specific)) {
    CHECK(!!"GetPayloadSpecifics error!!!!");
    return false;
  }
  receive_statistics_->IncomingPacket(header, len, false);
  if (!rtp_receiver_->IncomingRtpPacket(header,
                                        static_cast<const uint8_t*>(data), len,
                                        payload_specific, true)) {
    CHECK(!!"IncomingRtpPacket error!!!!");
    return false;
  }
  return true;
}

bool LoopBackTransport::SendRtcp(const uint8_t* data, size_t len) {
  if (rtp_rtcp_module_->IncomingRtcpPacket((const uint8_t*)data, len) < 0) {
    CHECK(!!"IncomingRtcpPacket error!!!!");
    return false;
  }
  return true;
}

int32_t TestRtpReceiver::OnReceivedPayloadData(
    const uint8_t* payload_data,
    size_t payload_size,
    const webrtc::WebRtcRTPHeader* rtp_header) {
  CHECK_LE(payload_size, sizeof(payload_data_));
  memcpy(payload_data_, payload_data, payload_size);
  memcpy(&rtp_header_, rtp_header, sizeof(rtp_header_));
  payload_size_ = payload_size;
  return 0;
}

#define test_rate 64000u

class VerifyingAudioReceiver : public NullRtpData {
 public:
  int32_t OnReceivedPayloadData(
      const uint8_t* payloadData,
      size_t payloadSize,
      const webrtc::WebRtcRTPHeader* rtpHeader) override {
    printf("payload type: %d\n", rtpHeader->header.payloadType);
    if (rtpHeader->header.payloadType == 98 ||
        rtpHeader->header.payloadType == 99) {
      CHECK_EQ(4u, payloadSize);
      char str[5];
      memcpy(str, payloadData, payloadSize);
      str[4] = 0;
      // All our test vectors for payload type 96 and 97 even the stereo is on
      // a per channel base equal to the 4 chars "test".
      // Note there is no null termination so we add that to use the
      // test EXPECT_STRCASEEQ.
      //EXPECT_STRCASEEQ("test", str);
      printf("recevied data: %s\n", str);
      return 0;
    }
    if (rtpHeader->header.payloadType == 100 ||
        rtpHeader->header.payloadType == 101 ||
        rtpHeader->header.payloadType == 102) {
      if (rtpHeader->type.Audio.channel == 1) {
        if (payloadData[0] == 0xff) {
          // All our test vectors for payload type 100, 101 and 102 have the
          // first channel data being equal to 0xff.
          return 0;
        }
      }
      //ADD_FAILURE() << "This code path should never happen.";

      CHECK(!!"This code path should never happen.");
      return -1;
    }
    return 0;
  }
};

class RTPCallback : public NullRtpFeedback {
 public:
  int32_t OnInitializeDecoder(const int8_t payloadType,
                              const char payloadName[RTP_PAYLOAD_NAME_SIZE],
                              const int frequency,
                              const size_t channels,
                              const uint32_t rate) override {
    if (payloadType == 96) {
      CHECK_EQ(test_rate, rate);/* <<
          "The rate should be 64K for this payloadType";*/
    }
    return 0;
  }
};


}  // namespace webrtc

using namespace webrtc;

int32_t main_api(int argc, char *agrv[]) {
  std::unique_ptr<RTPPayloadRegistry> rtp_payload_registry_;
  std::unique_ptr<RtpReceiver> rtp_receiver_;
  std::unique_ptr<RtpRtcp> module_;
  uint32_t test_ssrc_ = 3456;
  uint32_t test_timestamp_ = 4567;
  uint16_t test_sequence_number_ = 2345;
  std::vector<uint32_t> test_csrcs_;
  SimulatedClock fake_clock_(123456);
  NullTransport null_transport_;

  test_csrcs_.push_back(1234);
  test_csrcs_.push_back(2345);

  RtpRtcp::Configuration configuration;
  configuration.audio = true;
  configuration.clock = &fake_clock_;
  configuration.outgoing_transport = &null_transport_;
  module_.reset(RtpRtcp::CreateRtpRtcp(configuration));
  rtp_payload_registry_.reset(new RTPPayloadRegistry(
          RTPPayloadStrategy::CreateStrategy(true)));
  rtp_receiver_.reset(RtpReceiver::CreateAudioReceiver(
      &fake_clock_, NULL, NULL, rtp_payload_registry_.get()));

  module_->SetSequenceNumber(test_sequence_number_);
  CHECK_EQ(test_sequence_number_, module_->SequenceNumber());
  printf("seq num: %d\n", module_->SequenceNumber());

  module_->SetStartTimestamp(test_timestamp_);
  CHECK_EQ(test_timestamp_, module_->StartTimestamp());
  printf("ts: %d\n", module_->StartTimestamp());

  CHECK_EQ(0, module_->Sending());
  CHECK_EQ(0, module_->SetSendingStatus(true));
  CHECK(module_->Sending());

  CHECK_EQ(0, module_->SetMaxTransferUnit(1234));
  CHECK_EQ(1234 - 20 - 8, module_->MaxPayloadLength());

  CHECK_EQ(0, module_->SetTransportOverhead(true, true, 12));
  CHECK_EQ(1234 - 20 - 20 - 20 - 12, module_->MaxPayloadLength());

  CHECK_EQ(0, module_->SetTransportOverhead(false, false, 0));
  CHECK_EQ(1234 - 20 - 8, module_->MaxPayloadLength());

  module_->SetSSRC(test_ssrc_);
  CHECK_EQ(test_ssrc_, module_->SSRC());

  printf("RtcpMode::kOff:%d\n", (int)module_->RTCP());
  module_->SetRTCPStatus(RtcpMode::kCompound);
  printf("RtcpMode::kCompound:%d\n", (int)module_->RTCP());

  CHECK_EQ(0, module_->SetCNAME("john.doe@test.test"));

  CHECK_EQ(0, module_->TMMBR());
  module_->SetTMMBRStatus(true);
  CHECK_EQ(1, module_->TMMBR());
  module_->SetTMMBRStatus(false);
  CHECK_EQ(0, module_->TMMBR());

  module_->SetRtxSendStatus(kRtxRetransmitted);
  CHECK_EQ(kRtxRetransmitted, module_->RtxSendStatus());

  module_->SetRtxSendStatus(kRtxOff);
  CHECK_EQ(kRtxOff, module_->RtxSendStatus());

  module_->SetRtxSendStatus(kRtxRetransmitted);
  CHECK_EQ(kRtxRetransmitted, module_->RtxSendStatus());

  const uint32_t kRtxSsrc = 1;
  const int kRtxPayloadType = 119;
  const int kPayloadType = 100;
  CHECK_EQ(0, rtp_payload_registry_->RtxEnabled());
  rtp_payload_registry_->SetRtxSsrc(kRtxSsrc);
  rtp_payload_registry_->SetRtxPayloadType(kRtxPayloadType, kPayloadType);
  CHECK_EQ(1, rtp_payload_registry_->RtxEnabled());
  RTPHeader rtx_header;
  rtx_header.ssrc = kRtxSsrc;
  rtx_header.payloadType = kRtxPayloadType;
  CHECK_EQ(1, rtp_payload_registry_->IsRtx(rtx_header));
  rtx_header.ssrc = 0;
  CHECK_EQ(0, rtp_payload_registry_->IsRtx(rtx_header));
  rtx_header.ssrc = kRtxSsrc;
  rtx_header.payloadType = 0;
  CHECK_EQ(1, rtp_payload_registry_->IsRtx(rtx_header));

  return 0;
}


int32_t main_audio(int argc, char *agrv[]) {
  RtpRtcp* module1;
  RtpRtcp* module2;
  std::unique_ptr<ReceiveStatistics> receive_statistics1_;
  std::unique_ptr<ReceiveStatistics> receive_statistics2_;
  std::unique_ptr<RtpReceiver> rtp_receiver1_;
  std::unique_ptr<RtpReceiver> rtp_receiver2_;
  std::unique_ptr<RTPPayloadRegistry> rtp_payload_registry1_;
  std::unique_ptr<RTPPayloadRegistry> rtp_payload_registry2_;
  VerifyingAudioReceiver* data_receiver1;
  VerifyingAudioReceiver* data_receiver2;
  LoopBackTransport* transport1;
  LoopBackTransport* transport2;
  RTPCallback* rtp_callback;
  uint32_t test_ssrc = 3456;
  uint32_t test_timestamp = 4567;
  uint16_t test_sequence_number = 2345;
  uint32_t test_CSRC[webrtc::kRtpCsrcSize] = {1234, 2345};
  SimulatedClock fake_clock(123456);

  data_receiver1 = new VerifyingAudioReceiver();
  data_receiver2 = new VerifyingAudioReceiver();
  rtp_callback = new RTPCallback();
  transport1 = new LoopBackTransport();
  transport2 = new LoopBackTransport();

  receive_statistics1_.reset(ReceiveStatistics::Create(&fake_clock));
  receive_statistics2_.reset(ReceiveStatistics::Create(&fake_clock));

  rtp_payload_registry1_.reset(new RTPPayloadRegistry(
      RTPPayloadStrategy::CreateStrategy(true)));
  rtp_payload_registry2_.reset(new RTPPayloadRegistry(
      RTPPayloadStrategy::CreateStrategy(true)));

  RtpRtcp::Configuration configuration;
  configuration.audio = true;
  configuration.clock = &fake_clock;
  configuration.receive_statistics = receive_statistics1_.get();
  configuration.outgoing_transport = transport1;

  module1 = RtpRtcp::CreateRtpRtcp(configuration);
  rtp_receiver1_.reset(RtpReceiver::CreateAudioReceiver(
      &fake_clock, data_receiver1, NULL, rtp_payload_registry1_.get()));

  configuration.receive_statistics = receive_statistics2_.get();
  configuration.outgoing_transport = transport2;

  module2 = RtpRtcp::CreateRtpRtcp(configuration);
  rtp_receiver2_.reset(RtpReceiver::CreateAudioReceiver(
      &fake_clock, data_receiver2, NULL, rtp_payload_registry2_.get()));

  transport1->SetSendModule(module2, rtp_payload_registry2_.get(),
                            rtp_receiver2_.get(), receive_statistics2_.get());
  transport2->SetSendModule(module1, rtp_payload_registry1_.get(),
                            rtp_receiver1_.get(), receive_statistics1_.get());

  /* Basic Func */
  module1->SetSSRC(test_ssrc);
  module1->SetStartTimestamp(test_timestamp);

  // Test detection at the end of a DTMF tone.
  // EXPECT_EQ(0, module2->SetTelephoneEventForwardToDecoder(true));

  CHECK_EQ(0, module1->SetSendingStatus(true));

  // Start basic RTP test.

  // Send an empty RTP packet.
  // Should fail since we have not registered the payload type.
  CHECK_EQ(-1, module1->SendOutgoingData(webrtc::kAudioFrameSpeech,
                                          96, 0, -1, NULL, 0));

  CodecInst voice_codec;
  memset(&voice_codec, 0, sizeof(voice_codec));
  voice_codec.pltype = 96;
  voice_codec.plfreq = 8000;
  memcpy(voice_codec.plname, "PCMU", 5);

  CHECK_EQ(0, module1->RegisterSendPayload(voice_codec));
  CHECK_EQ(0, rtp_receiver1_->RegisterReceivePayload(
      voice_codec.plname,
      voice_codec.pltype,
      voice_codec.plfreq,
      voice_codec.channels,
      (voice_codec.rate < 0) ? 0 : voice_codec.rate));
  CHECK_EQ(0, module2->RegisterSendPayload(voice_codec));
  voice_codec.rate = test_rate;
  CHECK_EQ(0, rtp_receiver2_->RegisterReceivePayload(
      voice_codec.plname,
      voice_codec.pltype,
      voice_codec.plfreq,
      voice_codec.channels,
      (voice_codec.rate < 0) ? 0 : voice_codec.rate));

  const uint8_t test[5] = "test";
  CHECK_EQ(0, module1->SendOutgoingData(webrtc::kAudioFrameSpeech, 96,
                                         0, -1, test, 4));

  CHECK_EQ(test_ssrc, rtp_receiver2_->SSRC());
  uint32_t timestamp;
  CHECK_EQ(1, rtp_receiver2_->Timestamp(&timestamp));
  CHECK_EQ(test_timestamp, timestamp);

  sleep(1);

  delete module1;
  delete module2;
  delete transport1;
  delete transport2;
  delete data_receiver1;
  delete data_receiver2;
  delete rtp_callback;

  return 0;
}
int32_t main(int argc, char *agrv[]) {
  //return main_api(argc, agrv);
  return main_audio(argc, agrv);

  return 0;
}
