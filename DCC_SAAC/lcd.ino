#define SPEED_CHANGE_FACTOR 50 // how fast speed should change when holding the button. Lower value is faster

#define MENU_MAIN 1
#define MENU_SPEED 10
#define MENU_FUNCTION 11

int currentMenu = 1;
int prevMenu = 1;

int selectedTrain = 0;
int selectedFunction = 0;


void initLCD() {

  lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
// lcd.print(trains[0].getName()); // print a simple message
}


void lcdLoop() {

  lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
  lcd.print(currentDraw);      // display seconds elapsed since power-up


  lcd.setCursor(0,1);            // move to the begining of the second line
  
}

void drawMenu() {

  switch (currentMenu) {
    case MENU_SPEED: {
        subMenuSpeed();
        break;
    }
    case MENU_MAIN: {
        menuMain();
        break;
    }
    case MENU_FUNCTION: {
        subMenuFunction();
        break;
    }
  }

}

void menuMain() {
  
  lcd.setCursor(0,0);
  lcd.print("Main Menu");
  lcd.setCursor(11,0);
  if (trackPower) {
    lcd.print("P:On ");
  } else {
    lcd.print("P:Off");
  }
  lcd.setCursor(12,1);
  lcd.print(currentDraw);
  lcd.print("    ");
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.setCursor(2,1);
  lcd.print(">");
  lcd.print(trains[selectedTrain].getName()); // print a simple message
  if ( btnPressed == btnSELECT) {
    btnPressed = 0;
    currentMenu = MENU_SPEED;
    lcd.clear();
    return;
  }
  if ( btnPressed == btnRIGHT) {
    btnPressed = 0;
    selectNextTrain();
  }
  if ( btnPressed == btnLEFT) {
    btnPressed = 0;
    selectPrevTrain();
  }
  if ( btnPressed == btnUP) {
    trackPower = !trackPower;
    btnPressed = 0;
  }
  
  if (selectedTrain >= MAX_TRAINS) {
    selectedTrain = 0;
  }
  if (selectedTrain < 0) {
    selectedTrain = MAX_TRAINS-1;
  }
}

void selectNextTrain() {
  selectedTrain++;
  bool maxHit = false;
  while (!trains[selectedTrain].isConfigured() && !maxHit) {
    selectedTrain++;
    if (selectedTrain >= MAX_TRAINS) {
      selectedTrain = 0;
    }
  }
}
void selectPrevTrain() {
  selectedTrain--;
  bool maxHit = false;
  while (!trains[selectedTrain].isConfigured() && !maxHit) {
    selectedTrain--;
    if (selectedTrain < 0) {
      selectedTrain = MAX_TRAINS-1;
    }
  
  }
  
}

