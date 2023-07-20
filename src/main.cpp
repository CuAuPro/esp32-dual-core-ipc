/**                     //Documentation
 * file: main.c
 * author: Kristjan Cuznar
 * description: Multi Core Inter Process Communication
 */

#include <Arduino.h>
#include <freertos/semphr.h>      // Include the FreeRTOS semaphore header

#include "main.h"
#include "task1.h"
#include "task2.h"

TaskHandle_t Task1;               // Task handle for Task1
TaskHandle_t Task2;               // Task handle for Task2
SemaphoreHandle_t semaphore1[2];  // Semaphore array for read and write access to sharedMemory1
SemaphoreHandle_t semaphore2[2];  // Semaphore array for read and write access to sharedMemory2

// Define the size of the shared memory arrays
float sharedMemory1[SHARED_MEMORY_SIZE];
float sharedMemory2[SHARED_MEMORY_SIZE];


void Task2code(void *pvParameters);

/* ------------------------------ Setup section ----------------------------- */
void setup() {
  Serial.begin(115200);

  // Set LEDs as outputs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

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

  Task1_Init();
  Task2_Init();

}


/* ------------------------------ Infinite loop ----------------------------- */
void loop() {}



