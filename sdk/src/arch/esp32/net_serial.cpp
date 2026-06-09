/*
 *  RPLIDAR SDK
 *
 *  Copyright (c) 2009 - 2014 RoboPeak Team
 *  http://www.robopeak.com
 *  Copyright (c) 2014 - 2018 Shanghai Slamtec Co., Ltd.
 *  http://www.slamtec.com
 *
 */

#include "arch/esp32/net_serial.h"

#include "arch/esp32/arch_esp32.h"

// ESP-IDF UART driver
#include "driver/gpio.h"
#include "driver/uart.h"

namespace rp {
namespace arch {
namespace net {

raw_serial::raw_serial()
    : rp::hal::serial_rxtx(),
      _baudrate(0),
      _flags(0),
      _uart_num(-1),
      _is_opened(false) {
  _init();
}

raw_serial::~raw_serial() { close(); }

bool raw_serial::open() {
  if (_is_opened) close();

  // Map portName to uart_port_t
  // Expecting strings like "0", "1", "2" or "/dev/uart0", etc.
  if (strstr(_portName, "0") || strstr(_portName, "UART_NUM_0"))
    _uart_num = 0;
  else if (strstr(_portName, "1") || strstr(_portName, "UART_NUM_1"))
    _uart_num = 1;
  else if (strstr(_portName, "2") || strstr(_portName, "UART_NUM_2"))
    _uart_num = 2;
  else
    _uart_num = atoi(_portName);

  if (_uart_num < 0 || _uart_num >= UART_NUM_MAX) return false;

  uart_config_t uart_config;
  uart_config.baud_rate = _baudrate;
  uart_config.data_bits = UART_DATA_8_BITS;
  uart_config.parity = UART_PARITY_DISABLE;
  uart_config.stop_bits = UART_STOP_BITS_1;
  uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
  uart_config.rx_flow_ctrl_thresh = 122;
  uart_config.source_clk = UART_SCLK_APB;

  if (uart_param_config((uart_port_t)_uart_num, &uart_config) != ESP_OK)
    return false;

  // Using default pins, users should use uart_set_pin() externally if needed
  if (uart_driver_install((uart_port_t)_uart_num, SERIAL_RX_BUFFER_SIZE * 2, 0,
                          0, NULL, 0) != ESP_OK)
    return false;

  _is_opened = true;
  _operation_aborted = false;
  _is_serial_opened = true;

  return true;
}

bool raw_serial::bind(const char* portname, uint32_t baudrate, uint32_t flags) {
  strncpy(_portName, portname, sizeof(_portName));
  _baudrate = baudrate;
  _flags = flags;
  return true;
}

void raw_serial::close() {
  if (_is_opened) {
    uart_driver_delete((uart_port_t)_uart_num);
  }
  _is_opened = false;
  _is_serial_opened = false;
  _uart_num = -1;
}

int raw_serial::senddata(const unsigned char* data, size_t size) {
  if (!_is_opened) return 0;
  int len = uart_write_bytes((uart_port_t)_uart_num, (const char*)data, size);
  required_tx_cnt = (len < 0) ? 0 : len;
  return required_tx_cnt;
}

int raw_serial::recvdata(unsigned char* data, size_t size) {
  if (!_is_opened) return 0;
  int len = uart_read_bytes((uart_port_t)_uart_num, data, size, 0);
  required_rx_cnt = (len < 0) ? 0 : len;
  return required_rx_cnt;
}

void raw_serial::flush(_u32 flags) {
  if (!_is_opened) return;
  uart_flush_input((uart_port_t)_uart_num);
}

int raw_serial::waitforsent(_u32 timeout, size_t* returned_size) {
  if (returned_size) *returned_size = required_tx_cnt;
  return 0;
}

int raw_serial::waitforrecv(_u32 timeout, size_t* returned_size) {
  if (returned_size) *returned_size = required_rx_cnt;
  return 0;
}

int raw_serial::waitfordata(size_t data_count, _u32 timeout,
                            size_t* returned_size) {
  if (!_is_opened) return ANS_DEV_ERR;

  size_t length = 0;
  if (returned_size == NULL) returned_size = (size_t*)&length;
  *returned_size = 0;

  _u64 start_ms = rp_getms();

  while (true) {
    size_t available = 0;
    uart_get_buffered_data_len((uart_port_t)_uart_num, &available);
    *returned_size = available;

    if (available >= data_count) return ANS_OK;
    if (_operation_aborted) return ANS_TIMEOUT;

    if (timeout != (_u32)-1) {
      if (rp_getms() - start_ms > timeout) return ANS_TIMEOUT;
    }

    // Wait a bit
    usleep(1000);
  }
}

size_t raw_serial::rxqueue_count() {
  if (!_is_opened) return 0;
  size_t available = 0;
  uart_get_buffered_data_len((uart_port_t)_uart_num, &available);
  return available;
}

void raw_serial::setDTR() {
  // DTR is not a standard UART pin on ESP32, often used for motor control on
  // RPLIDAR A1. User needs to handle this via GPIO.
}

void raw_serial::clearDTR() {}

void raw_serial::_init() {
  _portName[0] = 0;
  required_tx_cnt = required_rx_cnt = 0;
  _operation_aborted = false;
  _is_opened = false;
  _uart_num = -1;
}

void raw_serial::cancelOperation() { _operation_aborted = true; }

}  // namespace net
}  // namespace arch
}  // namespace rp

namespace rp {
namespace hal {

serial_rxtx* serial_rxtx::CreateRxTx() {
  return new rp::arch::net::raw_serial();
}

void serial_rxtx::ReleaseRxTx(serial_rxtx* rxtx) { delete rxtx; }

}  // namespace hal
}  // namespace rp
