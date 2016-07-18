#include "rtcp.h"


using namespace rtcp;
Rtcp::Rtcp()
{
	header.firstOctet = 0b10000000;
}


Rtcp::~Rtcp()
{
}

void Rtcp::setVersion(int version)
{
	header.firstOctet = (header.firstOctet & 0x3F) + (version << 6); //0b00111111 
}

int Rtcp::getVersion() const
{
	return (header.firstOctet & 0xC0) >> 6;  //0b11000000
}

void Rtcp::setPadding(bool padding)
{
	if (padding) {
		header.firstOctet |= 0x20; //0b00100000
	}
	else {
		header.firstOctet &= 0xDF; //0b11011111
	}
}

bool  Rtcp::getPadding() const
{
	return ((header.firstOctet & 0x20) >> 5) == 1; //0b00100000
}

void Rtcp::setReportCount(int rc)
{
	header.firstOctet = (header.firstOctet & 0xE0) + rc; //0b11100000
}

int Rtcp::getReportCount()
{
	return (header.firstOctet & 0x1F);//0b00011111
}

void Rtcp::setPayload(rtcpPayloadTypes pc)
{
	header.payloadType = (int)pc;
}

void Rtcp::setHeaderLength(uint16_t length)
{
	header.length = length;
}

void Rtcp::setHeaderSSRC(uint32_t ssrc)
{
	SSRC = ssrc;
}

bool Rtcp::addReportBlock(reportBlock block)
{
	if (reportCount < 31) {
		reportCount++;
		reports.push_back(block);
		return true;
	}
	return false;
}

void Rtcp::resetReportBlock()
{
	reportCount = 0;
	reports.clear();
}

bool Rtcp::addSdesItem(sdesItem item)
{
	if (sdesCount < 31) {
		sdesCount++;
		items.push_back(item);
		return true;
	}
	return false;
}

void Rtcp::resetSdesItems()
{
	sdesCount = 0;
	items.clear();
}

bool Rtcp::addLeaver(uint32_t ssrc)
{

	if (leaverCount < 31)
	{
		otherLeavers.push_back(ssrc);
		leaverCount++;
		return true;
	}
	return false;
}


void Rtcp::resetLeavers()
{
	otherLeavers.clear();
	leaverCount = 0;
}

void Rtcp::addGoodbyeText(std::string text)
{
	int lengthDifference = 4 - ((text.length() + 3) % 4);
	for (int i = 0; i < lengthDifference; i++) 
	{
		goodbyeText.append(char(0b00000000));
	}
	goodbyeTextLength = goodbyeText.length();
	optGoodbyeText = true;
}

uint16_t rtcp::Rtcp::calculateHeaderLength()
{
	int length = 1;
	switch (getPayload())
	{
	case SenderReport:
		return 6;
	case ReceiverReport:
		return 1 + reportCount * 6;
	case SourceDescription:
		for (int i = 0; i < sdesCount; i++) {
			length += (items[i].itemLength / 4);
		}
		return length;
	case Goodbye:
		length += otherLeavers.size();
		length += optGoodbyeText ? ((1 + goodbyeTextLength) / 4) : 0;
		return length;
	case AppDef:
		return 0;
	default:
		return 0;

	}
}

bool Rtcp::validateHeader()
{
	if (getVersion() == 2) {
		if (getPayload() > 199) {
			if (getPayload() < 206) {
				return true;
			}
		}
	}
	return false;
}

rtcp::Rtcp::reportBlock::reportBlock()
{
	ssrc = 0;
	fractionLost = 0;
	packetsLost = 0;
	highestSeqNum = 0;
	interarrivalJitter = 0;
	lastSR = 0;
	delaySinceLSR = 0;
}

rtcp::Rtcp::sdesItem::sdesItem(int sdesType, std::string data)
{
	rtpSdesType = sdesType;
	itemLength = data.size();
	std::vector<char>item(data.begin(),data.end());
}


rtcp::Rtcp::sdesItem::sdesItem(int sdesType, std::vector<char> data)
{
	rtpSdesType = sdesType;
	itemLength = data.size();
	std::vector<char>item(data);
}