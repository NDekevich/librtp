#include "codecInterface.h"
#include "unordered_map"
#include <include/opus.h>
#include <boost/any.hpp>

class opusCodec :public codecInterface
{
	opusCodec(std::unordered_map<std::string, int> sets) {
		encoder = opus_encoder_create(sets["fs"], sets["channels"], sets["application"], error_encoder);
		decoder = opus_decoder_create(sets["fs"] , sets ["channels"], error_decoder);
	}
	~opusCodec() {
		opus_encoder_destroy(encoder);
		opus_decoder_destroy(decoder);
	}

	int createEncoder(std::unordered_map<std::string, int> sets) {
		encoder = opus_encoder_create(sets["fs"], sets["channels"], sets["application"], error_encoder);
		return *error_encoder;
	}

	int createDecoder(std::unordered_map<std::string, int> sets) {
		opus_decoder_destroy(decoder);
		decoder = opus_decoder_create(sets["fs"], sets["channels"], error_decoder);
		return *error_decoder;
	}
	
	int destroyEncoder() {
		opus_encoder_destroy(encoder);
	}
	
	int destroyDecoder() {
		opus_decoder_destroy(decoder);
	}
	
	virtual int codecFunction(std::vector<boost::any> arguments) {
		if (arguments.size() < 1) {
			return -1;
		}
///////////////HERE!!
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