#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>

#define OLED_CLK   2
#define OLED_MOSI  3
#define OLED_RESET 4
#define OLED_DC    5
#define OLED_CS    6
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define RECV_PIN   7
IRrecv irrecv(RECV_PIN);
decode_results results;

int data[128];
int end = 0;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.display();
  display.clearDisplay();

  for (int i = 0; i < display.width(); i++) {
    data[i] = 0;
  }
  // irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  int cpu = -1;
  if (Serial.available() > 0) {
    cpu = Serial.read();
    Serial.flush();
  }
  // checkIr();

  drawDisplay(cpu);
  delay(50);
}

void test() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print("test");
  display.display();

}

void drawDisplay(int cpu) {
  display.clearDisplay();
  graph(data, &end, cpu);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print(cpu);
  display.print("%");
  display.display();
}

void graph(int *data, int *end, int new_val) {
  data[*end] = new_val;
  *end = *end + 1;
  drawGraph(data, display.width(), *end);
  if (*end >= display.width()) {
    *end = 0;
  }
}

void drawGraph(int data[], int len, int end) {
  int i = 0;
  for (int x = end; x < len; x++) {
    drawLineBegin(i++, getYBegin(data[x]));
  }
  for (int x = 0; x < end; x++) {
    drawLineBegin(i++, getYBegin(data[x]));
  }
}

int getYBegin(int value) {
  int height = display.height() - 15;
  return display.height() - (value * height / 100);
}

void drawLineBegin(int x, int y) {
  display.drawLine(x, y, x, display.height(), WHITE);
}

void checkIr() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();
  } else {
    Serial.println(1234, HEX);
  }
}

