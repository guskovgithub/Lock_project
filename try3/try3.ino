//подключаем библиотеки
#include <LiquidCrystal_I2C.h>
#include <SPI.h> //интерфейс SPI
#include <Ethernet.h> //Ethernet-модуль
#include <Wire.h> //I2c интерфейс
#include <Keypad.h>//библиотека кейпада

#include <string.h> //работа со строками
#include <MD5.h> //Хэш-функция


byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 }; //массив, хранящий мак адрес Ethernet модуля
EthernetClient client;
char server[] = "93.175.29.245"; //IP адрес сервера стиралки
char lock_id[4] = "82";


int relay_pin = 2;  //Пин Реле

const byte numRows = 4; //размер кейпада(строки)
const byte numcols = 4;//размер кейпада(столбцы)
byte rowPins[numRows] = {A0, A1, A2, A3 }; //Rows 0 to 3
byte colPins[numcols] = { 8, 7, 6, 5 }; //columns 0 to 3

//ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
int curPosx , curPosy ; //две переменные хранящие текущее положение курсора
char login[5], password[5];  //две строки с логином и паролем

LiquidCrystal_I2C lcd(0x3F, 16, 2);  //инициализируем дисплей
char keymap[numRows][numcols] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'c' },
  { '*', '0', '#', 'D' } //организовали массив для кейпада
};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numcols);



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
  curPosx = 0;
  curPosy = 0;
}

void analyze()
{
  //формируем строку секрета согласно протоколу
  char getstr[256] = "\0";
  sprintf(getstr,"%swash8ka2014AS!%s%sCHECKPASS", login, password, lock_id);
  unsigned char* hash = MD5::make_hash(getstr); // хэшируем секрет
  char* md5str = MD5::make_digest(hash, 16);
  getstr[0] = '\0';
  // формируем строку запроса
  //sprintf(getstr, "GET /checkPin.php?login=", login, "&action=cHEcKPASS&pass=", password, "&secret=", md5str, "&lock_id=", lock_id, "\n");
  sprintf(getstr, "GET /checkPin.php?login=%s&action=CHECKPASS&pass=%s&secret=%s&lock_id=%s\n", login, password, md5str, lock_id);
   Serial.println(getstr);
  if (client.connect(server, 80))
  {
    //пытаемся подключиться к серверу
    client.print(getstr); //посылаем GET запрос
    delay(200);
    char answer_from_server ;
    while (client.available())
    { //если успешно
      char answer_from_server = client.read(); // читаем символ
    }
    client.stop();  //останавливаем передачу
    client.flush(); //очищаем
    delay(100);
    if (answer_from_server = 'Y')
    { //если позитивный отвеn
      lcd.print("OK");
      delay(500);
      digitalWrite(relay_pin, HIGH);
      lcd.clear();
      lcd.print("5...");
      delay(900);
      lcd.clear();
      lcd.print("4...");
      delay(900);
      lcd.clear();
      lcd.print("3...");
      delay(900);
      lcd.clear();
      lcd.print("2...");
      delay(900);
      lcd.clear();
      lcd.print("1...");
      delay(900);
      lcd.clear();
      lcd.print("LOCKED!");
      digitalWrite(relay_pin, LOW);
    }
    else {
      lcd.print("NO!");//иначе то же самое только без открытия реле
    }
    delay(1000);

  }
  else
  { //если не подключилось
    client.stop(); //останавливаем, ждем и пытаемся подключиться еще раз
    delay(1000);
    lcd.clear();
    lcd.print("NO CONECTION");
    lcd.setCursor(0, 1);
    lcd.print("TO THE NET");
    delay(3000);

  }
  free(hash); //что то освобождаем
  free(md5str);
  client.stop(); //останавливаем клиент
}


void setup()
{

  Ethernet.begin(mac);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  start_screen ();//вызываем функцию очищающую дисплей
  
  pinMode(relay_pin, OUTPUT);  //пин реле на запись
  digitalWrite(relay_pin, LOW); //подаем на реле единичку, закрывая замок
}


void loop()
{
  char keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY) { //если на кейпаде что то нажато

    //здесь мы определяем на какой позиции пароля или логина мы сейчас находимся и заполняем массивы а также выводим символы на экран
    if (curPosy == 0)
    {
      if (curPosx >= 0 && curPosx < 4)
      {
        if (keypressed >= '0' && keypressed <= '9')
        {
          lcd.print(keypressed);
          switch (curPosx)
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
          curPosx++;
        }
      }
      if (curPosx == 4)
      {
        curPosy = 1;
        curPosx = 0;
        lcd.setCursor(0, 1);
      }
    }
    else if (curPosy == 1)
    {
      if (curPosx >= 0 && curPosx < 4)
      {
        if (keypressed >= '0' && keypressed <= '9') {
          lcd.print('*');
          switch (curPosx) {
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
          curPosx++;
        }
      }
      if ((curPosx == 4) && (keypressed == '#')) { //если мы нажали сеточку очищаем дисплей
        lcd.clear(); //очищаем дисплей
        lcd.print("WAIT...");
        analyze(); //соединяемся с сервером и проверяем данные
        delay(1000);
        start_screen();
      }
    }
  }
}
