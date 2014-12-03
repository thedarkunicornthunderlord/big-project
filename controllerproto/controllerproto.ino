/* controller prototype of the controller code. purpose
is so that we can prove that the controller for the gameduino is actually working.
Drew wass
*/
int latchPin = 8;
int clockPin = 10;
int dataPin = 9;
//byte controller[2]; //2 for the possibility of 2 controllers
                      // could use a variable if it was just one controller 
                      //  use the array if it is two
byte Controller = [B00000000]; //set up the initial variable in order to have it nice and zero's out                    


void setup() {
  Serial.begin(9600); //generic output to see if controller is working properly
  pinMode (datapin,  INPUT); //read in from the data in
  pinMode (latchPin, OUTPUT);
  pinMode (clockPin, OUTPUT);
  digitalWrite (dataPin,  HIGH); // set all the values to a higher value as to effectively "zero it out"...mortal
  digitalWrite (clockPin, HIGH);
  digitalWrite (latchPin, HIGH);
  
}
void loop () {
  loop {
          Controller = ControllerGet(gameserial);
          //ControllerTranslate (Controller);
          delay(100)
}

int ControllerGet (){
  digitalWrite (LatchPin, LOW);
  shiftOut (dataPin, clockPin, MSBFIRST, gameserial);//this line is fcked up. fix it
  digitalWrite (LatchPin, HIGH);
  Serial.println (gameserial);
  return gameserial; //send the finished string back to the controller
}
// this function gathers the values from the controller //put back in when its working
//int ControllerTranslate(){
//}
