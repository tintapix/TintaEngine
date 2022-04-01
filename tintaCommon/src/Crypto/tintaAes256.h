/*  
*   Byte-oriented AES-256 implementation.
*   All lookup tables replaced with 'on the fly' calculations. 
*
*   Copyright (c) 2007-2009 Ilya O. Levin, http://www.literatecode.com
*   Other contributors: Hal Finney
*
*   Permission to use, copy, modify, and distribute this software for any
*   purpose with or without fee is hereby granted, provided that the above
*   copyright notice and this permission notice appear in all copies.
*
*   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
*   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
*   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
*   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
*   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
*   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
*   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov
tintapix.com
tintapix@gmail.com  */

#ifndef _TINTA_AES_256_H_
#define _TINTA_AES_256_H_


#include <string>
#include <vector>
#include <math.h>
#include <iostream>


namespace  Tinta {

	typedef  unsigned char uchar;
	typedef  std::string basicString;

	inline void PrintArray(basicString text,const unsigned char * data, unsigned datasize){
		std::cout<<text;
		for(unsigned i =0; i < datasize; i++){
			std::cout<<(int)data[i]<<", ";
		}
		std::cout<<"\n";
	}

	inline  void PrintArray(basicString text,const signed char * data, unsigned datasize){
		std::cout<<text;
		for(unsigned i =0; i < datasize; i++){
			std::cout<<(int)data[i]<<", ";
		}
		std::cout<<"\n";
	}

inline	void ToUnsignedChar(signed char * inData, unsigned char * outData, unsigned quantity){

	for(unsigned count = 0; count < quantity; count++) {		
		outData[count] = inData[count] > 0 ? 128 + inData[count] :  (unsigned char)abs(inData[count]);
	}
}

inline 	void ToSignedChar(unsigned char * inData, signed char * outData, unsigned quantity){

	for(unsigned count = 0; count < quantity; count++) {
		outData[count] = inData[count] <= 128 ?  0x0 - (int) inData[count] : inData[count] - 128;
	}
}



class  tintaAes256 {


public:
	const static unsigned mKeySize = 32;
	const static unsigned mDataBitSize = 16;

    struct Aes256Context {
        uchar key[mKeySize];
        uchar enckey[mKeySize];
        uchar deckey[mKeySize];
    } ;
    void Aes256DecryptEcbVoid( );

    tintaAes256();
    virtual ~tintaAes256();

    // returns alligned data size
    unsigned GetAllingedDataSize(unsigned oldSize );
    void 	 Aes256Init(uchar key[mKeySize] );
    void     Aes256Done();
  

	void Aes256DecryptData(unsigned char * inData, unsigned char * outData, unsigned dataSize);
	void Aes256EncryptData(unsigned char * inData, unsigned char * outData, unsigned dataSize);

private:

    Aes256Context mContext;

	void     Aes256EncryptEcb( uchar * inData);
	void     Aes256DecryptEcb( uchar * outData);

    void Aes256Init(tintaAes256::Aes256Context *,const uchar * );
    uchar rj_xtime(uchar x);
    void Aes256SubBytes(uchar *buf);
    void Aes256SubBytes_inv(uchar *buf);
    void Aes256AddRoundKey(uchar *buf, uchar *key);
    void Aes256AddRoundKeyCpy(uchar *buf, uchar *key, uchar *cpk);
    void Aes256ShiftRows(uchar *buf);
    void Aes256ShiftRows_inv(uchar *buf);
    void AesMixColumns(uchar *buf);
    void AesMixColumns_inv(uchar *buf);
    void AesExpandEncKey(uchar *k, uchar *rc);
    void AesExpandDecKey(uchar *k, uchar *rc);
};


}




#endif
