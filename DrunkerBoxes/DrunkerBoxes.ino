#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//WiFi Configuration (change these to your WiFi settings
#define WiFi_AP "OnePlus"
#define WiFi_PW "test1234"
#define WiFi_retry 500  // How many milliseconds before trying to connect to WiFi again?

//Other DrunkerTracker Settings
#define dt_pollingRate 57000  // How many seconds before checking the status again

//OLED Configuration
// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
#define LOGO16_GLCD_HEIGHT 48
#define LOGO16_GLCD_WIDTH  64
  

Adafruit_SSD1306 display(OLED_RESET); // Make a display and name it "display"
HTTPClient http;  // Make an HTTP Client named "http"

static const unsigned char PROGMEM logo16_glcd_bmp[] ={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x01, 0xD0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF0, 0x00, 0x03, 0xFF, 0xFF, 0x00,
  0x07, 0xFF, 0xF0, 0x00, 0x07, 0xFF, 0xFF, 0x80, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0xFE,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0xFF, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x83,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0xFF, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0xFE,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80,
  0x07, 0xFF, 0xF0, 0x00, 0x07, 0xFF, 0xFF, 0x80, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80,
  0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80, 0x07, 0xFF, 0xF0, 0x00, 0x06, 0x00, 0x01, 0x80,
  0x07, 0xFF, 0xF0, 0x00, 0x07, 0xFF, 0xFF, 0x80, 0x07, 0xFF, 0xF0, 0x00, 0x03, 0xFF, 0xFF, 0x00

};

//#if (SSD1306_LCDHEIGHT != 48)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif

void setup()   {
  // Set up Serial communication from the board to the computer
  Serial.begin(115200);

  //Set up a wifi connection
  //These are configure at the top of this sketch
  WiFi.begin(WiFi_AP, WiFi_PW);

  // Set up the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // Connect to the WiFi
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) // loop while the wifi status is not equal to 'true'
  {
    delay(WiFi_retry); // wait a bit
    Serial.print("."); // make the loading bar grow
    // This loop will never break for as long as it cannot connect to the network
  }
  // Once connected, the loop will break and display the device network address
  Serial.println(); // skip a line
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

}

void resetText() {
  //This function
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.display();
}

void loop() {
  resetText();
  // Moved up the Bitmap for testing 
  display.drawBitmap(0, 0, logo16_glcd_bmp, LOGO16_GLCD_WIDTH, LOGO16_GLCD_HEIGHT, 1);
  display.display();
  delay(3000);
  resetText();
  display.println(" Checking ");
  display.println("   ...    ");
  display.display();
  delay(1000);
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    http.begin("http://drunkerbot.hardwareflare.com/api/status/state");  //Check the bot API for the status
    if (http.GET() > 0) { //Check the returning code
      if (http.getString() == "true") { //Get the body of the reply and show it on the OLED

        http.end(); // End this http request so we can start a new one
        http.begin("http://drunkerbot.hardwareflare.com/api/status/host");  //Check the bot API for the streamer's name
        if (http.GET() > 0) {
          resetText();  // Get the screen ready for a message
          display.println("DrunkerBox");
          display.println("Hosted by:");
          display.println(http.getString());
          display.display();
        }
        http.end();
      }
      else {
        resetText();  // Get the screen ready for a message
        display.println("DrunkerBox");
        display.println(" Not Live ");
        display.display();
      }
    }
    else {
      http.end();   //Close connection
    }
  }
  delay(dt_pollingRate);  // Wait for the polling rate before checking again
  resetText();
  display.drawBitmap(0, 0, logo16_glcd_bmp, LOGO16_GLCD_WIDTH, LOGO16_GLCD_HEIGHT, 1);
  display.display();
  delay(3000);
}
