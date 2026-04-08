
#include<pwm.h>

// ================= MOTOR PINS =================
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

#define ENA 12
#define ENB 11

#define SPEED 800000

// ================= ULTRASONIC =================
#define TRIG_FRONT 22
#define ECHO_FRONT 23

#define TRIG_LEFT 24
#define ECHO_LEFT 25

#define TRIG_RIGHT 26
#define ECHO_RIGHT 27

// ================= DISTANCE RANGE =================
#define MIN_DIST 30
#define MAX_DIST 35

// ================= SETUP =================
void setup()
{
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  PWM.PWMC_Set_Period(ENA, 800000);
  PWM.PWMC_Set_Period(ENB, 800000);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);

  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);

  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  stopRobot();

  Serial.println("Sharvi-7 Uncaught Mode Ready");
}

// ================= LOOP =================
void loop()
{
  int front = getDistance(TRIG_FRONT, ECHO_FRONT);
  int left = getDistance(TRIG_LEFT, ECHO_LEFT);
  int right = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  Serial.print("Front: ");
  Serial.print(front);
  Serial.print(" Left: ");
  Serial.print(left);
  Serial.print(" Right: ");
  Serial.println(right);

  // ===== Distance Control =====
  if(front > MAX_DIST)
  {
    moveForward();   // object far
  }
  else if(front < MIN_DIST)
  {
    moveBackward();  // object too close
  }
  else
  {
    stopRobot();     // perfect range
  }

  // ===== Direction Control =====
  if(left < right - 5)
  {
    turnRight();     // object moved left
    delay(150);
  }
  else if(right < left - 5)
  {
    turnLeft();      // object moved right
    delay(150);
  }

  delay(50);
}

// ================= ULTRASONIC FUNCTION =================
long getDistance(int trig, int echo)
{
  digitalWrite(trig, LOW);
  delayMicroseconds(5);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 20000);
  long distance = duration * 0.034 / 2;

  return distance;
}

// ================= MOTOR FUNCTIONS =================

void moveForward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void moveBackward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void turnLeft()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void turnRight()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void stopRobot()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
