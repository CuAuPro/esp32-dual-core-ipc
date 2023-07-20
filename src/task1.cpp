#include "task1.h"
#include "globals.h"



void Task1_Init(void) {
  // Create Task1 on core 0
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0            /* pin task to core 0 */
  );
}

/* --------------------- Task1 Code - Running on Core 0 --------------------- */
void Task1code(void *pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    // Acquire the write semaphore for sharedMemory1
    if (xSemaphoreTake(semaphore1[SEM_WRITE], (TickType_t)portMAX_DELAY) == pdTRUE) {
      digitalWrite(LED1_PIN, HIGH);
      delay(1000);
      // Write to sharedMemory1
      for (int i = 0; i < SHARED_MEMORY_SIZE; i++) {
        sharedMemory1[i] = 1.0;
        Serial.print("Task1: sharedMemory1[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(sharedMemory1[i]);fflush(stdout);
      }
      digitalWrite(LED1_PIN, LOW);
      // Release the write semaphore for sharedMemory1
      xSemaphoreGive(semaphore1[SEM_READ]);
      //vTaskDelay(pdMS_TO_TICKS(1)); // Add a delay to observe the behavior
    }

    // Acquire the write semaphore for sharedMemory2
    if (xSemaphoreTake(semaphore2[SEM_WRITE], (TickType_t)portMAX_DELAY) == pdTRUE) {
      digitalWrite(LED3_PIN, HIGH);
      delay(1000);
      // Write to sharedMemory2
      for (int i = 0; i < SHARED_MEMORY_SIZE; i++) {
        sharedMemory2[i] = 2.0;
        Serial.print("Task1: sharedMemory2[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(sharedMemory2[i]);fflush(stdout);
      }
      digitalWrite(LED3_PIN, LOW);
      // Release the write semaphore for sharedMemory2
      xSemaphoreGive(semaphore2[SEM_READ]);
    }
  }
}