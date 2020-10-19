// С миру по нитке
// Функция кнопки от QWONE (сайт www.arduino.ru)

#include "ALSerial.h"
#include "uniFT897D.h"
#include <CustomSoftwareSerial.h>


//#define SOFTSERIAL  // если определено используем SofwareSerial, 
                      // НАЗВАНИЕ НЕ менять !!! , используется в библиотеке
                      // для выбора параметров компиляции
                      
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

#define BUTTON  10


float ftFreq;
unsigned long mill; // переменная под  millis()
uint8_t fl_proc = 0;
int8_t i = 0;
uint8_t fmode = 0;


/*** Обработчик кнопки ***/
//------Cl_Btn----------------------
enum {sbNONE = 0, sbClick, sbLong}; /*состояние не изменилось/клик/долгое нажатие*/
class Cl_Btn {
  protected:
    const byte pin;
    byte state;
    bool bounce = 0;
    bool btn = 1, oldBtn;
    unsigned long past;
    const uint32_t time = 500 ;
    bool flag = 0;
    uint32_t past_flag = 0 ;
  public:
    Cl_Btn(byte p): pin(p) {}
    /*инициализация-вставить в setup()*/
    void init() {
      pinMode(pin, INPUT_PULLUP);
    }
    /*работа-вставить в loop()*/
    void run() {
      state = sbNONE;
      bool newBtn = digitalRead(pin);
      if (!bounce && newBtn != btn) {
        bounce = 1;
        past = mill;
      }
      if (bounce && mill - past >= 10) {
        bounce = 0 ;
        oldBtn = btn;
        btn = newBtn;
        if (!btn && oldBtn) {
          flag = 1;
          past_flag = mill;
        }
        if (!oldBtn && btn && flag && mill - past_flag < time ) {
          flag = 0;
          state = sbClick;
        }
      }
      if (flag && mill - past_flag >= time ) {
        flag = 0;
        state = sbLong;
      }
    }
    byte read() {
      return state;
    }
};

Cl_Btn Btn1(/*пин*/BUTTON); //Экземпляр обработчика для кнопки


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

 pinMode(Button_GND,OUTPUT);
 pinMode(13,OUTPUT);
 digitalWrite(13,HIGH);
// pinMode(BUTTON,INPUT_PULLUP); // инициализируется в библиотеке (setup in function)

// Настроим трансивер
Radio.SetOperatingMode(TOperatingMode::FM);
delay(1000);
Radio.SetMainFreq(145.500);
Btn1.init(); 
}

