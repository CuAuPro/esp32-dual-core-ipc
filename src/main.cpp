/**                     //Documentation
 * file: main.c
 * author: Kristjan Cuznar
 * description: Multi Core Inter Process Communication
 */

#include <Arduino.h>
#include <freertos/semphr.h>      // Include the FreeRTOS semaphore header

#define SEM_READ 0                // Semaphore definition for reading access in semaphore set
#define SEM_WRITE 1               // Semaphore definition for reading access in semaphore set

TaskHandle_t Task1;               // Task handle for Task1
TaskHandle_t Task2;               // Task handle for Task2
SemaphoreHandle_t semaphore1[2];  // Semaphore array for read and write access to sharedMemory1
SemaphoreHandle_t semaphore2[2];  // Semaphore array for read and write access to sharedMemory2


// Define the size of the shared memory arrays
const uint8_t sharedMemorySize = 1;
float sharedMemory1[sharedMemorySize];
float sharedMemory2[sharedMemorySize];

// LED pins
const uint8_t led1 = 2;
const uint8_t led2 = 4;
const uint8_t led3 = 22;
const uint8_t led4 = 23;


void Task1code(void *pvParameters);
void Task2code(void *pvParameters);

/* ------------------------------ Setup section ----------------------------- */
void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Create the read semaphore for sharedMemory1
  semaphore1[SEM_READ] = xSemaphoreCreateBinary();
  if (semaphore1[SEM_READ] == NULL) {
    Serial.println("Read semaphore 1 creation failed!");
    while (1)
      ; // Stop here if semaphore creation failed
  }

  // Create the write semaphore for sharedMemory1
  semaphore1[SEM_WRITE] = xSemaphoreCreateBinary();
  if (semaphore1[SEM_WRITE] == NULL) {
    Serial.println("Write semaphore 1 creation failed!");
    while (1)
      ; // Stop here if semaphore creation failed
  }

  // Create the read semaphore for sharedMemory2
  semaphore2[SEM_READ] = xSemaphoreCreateBinary();
  if (semaphore2[SEM_READ] == NULL) {
    Serial.println("Read semaphore 2 creation failed!");
    while (1)
      ; // Stop here if semaphore creation failed
  }

  // Create the write semaphore for sharedMemory2
  semaphore2[SEM_WRITE] = xSemaphoreCreateBinary();
  if (semaphore2[SEM_WRITE] == NULL) {
    Serial.println("Write semaphore 2 creation failed!");
    while (1)
      ; // Stop here if semaphore creation failed
  }

  xSemaphoreGive(semaphore1[SEM_WRITE]); // Allow Task1 to start writing to sharedMemory1
  xSemaphoreGive(semaphore2[SEM_WRITE]); // Allow Task1 to start writing to sharedMemory2
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


/* ------------------------------ Infinite loop ----------------------------- */
void loop() {}


/* --------------------- Task1 Code - Running on Core 0 --------------------- */
void Task1code(void *pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    // Acquire the write semaphore for sharedMemory1
    if (xSemaphoreTake(semaphore1[SEM_WRITE], (TickType_t)portMAX_DELAY) == pdTRUE) {
      digitalWrite(led1, HIGH);
      delay(1000);
      // Write to sharedMemory1
      for (int i = 0; i < sharedMemorySize; i++) {
        sharedMemory1[i] = 1.0;
        Serial.print("Task1: sharedMemory1[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(sharedMemory1[i]);fflush(stdout);
      }
      digitalWrite(led1, LOW);
      // Release the write semaphore for sharedMemory1
      xSemaphoreGive(semaphore1[SEM_READ]);
      //vTaskDelay(pdMS_TO_TICKS(1)); // Add a delay to observe the behavior
    }

    // Acquire the write semaphore for sharedMemory2
    if (xSemaphoreTake(semaphore2[SEM_WRITE], (TickType_t)portMAX_DELAY) == pdTRUE) {
      digitalWrite(led3, HIGH);
      delay(1000);
      // Write to sharedMemory2
      for (int i = 0; i < sharedMemorySize; i++) {
        sharedMemory2[i] = 2.0;
        Serial.print("Task1: sharedMemory2[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(sharedMemory2[i]);fflush(stdout);
      }
      digitalWrite(led3, LOW);
      // Release the write semaphore for sharedMemory2
      xSemaphoreGive(semaphore2[SEM_READ]);
    }
  }
}

/* --------------------- Task2 Code - Running on Core 1 --------------------- */
void Task2code(void *pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    // Acquire the read semaphore for sharedMemory1
    if (xSemaphoreTake(semaphore1[SEM_READ], (TickType_t)portMAX_DELAY) == pdTRUE) {
      digitalWrite(led2, HIGH);
      delay(2000);
      // Read from sharedMemory1
      for (int i = 0; i < sharedMemorySize; i++) {
        sharedMemory1[i] -= 1.0;
        Serial.print("Task2: sharedMemory1[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(sharedMemory1[i]);fflush(stdout);
      }
      
      digitalWrite(led2, LOW);
      // Release the read semaphore for sharedMemory1
      xSemaphoreGive(semaphore1[SEM_WRITE]);
      //vTaskDelay(pdMS_TO_TICKS(1)); // Add a delay to observe the behavior
    }

    // Acquire the read semaphore for sharedMemory2
    if (xSemaphoreTake(semaphore2[SEM_READ], (TickType_t)portMAX_DELAY) == pdTRUE) {
      digitalWrite(led4, HIGH);
      delay(2000);
      // Read from sharedMemory2
      for (int i = 0; i < sharedMemorySize; i++) {
        sharedMemory2[i] -= 2.0;
        Serial.print("Task2: sharedMemory2[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(sharedMemory2[i]);fflush(stdout);
      }
      digitalWrite(led4, LOW);
      // Release the read semaphore for sharedMemory2
      xSemaphoreGive(semaphore2[SEM_WRITE]);
    }
  }
}