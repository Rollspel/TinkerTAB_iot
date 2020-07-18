#include <WiFi.h>
#include <SocketIoClient.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 16 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 60 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//18 19 23
uint32_t magenta = pixels.Color(255, 0, 255);
uint32_t yellow = pixels.Color(255, 255, 0);
uint32_t black = pixels.Color(0, 0, 0);
int playerTurn = 1;
int boardState[9] = {9,9,9, 9,9,9 ,9,9,9};

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
int i = 0;


const char* ssid     = "";
const char* password = "";
char host[] = "afternoon-forest-61554.herokuapp.com"; // Socket.IO Server Address
int port = 80; // Socket.IO Port Address
char * string;
const int buttonPin = 26;
int buttonState = 0;
const int rgb1 = 18;
const int rgb2 = 19;
const int rgb3 = 23;

SocketIoClient webSocket;
String stringzero, stringone, stringThree;
WiFiClient client;


void player(const char * payload, size_t length){
  Serial.println(playerTurn);
  Serial.println("=== * PLAYER WIN * ===");
  for(uint16_t i=0; i< 9; i++) {
    boardState[i] = 9;
  }
  Serial.println(payload);
  if(playerTurn == 1){
    colorWipe(yellow, 10);
  }else{
    colorWipe(magenta, 10);
  }
  colorWipe(black, 10);
}

void draw(const char * payload, size_t length){
  Serial.println(playerTurn);
  Serial.println("=== * DRAW * ===");
  for(uint16_t i=0; i< 9; i++) {
    boardState[i] = 9;
  }
  Serial.println(payload);
  colorWipeDraw(20);
  colorWipe(black, 10);
}

