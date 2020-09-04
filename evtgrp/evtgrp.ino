// evtgrp.ino 
// MIT License (see file LICENSE)

// LED is active high
#define GPIO_LED1     25
#define GPIO_LED2     26 

#define EVBLK2        0b0001
#define EVBLK3        0b0010
#define EVALL         0b0011

static EventGroupHandle_t hevt;

void blink2(void *arg) {

  for (;;) {
    // Call blocks until EVBLK2 bit set,
    // and same bit is cleared upon return
    xEventGroupWaitBits(
      hevt,
      EVBLK2,
      pdTRUE,
      pdFALSE,
      portMAX_DELAY
    );
    // Blink 2 times
    for ( int x=0; x < 2; ++x ) {
      digitalWrite(GPIO_LED1,HIGH);
      delay(120);
      digitalWrite(GPIO_LED1,LOW);
      delay(120);
    }
  }
}

void blink3(void *arg) {

  for (;;) {
    // Call blocks until EVBLK3 bit set,
    // and same bit is cleared upon return
    xEventGroupWaitBits(
      hevt,
      EVBLK3,
      pdTRUE,
      pdFALSE,
      portMAX_DELAY
    );
    // Blink 3 times
    for ( int x=0; x < 3; ++x ) {
      digitalWrite(GPIO_LED2,HIGH);
      delay(75);
      digitalWrite(GPIO_LED2,LOW);
      delay(75);
    }
  }
}

void setup() {
  int app_cpu = xPortGetCoreID();
  BaseType_t rc;

  pinMode(GPIO_LED1,OUTPUT);
  pinMode(GPIO_LED2,OUTPUT);
  digitalWrite(GPIO_LED1,LOW);
  digitalWrite(GPIO_LED2,LOW);

  delay(2000);

  hevt = xEventGroupCreate();
  assert(hevt);

  rc = xTaskCreatePinnedToCore(
    blink2,   // func
    "blink2", // name
    1024,     // stack bytes
    nullptr,  // arg ptr
    1,        // priority
    nullptr,  // ptr to task handle
    app_cpu   // cpu#
  );
  assert(rc == pdPASS);

  rc = xTaskCreatePinnedToCore(
    blink3,   // func
    "blink3", // name
    1024,     // stack bytes
    nullptr,  // arg ptr
    1,        // priority
    nullptr,  // ptr to task handle
    app_cpu   // cpu#
  );
  assert(rc == pdPASS);
}

void loop() {

  delay(1000);
  xEventGroupSetBits(
    hevt,
    EVBLK2|EVBLK3
  );
}

// End evtgrp.ino
