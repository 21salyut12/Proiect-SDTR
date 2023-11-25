#define TRIGGER_PIN 4 // Replace with the actual trigger pin connected to D4 on the ESP32
#define ECHO_PIN 2    // Replace with the actual echo pin connected to D2 on the ESP32

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

void senderTask(void *param) {
  (void) param;
  while (1) {
    // Perform other tasks or operations here if needed

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void receiverTask(void *param) {
  (void) param;
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), handleEcho, CHANGE);

  for (int i = 0; i < 10; i++) {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Wait for echo
    while (!echoReceived) {
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    // Calculate distance
    uint32_t duration = pulseEnd - pulseStart;
    int distance = duration * 0.034 / 2; // Calculate distance from duration

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    echoReceived = false; // Reset flag
    vTaskDelay(3000 / portTICK_PERIOD_MS); // Delay before next reading
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {} // Wait for Serial to initialize

  xTaskCreatePinnedToCore(senderTask, "Sender", configMINIMAL_STACK_SIZE, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(receiverTask, "Receiver", configMINIMAL_STACK_SIZE * 2, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

void loop() {
  // Nothing needed here as FreeRTOS tasks handle the execution
}