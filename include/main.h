/**
 * \file main.h
 *
 * Contains common definitions and includes for PROS projects with EZ Template.
 **/

#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PROS_USE_SIMPLE_NAMES
#define PROS_USE_LITERALS

#include "api.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "robot-config.h"
#endif

#endif  // _MAIN_H_
