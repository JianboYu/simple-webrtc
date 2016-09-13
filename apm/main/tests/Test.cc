#include <unistd.h>
#include "TestAllCodecs.h"

using namespace webrtc;
int main(int argc, char *argv[])  {
  TestAllCodecs oTest(0);
  oTest.Perform();
  return 0;

  scoped_ptr<AudioCodingModule> acm_a_;
  acm_a_.reset(AudioCodingModule::Create(0)),
  assert(0 == acm_a_->InitializeSender());
  assert(0 ==acm_a_->InitializeReceiver());


  // Get all codec parameters before registering
  //
  int32_t sampling_freq_hz = 8000;
  int32_t rate = 13300;
  int32_t packet_size = 240;
  CodecInst my_codec_param;
  const char *codec_name = "ILBC";
  assert(0 == AudioCodingModule::Codec(codec_name, &my_codec_param,
                                       sampling_freq_hz, 1));
  my_codec_param.rate = rate;
  my_codec_param.pacsize = packet_size;
  assert(AudioCodingModule::IsCodecValid(my_codec_param));
  assert(0 == acm_a_->RegisterSendCodec(my_codec_param));

  #if 0
  uint8_t num_encoders = acm_a_->NumberOfCodecs();
  //CodecInst my_codec_param;
  for (uint8_t n = 0; n < num_encoders; n++) {
    acm_a_->Codec(n, &my_codec_param);
    printf("Codec name: %s\n", my_codec_param.plname);
    if (!strcmp(my_codec_param.plname, "ILBC")) {
      my_codec_param.channels = 1;
      printf("ILBC Found\n");
      my_codec_param.rate = rate;
      my_codec_param.pacsize = packet_size;
      assert(0 == acm_a_->RegisterSendCodec(my_codec_param));
      break;
    }
  }
  #endif

  scoped_ptr<TestPack> channel_a_to_b_(new TestPack);
  acm_a_->RegisterTransportCallback(channel_a_to_b_.get());

  AudioFrame audio_frame;


  const std::string file_name = std::string("test.pcm");
  PCMFile infile_a_;
  infile_a_.Open(file_name, 8000, "rb");
  int error_count = 0;
  int counter = 0;
  int32_t ts = 0;
  while (!infile_a_.EndOfFile()) {
    // Add 10 msec to ACM.
    infile_a_.Read10MsData(audio_frame);
    audio_frame.timestamp_ = ts;
    assert(0 == acm_a_->Add10MsData(audio_frame));

    // Run sender side of ACM.
    //assert(0 == acm_a_->Process());
    acm_a_->Process();

    // Verify that the received packet size matches the settings.
    int32_t receive_size = channel_a_to_b_->payload_size();
    if (receive_size) {
      printf("receive_size: %d\n", receive_size);
      }

      // Verify that the timestamp is updated with expected length. The counter
      // is used to avoid problems when switching codec or frame size in the
      // test.
      int32_t timestamp_diff = channel_a_to_b_->timestamp_diff();
      printf("timestamp_diff : %d\n", timestamp_diff);
      
    ts += audio_frame.samples_per_channel_;
    printf("samples_per chn: %d\n", (int)audio_frame.samples_per_channel_);
    // Update loop counter
    counter++;
  }

  assert(0==error_count);

  if (infile_a_.EndOfFile()) {
    infile_a_.Rewind();
  }
  while(1) {
    printf(".\n");
    sleep(1);
  }
  return 0;
}
