/*
 * StringTools.cpp
 *
 *  Created on: 2014年5月15日
 *      Author: tate
 */

#include "StringTools.h"
#include <string.h>

unsigned computeUnicodeLengthFromUTF8(const char * utf8Str,
		unsigned utf8StrLen) {
	if (utf8Str == NULL || utf8StrLen == 0) {
		return 0;
	}
	unsigned length = strlen(utf8Str);
	length = length > utf8StrLen ? utf8StrLen : length;

	unsigned unicodeLength = 0;

	int i = 0;
	while (i < length) {
		if (utf8Str[i] >= 0x00 && utf8Str[i] <= 0x7f) {
			i++;
			unicodeLength += 1;
		} else if ((utf8Str[i] & (0xe0)) == 0xc0) {
			i += 2;
			unicodeLength += 1;
		} else if ((utf8Str[i] & (0xf0)) == 0xe0) {
			i += 3;
			unicodeLength += 1;
		}
	}

	return unicodeLength;
}
short UTF8ToUnicode(UNICODE_CHAR * out, unsigned outLength, const char * in,
		unsigned inLength) {

	if (out == NULL || outLength == 0 || in == NULL || inLength == 0) {
		return 0;
	}

	unsigned length = strlen(in);
	length = length > inLength ? inLength : length;

	int i = 0;
	int outi = 0;
	while (i < length && outi < outLength) {
		if (in[i] >= 0x00 && in[i] <= 0x7f) {
			out[outi] = in[i];
			outi++;
			i++;
		} else if ((in[i] & (0xe0)) == 0xc0) {
			out[outi] = ((in[i] & 0x1f) << 6) + (in[i + 1] & 0x3f);
			outi++;
			i += 2;
		} else if ((in[i] & (0xf0)) == 0xe0) {
			out[outi] = ((in[i] & 0xf) << 12) + ((in[i + 1] & 0x3f) << 6)
					+ (in[i + 2] & 0x3f);
			outi++;
			i += 3;
		}
	}

	return 1;
}
