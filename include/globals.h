#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <freertos/semphr.h>      // Include the FreeRTOS semaphore header

#include "main.h"

extern TaskHandle_t Task1;               // Task handle for Task1
extern TaskHandle_t Task2;               // Task handle for Task2
extern SemaphoreHandle_t semaphore1[2];  // Semaphore array for read and write access to sharedMemory1
extern SemaphoreHandle_t semaphore2[2];  // Semaphore array for read and write access to sharedMemory2


// Define the size of the shared memory arrays
extern float sharedMemory1[SHARED_MEMORY_SIZE];
extern float sharedMemory2[SHARED_MEMORY_SIZE];


#endif  // GLOBALS_H