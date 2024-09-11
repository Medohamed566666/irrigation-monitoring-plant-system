#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD address and dimensions
             

// Analog sensors
#define moisture A0
#define LM35 A1
//digital sensors
#define flame 9

// Digital pins
#define buzzer 28
//pump
#define EN 7
#define IN1 25
#define IN2 26
//fan1
#define ENA 48
#define IN3 49
#define IN4 50
//fan2
#define ENB 53
#define IN5 51
#define IN6 52

#define SW 22  // Switch for auto display
#define Pb 23  // Push button for manual display
//Mouse trap 
#define Relay 40
#define IR 41

// Functions
void moistureState();
void tempState();
void flameState();
void mouseTrap();


// Variables
bool autoDisplay = true;  // Mode: Auto or Manual display
int counter2 = 4;

void setup() {
  lcd.init();  // LCD setup
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(2,1);
  lcd.print("and plant monetor");
  lcd.setCursor(3,2);
  lcd.print("system");
  delay(500);
  lcd.clear();
  

  // Analog sensors setup
  pinMode(moisture, INPUT);
  pinMode(LM35, INPUT);
  pinMode(flame, INPUT);

  // Digital devices setup
  pinMode(buzzer, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
   pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
   pinMode(ENB, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);

  // Switches and buttons
  pinMode(SW, INPUT_PULLUP);
  pinMode(Pb, INPUT_PULLUP);

  //mouse trap
  pinMode(Relay,OUTPUT);

}

void loop() {


  if (digitalRead(SW) == LOW) {
    autoDisplay = true;  // Set to auto display
  } else {
    autoDisplay = false;  // Set to manual display
  }

  
    if (autoDisplay) {
      // Auto display mode: display sensor states automatically
      moistureState();
      delay(500);  
      tempState();
      delay(500);  
      flameState();
      delay(500);  
      mouseTrap();
      delay(500);
    } else {
      // Manual display mode
       while (1) {
        // Continuously update and display the sensor state based on counter2 value
        if (counter2 == 4) {
            moistureState();  
        } else if (counter2 == 3) {
            tempState();      
        } else if (counter2 == 2) {
            flameState();     
        }
        else if (counter2 == 1) {
            mouseTrap();    
        }
        // delay to reduce overload
        delay(250);

            if (digitalRead(Pb) == LOW) {    
                counter2--; // Decrement counter2 to switch to the next sensor
                if (counter2 < 1) {
                    counter2 = 4; // Reset counter2 to loop through sensors
                }
                // until the button is released
                while (digitalRead(Pb) == LOW) {
                    delay(10);
                }
               
            }
             break; // Exit the loop to update the display mode
       
    }
      
    }
  
}

void moistureState() {
  float moistureLevel = analogRead(moisture);
  float mapValue= map(moistureLevel,0,1023,0,100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(mapValue);
  lcd.print("%");

  // Control motor based on moisture level
  if (mapValue < 40) {
    analogWrite(EN, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    lcd.setCursor(0, 1);
  lcd.print("state: LOW ");
  lcd.setCursor(0, 2);
  lcd.print("PUMP IS ON ");
  } else {
    digitalWrite(EN, LOW);
    lcd.setCursor(0, 1);
  lcd.print("State: NORMAL ");
  lcd.setCursor(0, 2);
  lcd.print("PUMP IS OFF ");
  }
  
}


void tempState() {
  float val = analogRead(LM35);
  float volt = (val * 5.0 / 1023.0); 
  float temp = (volt / 0.01);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);

  // Control fan 1 based on temperature
  if (temp > 30) {
    lcd.setCursor(0, 1);
    lcd.print("high than normal");
    lcd.setCursor(0, 2);
    lcd.print("FAN 1: ON");
      // Turn fan on
     digitalWrite(ENA, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("normal temperature ");
    lcd.setCursor(0, 2);
    lcd.print("FAN 1: OFF");
    // Turn fan off
    digitalWrite(ENA, LOW);
  }
  
}


void flameState() {
  float flameValue = digitalRead(flame);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Flame sensor : ");
  lcd.print(flameValue);

  // Control fan 2 and buzzer based on flame value
  if (flameValue ==1) {
    lcd.setCursor(0, 1);
    lcd.print("WARNING! ");
    lcd.setCursor(0, 2);
    lcd.print("FAN 2: ON ");
    lcd.setCursor(0, 3);
    lcd.print("buzzer: ON ");
      // Turn fan on
    digitalWrite(ENB, HIGH);
    digitalWrite(IN5, HIGH);
    digitalWrite(IN6, LOW);
    digitalWrite(buzzer, HIGH);  // Turn buzzer on
    delay(100);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("it is safe ");
    lcd.setCursor(0, 2);
    lcd.print("FAN 2: OFF ");
    lcd.setCursor(0, 3);
    lcd.print("buzzer: OFF ");
     // Turn fan off
     digitalWrite(ENB, LOW);
    digitalWrite(buzzer, LOW);  // Turn buzzer off
  }
  
}


void mouseTrap(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("mouse trap : ");

  if(digitalRead(IR)==1){
    //buzzer run automaticly(built in connection on proteus)
    digitalWrite(Relay,HIGH);
    lcd.setCursor(0, 1);
    lcd.print("CATCHED! ");
    lcd.setCursor(0, 2);
    lcd.print("Buzzer: ON ");
    lcd.setCursor(0, 3);
    lcd.print("Trap activated ");
  }
  else{
    //buzzer off automaticly(built in connection on proteus)
    digitalWrite(Relay,LOW);
    lcd.setCursor(0, 1);
    lcd.print("there are no Mice ");
  }
}