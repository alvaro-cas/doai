#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneButton.h>

// Init display 0x3C
Adafruit_SSD1306 display(128, 32, &Wire, -1);

// Declare buttons
OneButton button7(7, true);
OneButton button8(8, true);

// Menu names
const char *txtMENU[] = {
  "  Get CPU \n  data",
  "  Get GPU \n  data",
  "  Get RAM \n  data",
  "  Get STORAGE \n  data",
};
 
const byte itemsMENU = sizeof(txtMENU)/sizeof(*txtMENU);
static byte menuSelec = 0;
int positionY = 0;
int seconds = 3;

// Get graphics from file
extern const unsigned char graphics [];

void setup() {
  Serial.begin(9600);

  // Init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  // Show menu
  menu(2);
  display.display();
  delay(200);

  // Attach actions to buttons
  button7.attachClick(prev);
  button8.attachClick(next);

  button8.attachDoubleClick(getData);
}

void loop() {
  button7.tick();
  button8.tick();
}

void menu(int op) {
  for (int i=0; i<8; i++) {
    if (op == 0) {
      positionY -= 4;
    } else if (op == 1) {
      positionY += 4;
    }
    // Set text properties
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Change position according to action next or prev
    for (int i=0; i<itemsMENU; i++) {
      display.setCursor(0, -positionY+9+(32*i));
      display.print(txtMENU[i]);
    }
    display.drawBitmap(80, -positionY, graphics, 45, 128, 1);
    display.display();
  }
}

void prev() {
  // Move menu backwards
  if(menuSelec > 0) {
    menuSelec -= 1;
    menu(0);
  }
}

void next() {
  // Move menu fordward
  if(menuSelec < itemsMENU-1) {
    menuSelec += 1;
    menu(1);
  }
}

void getData() {
  // Set bar to full
  int barCounter = 128;

  for (int i=0; i<seconds; i++) {
    // Substract value depending on seconds
    barCounter -= barCounter/seconds;

    // Get Serial data and convert to array
    String receivedData = Serial.readString();
    String data[25];
    int r=0,t=0;
    for(int i=0;i<receivedData.length();i++) {
      if(receivedData[i] == ',') {
        if (i-r > 1) {
          data[t] = receivedData.substring(r,i);
          t++;
        }
        r = (i+1);
      }
    }

    // Display data depending on Menu selected
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.setTextSize(2);
    display.print(data[menuSelec]);
    display.fillRect(0, 28, barCounter, 4, SSD1306_WHITE);
    display.display();
  }
  menu(2);
}
