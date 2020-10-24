//#include "ALSerial.h"
#include "uniFT897D.h"


void uniFT897D::ClearCmdBuffer(void)
{
  FPort.flush();
  memset(&FCommand, 0, COMMAND_SIZE);
}

void uniFT897D::SendCommand(const bool ANeedAnswer) const
{
    FPort.write((uint8_t *)(&FCommand), COMMAND_SIZE);
    if (!ANeedAnswer) {
      delay(100);
      while (FPort.available()) FPort.read();
    }
}

void uniFT897D::Init(const uint32_t ABaudRate, const uint16_t ADataParity)
{
  FPort.begin(ABaudRate, ADataParity);  // настроить скорость, формат по умолчанию (4800, 8N2) 
  delay(20);
  SetOperatingMode();                 // рабочий режим на ключ  - CW (по умолчанию)
}

void uniFT897D::SetLock(const bool AValue)
{
  ClearCmdBuffer();
  FCommand.Command = (AValue == ON) ? CMD_LOCK_ON : CMD_LOCK_OFF;
  SendCommand();
}

void uniFT897D::SetPTT(const bool AValue)
{
  ClearCmdBuffer();
  FCommand.Command = (AValue == ON) ? CMD_PTT_ON : CMD_PTT_OFF;
  SendCommand();

}

void uniFT897D::SetOperatingMode(const TOperatingMode AMode)
{
  if (AMode == TOperatingMode::Unknown) return;
  ClearCmdBuffer();
  FCommand.Byte0 = static_cast<uint8_t>(AMode);
  FCommand.Command = CMD_SET_OPERATING_MODE;
  SendCommand();
}

void uniFT897D::SetCLAR(const bool AValue)
{
  ClearCmdBuffer();
  FCommand.Command = (AValue == ON) ? CMD_CLAR_ON : CMD_CLAR_OFF;
  SendCommand();
}

void uniFT897D::SetCLARFreq(float AClarFreq)
{
  ClearCmdBuffer();
  if (AClarFreq < 0) {
    FCommand.Byte0 = 1;
    AClarFreq = -AClarFreq;
  }

  if (AClarFreq >= 100.00) AClarFreq = 99.99;

  const char* ptr = Freq2String(AClarFreq, 2, 4);

  StringToBCD(ptr, &FCommand.Byte2, 2);

  FCommand.Command = CMD_CLAR_SETFREQ;

  SendCommand();
}

void uniFT897D::SetSplit(const bool AValue)
{
  ClearCmdBuffer();
  FCommand.Command = (AValue == ON) ? CMD_SPLIT_ON : CMD_SPLIT_OFF;
  SendCommand();
}

void uniFT897D::SetRepeaterOffsetDir(const TRepeaterOffset AValue)
{
  ClearCmdBuffer();
  FCommand.Byte0 = static_cast<uint8_t>(AValue);
  FCommand.Command = CMD_SET_REPEATER_OFFSET;
  SendCommand();
}

void uniFT897D::SetRepeaterOffsetFreq(float ARepFreq)
{
  if (ARepFreq < 0) ARepFreq = -ARepFreq;
  if (ARepFreq >= 100.0) ARepFreq = 99.99;
  const char* ptr = Freq2String(ARepFreq, 2, 8);

  StringToBCD(ptr, &FCommand.Byte0, 4);

  FCommand.Command = CMD_SET_REPEATER_OFFSET_FREQ;

  SendCommand();

}

void uniFT897D::SetMainFreq(float AMainFreq)
{
  ClearCmdBuffer();

  if (AMainFreq < 0) AMainFreq = -AMainFreq;
  const char* ptr = Freq2String(AMainFreq, 3, 8);

  StringToBCD(ptr, &FCommand.Byte0, 4);

  FCommand.Command = CMD_SET_MAIN_FREQ;

  SendCommand();
}

// Установка частоты в десятках Герц
void uniFT897D::SetVfoFreq(uint32_t AVfoFreq)
{
    char out[9];
    memset(out, '0', 8); out[8] = 0x00;
 
    ClearCmdBuffer();
 
    char* ptr = &out[3];
    if (AVfoFreq >= 100000UL) ptr--;   // Если больше или равно 1.0 МГц сдвигаемся влево
    if (AVfoFreq >= 1000000UL) ptr--;  // Если больше или равно 10.0 МГц
    if (AVfoFreq >= 10000000) ptr--;   // Если больше или равно 100.0 МГц
 
    ultoa(AVfoFreq, ptr, 10);
 
    StringToBCD(out, &FCommand.Byte0, 4);
 
    FCommand.Command = CMD_SET_MAIN_FREQ;
 
    SendCommand(true);
}


void uniFT897D::ToggleVFO(void)
{
  ClearCmdBuffer();
  FCommand.Command = CMD_SWITCH_VFO;
  SendCommand();
}

void uniFT897D::SetDCSMode(const TDCS_Mode AMode)
{
  ClearCmdBuffer();
  FCommand.Byte0 = static_cast<uint8_t>(AMode);
  FCommand.Command = CMD_SET_DCS_MODE;
  SendCommand();
}

