              const uint8_t latchPin = 12;//Pin connected to ST_CP of 74HC595
const uint8_t clockPin = 8;//Pin connected to SH_CP of 74HC595
const uint8_t dataPin = 11; //Pin connected to DS of 74HC595
const uint8_t switchUpPin = 7;
const uint8_t switchDownPin = 6;
const uint8_t ledPin = 5;//Pin connected to floor led
const uint8_t ledSwitchPinUp = 2;//Pin For the switch led
const uint8_t ledSwitchPinDown = 3;//Pin For the switch led
uint16_t sensor1;
uint16_t sensor2;
uint8_t isCalled;//(Is lift called?) 1 For yes 0 for no
uint8_t liftDir; //(lift direction) 1 for up, 0 for down
uint8_t liftLevel = 2; //default level
/*=======================================================================================================
//display 0,1,2,3,4,5,6,7,8
Number 0 :  00000011     3
Number 1 :  10101111     159
Number 2 :  00100101     37
Number 3 :  00001101     13
Number 4 :  10011001     153
Number 5 :  01001001     73
Number 6 :  01000001     65
Number 7 :  00011111     31
Number 8 :  00000001     1

To display the dot, simply subtract 1.
example : 1 - 1 = 0  or b0000000 0x0 to display 8
*/
/*In order to display zero your array has decimal number 3. However using the shift register
it is actuall the binary number that is uploaded into the resitsre: zie the table above for zero: 00000011.
*/
int datArray[9] = {3, 159, 37, 13, 153, 73, 65, 31, 1}; // array without the dot to store the decimal vallue of the binary cofiguration for the 7 digit display

void setup ()
{
  
  Serial.begin(115200);//Enable serial for debug
  //set pins to output
  //pins of shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //Set the switch pins to use the internal pullups of the atmgega328p
  pinMode(switchUpPin, INPUT_PULLUP);
  pinMode(switchDownPin, INPUT_PULLUP);
  //elevator presence/arrival LED
  pinMode(ledPin, OUTPUT);
  //IRsensor 1 "OUT" pin
  pinMode(A0, INPUT); //analog pin for the ir sensor
  //IRsensor 2 "OUT" pin
  pinMode(A1, INPUT); //analog pin for the ir sensor
  //Swich leds
  pinMode(ledSwitchPinDown, OUTPUT);
  pinMode(ledSwitchPinUp, OUTPUT);
}
void loop(){

  //read analog value, save to ints
  sensor1 = analogRead(A0);
  sensor2 = analogRead(A1);

   //Write the current vallue of liftlevel to the display
   digitalWrite(latchPin,LOW);
   shiftOut(dataPin,clockPin,MSBFIRST, datArray[liftLevel]);
   digitalWrite(latchPin, HIGH);
  
  //elevator called for upwards
  if(digitalRead(switchUpPin) == LOW){ //check if switch is low (pressed) then...
    isCalled = 1;
    liftDir = 1;
    digitalWrite(ledSwitchPinUp, HIGH);// indicate that we pressed the switch
    //Debug
    Serial.println("[PH] UP");

  }
  else{isCalled = 0;
  liftDir = 255;}//set to 0 and 255 because we didn't call anything

  //elevator called for downwards
  if(digitalRead(switchDownPin) == LOW){//check if switch is low (pressed) then...
    isCalled = 1;
    liftDir = 0;
    digitalWrite(ledSwitchPinDown, HIGH);
    //Debug
    Serial.println("[PH] DOWN");
  }
  else{isCalled = 0;
  liftDir = 255;}//set to 0 and 255 because we didn't call anything, 255 is an invallid dirrection so the lift will do nothing with it.

    //both sensors high signal (signal is inversed ( Low is High), High is low))
    if (sensor1 < 50 && sensor2 < 50){
    digitalWrite(ledPin, HIGH);//enable led to show that elevator has arrived
    liftLevel = 2; //Set the current level of the elevator to write to the display
    //elevator arrived, not called anymore
    isCalled = 0;
    digitalWrite(ledSwitchPinUp, LOW);
    digitalWrite(ledSwitchPinDown, LOW);
}
else{
  digitalWrite(ledPin, LOW);//Elevator isn't there or hasn't arrived
  }
}
