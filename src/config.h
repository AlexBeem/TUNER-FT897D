// Включение обработки энкодера через прерывание
// обработка прерывания должна быть организована в теле скетча
#define EXTERNALPCINT0 
//#define EXTERNALPCINT1
//#define EXTERNALPCINT2
//#define EXTERNALPCINT3

#define SOFTSERIAL  // если определено используем SofwareSerial, 
                    // НАЗВАНИЕ не менять, используется в библиотеке
//#define SERIAL1   // 328PB ONLY если используется HARDWARE_SERIAL то 
                    // определяемся, какой именно