void loop() {

//   Radio.SetOperatingMode(TOperatingMode::FM);   // Работает
//   Radio.SetMainFreq(145.500);                   // Работает

// 1 Radio.SetLock(ON);                            // Работает
// 2 Radio.SetLock(OFF);                           // Работает

// 3 Radio.SetPTT(ON);                             // Работает
// 4 Radio.SetPTT(OFF);                            // Работает

// 5 Radio.SetSplit(ON);                           // Работает
// 6 Radio.SetSplit(OFF);                          // Работает

// 07 Radio.SetRepeaterOffsetFreq(0.16);           // Работает  - Меню №76
// 08 Radio.SetRepeaterOffsetFreq(0.06);           // Работает  - Меню №76

// 09 Radio.SetCLARFreq(2.5);                      // Работает
// 10 Radio.SetCLAR(ON);                           // Работает
// 11 Radio.SetCLAR(OFF);                          // Работает

// 12 Radio.SetCTCSSToneFreq(69.3, 254.1);          // Работает  - Меню №83
// 13 Radio.SetCTCSSToneFreq(254.1, 69.3);          // Работает  - Меню №83

// 14 Radio.SetDCSCode(23, 754 );                   // Работает  - Меню №33
// 15 Radio.SetDCSCode(754, 23);                    // Работает  - Меню №33

// 16 Radio.SetDCSMode(TDCS_Mode::DCS_ON);            // Работает = DCS  кодировка/декодировка
// 17 Radio.SetDCSMode(TDCS_Mode::DCS_DECODER_ON);    // ?
// 18 Radio.SetDCSMode(TDCS_Mode::DCS_ENCODER_ON);    // Работает = DEN  кодировка

// 19 Radio.SetDCSMode(TDCS_Mode::CTCSS_ON);          // Работает = TSQ  кодировка/декодировка
// 20  Radio.SetDCSMode(TDCS_Mode::CTCSS_DECODER_ON); // ?
// 21 Radio.SetDCSMode(TDCS_Mode::CTCSS_ENCODER_ON);  // Работает = TEN  кодировка
// 22 Radio.SetDCSMode(TDCS_Mode::OFF);               // 
// 23  Radio.ToggleVFO();                             // Работает

mill = millis();              // Без этой строки кнопка Квона не работает
Btn1.run(); 


if (Btn1.read() == sbClick)  { // Вызвать процедуру по короткому нажатию
i++;  fl_proc = 0; if(i > 25) i = 0;
digitalWrite(13,!digitalRead(13));
 }

if (Btn1.read() == sbLong)   { // Вызвать процедуру по длинному нажатию
i--; fl_proc = 0; if(i < 0) i = 0;
digitalWrite(13,!digitalRead(13));
 }

if(i==1 && !fl_proc){ Radio.SetLock(ON);  // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.010);
}

if(i==2 && !fl_proc){ Radio.SetLock(OFF); // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.020);
}

if(i==3 && !fl_proc){ Radio.SetPTT(ON);  // Работает
delay(100);
Radio.SetMainFreq(145.030);
}

if(i==4 && !fl_proc){ Radio.SetPTT(OFF); // Работает
delay(500);
Radio.SetMainFreq(145.040);
}

if(i==5 && !fl_proc){ Radio.SetSplit(ON); // Работает
delay(100);
Radio.SetMainFreq(145.050);
}

if(i==6 && !fl_proc){ Radio.SetSplit(OFF); // Работает
delay(100);
Radio.SetMainFreq(145.060);
}

if(i==7 && !fl_proc){Radio.SetRepeaterOffsetFreq(1.6);  // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.070);
}

if(i==8 && !fl_proc){ Radio.SetRepeaterOffsetFreq(0.6); // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.080);
}

if(i==9 && !fl_proc){ Radio.SetCLARFreq(2.5); // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.090);
}

if(i==10 && !fl_proc){ Radio.SetCLAR(ON);     // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.100);
}

if(i==11 && !fl_proc){ Radio.SetCLAR(OFF);    // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.110);
}

if(i==12 && !fl_proc){ Radio.SetCTCSSToneFreq(254.1, 69.3); // Работает
delay(100);
Radio.SetMainFreq(145.120);
}
if(i==13 && !fl_proc){ Radio.SetCTCSSToneFreq(69.3, 254.1); // Работает
delay(100);
Radio.SetMainFreq(145.130);
}
if(i==14 && !fl_proc){ Radio.SetDCSCode(23, 754);  
delay(100);
Radio.SetMainFreq(145.140);
}

if(i==15 && !fl_proc){ Radio.SetDCSCode(754, 23);  
delay(100);
Radio.SetMainFreq(145.150);
}

if(i==16 && !fl_proc){ Radio.SetDCSMode(TDCS_Mode::DCS_ON);
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.160);
}

if(i==17 && !fl_proc){ Radio.SetDCSMode(TDCS_Mode::DCS_DECODER_ON);
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.170);
}

if(i==18 && !fl_proc){ Radio.SetDCSMode(TDCS_Mode::DCS_ENCODER_ON);
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.180);
}

if(i==19 && !fl_proc){ Radio.SetDCSMode(TDCS_Mode::CTCSS_ON);
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.190);
}

if(i==20 && !fl_proc){ Radio.SetDCSMode(TDCS_Mode::CTCSS_DECODER_ON);
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.200);
}

if(i==21 && !fl_proc){ Radio.SetDCSMode(TDCS_Mode::CTCSS_ENCODER_ON);
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.210);
}

if(i==22 && !fl_proc){ Radio.SetDCSMode(TDCS_Mode::OFF);
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.220);
}

if(i==23 && !fl_proc){  Radio.ToggleVFO(); // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.230);
}

if(i==24 && !fl_proc){  Radio.ToggleVFO(); // Работает
fl_proc = 1;
delay(100);
Radio.SetMainFreq(145.240);
}

if(i==25 && !fl_proc){ //
fl_proc = 0;
i = 0;
delay(100);
Radio.SetMainFreq(145.000);
}

fl_proc = 1;  // Отработали функцию, ждём нажатия кнопки, короткое - вперёд, длинное - назад
}  // END LOOP
