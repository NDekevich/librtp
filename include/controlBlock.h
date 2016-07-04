//#include "libavcodec/avcodec.h"
#include "boost\asio.hpp"
#include "RTP.h"
#include "rtcp.h"


class controlBlock
{
public:
	controlBlock();
	~controlBlock();

	struct MetaData {
		uint32_t SSRC();



	};


	uint32_t generateRtpTimestamp();

	uint32_t generateSeqNum();

	uint64_t getCurrentNtpTimestamp();

	





private:
	std::vector<rtp::Rtp> rtpPackets;
	std::vector<rtcp::Rtcp> rtcpPackets;
	

};

