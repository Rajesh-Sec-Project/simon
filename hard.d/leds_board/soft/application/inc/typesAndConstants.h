/**
    * @file typesAndConstants.c
    * @brief Declares some custom types and constants for this proejct
    *
    * @author Romain TAPREST
    * @date 26 nov 2015
    */

#ifndef _TYPES_AND_CONSTANTS_H__
#define _TYPES_AND_CONSTANTS_H__

#include <stdint.h>

typedef enum {
    OFF = 0,
    ON = 1,
} LEDState;

// BOOLEAN
typedef uint8_t bool;
#define true 1
#define false 0
//

#endif // _TYPES_AND_CONSTANTS_H__
