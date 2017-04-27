//подключаем библиотеки
#include <LiquidCrystal_I2C.h>  //Дисплей
#include <SPI.h> //интерфейс SPI
#include <Ethernet.h> //Ethernet-модуль
#include <Wire.h> //I2C интерфейс
#include <Keypad.h>//библиотека кейпада

int relay_pin = 2;  //Пин Реле

const byte numRows = 4; //размер кейпада(строки)
const byte numCols = 4;//размер кейпада(столбцы)
byte rowPins[numRows] = {12, 11, 10, 9 }; //Rows 0 to 3
byte colPins[numCols] = { 8, 7, 6, 5 }; //Columns 0 to 3

//ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
int CurPosx , CurPosy ; //две переменные хранящие текущее положение курсора
char login[5], password[5];  //две строки с логином и паролем

LiquidCrystal_I2C lcd(0x3F, 16, 2);  //инициализируем дисплей
char keymap[numRows][numCols] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' } //организовали массив для кейпада
};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);



void start_screen()
{
    lcd.clear(); //очищаем дисплей
    login[0] = 0; //очищаем массив логина
    login[1] = 0;
    login[2] = 0;
    login[3] = 0;
    password[0] = 0; //очищаем массив пароля
    password[1] = 0;
    password[2] = 0;
    password[3] = 0;
    lcd.setCursor(4, 0); 
    lcd.print("<------LOGIN"); 
    lcd.setCursor(4, 1);
    lcd.print("<------PASSW");
    lcd.setCursor(0, 0);
    CurPosx = 0; 
    CurPosy = 0;
}

void analyze()
{ 
  //формируем строку секрета согласно протоколу
  char getstr[256] = "\0";
  strcat(getstr, login); 
  strcat(getstr, "wash8ka2014AS!");
  strcat(getstr, password);
  strcat(getstr, lock_id);
  strcat(getstr, "CHECKPASS");
  
}


void setup()
{
    lcd.init();
    lcd.backlight();
    start_screen ();//вызываем функцию очищающую дисплей
    pinMode(relay_pin, OUTPUT);  //пин реле на запись
    digitalWrite(relay_pin, HIGH); //подаем на реле единичку, закрывая замок
 }


void loop() 
{  
      char keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY) { //если на кейпаде что то нажато
        
        //здесь мы определяем на какой позиции пароля или логина мы сейчас находимся и заполняем массивы а также выводим символы на экран
        if (CurPosy == 0)
        { 
            if (CurPosx >= 0 && CurPosx < 4) 
            {
                if (keypressed >= '0' && keypressed <= '9') 
                {
                    lcd.print(keypressed);
                    switch (CurPosx) 
                    {
                    case 0:
                        login[0] = keypressed;
                        break;
                    case 1:
                        login[1] = keypressed;
                        break;
                    case 2:
                        login[2] = keypressed;
                        break;
                    case 3:
                        login[3] = keypressed;
                        break;
                    }
                    CurPosx++;
                }
            }
            if (CurPosx == 4)
            {
                CurPosy = 1;
                CurPosx = 0;
                lcd.setCursor(0, 1);
            }
        }
        else if (CurPosy == 1) 
        {
            if (CurPosx >= 0 && CurPosx < 4) 
            {
                if (keypressed >= '0' && keypressed <= '9') {
                    lcd.print('*');
                    switch (CurPosx) {
                    case 0:
                        password[0] = keypressed;
                        break;
                    case 1:
                        password[1] = keypressed;
                        break;
                    case 2:
                        password[2] = keypressed;
                        break;
                    case 3:
                        password[3] = keypressed;
                        break;
                    }
                    CurPosx++;
                }
              }
             if ((CurPosx == 4) && (keypressed == '#')) { //если мы нажали сеточку очищаем дисплей
             lcd.clear(); //очищаем дисплей
             lcd.print("WAIT...");
             analyze(); //соединяемся с сервером и проверяем данные
             delay(1000);
             start_screen();
             }
         }
    }
}
