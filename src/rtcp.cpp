#include "rtcp.h"



rtcp::rtcp()
{
}


rtcp::~rtcp()
{
}

rtcp::Rtcp::sdesItem::sdesItem(rtpSdesTypes type, uint8_t length, char * item)
{
}

void rtcp::Rtcp::setVersion(int version)
{
	header.firstOctet = (header.firstOctet & 0x3F) + (version << 6); //0b00111111 
}

int rtcp::Rtcp::getVersion() const
{
	return (header.firstOctet & 0xC0) >> 6;  //0b11000000
}

void rtcp::Rtcp::setPadding(bool padding)
{
	if (padding) {
		header.firstOctet |= 0x20; //0b00100000
	}
	else {
		header.firstOctet &= 0xDF; //0b11011111
	}
}

bool rtcp::Rtcp::getPadding() const
{
	return ((header.firstOctet & 0x20) >> 5) == 1; //0b00100000
}

void rtcp::Rtcp::setReportCount(int rc)
{
	header.firstOctet = (header.firstOctet & 0xE0) + rc; //0b11100000
}

int rtcp::Rtcp::getReportCount()
{
	return (header.firstOctet & 0x1F);//0b00011111
}

void rtcp::Rtcp::setPayload(rtcpPayloadTypes pc)
{
	header.payloadType = pc;
}

void rtcp::Rtcp::setHeaderLength(uint16_t length)
{
	header.length = length;
}

void rtcp::Rtcp::setHeaderSSRC(uint32_t ssrc)
{
	SSRC = ssrc;
}

bool rtcp::Rtcp::addReportBlock(reportBlock block)
{
	if (getReportCount() < 31) {
		setReportCount(getReportCount() + 1);
		reports.push_back(block);
		return true;
	}
	return false;
}

bool rtcp::Rtcp::addSdesItem(sdesItem item)
{
	if (getReportCount() < 31) {
		setReportCount(getReportCount() + 1);
		items.push_back(item);
		return true;
	}
	return false;
}

