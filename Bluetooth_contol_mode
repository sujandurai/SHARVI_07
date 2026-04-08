#include <Arduino.h>
#include <pwm.h>
#include <esp8266.h>

// ---------------- MOTOR PWM CHANNELS ----------------
#define RIGHT_FWD  0
#define RIGHT_REV  1
#define LEFT_FWD   2
#define LEFT_REV   3

#define PWM_PERIOD 800000
#define MOTOR_SPEED 500000

ESP8266Class Bluetooth(1);   // UART1 for Bluetooth

char cmd = 'S';

// ---------------- MOTOR FUNCTIONS ----------------
void stopMotors()
{
  analogWrite(RIGHT_FWD, 0);
  analogWrite(RIGHT_REV, 0);
  analogWrite(LEFT_FWD, 0);
  analogWrite(LEFT_REV, 0);
}

void moveForward()
{
  analogWrite(RIGHT_FWD, MOTOR_SPEED);
  analogWrite(RIGHT_REV, 0);
  analogWrite(LEFT_FWD, MOTOR_SPEED);
  analogWrite(LEFT_REV, 0);
}

void moveBackward()
{
  analogWrite(RIGHT_FWD, 0);
  analogWrite(RIGHT_REV, MOTOR_SPEED);
  analogWrite(LEFT_FWD, 0);
  analogWrite(LEFT_REV, MOTOR_SPEED);
}

void turnLeft()
{
  analogWrite(RIGHT_FWD, MOTOR_SPEED);
  analogWrite(RIGHT_REV, 0);
  analogWrite(LEFT_FWD, 0);
  analogWrite(LEFT_REV, 0);
}

void turnRight()
{
  analogWrite(RIGHT_FWD, 0);
  analogWrite(RIGHT_REV, 0);
  analogWrite(LEFT_FWD, MOTOR_SPEED);
  analogWrite(LEFT_REV, 0);
}

// ---------------- SETUP ----------------
void setup()
{
  Serial.begin(115200);
  Bluetooth.begin(9600);

  PWM.PWMC_Set_Period(RIGHT_FWD, PWM_PERIOD);
  PWM.PWMC_Set_Period(RIGHT_REV, PWM_PERIOD);
  PWM.PWMC_Set_Period(LEFT_FWD, PWM_PERIOD);
  PWM.PWMC_Set_Period(LEFT_REV, PWM_PERIOD);

  stopMotors();

  Serial.println("Sharvi O7 Bluetooth Control Ready");
}

// ---------------- LOOP ----------------
void loop()
{
  if (Bluetooth.available())
  {
    cmd = Bluetooth.read();
    Serial.print("Received: ");
    Serial.println(cmd);

    switch (cmd)
    {
      case 'F':
        moveForward();
        break;

      case 'B':
        moveBackward();
        break;

      case 'L':
        turnLeft();
        break;

      case 'R':
        turnRight();
        break;

      case 'S':
        stopMotors();
        break;

      default:
        stopMotors();
        break;
    }
  }
}
