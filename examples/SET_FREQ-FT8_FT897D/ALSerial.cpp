#include "ALSerial.h"

#define FLAG_HARDWARE_SERIAL 0
#define FLAG_SOFTWARE_SERIAL 1
#define FLAG_USBCON_SERIAL   2

ALSerial::ALSerial(HardwareSerial* _thisHardwareSerial) {
  thisHardwareSerial = _thisHardwareSerial;
  flag = FLAG_HARDWARE_SERIAL;
}

#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
ALSerial::ALSerial(CustomSoftwareSerial* _thisSoftwareSerial) {
  thisSoftwareSerial = _thisSoftwareSerial;
  flag = FLAG_SOFTWARE_SERIAL;
}

ALSerial::ALSerial(int rx, int tx) {
  thisSoftwareSerial = new CustomSoftwareSerial(rx, tx);
  flag = FLAG_SOFTWARE_SERIAL;
}
#endif

#ifdef USBCON
ALSerial::ALSerial(Serial_* _thisSerial_) {
  thisSerial_ = _thisSerial_;
  flag = FLAG_USBCON_SERIAL;
}
#endif

void ALSerial::begin(long speed, uint16_t parity) {
  switch (flag) {
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  case FLAG_SOFTWARE_SERIAL:
    thisSoftwareSerial->begin(speed, parity);
    break;
#endif
#ifdef USBCON
  case FLAG_USBCON_SERIAL:
    thisSerial_->begin(speed, parity);
    break;
#endif
  default: // FLAG_HARDWARE_SERIAL
    thisHardwareSerial->begin(speed,parity);
  }
}

void ALSerial::end() {
  switch (flag) {
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  case FLAG_SOFTWARE_SERIAL:
    thisSoftwareSerial->end();
    break;
#endif
#ifdef USBCON
  case FLAG_USBCON_SERIAL:
    thisSerial_->end();
    break;
#endif
  default: // FLAG_HARDWARE_SERIAL
    thisHardwareSerial->end();
  }
}

bool ALSerial::isHardwareSerial() {
  return flag == FLAG_HARDWARE_SERIAL;
}

bool ALSerial::isSoftwareSerial() {
  return flag == FLAG_SOFTWARE_SERIAL;
}

bool ALSerial::isSerial_() {
  return flag == FLAG_USBCON_SERIAL;
}

int ALSerial::peek() {
  switch (flag) {
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  case FLAG_SOFTWARE_SERIAL:
    return thisSoftwareSerial->peek();
#endif
#ifdef USBCON
  case FLAG_USBCON_SERIAL:
    return thisSerial_->peek();
#endif
  default: // FLAG_HARDWARE_SERIAL
    return thisHardwareSerial->peek();
  }
}

size_t ALSerial::write(uint8_t byte) {
  switch (flag) {
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  case FLAG_SOFTWARE_SERIAL:
    return thisSoftwareSerial->write(byte);
#endif
#ifdef USBCON
  case FLAG_USBCON_SERIAL:
    return thisSerial_->write(byte);
#endif
  default: // FLAG_HARDWARE_SERIAL
    return thisHardwareSerial->write(byte);
  }
}

int ALSerial::read() {
  switch (flag) {
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  case FLAG_SOFTWARE_SERIAL:
    return thisSoftwareSerial->read();
#endif
#ifdef USBCON
  case FLAG_USBCON_SERIAL:
    return thisSerial_->read();
#endif
  default: // FLAG_HARDWARE_SERIAL
    return thisHardwareSerial->read();
  }
}

int ALSerial::available() {
  switch (flag) {
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  case FLAG_SOFTWARE_SERIAL:
    return thisSoftwareSerial->available();
#endif
#ifdef USBCON
  case FLAG_USBCON_SERIAL:
    return thisSerial_->available();
#endif
  default: // FLAG_HARDWARE_SERIAL
    return thisHardwareSerial->available();
  }
}

void ALSerial::flush() {
  switch (flag) {
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  case FLAG_SOFTWARE_SERIAL:
    thisSoftwareSerial->flush();
    break;
#endif
#ifdef USBCON
  case FLAG_USBCON_SERIAL:
    thisSerial_->flush();
    break;
#endif
  default: // FLAG_HARDWARE_SERIAL
    thisHardwareSerial->flush();
  }
}
