
#include <Keypad.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3F,16,2);   // Задаем адрес и размерность дисплея. 
//Клавиатура
const byte ROWS = 4; // 4 строки
const byte COLS = 4; // 4 столбца
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; 
byte rowPins[ROWS] = {11,10, 9, 8}; 
byte colPins[COLS] = {7, 6, 5, 4}; 
Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup()
{
  lcd.init();                            // Инициализация lcd             
  lcd.backlight();                       // Включаем подсветку
  // Курсор находится в начале 1 строки
  lcd.print("8ka.mipt.ru!"); 

}

void loop()
{
    
}
