/*
* Скетч панели управления для трансиверов FT-897D/ FT-857D через CAT систему
* 25.10.2020 (с) UA6EM
*
*/

#include "ALSerial.h"
#include "uniFT897D.h"
#include <CustomSoftwareSerial.h>


#define SOFTSERIAL  // если определено используем SofwareSerial, 
                    // НАЗВАНИЕ не менять, используется в библиотеке
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

float ftFreq;
uint32_t ssbFreq;

void UA6EM_PCINT0_handler(void){
//encoder.tick(); 
}

void setup() {
#ifdef SOFTSERIAL
  Serial.begin(115200);
  delay(250);
  Serial.println("Start...");
  Radio.Init(4800,CSERIAL_8N2);
//  Radio.Init();  // Если инициализируем параметрами по умолчанию
#else
  Radio.Init(4800,SERIAL_8N2);
//  Radio.Init();    // Если инициализируем параметрами по умолчанию
#endif  
}

void loop() {

}
