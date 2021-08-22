#!/usr/bin/env python3

from sys import argv
import re

def byte_convert(value):
  return value
  if value > 127: return value - 256
  else: return value

def convert(message):
  new = ''
  alph = ((0xA0, 'БГЁЖЗИЙЛПУФЧШЪЫЭЮЯбвгёжзийклмнптчшъыьэюя'), (0xE0, 'ДЦЩдфцщ'))
  trans = ('АВЕКМНОРСТХЬаеорсух', 'ABEKMHOPCTXbaeopcyx')
  state = 0
  length = 0
  for char in message:
    if state == 0:
      if char == '\\':
        state = 1
      elif char == "'":
        new += "'\\'',"
      elif char in alph[0][1]:
        new += str(byte_convert(alph[0][0] + alph[0][1].index(char))) + ', '
        length += 1
      elif char in alph[1][1]:
        new += str(byte_convert(alph[1][0] + alph[1][1].index(char))) + ', '
        length += 1
      elif char in trans[0]:
        new += "'" + trans[1][trans[0].index(char)] + "', "
        length += 1
      else:
        new += "'" + char + "',"
        length += 1
    elif state == 1:
      if char == '\\':
        state = 0
        new += '\\'
        length += 1
      elif char == '(':
        state = 2
      else:
        state = 3
        new += '0x' + char
    elif state == 2:
      if char == ')':
        state = 0
        new += ', '
        length += 1
      else:
        new += char
    else:
      state = 0
      new += char + ', '
      length += 1

  return (length, new)


h_output = open(argv[2], 'w')
c_output = open(argv[3], 'w')

rx = re.compile(r'([/\\\- ])')
filename = rx.sub('_', argv[2].split('\\')[-1].split('.')[-2].upper())

h_output.write('''
#ifndef ''' + filename + '''_H_INCLUDED
#define ''' + filename + '''_H_INCLUDED

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
''')

c_output.write('''
#include "messages.h"

#if BT_DEBUG == BUILD_TYPE

''')

for line in open(argv[1], 'r'):
  key_value = line.strip("\n").split('=')
  if len(key_value) == 2:
    length, value = convert(key_value[1].strip(' '))
    h_output.write('extern const string m_{name};\n'.format(name=key_value[0].strip(' ')))
    c_output.write('const string m_{name} PROGMEM = {{ {length}, {{ {data} }} }};\n'.format(
			name=key_value[0].strip(' '),
			length=str(length),
			data=value,
		))

h_output.write('\n//#endif');
h_output.write('\n#endif // ' + filename + '_H_INCLUDED\n');
c_output.write('\n#endif // BUILD_TYPE\n')
h_output.close()
c_output.close()