void colorWipe(uint32_t c, uint8_t wait) {
  Serial.println("+++++++++++++++++++++++++++++++");
  for(uint16_t i=0; i< NUMPIXELS; i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void colorWipeDraw(uint8_t wait) {
  Serial.println("+++++++++++++++++++++++++++++++");
  for(uint16_t i=0; i< NUMPIXELS; i+=2) {
    pixels.setPixelColor(i, magenta);
    pixels.setPixelColor(i+1, yellow);
    pixels.show();
    delay(wait);
  }
}
 
void setup() {
Serial.begin(115200);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

pinMode(buttonPin, INPUT);
pinMode(rgb1, OUTPUT);
pinMode(rgb2, OUTPUT);
pinMode(rgb3, OUTPUT);
pixels.begin();
Serial.print("Connecting to ");
Serial.println(ssid);
pixels.clear(); 
WiFi.begin(ssid, password);
  
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}

//socket on state of the game
webSocket.on("receive_player_win",player);
webSocket.on("receive_player_draw",draw);

//send his gameboard ID to the server 
webSocket.emit("add_gameboard","\"1022\"");
   webSocket.begin(host, port, "/socket.io/?transport=websocket"); 
  
}

void loop() {
  webSocket.loop();
  
   Serial.println(touchRead(T0)); //io4
  // Serial.println(touchRead(T1)); //io0
  // Serial.println(touchRead(T2)); //io2
  // Serial.println(touchRead(T3)); //TDO
  // Serial.println(touchRead(T4)); //TCK
  // Serial.println(touchRead(T5)); //tdI
  // Serial.println(touchRead(T6)); //TMS
  // Serial.println(touchRead(T7)); //io27
  // Serial.println(touchRead(T8)); //io32
  // Serial.println(touchRead(T9)); //io33
  
   if (touchRead(T0) < 30){
    Serial.println("T0 touched");
    if(boardState[0] == 9 ){
    if(playerTurn == 1){
      boardState[0] = 0;
      pixels.setPixelColor(1, magenta);
      playerTurn = 2;
      webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[0,9,9],[9,9,9],[9,9,9]]}");
    } else{
      boardState[0] = 1;
      pixels.setPixelColor(1, yellow);
      webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[1,9,9],[9,9,9],[9,9,9]]}");
      playerTurn = 1;
      }
      pixels.show();
    }
  } else {
    Serial.println("T0 not touched");
  }
  
  /*if (touchRead(T1) < 50){
    Serial.println("touched");
    webSocket.emit("player_tap","{\"gameboardID\":\"2231\",\"board\":[[9,0,9],[9,9,9],[9,9,9]]}");
  }else {
    Serial.println("not touched");
  }
  
  if (touchRead(T2) < 50){
    Serial.println("touched");
    webSocket.emit("player_tap","{\"gameboardID\":\"2231\",\"board\":[[9,9,0],[9,9,9],[9,9,9]]}");
  }else {
     Serial.println("not touched");
  }*/
  
  if (touchRead(T3) < 30){
    Serial.println("T3 touched");
    if(boardState[1] == 9){
      if(playerTurn == 1){
        boardState[1] = 0;
        pixels.setPixelColor(6, magenta);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,0,9],[9,9,9],[9,9,9]]}");
        playerTurn = 2;
      }else{
        pixels.setPixelColor(6, yellow);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,1,9],[9,9,9],[9,9,9]]}");
        playerTurn = 1;
        boardState[1] = 1;
      }
      pixels.show();
    }
  }else {
     Serial.println("T3 not touched");
  }
  
  if (touchRead(T4) < 30){
    Serial.println("T4 touched");
    if(boardState[2] == 9){
      if(playerTurn == 1){
        boardState[2] = 0;
        pixels.setPixelColor(12, magenta);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,0],[9,9,9],[9,9,9]]}");
        playerTurn = 2;
      }else{
        boardState[2] = 1;
        pixels.setPixelColor(12, yellow);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,1],[9,9,9],[9,9,9]]}");
        playerTurn = 1;
      }
      pixels.show();
    }
  }else {
    Serial.println("T4 not touched");
  }
  
  if (touchRead(T5) < 30){
    Serial.println("T5 touched");
    if(boardState[3] == 9){
      if(playerTurn == 1){
        boardState[3] = 0;
        pixels.setPixelColor(36, magenta);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[0,9,9],[9,9,9]]}");
        playerTurn = 2;
      }else{
        boardState[3] = 1;
        pixels.setPixelColor(36, yellow);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[1,9,9],[9,9,9]]}");
        playerTurn = 1;
      }
      pixels.show();
    }
  }else {
    Serial.println("T5 not touched");
  }
  
  if (touchRead(T6) < 30){
    Serial.println("T6 touched");
    if(boardState[4] == 9){
      if(playerTurn == 1){
        boardState[4] = 0;
        pixels.setPixelColor(30, magenta);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,0,9],[9,9,9]]}");
        playerTurn = 2;
      }else{
        boardState[4] = 1;
        pixels.setPixelColor(30, yellow);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,1,9],[9,9,9]]}");
        playerTurn = 1;
      }
      pixels.show();
    }
  }else {
    Serial.println("T6 not touched");
  }
  
  if (touchRead(T7) < 30){
    Serial.println("T7 touched");
    if(boardState[5] == 9){
      if(playerTurn == 1){
        boardState[5] = 0;
        pixels.setPixelColor(25, magenta);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,9,0],[9,9,9]]}");
        playerTurn = 2;
      }else{
        boardState[5] = 1;
        pixels.setPixelColor(25, yellow);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,9,1],[9,9,9]]}");
        playerTurn = 1;
      }
      pixels.show();
    }
  }else {
    Serial.println("T7 not touched");
  }
  
  if (touchRead(T8) < 30){
    Serial.println("T8 touched");
    if(boardState[6] == 9){
      if(playerTurn == 1){
        boardState[6] = 0;
        pixels.setPixelColor(48, magenta);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,9,9],[0,9,9]]}");
        playerTurn = 2;
      }else{
        boardState[6] = 1;
        pixels.setPixelColor(48, yellow);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,9,9],[1,9,9]]}");
        playerTurn = 1;
      }
      pixels.show();
    }
  }else {
    Serial.println("T8 not touched");
  }
  
  if (touchRead(T9) < 30){
    Serial.println("T9 touched");
    if(boardState[7] == 9){
      if(playerTurn == 1){
        boardState[7] = 0;
        pixels.setPixelColor(54, magenta);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,9,9],[9,0,9]]}");
        playerTurn = 2;
      }else{
        boardState[7] = 1;
        pixels.setPixelColor(54, yellow);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,9,9],[9,1,9]]}");
        playerTurn = 1;
      }
      pixels.show();
    }
  }else {
    Serial.println("T9 not touched");
  }
  
  buttonState = digitalRead(buttonPin);
   if (buttonState == HIGH){
    Serial.println("button touched");
    if(boardState[8] == 9){
      if(playerTurn == 1){
        boardState[8] = 0;
        pixels.setPixelColor(59, magenta);   
        digitalWrite(rgb1, HIGH);
        digitalWrite(rgb2, LOW);
        digitalWrite(rgb3, LOW);
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,9,9],[9,9,0]]}");
        playerTurn = 2;
      }else{
        boardState[8] = 1;
        pixels.setPixelColor(59, yellow);
        digitalWrite(rgb1, LOW);
        digitalWrite(rgb2, LOW);
        digitalWrite(rgb3, HIGH);     
        webSocket.emit("player_tap","{\"gameboardID\":\"1022\",\"board\":[[9,9,9],[9,9,9],[9,9,1]]}");
        playerTurn = 1;
      }
      pixels.show();
    }
   }
  else {
    Serial.println("button not touched");
  }
  
  delay(500);
}
