#ifndef AL_SERIAL_H
#define AL_SERIAL_H

#include <Arduino.h>

#if defined(__arm__) || defined(ESP32) // arm boards and esp32 do not support CustomSoftwareSerial
#define AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
#endif

#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
#include <CustomSoftwareSerial.h>
#endif

#ifdef USBCON
#ifdef __arm__
#include <USB/USBAPI.h>
#else
#include <USBAPI.h>
#endif
#endif

class ALSerial : public Stream {
  public:
  ALSerial(HardwareSerial* _thisHardwareSerial);
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  ALSerial(CustomSoftwareSerial* _thisSoftwareSerial);
  ALSerial(int rx, int tx);
#endif
#ifdef USBCON
  ALSerial(Serial_* _thisSerial_);
#endif

  HardwareSerial* thisHardwareSerial;
#ifndef AL_SERIAL_NOT_SUPPORT_SOFTWARE_SERIAL
  CustomSoftwareSerial* thisSoftwareSerial;
#endif
#ifdef USBCON
  Serial_* thisSerial_;
#endif

  void begin(long speed, uint16_t parity);
  void end();
  bool isHardwareSerial();
  bool isSoftwareSerial();
  bool isSerial_();

  virtual int peek();
  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  virtual void flush();

  using Print::write;

  private:
  int flag;
};

#endif
