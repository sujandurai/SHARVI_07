#include <MD_MAX72xx.h>
#include <SPI.h>
#include <esp8266.h>

// ================= HARDWARE PINS =================
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1
#define CS_PIN 27
#define TOUCH_PIN 6       // Mode switch button
#define TOUCH_YES 5        // YES for quiz
#define TOUCH_NO 4         // NO for quiz

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, SPI0, CS_PIN, MAX_DEVICES);

// ================= BLUETOOTH =================
ESP8266Class Bluetooth(1);

// ================= DFPLAYER =================
ESP8266Class DFPlayer(2);



void sendCommand(uint8_t cmd, uint8_t p1, uint8_t p2)
{
  uint8_t buffer[10];

  buffer[0] = 0x7E;
  buffer[1] = 0xFF;
  buffer[2] = 0x06;
  buffer[3] = cmd;
  buffer[4] = 0x00;
  buffer[5] = p1;
  buffer[6] = p2;

  uint16_t checksum = 0 - (buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6]);

  buffer[7] = (checksum >> 8) & 0xFF;
  buffer[8] = checksum & 0xFF;

  buffer[9] = 0xEF;

  DFPlayer.write(buffer, 10);
  delay(200);
}

void playTrack(uint16_t track)
{
  sendCommand(0x03, track >> 8, track & 0xFF);
}

void setVolume(uint8_t vol)
{
  sendCommand(0x06, 0x00, vol);
}


// ================= GLOBALS =================
int mode = 0;
int lastState = 0;
int indexItem = 0;


int interval = 700;

bool blinkState = true;
bool mazeIntroPlayed = false;
int startX = 0;
int startY = 0;
// ================= GLOBAL TIMERS =================
unsigned long previousMillisShapes = 0;    // Tracks last update for shapes mode
unsigned long previousMillisNumbers = 0;   // Tracks last update for numbers mode
unsigned long previousMillisLetters = 0;   // Tracks last update for letters mode
unsigned long lastBlink = 0;               // Maze blink timer

// ================= INTERVALS =================
unsigned long intervalShapes = 3500;       // 3.5 seconds for shapes
unsigned long intervalNumbers = 1200;      // 1.2 seconds for numbers
unsigned long intervalLetters = 1200;      // 1.2 seconds for letters

