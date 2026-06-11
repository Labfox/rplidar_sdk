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

#include "hal/types.h"

namespace rp{ namespace arch{
    _u64 rp_getus();
    _u64 rp_getms();
}}

static inline void delay(_word_size_t ms){
    while (ms >= 1000) {
        usleep(1000*1000);
        ms -= 1000;
    };
    if (ms > 0) usleep(ms*1000);
}

#define getms() rp::arch::rp_getms()
#define getus() rp::arch::rp_getus()
