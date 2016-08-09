#pragma once
#include <stdint.h>
#include <unordered_map>
#include <boost/any.hpp>



class codecInterface
{
public:
	codecInterface();
	codecInterface(std::unordered_map<std::string, int> sets);
	virtual ~codecInterface();

	virtual int createEncoder();
	virtual int createDecoder();

	virtual int destroyEncoder();
	virtual int destroyDecoder();

	virtual void voidFunc() = 0;

	virtual void initialize();
	virtual int getCodecType();

	virtual uint8_t code(uint8_t rawData);
	virtual uint8_t decode(uint8_t codedData);

//	virtual int encoderFunction(std::list<boost::any> arguments);
//	virtual int decoderFunction(std::vector<boost::any> arguments);

	std::unordered_map < std::string, int > settings;
	const int codecCode;


};