//***********************************************************************************************************************************************************************//
//
//Computer science 207
// Drew Wass
// 8 button controller sketch
// this sketch is built around an circuit with 8 buttons all with pull down circuits
// this sketch detects when a putton is pushed and outputs a corrisonding value.
//
//
//
//
//
//***********************************************************************************************************************************************************************//
//constant variables for each button, 8 in total
const int buttonpin3 = 3;
const int buttonpin4 = 4;
const int buttonpin5 = 5;
const int buttonpin6 = 6;
// changing variables, each button set to zero as a default, or LOW
 int button3 = 0;
 int button4 = 0;
 int button5 = 0;
 int button6 = 0;
   void setup (){
       pinMode(buttonpin3, INPUT);
       pinMode(buttonpin4, INPUT);
       pinMode(buttonpin5, INPUT);
       pinMode(buttonpin6, INPUT);
      Serial.begin(9600); 
               }
               
               
               
    void loop() {
   //read in all the buttons status and set them to the according variable
      button3 = digitalRead (buttonpin3);
      button4 = digitalRead (buttonpin4);
      button5 = digitalRead (buttonpin5);
      button6 = digitalRead (buttonpin6);
      /*
      the next is a series of if statements thats sees if a button has been pushed and then outputs the button push to the serial moniter.
      */
         
         if (button4 == HIGH)     
        {
          Serial.println("4");    //button4
        }
         if (button5 == HIGH)
        {
          Serial.println("5");    //button5
        }

        delay(100);   // a small delay so that when the button is pressed once quickly, it should only output 1 of what the button was to output.

    }
