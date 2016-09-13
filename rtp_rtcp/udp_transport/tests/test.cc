#include <unistd.h>
#include "udp_transport/include/udp_transport.h"
#include "system_wrappers/interface/checks.h"

using namespace webrtc;

class UdpTransportDataImpl : public UdpTransportData {
 public:
  virtual ~UdpTransportDataImpl()  {};

  virtual void IncomingRTPPacket(const int8_t* incomingRtpPacket,
                                 const size_t rtpPacketLength,
                                 const char* fromIP,
                                 const uint16_t fromPort) {
    printf("IncomingRTPPacket....\n");
  }

  virtual void IncomingRTCPPacket(const int8_t* incomingRtcpPacket,
                                  const size_t rtcpPacketLength,
                                  const char* fromIP,
                                  const uint16_t fromPort) {
    printf("IncomingRTCPPacket....\n");
  }
};


int32_t main(int argc, char *argv[]) {
  int32_t id = 0;
  uint8_t threads = 1;
  UdpTransport* transport = UdpTransport::Create(id, threads);
  uint16_t dstRtpPort = 3000;
  uint16_t dstRtcpPort = 3001;

  uint16_t srcRtpPort = 4711;
  CHECK_EQ(0, transport->InitializeSendSockets("2.2.2.117",
                                          dstRtpPort,
                                          dstRtcpPort));
  UdpTransportDataImpl *pUdpData = new UdpTransportDataImpl();
  CHECK_EQ(0, transport->InitializeReceiveSockets(
        pUdpData,
        srcRtpPort));

  CHECK_EQ(0, transport->InitializeSourcePorts(4811, 4812));
  uint16_t rtpPort, rtcpPort;
  CHECK_EQ(0, transport->SourcePorts(rtpPort, rtcpPort));
  printf("rtpPort: %d rtcpPort: %d\n", rtpPort, rtcpPort);
  CHECK_EQ(0, transport->StartReceiving(20));

  int8_t data[10] = {4,3,2,1,2,5,9};
  while(1) {
    sleep(1);
    CHECK_EQ(10, transport->SendRTPPacketTo(data, 10, 4811));
    CHECK_EQ(10, transport->SendRTCPPacketTo(data, 10, 4722));
    CHECK_EQ(10, transport->SendRaw(data, 10, false));
    printf(".");
  }

  UdpTransport::Destroy(transport);

  return 0;
}
