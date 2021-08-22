/*
 * units_structure.c
 *
 * Created: 09.02.2020 16:35:19
 *  Author: Mintytail
 */
#include "units_structure.h"
#include "../usb2nrf/RF functions.h"
#include <unordered_map>
#include "pgmspace.h"

bool switches[5] = {
    true,
    false,
    true,
    false,
    true
};
bool relays[2] = {
    false,
    true
};
uint16_t analog_output = 12345;

const sChannel switchChannels[] PROGMEM = {
    {
        dataType: eCDTBit,
        value: {&switches[0]}
    },
    {
        dataType: eCDTBit,
        value: {&switches[1]}
    },
    {
        dataType: eCDTBit,
        value: {&switches[2]}
    },
    {
        dataType: eCDTBit,
        value: {&switches[3]}
    },
    {
        dataType: eCDTBit,
        value: {&switches[4]}
    },
};

const sChannel outputChannels[] PROGMEM = {
    {
        dataType: eCDTBit,
        value: {&relays[0]}
    },
    {
        dataType: eCDTBit,
        value: {&relays[1]}
    },
    {
        dataType: eCDTUnsigned,
        value: {tUInt: (uint32_t*) &analog_output}
    },//*/
};

const sUnit units[UNITS_LENGTH] PROGMEM = {
    {
        type: eUTSystem,
        channelsROLength: 0,
    },
    {
        type: eUTSSwitch,
        channelsROLength: 1,
        channelsRO: &switchChannels[0]
    },
    {
        type: eUTSSwitch,
        channelsROLength: 1,
        channelsRO: &switchChannels[1]
    },
    {
        type: eUTSCustom,
        channelsROLength: 3,
        channelsRO: &switchChannels[2]
    },
    {
        type: eUTARelay,
        channelsROLength: 0,
        channelsRO: nullptr,
        channelsWOLength: 0,
        channelsWO: nullptr,
        channelsRWLength: 1,
        channelsRW: &outputChannels[0]
    },
    {
        type: eUTARelay,
        channelsROLength: 0,
        channelsRO: nullptr,
        channelsWOLength: 1,
        channelsWO: &outputChannels[1],
        channelsRWLength: 0,
        channelsRW: nullptr
    },//*/
    {
        type: eUTAAnalog,
        channelsROLength: 0,
        channelsRO: nullptr,
        channelsWOLength: 0,
        channelsWO: nullptr,
        channelsRWLength: 1,
        channelsRW: &outputChannels[2]
    }//*/
};

const std::unordered_map<int, sChannel> testChannels = {
//    {0*CHANNELS_PER_UNIT, {eCDTBit, {&switches[0]}}},
    {1*CHANNELS_PER_UNIT, {eCDTBit, {&switches[0]}}},
    {2*CHANNELS_PER_UNIT, {eCDTBit, {&switches[1]}}},
    {3*CHANNELS_PER_UNIT+0, {eCDTBit, {&switches[2]}}},
    {3*CHANNELS_PER_UNIT+1, {eCDTBit, {&switches[3]}}},
    {3*CHANNELS_PER_UNIT+2, {eCDTBit, {&switches[4]}}},
    {4*CHANNELS_PER_UNIT+CHANNELS_PER_PERMISSION*2+0, {eCDTBit, {&relays[0]}}},
    {5*CHANNELS_PER_UNIT+CHANNELS_PER_PERMISSION*1+0, {
         dataType: eCDTBit,
         value: {tBit: &relays[1]}
     }},
    {6*CHANNELS_PER_UNIT+CHANNELS_PER_PERMISSION*2+0, {
         dataType: eCDTUnsigned,
         value: {reinterpret_cast<bool*>(&analog_output)}
     }},//*/
};
