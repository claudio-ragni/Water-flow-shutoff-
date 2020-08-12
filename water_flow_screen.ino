#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
float total_flow = 0;
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_min; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int button1 = A0;
int button2 = A1;
int state1=0;
int state2=0;
int in1 = 7;
int i = 0;
int max_amount=5;
String print_="";
void flow () // Interrupt function
{
   flow_frequency++;
}
void setup()
{
   pinMode(flowsensor, INPUT);
   pinMode(in1, OUTPUT);
   digitalWrite(in1, HIGH);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
   lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
   lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
}
void loop ()
{
      state1=analogRead(button1);
      state2=digitalRead(button2); 
      if (state1 > 1020){
        max_amount=max_amount+1;
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Max amount");
        lcd.print(max_amount);
        delay(700);
      }
      if (state2 > 1020){
        max_amount=max_amount-1;
        if (max_amount < 1){
          max_amount=1;
        }
}
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      state1=analogRead(button1);
      state2=digitalRead(button2); 
      if (state1 > 1020){
        max_amount=max_amount+1;
        lcd.setCursor(0,1);
        lcd.print("Max amount");
        lcd.print(max_amount);
      }
      if (state2 > 1020){
        lcd.setCursor(0,1);
        lcd.print("Max amount");
        lcd.print(max_amount);
        max_amount=max_amount-1;
        Serial.println("Hey");
        if (max_amount < 1){
          max_amount=1;
        }
      }
      cloopTime = currentTime; // Updates cloopTime
      l_min = (flow_frequency/ 7.5); 
      total_flow=(total_flow+(l_min*0.1));
      flow_frequency = 0; // Reset Counter
      lcd.setCursor(0,0); //First line
      lcd.print(l_min*0.1);
      lcd.print(" L/min");
      lcd.setCursor(0,1);
      lcd.print(total_flow);
      lcd.print(" Total flow");
      if (total_flow>max_amount )
   {
    digitalWrite(in1, LOW);   
   }
   }
}
