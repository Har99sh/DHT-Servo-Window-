#include <DHT.h>
#define PIN_SENSOR 7
DHT dht (PIN_SENSOR, DHT11);

#include <Servo.h>
#define PIN_SERVOMOTOR 8
Servo SM;

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);

const int LED1=13;
const int LED2=12;
const int LED3=11;
int PUSHIN = 4;
int PB = 0;
bool MUSTSD;


/* 
SM = Servomotor
WS = Window setting
MD = Menu display
MA = Manual
AU = Automatic
PB = Pushbotton
LED1 = GREEN Full Open
LED2 = YELLOW In Between
LED3 = RED Full Closed
*/



int MD;
int WS;
int MA;
int AU;



void menu()
{
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(PUSHIN, INPUT);
  lcd.begin(16, 2);
  lcd.setBacklightPin (3, POSITIVE);
  lcd.setBacklight (HIGH);
  lcd.setCursor (0, 0);
  lcd.clear();
  lcd.home();
  Serial.begin(9600);
  lcd.print("1.T y H / 2.Auto");
  lcd.setCursor (0,1);
  lcd.print("3.Manual set");
  dht.begin();
  SM.attach (PIN_SERVOMOTOR);
  
}

void setup()
{
 MUSTSD=false;
 Serial.flush();
 menu();
 attachInterrupt(digitalPinToInterrupt(3),SHUTDOWN,FALLING); 
}


void loop()
{
  Serial.flush();
  while (Serial.available() > 0)
  {
     if(MUSTSD==true)
     {  
      setup();
      }
    MD = Serial.parseInt();
      if (MD == 1)
      {
        MENUDHT();
      }
      else if (MD == 2)
      {
        MENUAWS();
      }
      else if (MD == 3)
      {
        MENUWS();
      }
  }      
}

void MENUDHT()
{
  lcd.clear();
  float T = dht.readTemperature();
  lcd.setCursor (0, 0);
  lcd.print("Temperature:" + String (T) + "ºC");
  float h = dht.readHumidity();
  lcd.setCursor (0, 1);
  lcd.print("Humidity:" + String (h) + "%");
  delay(5000);
  lcd.clear();
  menu();
}

void MENUAWS()
{
  float T = dht.readTemperature();
  if (T <= 15) 
  {SM.write (0);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  delay(1000);} 
  else if ((T > 15) && (T <= 20))
  {SM.write (45);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  delay(1000);}
  else if((T > 20) && (T <= 25))
  {SM.write (90);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  delay(1000);}
  else if((T > 25) && (T <= 30))
  {SM.write (135);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  delay(1000);}
  else if(T > 30)
  {SM.write(180);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  delay(1000);}
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print ("3.Back to MENU");
  Serial.flush();
  AU = 0;
  while (AU==0)
   {
     if(MUSTSD==true)
     {  
      setup();
     }
    if(Serial.available() > 0)
    {
     AU = Serial.parseInt();
     if (AU == 3)
     {menu();}
    }
   }
}

void MENUWS()
{
   lcd.clear ();
   lcd.setCursor (0, 0);
   lcd.print ("1.F Cld/2.F Opn");
   lcd.setCursor (0, 1); 
   lcd.print ("3.Back to MENU");

   Serial.flush();
   MA = 0;
   
   while (MA==0)
    {
     if(MUSTSD==true)
     {  
      setup();
     }
     if(Serial.available() > 0)
     {
      MA = Serial.parseInt(); 
    
      if (MA == 1)
      {SM.write (0);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      delay(1000);
      MENUWS();} 
      else if (MA == 2)      
      {SM.write (180);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(1000);      
      MENUWS();} 
      else if (MA == 3)      
      {menu();}
     }
    }
}

void SHUTDOWN()
{
  SM.write (0);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  MUSTSD = true;
}
