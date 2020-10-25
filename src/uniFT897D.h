// Заимствовано у DetDimen и переработано UA6EM
#pragma once

#include <Arduino.h>
#include "ALSerial.h"


const bool ON = true;
const bool OFF = false;


#pragma pack(push,1)

struct FT897DCommand {    // буфер команд
  uint8_t Byte0;
  uint8_t Byte1;
  uint8_t Byte2;
  uint8_t Byte3;
  uint8_t Command;
};

const uint8_t COMMAND_SIZE = sizeof(FT897DCommand); // длина буфера комманд


enum class TOperatingMode : uint8_t {  // режимы работы
  LSB = 0x00,
  USB = 0x01,
  CW  = 0x02,
  CWR = 0x03,
  AM  = 0x04,
  WFM = 0x06,
  FM  = 0x08,
  DIG = 0x0A,
  PKT = 0x0C,
  CWN = 0x82,
  CWNR= 0x83,
  AMN = 0x84,
  FMN = 0x88,
  Unknown = 0xFF
};

enum class TRepeaterOffset : uint8_t {
  Down    = 0x09,
  Up      = 0x49,
  Simplex = 0x89
};

enum class TDCS_Mode : uint8_t {
  DCS_ON = 0x0A,
  DCS_DECODER_ON = 0x0B,
  DCS_ENCODER_ON = 0x0C,
  CTCSS_ON = 0x2A,
  CTCSS_DECODER_ON = 0x3A,
  CTCSS_ENCODER_ON = 0x4A,
  OFF = 0x8A  
};

struct TRX_Status {
  uint8_t SMValue : 4;
  bool  unused    : 1;
  bool  Discr     : 1;
  bool  Code      : 1;
  bool  SQL       : 1;
};

struct TTX_Status {
  uint8_t PowerValue  : 4;
  bool    unused      : 1;
  bool  Split         : 1;
  bool  SWR           : 1;
  bool  PTT           : 1;
};

const uint8_t CMD_LOCK_ON = 0x00;   // блокировка вкл/выкл
const uint8_t CMD_LOCK_OFF  = 0x80;

const uint8_t CMD_PTT_ON  = 0x08;   // тангента вкл/выкл
const uint8_t CMD_PTT_OFF = 0x88;

const uint8_t CMD_SET_OPERATING_MODE = 0x07;

const uint8_t CMD_CLAR_ON = 0x05;  // Подстройка частоты (ON | OFF)
const uint8_t CMD_CLAR_OFF  = 0x85;
const uint8_t CMD_CLAR_SETFREQ = 0xF5; // подстройка частоты

const uint8_t CMD_SET_MAIN_FREQ = 0x01; // установка частоты PLL

const uint8_t CMD_SWITCH_VFO = 0x81;  // VFO-A/B

const uint8_t CMD_SPLIT_ON = 0x02;  // Разнос (split) (ON | OFF)
const uint8_t CMD_SPLIT_OFF = 0x82;

const uint8_t CMD_SET_REPEATER_OFFSET = 0x09;
const uint8_t CMD_SET_REPEATER_OFFSET_FREQ = 0xF9;

const uint8_t CMD_SET_DCS_MODE = 0x0A;

const uint8_t CMD_SET_CTCSS_TONE_FREQ = 0x0B;

const uint8_t CMD_SET_DCS_CODE = 0x0C;

const uint8_t CMD_READ_RX_STATUS = 0xE7;  // прочитать статус приема
const uint8_t CMD_READ_TX_STATUS = 0xF7;  // прочитать статус передачи

const uint8_t CMD_READ_LONG_STATUS = 0x03; // прочитать рабочий режим и установленную частоту


class uniFT897D {
protected:
  ALSerial& FPort;
  FT897DCommand   FCommand;


  uniFT897D() = delete;
  uniFT897D(uniFT897D& rvalue) = delete;

  void ClearCmdBuffer(void);   // Очищает(обнуляет) буфер комманд. Для внутреннего использования.

  void SendCommand(const bool ANeedAnswer = false) const; // Посылает 5 байт команд и данных в устройство

  // 
  // преобразует частоту в строку с выравниванием влево, спереди и сзади добивает '0' до 
  // длины ALength
  //
  // AFreq      - частота в виде числа с плав. точкой
  // AIntDigits - максимальная длина целой части числа, 1, 2 или 3
  // ALength    - общая длина всего текстового представления числа
  //
  // например у частоты 114.32145 число целых цифр 3 (114), а общая длина всех цифр - 8
  //
  // внутренний метод, снаружи не виден
  //
  const char* Freq2String(const float AFreq, const uint8_t AIntDigits = 3, const uint8_t ALength = 8);

