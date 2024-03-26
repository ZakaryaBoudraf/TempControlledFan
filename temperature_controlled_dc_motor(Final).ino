#include <LiquidCrystal_I2C.h>

//A motor speed based on temperature
//The motor can be used to power a fan in this case to make an automated cooling fan system
//The higher the temperature the faster the motor speed
//We use an H-bridge motor driver (L293D)

const int MOTOR_ENABLE = 5; //This pin is used to control the motor speed
const int MOTOR_IN1 = 4;
const int MOTOR_IN2 = 3;
const int Temp_Vout = A0;

int OneTimeCounter = 0;

LiquidCrystal_I2C lcd_1(32, 16, 2);

const float calibrationOffset = -0.49; // Adjust this value based on your measurements

void setup() {
  
  Serial.begin(9600);

  lcd_1.init();

  lcd_1.setCursor(0, 0);
  lcd_1.backlight();
  lcd_1.display();
  
  //pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(Temp_Vout, INPUT);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  
}

void loop()   {
  
  int Temp = analogRead(Temp_Vout);
  int MapTemp = map(Temp,153,358,0,255);
  Serial.print("Temperature: ");
  Serial.println(Temp);
  Serial.print("Motor Speed: ");
  Serial.println(MapTemp);
  int fanSpd = MapTemp;

  if(MapTemp>0 && MapTemp<250){
    Serial.print (MapTemp);
    analogWrite(MOTOR_ENABLE,MapTemp); //control motor driver
    // analogWrite(MOTOR_ENABLE,250); //for testing purposes
  }
  else if(MapTemp<=0){
    Serial.print (0);
    fanSpd = 0;
    analogWrite(MOTOR_ENABLE,0); //control motor driver
  }
  else{
    Serial.print (150);
    fanSpd = 150;
    analogWrite(MOTOR_ENABLE,255); //control motor driver
  }
  
  float voltage = Temp * (5.0 / 1024.0); // Convert to voltage

  // Single-point calibration: adjust by a fixed offset
  float realTemp = (voltage + calibrationOffset) / 0.01;
  int displayedTemp = static_cast<int>(realTemp);
  
  // Fan percentage to display on the LCD screen
  int fanPercent = map(fanSpd,0,150,0,100);
  
  lcd_1.setCursor(0, 0);
  lcd_1.print("Temp: ");
  lcd_1.print(realTemp);
  lcd_1.print(" C  ");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Fan Speed: ");
  lcd_1.print(fanPercent);
  lcd_1.print(" %  ");
  
  delay(1000); // Wait for 1000 millisecond(s)
  lcd_1.leftToRight();
  
  delay(50); //Adding delay to optimize the simulation
  
}