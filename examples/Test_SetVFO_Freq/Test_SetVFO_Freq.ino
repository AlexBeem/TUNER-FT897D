#include "ALSerial.h"
#include "uniFT897D.h"
#include "CustomSoftwareSerial.h"
#include <RotaryEncoder.h> // https://github.com/mathertel/RotaryEncoder/tree/master/examples
#include <GyverButton.h>

int freqUpPin = 4;                          // Define rotary encoder pins.
int freqDownPin = 5;
RotaryEncoder encoder(freqUpPin, freqDownPin);


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

#define Button_GND 12
#define BUTTON  10
#define BUTTON_ENCODER 6

/*
float ftFreq;
uint32_t freq = 145260000;
uint32_t RftFreq = 14526000;  // Частота для передачи через SetVfoFreq
uint32_t incr = 10;
*/
int32_t incr = 25000;
int32_t freq = 145000000;
int32_t freq_temp = 145000000;
float Ffreq = freq/100000.0f;;
uint32_t ftFreq;
uint8_t CW = 1;

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

//Cl_Btn Btn1(/*пин*/BUTTON); //Экземпляр обработчика для кнопки
Cl_Btn Btn2(/*пин*/BUTTON_ENCODER); //Экземпляр обработчика для кнопки


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
 digitalWrite(Button_GND,LOW);
 digitalWrite(13,HIGH);
// pinMode(BUTTON,INPUT_PULLUP); // инициализируется в библиотеке

 pinMode(freqUpPin, INPUT_PULLUP);      // Set pins for rotary encoders as INPUTS and enable
 pinMode(freqDownPin, INPUT_PULLUP);    // internal pullup resistors.
 
// Настроим трансивер
Radio.SetOperatingMode(TOperatingMode::FM);
delay(1000);
Radio.SetMainFreq(145.500);

#ifdef  SOFTSERIAL
Serial.println("Set VFO Freq = 145.500");
#endif

//Btn1.init(); // кнопка переключателя
Btn2.init(); // кнопка энкодера
}

void loop() {

mill = millis(); // Без этой строки кнопка Квона не работает    
Btn2.run();      // Опрос кнопки 2     
if (Btn2.read() == sbClick)  { // Вызвать процедуру по короткому нажатию
if(incr == 25000) {incr = 5000;}else{ incr = 25000;}
}

if (Btn2.read() == sbLong)   { // Вызвать процедуру по длинному нажатию
incr = 1000;
}
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
      if(max(newPos,pos) > pos){freq = freq +incr;}else{freq = freq - incr;}
      
    Serial.print("Freq = ");
//    freq = freq_temp +(incr * (newPos)); 
    Serial.println(freq/1000000.0f, 5);
    ftFreq = freq/10;
    Radio.SetVfoFreq(ftFreq);
    pos = newPos;
  }
}  // END LOOP
