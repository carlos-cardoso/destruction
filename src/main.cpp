#include <Stepper_28BYJ_48.h>

bool initLoop = true;
int path = 0;               // Direction of blind (1 = down, 0 = stop, -1 = up)
int setPos = 0;             // The set position 0-100% by the client
long currentPosition = 0;   // Current position of the blind
long maxPosition = 2000000; // Max position of the blind. Initial value
bool ccw{true};

Stepper_28BYJ_48 small_stepper(D1, D3, D2, D4); // Initiate stepper driver

/**
  Turn of power to coils whenever the blind
  is not moving
*/
void stopPowerToCoils() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
}

const int motionSensor = 14; // D5
#define timeSeconds 4
constexpr uint32_t locked{30 * 1000};
unsigned long now = 0;
unsigned long lastTrigger = 0;
boolean startTimer = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  // digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
  Serial.begin(9600);
  delay(100);
  // Serial.print("Starting now\n");
  currentPosition = 0;
  maxPosition = 2000000;
  // rotation direction
  ccw = true;

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING
  // mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
}

void loop() {

  static uint32_t last_move_ms{0};
  static uint32_t timestamp_stopped_ms{0};

  // Current time
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds
  // variable
  // if (startTimer millis() - last_move_ms > 20) {
  if (startTimer && (now - lastTrigger > (timeSeconds * 1000))) {
    // Serial.println("Motion stopped...");
    timestamp_stopped_ms = millis();
    stopPowerToCoils();
    Serial.println("Pausing");
    // while (millis() - timestamp_stopped_ms < locked)
    //   ;
    delay(locked);
    startTimer = false;
    Serial.println("Continue");
  } else if (startTimer && millis() - last_move_ms > 20) {
    long path = 1;
    small_stepper.step(ccw ? path : -path);
    currentPosition = currentPosition + path;
    last_move_ms = millis();
  }
  /*
     After running setup() the motor might still have
     power on some of the coils. This is making sure that
     power is off the first time loop() has been executed
     to avoid heating the stepper motor draining
     unnecessary current
  */
  if (initLoop) {
    initLoop = false;
    stopPowerToCoils();
  }
}
