/*
  Fade

  This example shows how to fade an LED on pin 9 using the analogWrite()
  function.

  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fade
*/

int Rot = 14;         // the PWM pin the LED is attached to
int Grun = 15;
int Blau = 16;
int brightness_rot = 255;  // how bright the LED is
int brightness_grun = 255;
int brightness_blau = 255;
int fadeAmount_rot = 0;  // how many points to fade the LED by
int fadeAmount_grun = 0;
int fadeAmount_blau = 0; 

// the setup routine runs once when you press reset:
void setup() {
  delay(300);
  
  // declare pin 9 to be an output:
  pinMode(Rot, OUTPUT);
  pinMode(Grun, OUTPUT);
  pinMode(Blau, OUTPUT);

  Serial.begin(9600); // Öffnet die serielle Schnittstelle bei 9600 Bit/s:
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of pin 9:
  analogWrite(Rot, brightness_rot);
  analogWrite(Grun, brightness_grun);
  analogWrite(Blau, brightness_blau);

  // change the brightness for next time through the loop:
  fadeAmount_rot = random(-1, 1);
  fadeAmount_grun = random(-1, 1);
  fadeAmount_blau = random(-1, 1);
  

  // reverse the direction of the fading at the ends of the fade:
  if (brightness_rot <= 0 || brightness_rot >= 255) {
    brightness_rot = 255;
  }
  if (brightness_grun <= 0 || brightness_grun >= 255) {
    brightness_grun = 255;
  }
  if (brightness_blau <= 0 || brightness_blau >= 255) {
    brightness_blau = 255;
  }

  // set the brightness for the led:
  brightness_rot = brightness_rot + fadeAmount_rot;
  brightness_grun = brightness_grun + fadeAmount_grun;
  brightness_blau = brightness_blau + fadeAmount_blau;


  Serial.print("Rot: ");
  Serial.print(brightness_rot);
  Serial.print("  Grün: ");
  Serial.print(brightness_grun);
  Serial.print("  Blau: ");
  Serial.println(brightness_blau);

  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}
