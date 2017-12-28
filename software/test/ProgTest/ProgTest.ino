/*
  Blink 2 LEDS
 
*/

const int RED_LED_PIN = 5;
const int GREEN_LED_PIN = 3;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  Serial.begin (57600);  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RED_LED_PIN, HIGH);   
  delay(250);                       
  digitalWrite(RED_LED_PIN, LOW);    
  digitalWrite(GREEN_LED_PIN, HIGH);
  
  delay(250);                       
  digitalWrite(GREEN_LED_PIN, LOW);
  

  Serial.println("Blink");

}
