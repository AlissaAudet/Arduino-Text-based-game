#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#define LCD_ADDR 0x27  
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);
const int button1 = 4; 
const int button2 = 2;
const int ledPin1 = 7;
const int ledPin2 = 8;

bool button1Prev;
bool button2Prev;

//game logic variable
int gameState = 0; 
unsigned long lastScrollTime = 0;
bool stateJustChanged = true;
bool readyForEndTransition = false;
bool waitingForButtonRelease = false;


//sprite
byte runner1[8] = {
  B00100,
  B01110,
  B00100,
  B01110,
  B10101,
  B00100,
  B01010,
  B10001
};

byte runner2[8] = {
  B00100,
  B01110,
  B00100,
  B01110,
  B10101,
  B00100,
  B10001,
  B01010
};

//Scroll the text
void scrollIfTimeElapsed(unsigned long &lastScrollTime, unsigned long interval = 300) {
  if (millis() - lastScrollTime > interval) {
    lcd.scrollDisplayLeft();
    lastScrollTime = millis();
  }
} 

void setup() {
  lcd.init();         
  lcd.backlight(); 
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  button1Prev = digitalRead(button1);
  button2Prev = digitalRead(button2);
  
  lcd.createChar(0, runner1);
  lcd.createChar(1, runner2);
  
  Serial.begin(9600);
}

void loop() {
  bool button1Current = digitalRead(button1);
  bool button2Current = digitalRead(button2);

  bool button1JustPressed = (button1Prev == HIGH && button1Current == LOW);
  bool button2JustPressed = (button2Prev == HIGH && button2Current == LOW);  

  switch (gameState) {
    case 0:
      if (stateJustChanged) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" You need to find your way out of a cave");
        lcd.setCursor(0, 1);
        lcd.print("     Press any button   Press any button");
        lastScrollTime = millis();
        stateJustChanged = false;
        waitingForButtonRelease = false; 
        readyForEndTransition = false;
      }

      scrollIfTimeElapsed(lastScrollTime);

      if (button1JustPressed || button2JustPressed) {
        gameState = 1;
        stateJustChanged = true;
      }
      break;

    case 1:
      if (stateJustChanged) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("1. Go Left");
        lcd.setCursor(0, 1);
        lcd.print("2. Go Right");
        stateJustChanged = false;
      }

      if (button1JustPressed) {
        gameState = 2;
        stateJustChanged = true;
      } else if (button2JustPressed) {
        gameState = 3;
        stateJustChanged = true;
      }
      break;

    case 2:
      if (stateJustChanged) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A dragon appear");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("1. Hug dragon");
        lcd.setCursor(0, 1);
        lcd.print("2. Kill dragon");
        stateJustChanged = false;
      }

      if (button1JustPressed) {
        gameState = 4;
        stateJustChanged = true;
      } else if (button2JustPressed) {
        gameState = 5;
        stateJustChanged = true;
      }
      break;

    case 3:
      if (stateJustChanged) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gollum appear");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("1. Steal Ring");
        lcd.setCursor(0, 1);
        lcd.print("2. Run");
        stateJustChanged = false;
      }

      if (button1JustPressed) {
        gameState = 6;
        stateJustChanged = true;
      } else if (button2JustPressed) {
        gameState = 7;
        stateJustChanged = true;
      }
      break;

    case 4:
      if (stateJustChanged) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" The dragon help you escape");
        lcd.setCursor(0, 1);
        lcd.print(" The end. Press any button to try again");
        lastScrollTime = millis();
        stateJustChanged = false;
        readyForEndTransition = true;
      }
      scrollIfTimeElapsed(lastScrollTime);
      break;

    case 5:
      if (stateJustChanged) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" The dragon eat you alive");
        lcd.setCursor(0, 1);
        lcd.print(" The end. Press any button to try again");
        lastScrollTime = millis();
        stateJustChanged = false;
        readyForEndTransition = true;
      }
      scrollIfTimeElapsed(lastScrollTime);
      break;

    case 6:
      if (stateJustChanged) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" You changed the course of history");
        lcd.setCursor(0, 1);
        lcd.print(" The end. Press any button to try again");
        lastScrollTime = millis();
        stateJustChanged = false;
        readyForEndTransition = true;
      }
      scrollIfTimeElapsed(lastScrollTime);
      break;

    case 7:
      if (stateJustChanged) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" You fall in a hole ;)");
        lcd.setCursor(0, 1);
        lcd.print(" The end.  Press any button to try again");
        lastScrollTime = millis();
        stateJustChanged = false;
        readyForEndTransition = true;
      }
      scrollIfTimeElapsed(lastScrollTime);
      break;

    case 8:
      for (int i = 0; i < 16; i++) {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("try again");
        lcd.setCursor(i, 0);
        lcd.write(byte(i % 2));
        delay(200);
      }
      gameState = 0;
      stateJustChanged = true;
      readyForEndTransition = false;
      waitingForButtonRelease = true;
      break;
  }

  // Transition to animation if end screen and button pressed (but only once released)
  if (waitingForButtonRelease) {
    if (button1Current == HIGH && button2Current == HIGH) {
      waitingForButtonRelease = false;
    }
  } else {
    if ((gameState == 4 || gameState == 5 || gameState == 6 || gameState == 7) &&
        readyForEndTransition &&
        (button1JustPressed || button2JustPressed)) {
      gameState = 8;
      stateJustChanged = true;
      readyForEndTransition = false;
      waitingForButtonRelease = true;
    }
  }
  
  if (waitingForButtonRelease &&
    button1Current == HIGH &&
    button2Current == HIGH) {
  waitingForButtonRelease = false;
}


  // LED indicators
  if (button1JustPressed) {
    digitalWrite(ledPin1, HIGH);
    delay(300);
    digitalWrite(ledPin1, LOW);
  }

  if (button2JustPressed) {
    digitalWrite(ledPin2, HIGH);
    delay(300);
    digitalWrite(ledPin2, LOW);
  }

  // Update previous states
  button1Prev = button1Current;
  button2Prev = button2Current;
}




