#include <Arduino.h>
#include <pwm.h>
#include <esp8266.h>   // used as UART wrapper

// ---------------- PWM CHANNELS ----------------
#define RIGHT_FWD  0
#define RIGHT_REV  1
#define LEFT_FWD   2
#define LEFT_REV   3

#define PWM_PERIOD 800000
#define SPEED      600000

ESP8266Class BT(1);   // UART1

char cmd;

// ---------------- MOTOR CONTROL ----------------
void stopMotor()
{
    analogWrite(RIGHT_FWD, 0);
    analogWrite(RIGHT_REV, 0);
    analogWrite(LEFT_FWD, 0);
    analogWrite(LEFT_REV, 0);
}

void forward()
{
    analogWrite(RIGHT_FWD, SPEED);
    analogWrite(RIGHT_REV, 0);
    analogWrite(LEFT_FWD, SPEED);
    analogWrite(LEFT_REV, 0);
}

void backward()
{
    analogWrite(RIGHT_FWD, 0);
    analogWrite(RIGHT_REV, SPEED);
    analogWrite(LEFT_FWD, 0);
    analogWrite(LEFT_REV, SPEED);
}

void left()
{
    analogWrite(RIGHT_FWD, SPEED);
    analogWrite(RIGHT_REV, 0);
    analogWrite(LEFT_FWD, 0);
    analogWrite(LEFT_REV, SPEED/4);
}

void right()
{
    analogWrite(RIGHT_FWD, 0);
    analogWrite(RIGHT_REV, SPEED/4);
    analogWrite(LEFT_FWD, SPEED);
    analogWrite(LEFT_REV, 0);
}

// ---------------- SETUP ----------------
void setup()
{
    Serial.begin(115200);
    BT.begin(9600);

    // Set PWM period
    PWM.PWMC_Set_Period(RIGHT_FWD, PWM_PERIOD);
    PWM.PWMC_Set_Period(RIGHT_REV, PWM_PERIOD);
    PWM.PWMC_Set_Period(LEFT_FWD, PWM_PERIOD);
    PWM.PWMC_Set_Period(LEFT_REV, PWM_PERIOD);

    stopMotor();

    Serial.println("SHARVI O7 READY (ARIES V2)");
}

// ---------------- LOOP ----------------
void loop()
{
    if (BT.available())
    {
        cmd = BT.read();
        Serial.println(cmd);

        if (cmd == 'F') forward();
        else if (cmd == 'B') backward();
        else if (cmd == 'L') left();
        else if (cmd == 'R') right();
        else if (cmd == 'S') stopMotor();
    }
}
