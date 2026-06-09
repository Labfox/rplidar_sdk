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

#include "hal/abs_rxtx.h"

namespace rp {
namespace arch {
namespace net {

class raw_serial : public rp::hal::serial_rxtx {
 public:
  enum {
    SERIAL_RX_BUFFER_SIZE = 1024,
    SERIAL_TX_BUFFER_SIZE = 1024,
  };

  raw_serial();
  virtual ~raw_serial();
  virtual bool bind(const char* portname, uint32_t baudrate,
                    uint32_t flags = 0);
  virtual bool open();
  virtual void close();
  virtual void flush(_u32 flags);

  virtual int waitfordata(size_t data_count, _u32 timeout = -1,
                          size_t* returned_size = NULL);

  virtual int senddata(const unsigned char* data, size_t size);
  virtual int recvdata(unsigned char* data, size_t size);

  virtual int waitforsent(_u32 timeout = -1, size_t* returned_size = NULL);
  virtual int waitforrecv(_u32 timeout = -1, size_t* returned_size = NULL);

  virtual size_t rxqueue_count();

  virtual void setDTR();
  virtual void clearDTR();

  virtual void cancelOperation();

 protected:
  void _init();

  char _portName[200];
  uint32_t _baudrate;
  uint32_t _flags;

  int _uart_num;
  bool _is_opened;

  size_t required_tx_cnt;
  size_t required_rx_cnt;

  bool _operation_aborted;
};

}  // namespace net
}  // namespace arch
}  // namespace rp
