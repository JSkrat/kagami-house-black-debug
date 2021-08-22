/*
 * units_structure.h
 *
 * Created: 09.02.2020 16:35:36
 *  Author: Mintytail
 */


#ifndef UNITS_STRUCTURE_H_
#define UNITS_STRUCTURE_H_
#include <stdbool.h>
#include "RF functions.h"
#ifdef __cplusplus
    #include <unordered_map>
#endif

#define CHANNELS_PER_UNIT 48
#define CHANNELS_PER_PERMISSION 16

extern bool switches[5];
extern bool relays[2];
extern uint16_t analog_output;

#define UNITS_LENGTH 7
extern const sUnit units[UNITS_LENGTH];

#ifdef __cplusplus
    extern const std::unordered_map<int, sChannel> testChannels;
#endif

#endif /* UNITS_STRUCTURE_H_ */
