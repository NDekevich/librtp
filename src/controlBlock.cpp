#include "controlBlock.h"
#include "boost\asio.hpp"


controlBlock::controlBlock()
{
	generateSSRC();
	initializeOut();
	generateSeqNum();
	generateRtpTimestamp();


}


controlBlock::~controlBlock()
{
}

void controlBlock::setSdesItems(rtcp::Rtcp::rtpSdesTypes type, std::string value)
{

	if (((int)type > 0)&&((int)type<15)) {
	outInfo.sdesInfo[(int)type - 1] = value;
	}

}

void controlBlock::initializeOut() {

	outInfo.timeLastRtcp = 0;
	outInfo.currentTime = 0;
	outInfo.senders = 0;
	outInfo.pmembers = 0;
	outInfo.members = 0;
	outInfo.we_sent = false;
	//outInfo.rtcp_bw = 123123123;
	outInfo.initial = true;
	calculateRtcpInterval();

}


