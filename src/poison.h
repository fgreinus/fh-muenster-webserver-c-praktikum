/**
 * @file poison.h
 * @author Leaf Security Research
 * @author PSE Gruppe 10
 * @copyright 2013 - Leaf Security Research http://leafsr.com
 * @date SoSe 2017
 * @brief Basic definitions used in the whole codebase
 * A C header file for poisoning unsafe C/C++ functions. This is far from complete, you will need to
 * add your own in-house deprecated and insecure APIs for it to be very effective
 * (edited by PSE Gruppe 10)
 */

#pragma once

#pragma GCC poison strcpy strncpy
#pragma GCC poison scanf sscanf vscanf fwscanf swscanf wscanf
#pragma GCC poison gets puts
#pragma GCC poison sprintf vsprintf vfprintf
#pragma GCC poison strcat strncat
