/*
 *  RPLIDAR SDK
 *
 *  Copyright (c) 2009 - 2014 RoboPeak Team
 *  http://www.robopeak.com
 *  Copyright (c) 2014 - 2018 Shanghai Slamtec Co., Ltd.
 *  http://www.slamtec.com
 *
 */

#pragma once

// libc dep
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// libc++ dep
#include <iostream>
#include <string>

// esp32 specific
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "timer.h"
