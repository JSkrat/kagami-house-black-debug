#ifndef MY_STDLIB_H_INCLUDED
#define MY_STDLIB_H_INCLUDED
#include <inttypes.h>
#include "messages.h"

string* Str(uint16_t value);
string* SStr(int16_t value);
string* StrLen(uint8_t value, uint8_t len);
string* Hex(uint8_t value);
string* Hex16(uint16_t);

#endif // MY_STDLIB_H_INCLUDED
