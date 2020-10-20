/*
* Программа сканирует диапазон частот транчивера с шагом 1 Мегагерц
*/

#include "ALSerial.h"
#include "uniFT897D.h"
#include <CustomSoftwareSerial.h>


#define SOFTSERIAL // если определено используем SofwareSerial
//#define SERIAL1   //328PB ONLY

#ifdef  SOFTSERIAL
ALSerial RadioPort(2, 3); // RX, TX
uniFT897D Radio(RadioPort);
#else
#ifdef SERIAL1
ALSerial RadioPort(&Serial1);
uniFT897D Radio(RadioPort);
#else
ALSerial RadioPort(&Serial);
uniFT897D Radio(RadioPort);
#endif
#endif

float ftFreq = 1.000;


void setup() {
#ifdef SOFTSERIAL
  Serial.begin(115200);
  delay(250);
  Serial.println("Start...");
  Radio.Init(4800,CSERIAL_8N2);
#else
  Radio.Init(4800,SERIAL_8N2);
#endif  
}

void loop() {

#ifdef SOFTSERIAL 
 Serial.print(" Устанавливаю F = ");
 Serial.println(ftFreq,5);
#endif 
 Radio.SetMainFreq(ftFreq);
 // TRX_Status rx_status = Radio.ReadRXStatus(); 
 float TftFreq = Radio.GetFrequency();
 
#ifdef SOFTSERIAL
 Serial.print(" Freq   = ");
 Serial.println(TftFreq,5);
#endif
 ftFreq =  ftFreq + 1.00;

#ifdef SOFTSERIAL
 Serial.println();
#endif 
 delay(500);
}
