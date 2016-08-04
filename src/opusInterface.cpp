#include "codecInterface.h"
#include "unordered_map"
#include <include/opus.h>

class opusCodec :public codecInterface
{
	opusCodec(std::unordered_map<std::string, int> sets) {
		encoder = opus_encoder_create(sets["fs"], sets["channels"], sets["application"], error_encoder);
		decoder = opus_decoder_create(sets["fs"] , sets ["channels"], error_decoder);
	}

	OpusEncoder* intialization(std::unordered_map<std::string, int> set){
		settings = set;
		return 0;
	}

	std::vector<uint8_t> code(std::vector<uint8_t>){
	}

	std::vector<uint8_t> decode(std::vector<uint8_t>){
	}


	std::unordered_map<std::string, int> settings;
	OpusEncoder* encoder;
	OpusDecoder* decoder;
	int* error_encoder;
	int* error_decoder;
};