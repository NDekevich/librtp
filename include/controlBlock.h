//#include "libavcodec/avcodec.h"
#include "boost\asio.hpp"
#include "RTP.h"
#include "rtcp.h"
#include <unordered_map>

class controlBlock
{
public:
	controlBlock();
	~controlBlock();

	struct metaData {
		boost::asio::ip::udp::endpoint endpoint;
		uint32_t packetsRecieved;
		uint32_t octetsRecieved;
		int32_t packetsLost;
		int32_t octetsLost;
		uint32_t highestSeqNum;
		uint32_t lastRtpTimestamp;
		uint32_t lastSR;
		uint32_t jitter;
	};


	uint32_t generateRtpTimestamp();

	uint32_t generateSeqNum();

	uint64_t getCurrentNtpTimestamp();

	template<typename IOPacket>
	void receivePacket(IOPacket);
	

	void sentRtcpPacket(int data, int type, int SSRC);
	void sentRtpPacket(int data, int payload, int SSRC); //chose type
	/*
	int* rawToCodec(int codecType,int* data);

	int* codecToRaw(int codecType, int* data);
	*/


	bool createNewListenerSocket(boost::asio::ip::udp::endpoint endpoint);
	
	bool createNewSenderSocket(boost::asio::ip::udp::endpoint endpoint);

	




private:
	std::vector<rtp::Rtp> rtpPackets;
	std::vector<rtcp::Rtcp> rtcpPackets;
	std::unordered_map<uint32_t,metaData> recievedPackets;//CSRC <-> data about that sender
	uint32_t packetsSent;
	uint32_t octetsSent;


};