void uniFT897D::SetCTCSSToneFreq(float ATXFreq, float ARXFreq)
{
  if (ATXFreq < 0) ATXFreq = -ATXFreq;
  if (ARXFreq < 0) ARXFreq = -ARXFreq;

  ClearCmdBuffer();

  const char* ptr = Freq2String(ATXFreq, 3, 4);
  StringToBCD(ptr, &FCommand.Byte0, 2);

  ptr = Freq2String(ARXFreq, 3, 4);
  StringToBCD(ptr, &FCommand.Byte2, 2);

  FCommand.Command = CMD_SET_CTCSS_TONE_FREQ;

  SendCommand();

}

void uniFT897D::SetDCSCode(const uint16_t ATXCode, const uint16_t ARXCode)
{
  ClearCmdBuffer();
  
  const char* ptr = Freq2String(ATXCode / 100.0, 2, 4);
  StringToBCD(ptr, &FCommand.Byte0, 2);

  ptr = Freq2String(ARXCode / 100.0, 2, 4);
  StringToBCD(ptr, &FCommand.Byte2, 2);

  FCommand.Command = CMD_SET_DCS_CODE;

  SendCommand();
}

TRX_Status uniFT897D::ReadRXStatus()
{
  FPort.flush();
  ClearCmdBuffer();
  FCommand.Command = CMD_READ_RX_STATUS;
  SendCommand(true);

  TRX_Status result;
  uint8_t value = ReadByteFromPort(1000);

  memcpy(&result, &value, 1);

  return result;
}

TTX_Status uniFT897D::ReadTXStatus()
{
  FPort.flush();
  ClearCmdBuffer();

  FCommand.Command = CMD_READ_TX_STATUS;
  SendCommand(true);

  TTX_Status result;
  uint8_t value = ReadByteFromPort(1000);

  memcpy(&result, &value, 1);

  return result;
}

TOperatingMode uniFT897D::GetOperatingMode(void)
{
  TOperatingMode result = ReadLongStatus(2000) ? static_cast<TOperatingMode>(FCommand.Command) : TOperatingMode::Unknown;
  
  return result;
}

float uniFT897D::GetFrequency(void)
{
  const uint8_t MAX_LENGTH = 0x09;

  if (!ReadLongStatus(2000)) return 0.0f;

  char buf[MAX_LENGTH];
  memset(buf, 0, MAX_LENGTH);

  uint8_t* ptr = (uint8_t*)(&FCommand.Byte0);

  uint8_t idx = 0;

  for (uint8_t i = 0; i < 4; ++i) {
    buf[idx] = (ptr[i] >> 4) + '0';
    buf[idx+1] = (ptr[i] & 0x0F) + '0';
    idx += 2;
  }

  uint32_t result = atol(buf);

  return (result / 100000.0f);
}

const char* uniFT897D::Freq2String(const float AFreq, const uint8_t AIntDigits, const uint8_t ALength)
{
  static char outbuf[10];
  memset(outbuf, '0', ALength);
  outbuf[ALength] = 0x00;

  uint32_t Multiplier = 1;
  for (uint8_t i = 0; i < (ALength-AIntDigits); ++i) { Multiplier *= 10; };

  uint32_t Freq = AFreq * Multiplier;

  char* ptr = &outbuf[AIntDigits];
  if (AFreq >= 1.0) ptr--;
  if (AFreq >= 10.0) ptr--;
  if (AFreq >= 100.0) ptr--;

  ultoa(Freq, ptr, 10);

  return outbuf;
}

void uniFT897D::StringToBCD(const char* AFrom, uint8_t* ATo, const uint8_t ALen)
{
  uint8_t idx = 0;
  for (uint8_t i = 0; i < ALen; ++i) {
    ATo[i] = ((AFrom[idx] - '0') << 4) | (AFrom[idx + 1] - '0');
    idx += 2;
  }
}

uint8_t uniFT897D::ReadByteFromPort(const uint16_t ATimeoutMS)
{
  uint32_t now = millis();
  while (!FPort.available()) {
    if (millis() - now > ATimeoutMS) return false;
  }
  return FPort.read();
}

bool uniFT897D::ReadLongStatus(const uint16_t ATimeoutMS)
{
  const uint8_t ANSWER_LENGTH = 5;

  FPort.flush();

  ClearCmdBuffer();
  FCommand.Command = CMD_READ_LONG_STATUS;
  SendCommand(true);
  delay(20);
  uint32_t now = millis();
  uint8_t* buf = (uint8_t*)(&FCommand);

  while (FPort.available() < ANSWER_LENGTH) {
    if (millis() - now > ATimeoutMS) return false;
  }

  FPort.readBytes(buf, ANSWER_LENGTH);

  return true;
}


uniFT897D::uniFT897D(ALSerial& ASerialPort) : FPort(ASerialPort)
{
  ClearCmdBuffer();
}
