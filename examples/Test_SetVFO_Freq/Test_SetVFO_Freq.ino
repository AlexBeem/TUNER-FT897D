// 26.10.2020 Добавлена возможность работы Энкодера через прерывание PCINT0 для этого немного
// исправлена библиотека CustomSoftwareSerial, высвобождено это прерывание
// 28.10.2020 опробовано совместное использование PCINT0 между кастомным софтовым сериалом
// и энкодером, работает! Библиотеки и скетч примера поправлен

#include <ALSerial.h>
#include <uniFT897D.h>
#include <CustomSoftwareSerial.h>
#include <RotaryEncoder.h> // https://github.com/mathertel/RotaryEncoder/tree/master/examples
//#include <GyverButton.h>

int freqUpPin = 4;                          // Define rotary encoder pins.
int freqDownPin = 5;
//RotaryEncoder encoder(freqUpPin, freqDownPin);
//RotaryEncoder encoder2(A2, A3);
RotaryEncoder encoder(9, 8); // если отчет обратный поменяйте пины местами


#define SOFTSERIAL  // если определено используем SofwareSerial, 
                    // НАЗВАНИЕ не менять, используется в библиотеке
//#define SERIAL1   //328PB ONLY

#ifdef  SOFTSERIAL
//ALSerial RadioPort(2, 3); // RX, TX
ALSerial RadioPort(11, 12); // RX, TX
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

int32_t incr = 25000;
int32_t freq = 145000000;
float Ffreq;
uint32_t ftFreq;
uint32_t freq_tmp;
uint32_t my_micros;

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
Cl_Btn Btn2(/*пин*/BUTTON_ENCODER); //Экземпляр обработчика для кнопки

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

 // You may have to modify the next 2 lines if using other pins than A2 and A3
 // PCICR |= (1 << PCIE0 /*D8-D13*/) | (1 << PCIE1 /*A0-A5*/) | (1 << PCIE2 /*D0-D7*/);    // Замаркировать неиспользуемые прерывания
  PCICR |= (1 << PCIE0 /*D8-D13*/); 
 // PCICR |= (1 << PCIE1 /*A0-A5*/); 
 // PCICR |= (1 << PCIE2 /*D0-D7*/); 
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1);    //  D8-PCINT0 и D13-PCINT5
 // PCMSK0 |= (1 << PCINT0) | (1 << PCINT1);
 // PCMSK1 |= (1 << PCINT8) | (1 << PCINT13);   //  A0 и A5
 // PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);   //  A2 и A3
 // PCMSK2 |= (1 << PCINT16) | (1 << PCINT23);  //  D0 и D7
}
/* *
#ifndef(PCINT1_vect)
ISR(PCINT1_vect)
{
 encoder2.tick();
}
#endif

**
ISR(PCINT0_vect) {
  encoder.tick(); // 
}
**/


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
//  static int pos2 = 0;
  
//  encoder2.tick();
//  int newPos2 = encoder2.getPosition();

 // encoder.tick();
  int newPos = encoder.getPosition();
  
  if (pos != newPos) {
    if(max(newPos,pos) > pos){freq = freq +incr;}else{freq = freq - incr;}
    ftFreq = freq/10;  
    Radio.SetVfoFreq(ftFreq);
    pos = newPos;

    #ifdef  SOFTSERIAL  // Выведем значение частоты в порт
    Serial.print("Freq = ");
    Serial.print(freq/1000000);
    Serial.print(".");
    freq_tmp = freq % 1000000;
    if(freq_tmp < 100000)  Serial.print("0");  // дополним лидирующим 0 
    Serial.println((freq_tmp));   
    #endif 
  }
  
//    Serial.println((micros() - my_micros));  // для отладки замер длительности цикла
//    my_micros = micros();    
  
}  // END LOOP
