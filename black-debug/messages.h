
#ifndef BLACK_DEBUG_MESSAGES_H_INCLUDED
#define BLACK_DEBUG_MESSAGES_H_INCLUDED

#ifndef UNIT_TESTING
    #include <avr/pgmspace.h>
#else
	#include "../usb2nrf_tests/pgmspace.h"
#endif
#include <stdint.h>

typedef struct {
	uint8_t length;
	uint8_t data[];
} string;

//#define cTerm    16
#define cCheck   94
#define cCelsius 96

//#ifndef UNIT_TESTING
extern const string m_hello;
extern const string m_sendQueueSize;
extern const string m_usNotInit;
extern const string m_usErrorProto;
extern const string m_usErrorCmd;
extern const string m_usErrorLength;
extern const string m_usErrorData	;
extern const string m_usProtoVer;
extern const string m_usCommand;
extern const string m_usPDataLength;
extern const string m_usPData;
extern const string m_usEnd;
extern const string m_uPacketLen;
extern const string m_rEN_RXADDR;
extern const string m_mDebug;
extern const string m_mSlave;
extern const string m_mMaster;

//#endif
#endif // BLACK_DEBUG_MESSAGES_H_INCLUDED
