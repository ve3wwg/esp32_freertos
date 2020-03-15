// semaphores.ino
// Practical ESP32 Multitasking
// Binary Semaphores

#define LED1_GPIO   25
#define LED2_GPIO   26

static SemaphoreHandle_t hsem;

void led_task(void *argp) {
  int led = (int)argp;
  BaseType_t rc;

  pinMode(led,OUTPUT);
  digitalWrite(led,0);

  for (;;) {
    // First gain control of hsem
    rc = xSemaphoreTake(hsem,portMAX_DELAY);
    assert(rc == pdPASS);

    for ( int x=0; x<6; ++x ) {
      digitalWrite(led,digitalRead(led)^1);      
      delay(500);
    }

    rc = xSemaphoreGive(hsem);
    assert(rc == pdPASS);
  }
}

void setup() {
  int app_cpu = xPortGetCoreID();
  BaseType_t rc;  // Return code

  hsem = xSemaphoreCreateBinary();
  assert(hsem);

  rc = xTaskCreatePinnedToCore(
    led_task,   // Function
    "led1task", // Task name
    3000,       // Stack size
    (void*)LED1_GPIO, // arg
    1,          // Priority
    nullptr,    // No handle returned
    app_cpu);   // CPU
  assert(rc == pdPASS);

  // Allow led1task to start first
  rc = xSemaphoreGive(hsem);
  assert(rc == pdPASS);

  rc = xTaskCreatePinnedToCore(
    led_task,   // Function
    "led2task", // Task name
    3000,       // Stack size
    (void*)LED2_GPIO, // argument
    1,          // Priority
    nullptr,    // No handle returned
    app_cpu);   // CPU
  assert(rc == pdPASS);
}

// Not used
void loop() {
  vTaskDelete(nullptr);
}
