// testsRtp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"

#include "RTP.h"

using namespace rtp;


TEST(RtpTest, versionTest)
{
	Rtp testPacket(false,false);
	ASSERT_EQ(testPacket.getVersion(),2)<<"Starting";
	testPacket.setVersion(1);
	ASSERT_EQ(testPacket.getVersion(),1) << "Changed";
	;
}

TEST(RtpTest, paddingTest)
{
	Rtp testPacket(false, false);
	ASSERT_FALSE(testPacket.getPadding()) << "Initial";
	testPacket.setPadding(true);
	ASSERT_TRUE(testPacket.getPadding()) << "Changed";
}

TEST(RtpTest, extensionTest)
{
	Rtp testPacket(false,false);
	ASSERT_FALSE(testPacket.getExtension()) << "Initial";
	testPacket.setExtension(true);
	ASSERT_TRUE(testPacket.getVersion()) << "Changed";
}

TEST(RtpTest, markerTest)
{
	Rtp testPacket(false, false);
	ASSERT_FALSE(testPacket.getMarker()) << "Initial";
	testPacket.setMarker(true);
	ASSERT_TRUE(testPacket.getMarker()) << "Changed";
}


TEST(RtpTest, ccTest)
{
	Rtp testPacket(false, false);
	ASSERT_EQ(testPacket.getCSRCcount(), 0);
	testPacket.setCSRCcount(10);
	ASSERT_EQ(testPacket.getCSRCcount(), 10);
	testPacket.setCSRCcount(testPacket.getCSRCcount() + 1);
	ASSERT_EQ(testPacket.getCSRCcount(), 11);
}

TEST(RtpTest, payloadTypeTest)
{
	Rtp testPacket(false, false);
	ASSERT_EQ(testPacket.getPayloadType(),testPacket.PCMU);
	testPacket.setPayloadType(testPacket.PCMA);
	ASSERT_EQ(testPacket.getPayloadType(), testPacket.PCMA);
}

TEST(RtpTest, SeqNum)
{
	Rtp testPacket(false, false);
	ASSERT_EQ(testPacket.getSeqNum(), 0);
	testPacket.setSeqNum(123);
	ASSERT_EQ(testPacket.getSeqNum(), 123);
}

TEST(RtpTest, timeStamp)
{
	Rtp testPacket(false, false);
	ASSERT_EQ(testPacket.getTimestamp(), 0);
	testPacket.setTimestamp(123);
	ASSERT_EQ(testPacket.getTimestamp(), 123);

}

TEST(RtpTest, ssrc) {
	Rtp testPacket(false, false);
	ASSERT_EQ(testPacket.getSSRC(), 0);
	testPacket.setSSRC(123456789);
	ASSERT_EQ(testPacket.getSSRC(), 123456789);

}

TEST(RtpTest, CSRC) 
{
	Rtp testPacket(false, false);
	testPacket.addCSRC(123);
	std::vector<uint32_t> csrc = testPacket.getCSRC();
	ASSERT_EQ(csrc[0], 123);

}


//НЕ проходит
TEST(RtpTest, packetFormation) {
	Rtp firstPacket(false, false);
	firstPacket.setMarker(true);
	firstPacket.addCSRC(1);
	firstPacket.addCSRC(2);
	firstPacket.setSeqNum(42);
	firstPacket.setPayloadType(firstPacket.PCMU);
	firstPacket.setTimestamp(0);
	firstPacket.setSSRC(1);
	uint8_t data = 1;
	firstPacket.setPayload(&data);
	firstPacket.setSizeofPayload(1);
	
	//Вот эти строчки снизу


	//std::shared_ptr<uint8_t> start = firstPacket.createRtpPacket();
	uint8_t* start = firstPacket.createRtpPacket().get();
	ASSERT_EQ(*start, 130) << "vvpxcc";
	/*start++;
	ASSERT_EQ(*start, 128) <<"mpl";
	start++;
	ASSERT_EQ(*start, 42) << "seqnum1";
	start++;
	ASSERT_EQ(*start, 0) << "seqnum2";
	start++;
	ASSERT_EQ(*start, 0) << "ts1";
	start++;
	ASSERT_EQ(*start, 0) << "ts2";
	start++;
	ASSERT_EQ(*start, 0) << "ts3";
	start++;
	ASSERT_EQ(*start, 0) << "ts4";
	start++;
	ASSERT_EQ(*start, 1) << "ssrc1";
	start++;
	ASSERT_EQ(*start, 0) << "ssrc2";
	start++;
	ASSERT_EQ(*start, 0) << "ssrc3";
	start++;
	ASSERT_EQ(*start, 0) << "ssrc4";
	start++;
	ASSERT_EQ(*start, 1) << "csrc11";
	start++;
	ASSERT_EQ(*start, 0) << "csrc12";
	start++;
	ASSERT_EQ(*start, 0) << "csrc13";
	start++;
	ASSERT_EQ(*start, 0) << "csrc14";
	start++;
	ASSERT_EQ(*start, 2) << "csrc21";
	start++;
	ASSERT_EQ(*start, 0) << "csrc22";
	start++;
	ASSERT_EQ(*start, 0) << "csrc23";
	start++;
	ASSERT_EQ(*start, 0) << "csrc24";
	start++;
	ASSERT_EQ(*start, 1); //fails!!!!=/
	*/
	
	
		
	
}






int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar(); 
}