/*
* Программа разбирает парметры приёма и передачи передаваемые через CAT интерфейс
*/

#include "ALSerial.h"
#include "uniFT897D.h"
#include <CustomSoftwareSerial.h>


#define SOFTSERIAL  // если определено используем SofwareSerial
#define SERIAL1   //328PB ONLY

#ifdef  SOFTSERIAL
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

void UA6EM_PCINT0_handler(void){
//encoder.tick(); 
}

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
  uint8_t pw,tpw,ptt; 
  
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
  ptt = 0;
  }else{ ptt = 1;
  Serial.print(" PTT    = ");  
  Serial.println("ON");
  Serial.print(" Power  = ");
  pw = tx_status.PowerValue;
//  if(pw >=8 && pw <= 10)tpw = pw*10;
//  if(pw < 8)tpw = (pw-1)*10;
  if(pw == 2) tpw = 5;
  if(pw == 3) tpw = 10;
  if(pw == 4) tpw = 20;
  if(pw == 5) tpw = 30;
  if(pw == 6) tpw = 40;
  if(pw == 7) tpw = 60;
  if(pw == 8) tpw = 80;
  if(pw == 9) tpw = 90;
  if(pw == 10) tpw = 100; 
  Serial.print(tpw);
  Serial.println(" Ватт");
  Serial.print(" HSWR -  ");  
  if(tx_status.SWR){
  Serial.println("YES");
  }else{
  Serial.println("NO");}
  }
  
  TRX_Status rx_status = Radio.ReadRXStatus();
  if(ptt == 0){
  Serial.print(" S-Metr    = S");
  uint8_t s = rx_status.SMValue;
  if(s > 9){
  uint8_t s1 = (s-9)*6;
  Serial.print(9);
  Serial.print(" + ");
  Serial.print(s1);
  Serial.println("dB");
  }else{
  Serial.println(s);
   }
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

}
