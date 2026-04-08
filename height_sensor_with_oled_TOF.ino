#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_VL53L0X.h>


// =====================
// ====== CONFIG =======
// =====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_SDA 22
#define OLED_SCL 23
#define XSHUT_PIN 21

float totalHeight = 22.5; // Sensor mounting height from floor in cm

// Filtering Constants
#define MAX_SAMPLES 7      // For Median Filter (must be odd)
float samples[MAX_SAMPLES];
float filteredHeight = 0;
float alpha = 0.15;        // EMA factor (lower = smoother, but slower)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Helper for sorting (Median Filter)
void sort(float a[], int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (a[j] > a[j + 1]) {
        float temp = a[j];
        a[j] = a[j + 1];
        a[j + 1] = temp;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(XSHUT_PIN, OUTPUT);
  digitalWrite(XSHUT_PIN, HIGH);
  delay(10);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println("Stabilizing...");
  display.display();

  if (!lox.begin()) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sensor Error");
    display.display();
    while (1);
  }

  // KEY STABILIZATION: Increase timing budget to 200ms (High Accuracy Mode)
  // This reduces the raw jitter from the hardware level.
  lox.setMeasurementTimingBudgetMicroSeconds(200000);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  static int sampleIdx = 0;

  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    float rawDistance_cm = measure.RangeMilliMeter / 10.0;
    float currentHeight = totalHeight - rawDistance_cm;

    // 1. Fill Median Buffer
    samples[sampleIdx] = currentHeight;
    sampleIdx = (sampleIdx + 1) % MAX_SAMPLES;

    // 2. Calculate Median (removes random spikes)
    float tempSamples[MAX_SAMPLES];
    memcpy(tempSamples, samples, sizeof(samples));
    sort(tempSamples, MAX_SAMPLES);
    float medianHeight = tempSamples[MAX_SAMPLES / 2];

    // 3. Exponential Moving Average (smooths the transition)
    // If it's the first reading, just set it.
    if (filteredHeight == 0) filteredHeight = medianHeight;
    filteredHeight = (alpha * medianHeight) + ((1.0 - alpha) * filteredHeight);

    // Update Display
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Raw: "); display.print(currentHeight, 1);
    
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.println("Height:");
    
    display.setTextSize(3);
    display.setCursor(0, 40);
    display.print(filteredHeight, 1); // 1 decimal point for stability
    display.print("cm");
    display.display();
  } else {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Out of Range");
    display.display();
  }

  delay(50); 
}