#include <SPI.h> //do polaczen SPI
#include <Adafruit_GFX.h> //graficzna biblioteka
#include <Adafruit_PCD8544.h> //do obslugi

//Screen charactersitic:
//  Size 84x48pixels
//  Name PCD8544

// SCLK - clock line SPI (CLK)
const uint8_t CLK = 3;
// SDIN - data line SPI (DIN)
const uint8_t DIN = 4;
// D/nC - line for deciding if we are sending data or driver commands (D/C)
const uint8_t DC = 7;
// nRES - zero signal (RES)
const uint8_t RES = 6;
// nSCE - input for activating serial interface (SCE)
const uint8_t SCE = 5;


//Buttons
const uint8_t UP = 12;
const uint8_t DOWN = 11;

//Hitboxes
const uint8_t ArrowWidth = 8;
const uint8_t ArrowHeight =5 ;

const uint8_t HurdleWidth = 5;
const uint8_t HurdleHeight = 7;

const uint8_t DinoDuckX = 29;
const uint8_t DinoDuckY = 11;
const uint8_t DinoX = 22;
const uint8_t DinoY = 15;


//Variables
unsigned long points =0;  //score
unsigned int width =6;    //for dinosaur size
unsigned int height = 46;
bool duck = false;      //*
bool rise = false;      //* Variables ffor donosaur movement, it hase to be done this way bacouse of arduino having one thread
bool fall = false;      //*
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, SCE, RES); //to use nokia screen
unsigned int objectX=84;    //parametres for arrows,hurdles
int objectY =random(1,48);
long timeer = objectY;


void setup() {  
pinMode(UP,INPUT_PULLUP);
pinMode(DOWN,INPUT_PULLUP);
display.begin();                          //start screen
display.setContrast(40);                  //set contrast
display.clearDisplay();                   //clean screen buffor

}
void loop() {
  draw();
  delay(100);
  //Button usage
  if(digitalRead(UP) ==HIGH&&rise==false&&fall==false){
    rise = true;
    digitalWrite(UP,LOW);
  }
  if(rise==true&&height!=30){
    height--;
  }else{
    rise =false;
    fall = true;
  }
  if(fall==true&&height!=46){
    height++;
  }else{
    fall=false;
  }
  if(digitalRead(DOWN) ==HIGH){
    duck = true;
  }else{
    duck = false;
    digitalWrite(DOWN,LOW);
  }
  if(objectX>100){  //overflow happens
    objectY = random(15,40);
    objectX = 84;
    points++;
    timeer+=random(1000,99999999);
  }
  if(timeer%2){
  drawArrow(objectX,objectY);
  }else{
    drawHurdle(timeer%3,objectX);
  }
  objectX=objectX-2;
}


void draw(){
  display.fillRect(0, 47, 84, 3, BLACK);
  drawDinosaur(width,height,duck);
  drawScore();
  display.display();
  display.clearDisplay();
}
void collisionCheck(){

}
//Display section
void drawDinosaur(unsigned int x, unsigned int y,bool isDucking) {

  display.fillRect(x+5, y-7, 10, 7, BLACK); //Body
  if(isDucking){
    display.fillRect(x+12,y-6,10,4,BLACK);
    display.fillRect(x+14,y-5,4,1,WHITE);
    display.drawPixel(x+21,y-4,WHITE);
  }else{
    display.fillRect(x+12, y-14, 5, 10, BLACK);//Head
    display.fillRect(x+14, y-13,1,4, WHITE); // Eye
    display.drawPixel(x+16,y-11,WHITE);
  }
  
  display.drawPixel(x+6, y, BLACK); //Leg 1
  display.drawPixel(x+9, y, BLACK); //Leg 2
  

  display.drawLine(x, y-5, x+5, y-7, BLACK);//tail
  

}
void drawHurdle(unsigned int size,int x){
  display.fillRect(x, 40, size*HurdleWidth, HurdleHeight,BLACK);
}
void drawArrow(int x, int y) {
  // Vertical line of the arrow
  display.drawLine(x, y, x-ArrowWidth, y, BLACK);
  display.drawLine(x-1, y-1, x-ArrowWidth, y, BLACK);

  // Left diagonal line of the arrowhead
  display.drawLine(x-ArrowWidth, y, x - 3, y - 3, BLACK);

  // Right diagonal line of the arrowhead
  display.drawLine(x-ArrowWidth, y, x - 3, y + 3, BLACK);
}
void gameOverScreen(){
   display.setTextSize(3);       
  display.setTextColor(BLACK);  
  display.setCursor(20, 20);    
  display.print("GAME OVER");
}
void drawScore(){
     display.setTextSize(0.5);       
  display.setTextColor(BLACK);  
  display.setCursor(1,5);    
  display.print(points);
}
