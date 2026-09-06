#include <Adafruit_SoftServo.h>
#define REFRESH_INTERVAL 4000

#define Servo_Base_Pin 2
#define Servo_Arm_1_Pin 3
#define Servo_Arm_2_Pin 4
#define Servo_Kopf_Pin 5
#define Servo_Greifer_Pin 6

#define Servo_Base_Min 0
#define Servo_Arm_1_Min 0
#define Servo_Arm_2_Min 0
#define Servo_Kopf_Min 0
#define Servo_Greifer_Min 65

#define Servo_Base_Max 213
#define Servo_Arm_1_Max 200
#define Servo_Arm_2_Max 212 //Error
#define Servo_Kopf_Max 212
#define Servo_Greifer_Max 200

#define Joistick_o_u A0 //Mitte 1900; zu Kontakten 0; weg 4095
#define Joistick_l_r A1 //Mitte 1900; zu Knopf 4096; weg 0
#define Joistick_sw 7 //gedrückt 0
#define Rotary_encoder_clk A2 //bewegt 0
#define Rotary_encoder_dt A3 //bewegt 0
#define Rotary_encoder_sw 8 //gedrückt 0
#define LED_1 11
#define LED_2 10
#define LED_3 9

Adafruit_SoftServo servo_Base;  // create servo object to control a servo
Adafruit_SoftServo servo_Arm_1;
Adafruit_SoftServo servo_Arm_2;
Adafruit_SoftServo servo_Kopf;
Adafruit_SoftServo servo_Greifer;

int servo_Base_num = Servo_Base_Max;
int servo_Arm_1_num = Servo_Arm_1_Min;
int servo_Arm_2_num = Servo_Arm_2_Min;
int servo_Kopf_num = Servo_Kopf_Max;
int servo_Greifer_num = Servo_Greifer_Max;
int button_J = 1;
int button_R = 0;

bool Rotary_sperren = false;
int Joistick_o_u_mitte = 1870;
int Joistick_l_r_mitte = 1900;

void setup() {
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
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("    ");
  Serial.print(analogRead(Joistick_o_u));
  Serial.print("    ");
  Serial.print(analogRead(Joistick_l_r));
  Serial.print("    ");
  Serial.print(digitalRead(Joistick_sw));
  Serial.print("    ");
  Serial.print(digitalRead(Rotary_encoder_clk));
  Serial.print("    ");
  Serial.print(digitalRead(Rotary_encoder_dt));
  Serial.print("    ");
  Serial.println(digitalRead(Rotary_encoder_sw));

  if (digitalRead(Rotary_encoder_clk) == 1) {Rotary_sperren = false;}
  if (digitalRead(Rotary_encoder_sw) == 0) {servo_Greifer_num = (Servo_Greifer_Max + Servo_Greifer_Min) / 2;}
  if (digitalRead(Rotary_encoder_clk) == 0 && digitalRead(Rotary_encoder_dt) == 0 && Rotary_sperren == false) {servo_Greifer_num = min(servo_Greifer_num + 10, Servo_Greifer_Max); Rotary_sperren = true;}
  if (digitalRead(Rotary_encoder_clk) == 0 && digitalRead(Rotary_encoder_dt) == 1 && Rotary_sperren == false) {servo_Greifer_num = max(Servo_Greifer_Min, servo_Greifer_num - 10); Rotary_sperren = true;}

  if (digitalRead(Joistick_sw) == LOW) {button_J++; delay(500);}
  if (button_J >= 4) {button_J = 1;}
  if (analogRead(Joistick_o_u) < (Joistick_o_u_mitte - 50) && button_J == 1) {servo_Arm_1_num = min(servo_Arm_1_num + 1, Servo_Arm_1_Max);}
  if (analogRead(Joistick_o_u) > (Joistick_o_u_mitte + 50) && button_J == 1) {servo_Arm_1_num = max(Servo_Arm_1_Min, servo_Arm_1_num - 1);}
  if (button_J == 1) {digitalWrite(LED_1, HIGH); digitalWrite(LED_2, LOW); digitalWrite(LED_3, LOW);}
  if (analogRead(Joistick_o_u) < (Joistick_o_u_mitte - 50) && button_J == 2) {servo_Arm_2_num = min(servo_Arm_2_num + 1, Servo_Arm_2_Max);}
  if (analogRead(Joistick_o_u) > (Joistick_o_u_mitte + 50) && button_J == 2) {servo_Arm_2_num = max(Servo_Arm_2_Min, servo_Arm_2_num - 1);}
  if (button_J == 2) {digitalWrite(LED_1, LOW); digitalWrite(LED_2, HIGH); digitalWrite(LED_3, LOW);}
  if (analogRead(Joistick_o_u) < (Joistick_o_u_mitte - 50) && button_J == 3) {servo_Kopf_num = max(Servo_Kopf_Min, servo_Kopf_num - 1);}
  if (analogRead(Joistick_o_u) > (Joistick_o_u_mitte + 50) && button_J == 3) {servo_Kopf_num = min(servo_Kopf_num + 1, Servo_Kopf_Max);}
  if (button_J == 3) {digitalWrite(LED_1, LOW); digitalWrite(LED_2, LOW); digitalWrite(LED_3, HIGH);}

  if (analogRead(Joistick_l_r) < (Joistick_l_r_mitte - 50)) {servo_Base_num = max(Servo_Base_Min, servo_Base_num - 1);}
  if (analogRead(Joistick_l_r) > (Joistick_l_r_mitte + 50)) {servo_Base_num = min(servo_Base_num + 1, Servo_Base_Max);}

  servo_Base.write(servo_Base_num);
  servo_Arm_1.write(servo_Arm_1_num);
  servo_Arm_2.write(servo_Arm_2_num);
  servo_Kopf.write(servo_Kopf_num);
  servo_Greifer.write(servo_Greifer_num);
  
  servo_Base.refresh();
  servo_Arm_1.refresh();
  servo_Arm_2.refresh();
  servo_Kopf.refresh();
  servo_Greifer.refresh();
  delay(2);
}