void testMenu() {
  lcd.setCursor(9,0);
  if (btnPressed) {
    switch (btnHold)    {          // depending on which button was pushed, we perform an action
 
      case btnRIGHT: {
        lcd.print("RIGHT ");
        break;
      }
      case btnLEFT:  {
        lcd.print("LEFT   ");
        break;
      }
      case btnUP:   {
        lcd.print("UP    ");
         break;
      }
      case btnDOWN:  {
        lcd.print("DOWN  ");
        break;
      }
      case btnSELECT: {
         lcd.print("SELECT");
        break;
      }
      case btnNONE:  {
        //lcd.print("NONE  ");
        break;
      }
    }
  }
  lcd.setCursor(0,1);
  if (btnHold != btnNONE) {
    lcd.print(longPressTime);
  }
  switch (btnHold)    {          // depending on which button was pushed, we perform an action
 
    case btnRIGHT: {
      lcd.print("RIGHT ");
      digitalWrite(DCC_POWER, HIGH);
      break;
    }
    case btnLEFT:  {
      lcd.print("LEFT   ");
      break;
    }
    case btnUP:   {
      lcd.print("UP    ");
       break;
    }
    case btnDOWN:  {
      lcd.print("DOWN  ");
      break;
    }
    case btnSELECT: {
       lcd.print("SELECT");
      break;
    }
    case btnNONE:  {
      //lcd.print("NONE  ");
      digitalWrite(DCC_POWER, LOW);
      break;
    }
  }
}
void subMenuSpeed() {
  if ( btnHold == btnRIGHT) {
    btnHold = 0;
    
    unsigned long change = (longPressTime-MIN_BUTTON_TRIGGER)/SPEED_CHANGE_FACTOR;
    if (change>=1) {
      longPressTime = MIN_BUTTON_TRIGGER;
      int currentSpeed = trains[selectedTrain].getSpeed();
      currentSpeed += (int)change;
      trains[selectedTrain].setSpeed(currentSpeed);
    }
  }
  if ( btnHold == btnLEFT) {
    btnHold = 0;
    unsigned long change = (longPressTime-MIN_BUTTON_TRIGGER)/SPEED_CHANGE_FACTOR;
    if (change>=1) {
      longPressTime = MIN_BUTTON_TRIGGER;
      int currentSpeed = trains[selectedTrain].getSpeed();
      currentSpeed -= (int)change;
      trains[selectedTrain].setSpeed(currentSpeed);
    }
  }
  if ( btnPressed == btnRIGHT) {
    btnPressed = 0;
    
    
      int currentSpeed = trains[selectedTrain].getSpeed();
      currentSpeed++;
      trains[selectedTrain].setSpeed(currentSpeed);
    
  }
  if ( btnPressed == btnDOWN) {

    trains[selectedTrain].setSpeed(0);
    btnPressed = 0;
    Serial.println("Stop train");
  }
  if ( btnPressed == btnSELECT) {
    btnPressed = 0;
    currentMenu = MENU_MAIN;
    lcd.clear();
    return;
  }
  if ( btnPressed == btnUP) {
    Serial.println("Change menu func");
    btnPressed = 0;
    currentMenu = MENU_FUNCTION;
    lcd.clear();
    return;
  }
  
  lcd.setCursor(0,0);
  lcd.print(trains[selectedTrain].getName()); // print a simple message
  lcd.setCursor(9,0);
  lcd.print("S:");
  if (trains[selectedTrain].getFormat() == MOTOROLA) {
    lcd.print(trains[selectedTrain].getSpeed()/9);
    lcd.print("    ");
    lcd.setCursor(9,1);
    if (trains[selectedTrain].currentDirection) {
      lcd.print("Forward");
    } else {
      lcd.print("Reverse");
    }
  } else {
    lcd.print(trains[selectedTrain].getSpeed());
  }
  
  lcd.print("    ");
}
void subMenuFunction() {

  if ( btnPressed == btnRIGHT) {
    selectedFunction++;
    if (selectedFunction>12) {
      selectedFunction=12;
    }

    btnPressed = 0;

  }
  if ( btnPressed == btnLEFT) {
    selectedFunction--;
    if (selectedFunction<0) {
      selectedFunction=0;
    }
    btnPressed = 0;

  }
  if ( btnPressed == btnDOWN) {

    trains[selectedTrain].toggleFunction(selectedFunction);
    btnPressed = 0;

  }
  if ( btnPressed == btnSELECT) {
    btnPressed = 0;
    currentMenu = MENU_MAIN;
    lcd.clear();
    return;
  }
  if ( btnPressed == btnUP) {
    btnPressed = 0;
    currentMenu = MENU_SPEED;
    lcd.clear();
    return;
  }
  
  lcd.setCursor(0,0);
  lcd.print(trains[selectedTrain].getName()); // print a simple message
  lcd.setCursor(0,1);
  lcd.print("F:");
  lcd.print(selectedFunction);
  lcd.print("  ");
  lcd.setCursor(5,1);
  if ( trains[selectedTrain].getFunction(selectedFunction) ) {
    lcd.print("On ");
  } else {
    lcd.print("Off");
  }
}
