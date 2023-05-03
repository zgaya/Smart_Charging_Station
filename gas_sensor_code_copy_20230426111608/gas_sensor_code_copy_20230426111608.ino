#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); /*I2C address 0x27, 16 column and 2 rows*/
int redLed = 12;
int greenLed = 11;
int buzzer = 13;
int smokeA0 = A0;
char data;
int sensorThres = 750; /*Your threshold value*/

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);

  
  lcd.begin(16, 2); /*16 colones 2 lignes*/
 
 
  lcd.init(); /*initialize the lcd*/
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("WELCOME");
  lcd.setCursor(5,1); 
  Serial.begin(9600);/*permet d’initialiser la liaison Série à 9600 */
}

void loop() {
  int analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor); /*Checks if it has reached the threshold value*/ 
  

     if(Serial.available()>0)
    {
      data=Serial.read();
      if(data == '1') 
      {
        if(analogSensor>sensorThres)
        {   
            lcd.clear();
            Serial.write("ON");
            digitalWrite(buzzer,HIGH);
            delay(1000);
             String CIN="";
      for(int i=0;i<16;i++)
          {   
            
               data=Serial.read();
               CIN+=data;
               lcd.setCursor(3,0);
               lcd.print(CIN);
               lcd.setCursor(0,1);
               lcd.print("Employe Notifié" );        
          }
        }
        else {
             lcd.clear();
             lcd.setCursor(0,0);
             lcd.print("Alarme non activé");
             lcd.setCursor(0,1);
             lcd.print("Fausse Alerte" );
             }
      }
      else if(data == '0') 
      {   
            lcd.clear();
            Serial.write("OFF"); 
            digitalWrite(buzzer,LOW);  
            data=Serial.read();
            String CIN="";
         for(int i=0;i<16;i++)
           {    
             data=Serial.read();
             CIN+=data;
             lcd.setCursor(0,0);
             lcd.print(CIN);
             lcd.setCursor(0,1);
             lcd.print("Quitter" );        
          }
       
    }
  }    
}
