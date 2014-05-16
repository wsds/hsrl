/*
 * StringTools.h
 *
 *  Created on: 2014年5月15日
 *      Author: tate
 */

#ifndef STRINGTOOLS_H_
#define STRINGTOOLS_H_
#include <stddef.h>
#include <stdint.h>
typedef uint16_t UNICODE_CHAR;
extern "C" {
unsigned computeUnicodeLengthFromUTF8(const char * utf8Str,
		unsigned utf8StrLen);
short UTF8ToUnicode(UNICODE_CHAR * out, unsigned outLength, const char * in,
		unsigned inLength);
}
#endif /* STRINGTOOLS_H_ */
