#include <pwm.h>

// ===== Direction Pins =====
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// ===== PWM Enable Channels =====
#define ENA 0
#define ENB 1

#define PERIOD 800000
#define SPEED  500000   // Duty cycle

// ================= SETUP =================
void setup()
{
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set PWM period for enable pins
  PWM.PWMC_Set_Period(ENA, PERIOD);
  PWM.PWMC_Set_Period(ENB, PERIOD);

  stopMotor();
}

// ================= LOOP =================
void loop()
{
  forward();
  delay(3000);

  backward();
  delay(3000);

  rightTurn();
  delay(2000);

  leftTurn();
  delay(2000);

  stopMotor();
  delay(2000);
}

// ================= MOTOR FUNCTIONS =================

void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void backward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void rightTurn()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void leftTurn()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void stopMotor()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
