#include "controlBlock.h"
#include "boost\asio.hpp"


controlBlock::controlBlock()
{
	
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


