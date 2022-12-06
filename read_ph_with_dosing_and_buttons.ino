int motor1Pin = 8;
int motor2Pin = 9;
String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
float pH;  
char currentPage = '0';

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];//used to hold a floating point number that is the pH
#include <URTouch.h>
#include <UTFT.h>
extern uint8_t BigFont[];
UTFT    myGLCD(CTE32_R2,38,39,40,41);
URTouch  myTouch( 6, 5, 4, 3, 2);
int x, y;
char stCurrent[20]="";
int stCurrentLen=0;
char stLast[20]="";
void setup() {                                        //set up the hardware
// Initial setup
  currentPage = '0';
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.clrScr();

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  drawHome();


  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial3.begin(9600);                                //set baud rate for software serial port_3 to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
}

void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}


void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  sensorstring = Serial3.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}


void loop() { 
    if (currentPage == '0') {
       if (myTouch.dataAvailable()) {
      myTouch.read();
      y=myTouch.getX(); // X coordinate where the screen has been pressed
      x=myTouch.getY(); // Y coordinates where the screen has been pressed 
      Serial.println(x); 
      if ((y>=210) && (y<=230) && (x>=60) && (x<=180)) {  // (ymax = 320-y2) (ymin = 320 -y1
        currentPage = '1';
        myGLCD.clrScr();
        drawPH();
       }
       if ((y>=160) && (y<=190) && (x>=60) && (x<=180)) {  // (ymax = 320-y2) (ymin = 320 -y1
        currentPage = '2';
        myGLCD.clrScr();
        drawPrime();
       }
    }
   
    }
    if (currentPage == '1') {
      ph();
       if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX(); // X coordinate where the screen has been pressed
      y=myTouch.getY(); // Y coordinates where the screen has been pressed  
      if ((x>=289) && (x<=310) && (y>=10) && (y<=60)) {
        currentPage = '0';
        myGLCD.clrScr();
        drawHome();
        }
        }
        }
        if (currentPage == '2') {
       if (myTouch.dataAvailable()) {
      myTouch.read();
      y=myTouch.getX(); // X coordinate where the screen has been pressed
      x=myTouch.getY(); // Y coordinates where the screen has been pressed  
      Serial.println(x);
      if ((y>=289) && (y<=310) && (x>=10) && (x<=60)) {
        currentPage = '0';
        myGLCD.clrScr();
        drawHome();
        }
        if ((y>=200) && (y<=225) && (x>=130) && (x<=190)) {// (ymax = 320-y2) (ymin = 320 -y1
        pump1Off();
        }
        if ((y>=200) && (y<=225) && (x>=40) && (x<=100)) {      
        pump1On();
        }
        if ((y>=110) && (y<=135) && (x>=130) && (x<=190)) {// (ymax = 320-y2) (ymin = 320 -y1
        pump2Off();
        }
        if ((y>=110) && (y<=135) && (x>=40) && (x<=100)) {      
        pump2On();
        }
        }
        }
        }
   

void ph(){
  //here we go...
  static int setTime = 0;
  static long startTime = 0;
  long currentTime = millis();
  static long timeElapsed;
  if (setTime == 0){
    startTime = millis();                                   // store the start time
    setTime = 1; 
  }
  timeElapsed = currentTime - startTime;

  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    Serial3.print(inputstring);                       //send that string to the Atlas Scientific product
    Serial3.print('\r');                              //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }
  

  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    static float prevph = 0;
    Serial.println(sensorstring);                     //send that string to the PC's serial monitor                                                 //uncomment this section to see how to convert the pH reading from a string to a float 
          pH = sensorstring.toFloat();  //convert the string to a floating point number so it can be evaluated by the Arduino

    if (isdigit(sensorstring[0])) {                   //if the first character in the string is a digit
   
      
      myGLCD.printNumF(pH,3, CENTER, 113);
      //delay(500);
      if (pH < 14) {                               //if the pH is less than or equal to 6.99
        Serial.print("Timeelasped: " );
        Serial.println(timeElapsed);
        if (timeElapsed > 10000){
         // myGLCD.setColor(VGA_BLACK);
         // myGLCD.fillRect(30,90,250,250);
         // myGLCD.setColor(VGA_WHITE);
        //  myGLCD.print("Pump1 ON",CENTER, 113);
         // pump1On();
         // delay(1000);
          //pump1Off();
          myGLCD.setColor(VGA_BLACK);
          myGLCD.fillRect(30,90,250,250);
          myGLCD.setColor(VGA_WHITE);
          //myGLCD.print("Pump2 ON",CENTER, 113);
         // pump2On();
         // delay(1000);
        //  pump2Off();
          myGLCD.setColor(VGA_BLACK);
          myGLCD.fillRect(30,90,250,250);
          myGLCD.setColor(VGA_WHITE);
          
          startTime = millis();
          setTime = 1;
        }
       
      }
    }
  
  }
  sensorstring = "";                                  //clear the string:
  sensor_string_complete = false;                     //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
}


void drawHome() {
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 255);
 
  myGLCD.setColor(0, 0, 255);   
  myGLCD.fillRoundRect (60, 80, 180, 110);     //(x1,y2,x2,y1)
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (60, 80, 180, 110);
  myGLCD.print("View PH", CENTER, 87);

  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (60, 130, 180, 160);     //(x1,y2,x2,y1)
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (60, 130, 180, 160);
  myGLCD.print("Prime", CENTER, 137);
    
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("PH Regulator", CENTER, 30);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0,55,319,55);
  myGLCD.setBackColor(0, 0, 0);
 
}

void drawPH(){
    myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("PH:",CENTER, 90);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
}

void drawPrime(){
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Pump 1", CENTER, 70);

  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 255);
 
  myGLCD.setColor(0, 0, 255);   
  myGLCD.fillRoundRect (50, 95, 110, 120);     //(x1,y2,x2,y1)
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (50, 95, 110, 120);
  myGLCD.print("ON", 62, 100);

  myGLCD.setColor(0, 0, 255);   
  myGLCD.fillRoundRect (140, 95, 200, 120);     //(x1,y2,x2,y1)
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (140, 95, 200, 120);
  myGLCD.print("OFF", 148, 100);

    myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Pump 2", CENTER, 160);

  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 255);
 
  myGLCD.setColor(0, 0, 255);   
  myGLCD.fillRoundRect (50, 185, 110, 210);     //(x1,y2,x2,y1)
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (50, 185, 110, 210);
  myGLCD.print("ON", 62, 190);

  myGLCD.setColor(0, 0, 255);   
  myGLCD.fillRoundRect (140, 185, 200, 210);     //(x1,y2,x2,y1)
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (140, 185, 200, 210);
  myGLCD.print("OFF", 148, 190);
}

void pump1On(){
   analogWrite(motor1Pin, 255);
   }

void pump1Off(){
  analogWrite(motor1Pin, 0);
}

void pump2On(){
   analogWrite(motor2Pin, 255);
   }

void pump2Off(){
  analogWrite(motor2Pin, 0);
}

