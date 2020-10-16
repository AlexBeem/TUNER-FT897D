#include "ALSerial.h"
#include "uniFT897D.h"
#include <CustomSoftwareSerial.h>


#define SOFTPORT  // если определено используем SofwareSerial
#define SERIAL1   //328PB ONLY

#ifdef  SOFTPORT
ALSerial RadioPort(2, 3); // RX, TX
uniFT897D Radio(RadioPort);
#else
#ifdef SERIAL1
ALSerial RadioPort(&Serial1);
#else
ALSerial RadioPort(&Serial);
#endif
#endif

float ftFreq;


void ModeToSerial(const TOperatingMode AMode) {
  switch (AMode)
  {
    case TOperatingMode::LSB:
      Serial.println("LSB");
      break;
    case TOperatingMode::USB:
      Serial.println("USB");
      break;
    case TOperatingMode::CW:
      Serial.println("CW");
      break;
    case TOperatingMode::CWR:
      Serial.println("CWR");
      break;
    case TOperatingMode::AM:
      Serial.println("AM");
      break;
    case TOperatingMode::WFM:
      Serial.println("WFM");
      break;
    case TOperatingMode::FM:
      Serial.println("FM");
      break;
    case TOperatingMode::DIG:
      Serial.println("DIG");
      break;
    case TOperatingMode::PKT:
      Serial.println("PKT");
      break;
    case TOperatingMode::CWN:
      Serial.println("CWN");
      break;
    case TOperatingMode::CWNR:
      Serial.println("CWNR");
      break;     
    case TOperatingMode::FMN:
      Serial.println("FMN");
      break;
    case TOperatingMode::AMN:
      Serial.println("AMN");
      break;
    default:
      Serial.println("Unknown mode");
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("Start...");
  Radio.Init(4800,CSERIAL_8N2);
}

void loop() {
  
  ftFreq = Radio.GetFrequency();
  Serial.print(" Freq   = ");
  Serial.println(ftFreq,5);
  
  TTX_Status tx_status = Radio.ReadTXStatus();
  
  Serial.print(" Split  = ");
  if(tx_status.Split){
  Serial.println("OFF");
  }else{
  Serial.println("ON");
  }

  if(tx_status.PTT){
  Serial.print(" PTT    = ");
  Serial.println("OFF");
  }else{ 
  Serial.print(" PTT    = ");  
  Serial.println("ON");
  Serial.print(" Power  = ");
  Serial.println(tx_status.PowerValue);
  Serial.print(" HSWR   = ");  
  if(tx_status.SWR){
  Serial.println("YES");
  }else{
  Serial.println("NO");}
  }
  
  TRX_Status rx_status = Radio.ReadRXStatus();
  Serial.print(" S-Metr    = S");
  uint8_t s = rx_status.SMValue;
  if(s > 9){
  uint8_t s1 = (s-9)*6;
  Serial.print(9);
  Serial.print(" + ");
  Serial.print(s1);
  Serial.println("_Db");
  }else{
  Serial.println(s);
  }
  
  Serial.print(" DISCRIM   = ");
  if(!rx_status.Discr){
  Serial.println("OFF");
  }else{
  Serial.println("ON");}

  Serial.print(" DCS/CTCSS = ");
  if(!rx_status.Code){
  Serial.println("OFF");
  }else{
  Serial.println("ON");}
    
  Serial.print(" AF/SQL    = ");
  if(!rx_status.SQL){
  Serial.println("OFF");
  }else{
  Serial.println("ON");}

  TOperatingMode trxmode = Radio.GetOperatingMode();
  Serial.print(" MODE = ");
  ModeToSerial(trxmode);
  Serial.println();
  delay(3000);

// Управляем трансивером
/* 
  Radio.Init(9600);
  Radio.SetMainFreq(14.25);
  Radio.ToggleVFO();
  Radio.SetRepeaterOffsetFreq(5.4321);
  Radio.SetCLAR(ON);
  Radio.SetCLARFreq(12.5);
  Radio.SetDCSMode(TDCS_Mode::DCS_ENCODER_ON);
  Radio.SetCTCSSToneFreq(88.0, 100.0);
  Radio.SetDCSCode(50, 9500);
  */
}
