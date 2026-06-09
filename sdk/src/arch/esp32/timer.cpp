/*
 *  RPLIDAR SDK
 *
 *  Copyright (c) 2009 - 2014 RoboPeak Team
 *  http://www.robopeak.com
 *  Copyright (c) 2014 - 2018 Shanghai Slamtec Co., Ltd.
 *  http://www.slamtec.com
 *
 */

#include "arch/esp32/arch_esp32.h"

namespace rp {
namespace arch {
_u64 rp_getus() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec * 1000000LL + t.tv_nsec / 1000;
}
_u64 rp_getms() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}
}  // namespace arch
}  // namespace rp