  void StringToBCD(const char* AFrom, uint8_t* ATo, const uint8_t ALen);

  uint8_t ReadByteFromPort(const uint16_t ATimeoutMS);

  bool   ReadLongStatus(const uint16_t ATimeoutMS);


public:

  // конструктор. Принимает ранее созданный ALSerial
  uniFT897D(ALSerial& ASerialPort); 
    
  // инициализация класса
#ifdef SOFTSERIAL
    void Init(const uint32_t ABaudRate = 4800, const uint16_t ADataParity = CSERIAL_8N2); 
#else
    void Init(const uint32_t ABaudRate = 4800, const uint16_t ADataParity = SERIAL_8N2); 
#endif

  // AValue == (ON | OFF)  блокировка вкл/выкл
  void SetLock(const bool AValue);  

  // AValue == (ON | OFF)  тангента вкл/выкл
  void SetPTT(const bool AValue);   

  // по умолчанию рабочий режим - ключ
  void SetOperatingMode(const TOperatingMode AMode = TOperatingMode::CW); 

  // AValue == (ON | OFF)  подстройка частоты вкл/выкл
  void SetCLAR(const bool AValue); 

  // установка частоты подстройки AClarFreq = -99.99...+99.99
  void SetCLARFreq(float AClarFreq);

  // Разнос частот (split) вкл/выкл
  void SetSplit(const bool AValue);

  // Установка направления смещения для репитёра 
  void SetRepeaterOffsetDir(const TRepeaterOffset AValue);

  // Установка частоты смещения для репитёра ARepFreq = частота, например 12.34
  void SetRepeaterOffsetFreq(float ARepFreq);

  // Установка частоты в понятных цифрах, например 14.1234 MHz
  void SetMainFreq(float AMainFreq); 
  
  // Установка частоты в десятках Герц, для установки точной частоты через энкодер
  void SetVfoFreq(uint32_t AVfoFreq);
  
  // переключить VFO A/B
  void ToggleVFO(void); 

  // установить режим DCS | CTCSS
  void SetDCSMode(const TDCS_Mode AMode);

  // Установить тоновую частоту режима CTCSS для приема и передачи 
  // 0..999Гц и та и другая
  // Частоты: 67.0, 69.3, 71.9, 74.4, 77.0, 79.7, 82.5, 85.4, 88.5,  91.5,  94.8,  97.4, 100.0, 103.5, 107.2, 110.9,
  // 114.8, 118.8, 123.0, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 159.8, 162.2, 165.5, 167.9, 171.3, 173.8,
  // 177.3, 179.9, 183.5, 186.2, 189.9, 192.8, 196.6, 199.5, 203.5, 206.5, 210.7, 218.1, 225.7, 229.1, 233.6, 241.8,
  // 250.3, 254.1
  void SetCTCSSToneFreq(float ATXFreq, float ARXFreq);

  // установить код для DCS для передачи и приема
  // значения 0..9999 и для того и для другова
  // DCS Коды: 023, 025, 026, 031, 032, 036, 043, 047, 051, 053, 054, 065, 071, 073, 074, 114, 115, 116, 122,
  // 125, 131, 132, 134, 143, 145, 152, 155, 156, 162, 172, 174, 205, 212, 223, 225, 226, 243, 244, 245, 246, 
  // 251, 252, 255, 261, 263, 265, 266, 271, 274, 306, 311, 315, 325, 331, 332, 343, 346, 351, 356, 364, 365,
  // 371, 411, 412, 413, 423, 431, 432, 445, 446, 452, 454, 455, 462, 464, 465, 466, 503, 506, 516, 523, 526,
  // 532, 546, 565, 606, 612, 624, 627, 631, 632, 654, 662, 664, 703, 712, 723, 731, 732, 734, 743, 754 
  void SetDCSCode(const uint16_t ATXCode, const uint16_t ARXCode);

  // Прочитать статус приема
  //
  TRX_Status ReadRXStatus();

  // Прочитать статус передачи
  //
  TTX_Status ReadTXStatus();

  // Прочитать в каком рабочем режиме находится трансивер
  //
  TOperatingMode GetOperatingMode(void);

  // Прочитать, на какую частоту настроен трансивер
  //
  float GetFrequency(void);

//  uint32_t GetFrequency(void);

};

#pragma pack(pop)

