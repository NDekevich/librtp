#include <stdint.h>


class codecRtpAsio 
{
	public :
		

		virtual int getCodecCode() = 0;

		virtual uint8_t code(uint8_t rawData) = 0;
		virtual uint8_t decode(uint8_t codedData) = 0;



	private :
		const int codecCode;


};