// ================= LETTERS =================
const uint8_t LETTERS[26][8] = {
  {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00},{0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00},
  {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00},{0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00},
  {0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00},{0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00},
  {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00},{0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00},
  {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},{0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00},
  {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00},{0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00},
  {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00},{0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00},
  {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},{0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00},
  {0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00},{0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00},
  {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00},{0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
  {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},{0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00},
  {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00},{0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00},
  {0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00},{0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}
};
const char* LETTER_NAMES[26] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

// ================= NUMBERS =================
const uint8_t NUMBERS[10][8] = {
  {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00},{0x18,0x38,0x18,0x18,0x18,0x18,0x3C,0x00},
  {0x3C,0x66,0x06,0x0C,0x18,0x30,0x7E,0x00},{0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00},
  {0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0x00},{0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00},
  {0x3C,0x60,0x7C,0x66,0x66,0x66,0x3C,0x00},{0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00},
  {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00},{0x3C,0x66,0x66,0x3E,0x06,0x06,0x3C,0x00}
};
const char* NUMBER_NAMES[10] = {"0","1","2","3","4","5","6","7","8","9"};

// ================= SHAPES =================
const uint8_t SHAPES[12][8] = {
  {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},  // Square
  {0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00},  // Rectangle
  {0x00,0x00,0x08,0x1C,0x3E,0x7F,0x00,0x00},  // Triangle
  {0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C},  // Circle
  {0x00,0x18,0x3C,0x7E,0xFF,0x7E,0x3C,0x18},  // Pentagon
  {0x00,0x3C,0x7E,0xFF,0xFF,0x7E,0x3C,0x00},  // Oval
  {0x00,0x3C,0x7E,0xFF,0xFF,0x7E,0x3C,0x00},  // Hexagon
  {0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81},  // Cross
  {0x00,0x66,0xFF,0xFF,0xFF,0x7E,0x3C,0x18},  // Heart
  {0x18,0x3C,0x7E,0xFF,0x7E,0x3C,0x18,0x00},  // Diamond
  {0x08,0x0C,0xFE,0xFF,0xFE,0x0C,0x08,0x00},  // Arrow
  {0x3C,0x42,0x7E,0x42,0x42,0x7E,0x42,0x3C}   // Cylinder
};

const char* SHAPE_NAMES[12] = {
  "SQUARE", "RECTANGLE", "TRIANGLE", "CIRCLE", "PENTAGON",
  "OVAL", "HEXAGON", "CROSS", "HEART", "DIAMOND", "ARROW", "CYLINDER"
};

// ================= FEEDBACK FACES =================
const uint8_t SMILE[8] = {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
const uint8_t SAD[8]   = {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};

// ================= MAZE =================
#define UP 'T'
#define DOWN 'B'
#define LEFT 'L'
#define RIGHT 'R'

int playerX = 0, playerY = 7;
int finishX = 7, finishY = 0;
int level = 0;
byte mazes[7][8][8] = {
  {{0,0,0,1,0,0,0,0},{1,1,0,1,0,1,1,0},{0,0,0,0,0,0,1,0},{0,1,1,1,1,0,1,0},{0,0,0,0,1,0,0,0},{0,1,1,0,1,1,1,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
  {{0,1,0,0,0,1,0,0},{0,1,0,1,0,1,0,1},{0,0,0,1,0,0,0,1},{1,1,0,1,1,1,0,0},{0,0,0,0,0,1,0,1},{0,1,1,1,0,1,0,0},{0,0,0,1,0,0,0,0},{0,0,0,0,0,0,0,0}},
  {{0,0,1,0,0,0,1,0},{1,0,1,0,1,0,1,0},{1,0,0,0,1,0,0,0},{1,1,1,0,1,1,1,0},{0,0,0,0,0,0,1,0},{0,1,1,1,1,0,1,0},{0,0,0,0,1,0,0,0},{0,0,0,0,0,0,0,0}},
  {{0,0,0,0,1,0,0,0},{1,1,1,0,1,0,1,0},{0,0,0,0,1,0,1,0},{0,1,1,1,1,0,1,0},{0,0,0,0,0,0,1,0},{0,1,1,1,1,1,1,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
  {{0,1,0,0,0,1,0,0},{0,1,0,1,0,1,0,1},{0,0,0,1,0,0,0,1},{1,1,0,1,1,1,0,0},{0,0,0,0,0,1,0,1},{0,1,1,1,0,1,0,0},{0,0,0,1,0,0,0,0},{0,0,0,0,0,0,0,0}},
  {{0,0,1,0,0,1,0,0},{1,0,1,0,1,1,0,1},{1,0,0,0,0,0,0,1},{1,1,1,1,1,1,0,0},{0,0,0,0,0,1,0,1},{0,1,1,1,0,1,0,0},{0,0,0,1,0,0,0,0},{0,0,0,0,0,0,0,0}},
  {{0,1,0,0,0,0,1,0},{0,1,0,1,1,0,1,0},{0,0,0,0,1,0,0,0},{1,1,1,0,1,1,1,0},{0,0,0,0,0,0,1,0},{0,1,1,1,1,0,1,0},{0,0,0,0,1,0,0,0},{0,0,0,0,0,0,0,0}}
};

// ================= DISPLAY PATTERN =================
void displayPattern(const uint8_t pattern[8]){
  mx.clear();
  for(int i=0;i<8;i++) mx.setRow(0,i,pattern[i]);
  mx.update();
}

// ================= DRAW MAZE =================
void drawGame(){
  mx.clear();
  for(int y=0;y<8;y++)
    for(int x=0;x<8;x++)
      if(mazes[level][y][x]==1) mx.setPoint(y,x,true);
 
  if(blinkState){
    mx.setPoint(7,0,true);
    mx.setPoint(0,7,true);
  }

  mx.setPoint(playerY,playerX,true);
  mx.update();
}

void winAnimation()
{
  playTrack(59);            // Level complete sound
     delay(2000);  
  for(int i=0;i<4;i++)
  {
    mx.clear();
    delay(200);
    drawGame();
    delay(200);
  }
}

void resetGame()
{
  playerX = startX;
  playerY = startY;
  drawGame();
}
// ================= MOVE PLAYER =================
void executeCommand(char command)
{
  switch(command)
  {
    case UP:
      if(playerY>0 && mazes[level][playerY-1][playerX]==0) playerY--;
    break;

    case DOWN:
      if(playerY<7 && mazes[level][playerY+1][playerX]==0) playerY++;
    break;

    case LEFT:
      if(playerX>0 && mazes[level][playerY][playerX-1]==0) playerX--;
    break;

    case RIGHT:
      if(playerX<7 && mazes[level][playerY][playerX+1]==0) playerX++;
    break;
  }

  // Level completed
static bool levelWon = false;  // keeps track if level has already been won

if(playerX == finishX && playerY == finishY && !levelWon)
{
  levelWon = true;          // mark level as won            // optional, can be replaced with millis() for non-blocking
  winAnimation();

  level++;
  if(level > 6) level = 0;

  resetGame();
  levelWon = false;         // ready for next level
}
}

// ================= QUIZ MODE =================
void runQuiz() {
  int currentType = random(0,3); // 0=Shape,1=Number,2=Letter
  int currentIndex;

  // Pick random item
  switch(currentType){
    case 0: currentIndex = random(0,5); displayPattern(SHAPES[currentIndex]); Serial.print("Is this a "); Serial.print(SHAPE_NAMES[currentIndex]); break;
    case 1: currentIndex = random(0,10); displayPattern(NUMBERS[currentIndex]); Serial.print("Is this the number "); Serial.print(NUMBER_NAMES[currentIndex]); break;
    case 2: currentIndex = random(0,26); displayPattern(LETTERS[currentIndex]); Serial.print("Is this the letter "); Serial.print(LETTER_NAMES[currentIndex]); break;
  }
  Serial.println("? YES or NO");
  Serial.println("------------------------");

  bool answered = false;
  while(!answered){
    int stateYes = digitalRead(TOUCH_YES);
    int stateNo = digitalRead(TOUCH_NO);
    int stateClear = digitalRead(6); // Pin 6 check for clearing

    if(stateClear == HIGH){
      mx.clear();
      mx.update();
      Serial.println("Quiz cleared!");
      return; // exit the function immediately
    }

    if(stateYes == HIGH){
      displayPattern(SMILE);
      Serial.println("Pressed: YES\nResult: CORRECT");
       playTrack(60);      // YES → play track 60
      answered = true;
    }

    if(stateNo == HIGH){
      displayPattern(SAD);
      Serial.println("Pressed: NO\nResult: WRONG");
       playTrack(61);      // YES → play track 60
      answered = true;
    }
  }
  delay(800);
}

// ================= SETUP =================
void setup(){
  Serial.begin(115200);
  Bluetooth.begin(9600);
  DFPlayer.begin(9600);

   delay(2000);

  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY,5);
  mx.clear();

  pinMode(TOUCH_PIN,INPUT);
  pinMode(TOUCH_YES,INPUT);
  pinMode(TOUCH_NO,INPUT);

 DFPlayer.begin(9600);
delay(2000);

setVolume(10);
playTrack(1);   // greeting audio
delay(3000);
playTrack(2);   // greeting audio
delay(3000);
 playTrack(3);   // greeting audio
 delay(3000);
  randomSeed(analogRead(0));

  drawGame();
}

void checkModeButton()
{
  int state = digitalRead(TOUCH_PIN);

  if(state == LOW && lastState == HIGH)
  {
    mode++;
    if(mode > 5) mode = 1;

    indexItem = 0;
    mazeIntroPlayed = false;

    delay(200);
  }

  lastState = state;
}

// ================= LOOP =================
void loop()
{
  checkModeButton();
 switch(mode) {

  case 1: // Shapes mode
      if(indexItem == 0)   // When entering shapes mode
  {
    playTrack(44);     // "Shapes mode activated"
    delay(3000);       // wait for intro audio
  }
    if(millis() - previousMillisShapes > intervalShapes) {
    previousMillisShapes = millis();
    displayPattern(SHAPES[indexItem]);
    playTrack(45 + indexItem);

    indexItem++;
    if(indexItem >= 12) indexItem = 0;
}
    break;

  case 2: // Numbers mode
   if(indexItem == 0)   // When numbers mode starts
  {
    playTrack(33);     // Numbers mode activated
    delay(2000);
  }
   if(millis() - previousMillisNumbers > intervalNumbers) {
    previousMillisNumbers = millis();
    displayPattern(NUMBERS[indexItem]);
    playTrack(34 + indexItem);

    indexItem++;
    if(indexItem >= 10) indexItem = 0;
}
    break;

  case 3: // Letters mode
    if(indexItem == 0)   // When alphabet mode starts
  {
    playTrack(6);      // Alphabet mode activated
    delay(2000);
  }
    if(millis() - previousMillisLetters > intervalLetters) {
    previousMillisLetters = millis();
    displayPattern(LETTERS[indexItem]);
    playTrack(7 + indexItem);

    indexItem++;
    if(indexItem >= 26) indexItem = 0;
}
    break;

 case 4: // Maze Mode
{
    // --- Play Maze Intro only once ---
    if(!mazeIntroPlayed) {
        playTrack(57);   // Maze intro
        mazeIntroPlayed = true;
        // Record when intro started
        previousMillisShapes = millis(); // can reuse a timer variable
    }

    // --- Blink the finish points ---
    if(millis() - lastBlink > 400) {
        blinkState = !blinkState;
        lastBlink = millis();
        drawGame();
    }

    // --- Read Bluetooth commands ---
    if(Bluetooth.available() > 0) {
        char command = toupper(Bluetooth.read());
        executeCommand(command);
    }
}
break;

  case 5: // Quiz
    playTrack(58);
    runQuiz();
    break;

  default:
    mx.clear();
    break;
}
}
