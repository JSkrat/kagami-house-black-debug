#include "my-stdlib.h"
#include "messages.h"

#ifdef USE_MY_STDLIB

union {
	uint8_t bytes[8];
	string str;
} strBuf;

const char* alph = "0123456789ABCDEF";

string* Str(uint16_t value) {
    uint8_t* pointer = &(strBuf.bytes[0]) + 6; // на 16-бит максимум 5 разрядов, плюс место для знака (SStr)
    if (value == 0) {
        *(--pointer) = '0';
    } else while (value > 0) {
        *(--pointer) = '0' + value % 10;
        value /= 10;
    }
	*(--pointer) = ' ';
	*(pointer) = 6 - (pointer - &(strBuf.bytes[0]));
    return (string*) pointer;
}

string* SStr(int16_t value) {
    uint8_t* pointer;
    if (value < 0) {
        pointer = (uint8_t*) Str(-value);
        *(pointer+1) = '-'; // Str нам оставила один байт перед
    } else {
        pointer = (uint8_t*) Str(value);
    }
    return (string*) pointer;
}

string* StrLen(uint8_t value, uint8_t len) {
    uint8_t* pointer = &(strBuf.bytes[0]) + len + 1;
	strBuf.str.length = len;
    while (len-- > 0) {
        *(--pointer) = '0' + value % 10;
        value /= 10;
    }
    return (string*) pointer;
}

string* Hex(uint8_t value) {
	strBuf.str.length = 2;
    strBuf.str.data[0] = alph[value >> 4];
    strBuf.str.data[1] = alph[value & 0x0F];
    return &(strBuf.str);
}

string* Hex16(uint16_t value) {
	strBuf.str.length = 4;
	strBuf.str.data[0] = alph[value >> 12];
	strBuf.str.data[1] = alph[(value >> 8) & 0x0F];
	strBuf.str.data[2] = alph[(value >> 4) & 0x0F];
	strBuf.str.data[3] = alph[(value) & 0x0F];
    return &(strBuf.str);
}

#endif
