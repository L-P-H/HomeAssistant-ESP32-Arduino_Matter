#include <Servo.h>

#define Servo_Base_Pin 2
#define Servo_Arm_1_Pin 3
#define Servo_Arm_2_Pin 4
#define Servo_Kopf_Pin 5
#define Servo_Greifer_Pin 6

#define Servo_Base_Min 0
#define Servo_Arm_1_Min 0
#define Servo_Arm_2_Min 0
#define Servo_Kopf_Min 0
#define Servo_Greifer_Min 20

#define Servo_Base_Max 180
#define Servo_Arm_1_Max 180
#define Servo_Arm_2_Max 180
#define Servo_Kopf_Max 180
#define Servo_Greifer_Max 180

#define Joistick_o_u A0 //Mitte 1900; zu Kontakten 0; weg 4095
#define Joistick_l_r A1 //Mitte 1900; zu Knopf 4096; weg 0
#define Joistick_sw 7 //gedrückt 0
#define Rotary_encoder_clk A2 //bewegt 0
#define Rotary_encoder_dt A3 //bewegt 0
#define Rotary_encoder_sw 8 //gedrückt 0
#define LED_1 9
#define LED_2 10
#define LED_3 11

Servo servo_Base;  // create servo object to control a servo
Servo servo_Arm_1;
Servo servo_Arm_2;
Servo servo_Kopf;
Servo servo_Greifer;

int servo_Base_num = 0;
int servo_Arm_1_num = 0;
int servo_Arm_2_num = 0;
int servo_Kopf_num = 0;
int servo_Greifer_num = 0;
int button_J = 1;
int button_R = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(Joistick_o_u, INPUT);
  pinMode(Joistick_l_r, INPUT);
  pinMode(Joistick_sw, INPUT_PULLUP);
  pinMode(Rotary_encoder_clk, INPUT);
  pinMode(Rotary_encoder_dt, INPUT);
  pinMode(Rotary_encoder_sw, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  
  servo_Base.attach(Servo_Base_Pin);  // attaches the servo on pin 9 to the servo objectur
  servo_Arm_1.attach(Servo_Arm_1_Pin);
  servo_Arm_2.attach(Servo_Arm_2_Pin);
  servo_Kopf.attach(Servo_Kopf_Pin);
  servo_Greifer.attach(Servo_Greifer_Pin);
  
  servo_Base.write(Servo_Base_Max);   // rotate slowly servo to 0 degrees immediately
  delay(3000);
  servo_Base.write(Servo_Base_Min);
  
  servo_Arm_1.write(Servo_Arm_1_Max);
  delay(3000);
  servo_Arm_1.write(Servo_Arm_1_Min);
  
  servo_Arm_2.write(Servo_Arm_2_Max);
  delay(3000);
  servo_Arm_2.write(Servo_Arm_2_Min);
  
  servo_Kopf.write(Servo_Kopf_Max);
  delay(3000);
  servo_Kopf.write(Servo_Kopf_Min);
  
  servo_Greifer.write(Servo_Greifer_Max);
  delay(3000);
  servo_Greifer.write(Servo_Greifer_Min);
}

void loop()
{
  if (digitalRead(Joistick_b) == LOW) {button_J++; delay(500);}
  if (button_J >= 4) {button_J = 1;}
  if (button_J == 1) {digitalWrite(LED_1, HIGH); digitalWrite(LED_2, LOW); digitalWrite(LED_3, LOW);}
  else if (button_J == 2) {digitalWrite(LED_1, LOW); digitalWrite(LED_2, HIGH); digitalWrite(LED_3, LOW);}
  else if (button_J == 3) {digitalWrite(LED_1, LOW); digitalWrite(LED_2, LOW); digitalWrite(LED_3, HIGH);}
  else {digitalWrite(LED_1, LOW); digitalWrite(LED_2, LOW); digitalWrite(LED_3, LOW);}
  
  if (analogRead(Joistick_l_r) < Servo_Base_Min) {servo_Base_num = Servo_Base_Min;}
  else if (analogRead(Joistick_l_r) > Servo_Base_Max) {servo_Base_num = Servo_Base_Max;}
  else {servo_Base_num = analogRead(Joistick_l_r);}
  servo_Base.write(servo_Greifer_num);
  
  if (analogRead(Joistick_o_u) < Servo_Arm_1_Min && button_J == 1) {servo_Base_num = Servo_Arm_1_Min;}
  else if (analogRead(Joistick_o_u) > Servo_Arm_1_Max && button_J == 1) {servo_Base_num = Servo_Arm_1_Max;}
  else if (button_J == 1) {servo_Arm_1_num = analogRead(Joistick_o_u);}
  servo_Arm_1.write(servo_Arm_1_num);
  
  if (analogRead(Joistick_o_u) < Servo_Arm_2_Min && button_J == 2) {servo_Base_num = Servo_Arm_2_Min;}
  else if (analogRead(Joistick_o_u) > Servo_Arm_2_Max && button_J == 2) {servo_Base_num = Servo_Arm_2_Max;}
  else if (button_J == 2) {servo_Arm_2_num = analogRead(Joistick_o_u);}
  servo_Arm_2.write(servo_Arm_2_num);
  
  if (analogRead(Rotary_encoder) < Servo_Greifer_Min) {servo_Greifer_num = Servo_Greifer_Min;}
  else if (analogRead(Rotary_encoder) > Servo_Greifer_Max) {servo_Greifer_num = Servo_Greifer_Max;}
  else {servo_Greifer_num = analogRead(Rotary_encoder);}
  if (
  servo_Greifer.write(servo_Greifer_num);
  
  
  delay(10); // Delay a little bit to improve simulation performance
}