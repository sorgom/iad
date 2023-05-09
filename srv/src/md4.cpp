//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "md4.hpp"
#include "md4_macros.hpp"

#include <cstring>


namespace md4
{

//!	4 byte unsigned.
	typedef unsigned t_u4;

	struct MD4_CTX {
	//	state (ABCD)
		t_u4 state[4];
	//	number of bits, modulo 2^64 (lsb first)
		t_u4 count[2];
	//	input buffer
		unsigned char buffer[64];
	};

	unsigned char PADDING[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

//!	MD4 initialization.
//!	Begins an MD4 operation, writing a new context.
	void MD4Init(MD4_CTX* context)
	{
		context->count[0] = context->count[1] = 0;
	//	Load magic initialization constants.
		context->state[0] = 0x67452301;
		context->state[1] = 0xefcdab89;
		context->state[2] = 0x98badcfe;
		context->state[3] = 0x10325476;
	}

//!	MD4Encodes input t_u4 into output unsigned char.
//!	Assumes len is a multiple of 4.
	void MD4Encode(
		unsigned char *output,
		t_u4 *input,
		unsigned len
	)
	{
		unsigned i, j;
		for (i = 0, j = 0; j < len; i++, j += 4) {
			output[j]		= static_cast<unsigned char>((input[i] & 0xff));
			output[j + 1]	= static_cast<unsigned char>(((input[i] >> 8) & 0xff));
			output[j + 2]	= static_cast<unsigned char>(((input[i] >> 16) & 0xff));
			output[j + 3]	= static_cast<unsigned char>(((input[i] >> 24) & 0xff));
		}
	}

//!	MD4Decodes input unsigned char into output t_u4.
//!	Assumes len is a multiple of 4.

	void MD4Decode (
		t_u4 *output,
		const unsigned char *input,
		unsigned len
	)
	{
		unsigned i, j;

		for (i = 0, j = 0; j < len; i++, j += 4)
			output[i] =
				( static_cast<t_u4>(input[j]	)		) |
				((static_cast<t_u4>(input[j + 1])) << 8	) |
				((static_cast<t_u4>(input[j + 2])) << 16) |
				((static_cast<t_u4>(input[j + 3])) << 24);
	}



//	MD4 basic transformation. Transforms state based on block.
	void MD4Transform (
		t_u4 state[4],
		const unsigned char block[64]
	)
	{
		t_u4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

		MD4Decode(x, block, 64);

	//	Round 1
		FF (a, b, c, d, x[ 0], S11)
		FF (d, a, b, c, x[ 1], S12)
		FF (c, d, a, b, x[ 2], S13)
		FF (b, c, d, a, x[ 3], S14)
		FF (a, b, c, d, x[ 4], S11)
		FF (d, a, b, c, x[ 5], S12)
		FF (c, d, a, b, x[ 6], S13)
		FF (b, c, d, a, x[ 7], S14)
		FF (a, b, c, d, x[ 8], S11)
		FF (d, a, b, c, x[ 9], S12)
		FF (c, d, a, b, x[10], S13)
		FF (b, c, d, a, x[11], S14)
		FF (a, b, c, d, x[12], S11)
		FF (d, a, b, c, x[13], S12)
		FF (c, d, a, b, x[14], S13)
		FF (b, c, d, a, x[15], S14)

	//	Round 2
		GG (a, b, c, d, x[ 0], S21)
		GG (d, a, b, c, x[ 4], S22)
		GG (c, d, a, b, x[ 8], S23)
		GG (b, c, d, a, x[12], S24)
		GG (a, b, c, d, x[ 1], S21)
		GG (d, a, b, c, x[ 5], S22)
		GG (c, d, a, b, x[ 9], S23)
		GG (b, c, d, a, x[13], S24)
		GG (a, b, c, d, x[ 2], S21)
		GG (d, a, b, c, x[ 6], S22)
		GG (c, d, a, b, x[10], S23)
		GG (b, c, d, a, x[14], S24)
		GG (a, b, c, d, x[ 3], S21)
		GG (d, a, b, c, x[ 7], S22)
		GG (c, d, a, b, x[11], S23)
		GG (b, c, d, a, x[15], S24)

	//	Round 3
		HH (a, b, c, d, x[ 0], S31)
		HH (d, a, b, c, x[ 8], S32)
		HH (c, d, a, b, x[ 4], S33)
		HH (b, c, d, a, x[12], S34)
		HH (a, b, c, d, x[ 2], S31)
		HH (d, a, b, c, x[10], S32)
		HH (c, d, a, b, x[ 6], S33)
		HH (b, c, d, a, x[14], S34)
		HH (a, b, c, d, x[ 1], S31)
		HH (d, a, b, c, x[ 9], S32)
		HH (c, d, a, b, x[ 5], S33)
		HH (b, c, d, a, x[13], S34)
		HH (a, b, c, d, x[ 3], S31)
		HH (d, a, b, c, x[11], S32)
		HH (c, d, a, b, x[ 7], S33)
		HH (b, c, d, a, x[15], S34)

		state[0] += a;
		state[1] += b;
		state[2] += c;
		state[3] += d;

	//	Zeroize sensitive information.
		memset(reinterpret_cast<unsigned char*>(x), 0, sizeof(x));
	}


//!	MD4 block update operation.
//!	Continues an MD4 message-digest operation,
//!	processing another message block and updating the context.

	void MD4Update (
		MD4_CTX *context,                                        // context
		const unsigned char *input,                                // input block
		unsigned inputLen                     // length of input block
	)
	{
		unsigned i, index, partLen;

	//	Compute number of bytes mod 64
		index = static_cast<unsigned>((context->count[0] >> 3) & 0x3F);

	//	Update number of bits
		if (
			(context->count[0] += (static_cast<t_u4>(inputLen) << 3))
			< (static_cast<t_u4>(inputLen) << 3)
		)	context->count[1]++;

		context->count[1] += (static_cast<t_u4>(inputLen) >> 29);

		partLen = 64 - index;

	//	Transform as many times as possible.
		if (inputLen >= partLen) {
			memcpy(
				reinterpret_cast<unsigned char*>(&context->buffer[index]),
				reinterpret_cast<const unsigned char*>(input),
				partLen
			);

			MD4Transform (context->state, context->buffer);

			for (i = partLen; i + 63 < inputLen; i += 64) MD4Transform(context->state, &input[i]);

			index = 0;
		}
		else i = 0;

	//	Buffer remaining input
		memcpy(
			reinterpret_cast<unsigned char*>(&context->buffer[index]),
			reinterpret_cast<const unsigned char*>(&input[i]),
			inputLen - i
		);
	}

//!	MD4 finalization.
//!	Ends an MD4 message-digest operation, writing the the message digest and zeroizing the context.
	void MD4Final (
		unsigned char digest[16],                         // message digest
		MD4_CTX *context                                        // context
	)
	{
		unsigned char bits[8];
		unsigned index, padLen;

	//	Save number of bits
		MD4Encode (bits, context->count, 8);

	//	Pad out to 56 mod 64.

		index = static_cast<unsigned>((context->count[0] >> 3) & 0x3f);
		padLen = (index < 56) ? (56 - index) : (120 - index);
		MD4Update (context, PADDING, padLen);

	//	Append length (before padding)
		MD4Update(context, bits, 8);
	//	Store state in digest
		MD4Encode(digest, context->state, 16);

	//	Zeroize sensitive information.
		memset(reinterpret_cast<unsigned char*>(context), 0, sizeof (*context));
	}


	void encode(unsigned char out[16], const unsigned char* in, unsigned len)
	{
		if (!len) return;
		MD4_CTX context;
		MD4Init(&context);
		MD4Update(&context, in, len);
		MD4Final(out, &context);
	}

} // namespace md4
