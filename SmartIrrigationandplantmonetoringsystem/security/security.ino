#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Key.h>
#include <Keypad.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD address and dimensions
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 13, 12, 11, 10 };
byte colPins[COLS] = { 9, 8, 7 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// Variables
String correctPassword[3] = { "1234", "4567", "6789" };
int counter = 3;
bool flag = true;



void setup() {
  lcd.init();  // LCD setup
  lcd.backlight();
 
}

void loop() {
  if (flag) {
    while (1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter your Id");
      String enteredPassword = getPassword();
      if (enteredPassword == correctPassword[0] || enteredPassword == correctPassword[1] || enteredPassword == correctPassword[2]) {
        counter = 3;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Welcome user");
        delay(250);
        lcd.clear();
        flag = !flag;
        break;
      } else {
        counter--;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ACCESS DENIED");
        lcd.setCursor(0, 1);
        lcd.print("Attempts left: ");
        lcd.print(counter);
        delay(250);
        if (counter == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("HELP ?");
          lcd.setCursor(0, 1);
          lcd.print("CALL: 16591");
          delay(500);
          lcd.clear();
          counter = 3;
        }
        flag = flag;
        break;
      }
    }
  } 
 char key = keypad.getKey();
 if (key == '#') {  // Confirm the password
  flag = !flag;
  lcd.print("Logging out......");
  delay(250);    
}
}
// Function to get password
String getPassword() {
  int count=0;
  String password = "";
  char key;
  lcd.setCursor(0, 1);

  while (1) {
    key = keypad.getKey();
    if (key) {
      if (key == '*') {  // Delete the last character
        if (password.length() > 0) {
          password.remove(password.length() - 1);
          lcd.setCursor(password.length(), 1);
          lcd.print(' ');  // Clear the last character on LCD
          lcd.setCursor(password.length(), 1);
        }
      } else if (key == '#') {  // Confirm the password
        if (password.length() == 4 ) {  // Only confirm if the password is 4 characters long
          return password;
          
        }  
        else {
          lcd.setCursor(0, 2);
          lcd.print("unknown");
          delay(500);
          lcd.setCursor(0, 2);
          lcd.print("          ");  // Clear the message
        }
      } else if (password.length() < 4) {
        password += key;
        lcd.print('*');
      }
    }
  }
}