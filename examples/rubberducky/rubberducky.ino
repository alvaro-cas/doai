#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneButton.h>
#include <Keyboard.h>

// Init display 0x3C
Adafruit_SSD1306 display(128, 32, &Wire, -1);

// Declare buttons
OneButton button7(7, true);
OneButton button8(8, true);

// Menu names
const char *txtMENU[] = {
  "  Update",
  "  Open Firefox",
  "  Create file"
};
 
const byte itemsMENU = sizeof(txtMENU)/sizeof(*txtMENU);
static byte menuSelec = 0;
int positionY = 0;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();

  // Init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  menu();
  display.display();

  // Attach actions to buttons
  button7.attachClick(prev);
  button8.attachClick(next);

  button8.attachDoubleClick(sendLoad);
}

void loop() {
  button7.tick();
  button8.tick();
}

void menu() {
  display.clearDisplay();
  // Draw rectangle
  display.fillRect(0, 9, 128, 10, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_INVERSE);

  for (int i=1; i<itemsMENU+1; i++) {
    // Set text properties
    display.setCursor(0, -positionY+(10*i));
    display.print(txtMENU[i-1]);
  }

  display.display();
}

void prev() {
  // Move menu backwards
  if(menuSelec > 0) {
    menuSelec -= 1;
    positionY -= 10;
    menu();
  }
}

void next() {
  // Move menu fordward
  if(menuSelec < itemsMENU-1) {
    menuSelec += 1;
    positionY += 10;
    menu();
  }
}

void sendLoad() {
  switch(menuSelec) {
    case 0:
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.releaseAll();
      delay(1000);

      Keyboard.print("sudo pacman -Syu");
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.releaseAll();
      break;
    case 1:
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(112);
      delay(500);
      Keyboard.releaseAll();
      delay(200);

      Keyboard.print("Firefox Developer");
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.releaseAll();
      break;
    case 2:
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.releaseAll();
      delay(1000);

      Keyboard.print("nvim ducky.txt");
      Keyboard.press(KEY_RETURN);
      delay(500);
      Keyboard.releaseAll();

      Keyboard.press(105);
      delay(100);
      Keyboard.releaseAll();

      Keyboard.print("You are hacked!");
      Keyboard.press(KEY_ESC);
      delay(400);
      Keyboard.releaseAll();

      Keyboard.print(":wq");
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.releaseAll();

      Keyboard.print("cat ./ducky.txt");
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.releaseAll();
      break;
    default:
      Serial.print("Default case!");
  }
}


