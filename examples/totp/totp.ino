#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <sha1.h>
#include <TOTP.h>

// Init display 0x3C
Adafruit_SSD1306 display(128, 32, &Wire, -1);

// Keys in HEX
uint8_t key1[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x6f, 0x72};
uint8_t key2[] = {0x54, 0x65, 0x73, 0x74, 0x65, 0x72};

// Array of all Keys
const uint8_t *keys[] = { (uint8_t *)key1,
                          (uint8_t *)key2 };

// Name of Keys
const char *keyNames[] = {  " Account 1",
                            " Account 2" };

// Length of each Key
const int keyLength[] = { 8,
                          6 };

const byte itemsNames = sizeof(keyNames)/sizeof(*keyNames);

void setup() {
  Serial.begin(9600);
 
  // Init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
}

void loop() {
  getCode();
}

void getCode() {
  // Clear and set text properties
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  for (int i=0; i<itemsNames; i++) {
    // Init TOTP
    TOTP totp = TOTP(keys[i], keyLength[i]);
    char code[7];

    // Read Serial string as int
    String msg = Serial.readString();
    msg.trim();
    long GMT = msg.toInt();

    // Get TOTP code
    char* newCode = totp.getCode(GMT);

    // Compare and then copy
    if(strcmp(code, newCode) != 0) {
      strcpy(code, newCode);
      display.setCursor(84, 10*i);
      display.print(code);
      display.setCursor(0, 10*i);
      display.print(keyNames[i]);
    }
  }

  // Display code
  display.display();
}
