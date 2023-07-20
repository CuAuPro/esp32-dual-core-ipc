
#include "task2.h"
#include "globals.h"


void Task2_Init(void) {
  // Create Task2 on core 1
  xTaskCreatePinnedToCore(
    Task2code,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    1            /* pin task to core 1 */
  );
}


/* --------------------- Task2 Code - Running on Core 1 --------------------- */
void Task2code(void *pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    // Acquire the read semaphore for sharedMemory1
    if (xSemaphoreTake(semaphore1[SEM_READ], (TickType_t)portMAX_DELAY) == pdTRUE) {
      digitalWrite(LED2_PIN, HIGH);
      delay(2000);
      // Read from sharedMemory1
      for (int i = 0; i < SHARED_MEMORY_SIZE; i++) {
        sharedMemory1[i] -= 1.0;
        Serial.print("Task2: sharedMemory1[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(sharedMemory1[i]);fflush(stdout);
      }
      
      digitalWrite(LED2_PIN, LOW);
      // Release the read semaphore for sharedMemory1
      xSemaphoreGive(semaphore1[SEM_WRITE]);
      //vTaskDelay(pdMS_TO_TICKS(1)); // Add a delay to observe the behavior
    }

    // Acquire the read semaphore for sharedMemory2
    if (xSemaphoreTake(semaphore2[SEM_READ], (TickType_t)portMAX_DELAY) == pdTRUE) {
      digitalWrite(LED4_PIN, HIGH);
      delay(2000);
      // Read from sharedMemory2
      for (int i = 0; i < SHARED_MEMORY_SIZE; i++) {
        sharedMemory2[i] -= 2.0;
        Serial.print("Task2: sharedMemory2[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(sharedMemory2[i]);fflush(stdout);
      }
      digitalWrite(LED4_PIN, LOW);
      // Release the read semaphore for sharedMemory2
      xSemaphoreGive(semaphore2[SEM_WRITE]);
    }
  }
}