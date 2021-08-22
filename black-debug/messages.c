
#include "messages.h"

#if BT_DEBUG == BUILD_TYPE

const string m_hello PROGMEM = { 16, { 0x20, 'k','a','g','a','m','i',' ','u','s','b','2','n','r','f',0x20,  } };
const string m_sendQueueSize PROGMEM = { 8, { 'S','Q','u','e','u','e',':',0x20,  } };
const string m_usNotInit PROGMEM = { 12, { '	','	','n','/','i',' ',' ',' ',' ',' ',' ',0x20,  } };
const string m_usErrorProto PROGMEM = { 10, { 'e','r','r',' ','p','r','o','t','o',0x20,  } };
const string m_usErrorCmd PROGMEM = { 11, { '	','e','r','r',' ','c','m','d',' ',' ',0x20,  } };
const string m_usErrorLength PROGMEM = { 10, { 'e','r','r',' ','l','e','n',' ',' ',0x20,  } };
const string m_usErrorData	 PROGMEM = { 11, { '	','e','r','r',' ','d','a','t','a',' ',0x20,  } };
const string m_usProtoVer PROGMEM = { 10, { 'v','e','r',' ',' ',' ',' ',' ',' ',0x20,  } };
const string m_usCommand PROGMEM = { 10, { 'c','o','m','m',' ',' ',' ',' ',' ',0x20,  } };
const string m_usPDataLength PROGMEM = { 10, { 'l','e','n',' ',' ',' ',' ',' ',' ',0x20,  } };
const string m_usPData PROGMEM = { 10, { 'd','a','t','a',' ',' ',' ',' ',' ',0x20,  } };
const string m_usEnd PROGMEM = { 13, { '	','	','	','e','n','d',' ',' ',' ',' ',' ',' ',0x20,  } };
const string m_uPacketLen PROGMEM = { 9, { 'p','a','c','k',' ','l','e','n',0x20,  } };
const string m_rEN_RXADDR PROGMEM = { 10, { 'E','N','_','R','X','A','D','D','R',0x20,  } };
const string m_mDebug PROGMEM = { 6, { 'd','e','b','u','g',0x20,  } };
const string m_mSlave PROGMEM = { 6, { 's','l','a','v','e',0x20,  } };
const string m_mMaster PROGMEM = { 6, { 'm','a','s','t','e','r', } };

#endif // BUILD_TYPE
