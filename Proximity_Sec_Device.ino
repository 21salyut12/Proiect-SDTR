#define TRIGGER_PIN 4 // Trigger pin connected to D4 on the ESP32
#define ECHO_PIN 2    // Echo pin connected to D2 on the ESP32

volatile uint32_t pulseStart = 0;
volatile uint32_t pulseEnd = 0;
volatile bool echoReceived = false;

void IRAM_ATTR handleEcho() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    pulseStart = micros();
  } else {
    pulseEnd = micros();
    echoReceived = true;
  }
}

void measuredDistance(void *pvParameters) {
  (void)pvParameters;
  
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), handleEcho, CHANGE);
  
  for (;;) {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);

    // Trigger the sensor by sending a 10 microsecond pulse
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Wait for echo or timeout
    unsigned long timeout = micros() + 30000; // Set a timeout of 30ms
    while (!echoReceived && micros() < timeout) {
      // Delay added here to avoid busy-waiting
      vTaskDelay(pdMS_TO_TICKS(1));
    }

    // Calculate distance if echo is received
    if (echoReceived) {
      uint32_t duration = pulseEnd - pulseStart;
      float distance_cm = duration * 0.034 / 2.0; // Calculate distance in centimeters

      // Print distance to Serial Monitor
      Serial.print("Distance: ");
      Serial.print(distance_cm);
      Serial.println(" cm");

      echoReceived = false; // Reset flag
    } else {
      Serial.println("No echo received");
    }

    vTaskDelay(pdMS_TO_TICKS(3000)); // Delay before next reading
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {} // Wait for Serial to initialize
  
  xTaskCreate(measuredDistance, "MeasuredDistance", configMINIMAL_STACK_SIZE * 2, NULL, 1, NULL);
}

void loop() {
  // Leave loop() empty as FreeRTOS tasks handle the execution
}
