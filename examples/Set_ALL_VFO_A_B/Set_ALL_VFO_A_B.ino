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
encoder.tick(); 
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

Radio.SetOperatingMode(TOperatingMode::DIG);
delay(2000);
Radio.SetMainFreq(1.840);
delay(2000);
Radio.SetMainFreq(1.908);
delay(2000);
Radio.SetMainFreq(3.573);
delay(2000);
Radio.SetMainFreq(7.074);
delay(2000);
Radio.SetMainFreq(10.136);
delay(2000);
Radio.SetMainFreq(14.074);
delay(2000);
Radio.SetMainFreq(18.100);
delay(2000);
Radio.SetMainFreq(21.074);
delay(2000);
Radio.SetMainFreq(24.915);
delay(2000);
Radio.SetMainFreq(28.074);
delay(2000);
Radio.SetMainFreq(50.313);
delay(2000);
Radio.SetMainFreq(50.070);
delay(2000);
Radio.SetMainFreq(144.174);
delay(2000);
Radio.SetMainFreq(434.174);
delay(2000);

Radio.ToggleVFO();
delay(2000);
Radio.SetOperatingMode(TOperatingMode::LSB);
delay(2000);
ssbFreq = 1900000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
ssbFreq = 3650000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
ssbFreq = 7100000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
Radio.SetOperatingMode(TOperatingMode::USB);
delay(2000);
ssbFreq = 10150000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
ssbFreq = 14150000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
ssbFreq = 18150000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
ssbFreq = 21150000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
ssbFreq = 24940000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
ssbFreq = 28500000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
Radio.SetOperatingMode(TOperatingMode::FM);
ssbFreq = 145500000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
ssbFreq = 435500000/10;
Radio.SetVfoFreq(ssbFreq);
delay(2000);
Radio.ToggleVFO();
delay(2000);

}// END LOOP


 
