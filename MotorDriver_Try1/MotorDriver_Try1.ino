#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// ---------------- Pin Definitions ----------------
// Motor A
#define PWMA 5      // D5 -> GPIO5
#define AIN1 18     // D18 -> GPIO18
#define AIN2 19     // D19 -> GPIO19

// Motor B
#define PWMB 27     // D27 -> GPIO27
#define BIN1 26     // D26 -> GPIO26
#define BIN2 25     // D25 -> GPIO25

// Standby (moved from GPIO34 to GPIO4)
#define STBY 4      // D4 -> GPIO4

// ---------------- PWM Configuration (Core 2.x) ----------------
#define CHANNEL_A 0
#define CHANNEL_B 1
#define PWM_FREQ 1000
#define PWM_RESOLUTION 8   // 0–255

int motorSpeed = 200;

// ---------------- Setup ----------------
void setup()
{
  Serial.begin(115200);
  Dabble.begin("ESP32_Car");

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Enable motor driver
  digitalWrite(STBY, HIGH);

  // LEDC setup (ESP32 Core 2.x)
  ledcSetup(CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWMA, CHANNEL_A);

  ledcSetup(CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWMB, CHANNEL_B);
}

// ---------------- Main Loop ----------------
void loop()
{
  Dabble.processInput();

  if (GamePad.isUpPressed())
  {
    moveForward(motorSpeed);
  }
  else if (GamePad.isDownPressed())
  {
    moveBackward(motorSpeed);
  }
  else if (GamePad.isLeftPressed())
  {
    turnLeft(motorSpeed);
  }
  else if (GamePad.isRightPressed())
  {
    turnRight(motorSpeed);
  }
  else
  {
    stopMotors();
  }
}

// ---------------- Movement Functions ----------------
void moveForward(int speed)
{
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(CHANNEL_A, speed);
  ledcWrite(CHANNEL_B, speed);
}

void moveBackward(int speed)
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  ledcWrite(CHANNEL_A, speed);
  ledcWrite(CHANNEL_B, speed);
}

void turnLeft(int speed)
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(CHANNEL_A, speed);
  ledcWrite(CHANNEL_B, speed);
}

void turnRight(int speed)
{
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  ledcWrite(CHANNEL_A, speed);
  ledcWrite(CHANNEL_B, speed);
}

void stopMotors()
{
  ledcWrite(CHANNEL_A, 0);
  ledcWrite(CHANNEL_B, 0);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}