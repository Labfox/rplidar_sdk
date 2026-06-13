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

namespace rp{ namespace hal{

Thread Thread::create(thread_proc_t proc, void * data)
{
    Thread newborn(proc, data);

    assert( sizeof(newborn._handle) >= sizeof(pthread_t));

    pthread_create((pthread_t *)&newborn._handle, NULL, (void * (*)(void *))(void *)proc, data);

    return newborn;
}

u_result Thread::terminate()
{
    if (!this->_handle) return RESULT_OK;

    return RESULT_OPERATION_NOT_SUPPORT;
}

u_result Thread::SetSelfPriority( priority_val_t p)
{
    return RESULT_OK;
}

Thread::priority_val_t Thread::getPriority()
{
    return PRIORITY_NORMAL;
}

u_result Thread::join(unsigned long timeout)
{
    if (!this->_handle) return RESULT_OK;

    pthread_join((pthread_t)(this->_handle), NULL);
    this->_handle = 0;
    return RESULT_OK;
}

}}
