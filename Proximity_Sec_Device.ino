#include <ArduinoBLE.h>
#include <ArduinoJson.h>

#define TRIGGER_PIN 18 // Trigger pin connected to D18 on the ESP32
#define ECHO_PIN 5    // Echo pin connected to D5 on the ESP32

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

  float smoothedDistance = 0; // Variable to store smoothed distance
  float alpha = 0.2; // Smoothing factor (adjustable)
  const float maxRange = 200.0; // Maximum reasonable range (adjustable)
  const int maxValidReadings = 4; // Maximum valid readings to consider for averaging
  
  for (;;) {
    unsigned long totalDuration = 0;
    int validReadings = 0;

    for (int i = 0; i < maxValidReadings; ++i) {
      digitalWrite(TRIGGER_PIN, LOW);
      delayMicroseconds(2);

      // Trigger the sensor by sending a 10 microsecond pulse
      digitalWrite(TRIGGER_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIGGER_PIN, LOW);

      unsigned long timeout = micros() + 23200; // Adjusted timeout for 4 meters
      while (!echoReceived && micros() < timeout) {
        delayMicroseconds(10);
      }

      if (echoReceived) {
        totalDuration += pulseEnd - pulseStart;
        echoReceived = false;
        validReadings++;
      }
      vTaskDelay(pdMS_TO_TICKS(20)); // Small delay between readings
    }

    if (validReadings > 0) {
      float distance_cm = (totalDuration / validReadings) * 0.034 / 2.0; // Calculate average distance

      // Smoothing the distance using Exponential Moving Average (EMA)
      smoothedDistance = alpha * distance_cm + (1 - alpha) * smoothedDistance;

      // Print smoothed distance for monitoring
      Serial.print("Smoothed Distance: ");
      Serial.print(smoothedDistance);
      Serial.println(" cm");
    } else {
      Serial.println("No Valid Reading");
    }

    vTaskDelay(pdMS_TO_TICKS(3000)); // Delay before next reading
  }
}

BLEService distanceService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEFloatCharacteristic distanceCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setupBLE() {
  BLE.begin();
  BLE.setLocalName("DistanceSensor");

  distanceService.addCharacteristic(distanceCharacteristic);
  BLE.addService(distanceService);
  distanceCharacteristic.writeValue(0);

  BLE.advertise();

  Serial.println("Bluetooth setup complete!");
}

void sendDistanceOverBLE(float distance) {
  distanceCharacteristic.setValue(distance);
} 

void setup() {
  Serial.begin(115200);
  while (!Serial) {} // Wait for Serial to initialize
  
  xTaskCreate(measuredDistance, "MeasuredDistance", 1024, NULL, 1, NULL);
  
  setupBLE();
}

void loop() {
  // Leave loop() empty as FreeRTOS tasks handle the execution
}
