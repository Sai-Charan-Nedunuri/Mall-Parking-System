/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
 *
 * NOTE 2:  This project provides two demo applications.  A simple blinky style
 * project, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting in main.c is used to select
 * between the two.  See the notes on using mainCREATE_SIMPLE_BLINKY_DEMO_ONLY
 * in main.c.  This file implements the simply blinky version.  Console output
 * is used in place of the normal LED toggling.
 *
 * NOTE 3:  This file only contains the source code that is specific to the
 * basic demo.  Generic functions, such FreeRTOS hook functions, are defined
 * in main.c.
 ******************************************************************************
 *
 * main_blinky() creates one queue, one software timer, and two tasks.  It then
 * starts the scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  It uses vTaskDelayUntil() to create a periodic task that sends
 * the value 100 to the queue every 200 milliseconds (please read the notes
 * above regarding the accuracy of timing under Windows).
 *
 * The Queue Send Software Timer:
 * The timer is a one-shot timer that is reset by a key press.  The timer's
 * period is set to two seconds - if the timer expires then its callback
 * function writes the value 200 to the queue.  The callback function is
 * implemented by prvQueueSendTimerCallback() within this file.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() waits for data to arrive on the queue.
 * When data is received, the task checks the value of the data, then outputs a
 * message to indicate if the data came from the queue send task or the queue
 * send software timer.
 *
 * Expected Behaviour:
 * - The queue send task writes to the queue every 200ms, so every 200ms the
 *   queue receive task will output a message indicating that data was received
 *   on the queue from the queue send task.
 * - The queue send software timer has a period of two seconds, and is reset
 *   each time a key is pressed.  So if two seconds expire without a key being
 *   pressed then the queue receive task will output a message indicating that
 *   data was received on the queue from the queue send software timer.
 *
 * NOTE:  Console input and output relies on Windows system calls, which can
 * interfere with the execution of the FreeRTOS Windows port.  This demo only
 * uses Windows system call occasionally.  Heavier use of Windows system calls
 * can crash the port.
 */

/* Standard includes. */
#include <stdio.h>
#include <conio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The times are converted from
milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 2000UL )
#define delayTime 2000
SemaphoreHandle_t xSemaphore;

void task_one()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 2000)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-1\r\n");
			// Give Semaphore back:
			xSemaphoreGive(xSemaphore);
			vTaskDelay(delayTime);
		}
		else {
			printf("Car-1 is waiting\r\n");
		}
	}
}
void task_two()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 100)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-2\r\n");
			// Give Semaphore back:
			
			vTaskDelay(1500);
			xSemaphoreGive(xSemaphore);
			vTaskDelay(1000);
		}
		else {
			printf("Car-2 is waiting\r\n");
		}
	}
}

void task_three()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 1500)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-3\r\n");
			// Give Semaphore back:
			xSemaphoreGive(xSemaphore);
			vTaskDelay(delayTime);
		}
		else {
			printf("Car-3 is waiting\r\n");
		}
	}
}

void task_fourth()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 1100)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-4\r\n");
			// Give Semaphore back:
			vTaskDelay(300);
			xSemaphoreGive(xSemaphore);
			vTaskDelay(700);
		}
		else {
			printf("Car-4 is waiting\r\n");
		}
	}
}
void task_five()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 2000)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-5\r\n");
			// Give Semaphore back:
			xSemaphoreGive(xSemaphore);
			vTaskDelay(delayTime);
		}
		else {
			printf("Car-5 is waiting\r\n");
		}
	}
}
void task_six()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 20)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-6\r\n");
			// Give Semaphore back:
			vTaskDelay(2000);
			xSemaphoreGive(xSemaphore);
			vTaskDelay(100);
		}
		else {
			printf("Car-6 is waiting\r\n");
			vTaskDelay(2000);
		}
	}
}
void task_seven()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 1000)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-7\r\n");
			// Give Semaphore back:
			vTaskDelay(1000);
			xSemaphoreGive(xSemaphore);
			vTaskDelay(200);
		}
		else {
			printf("Car-7 is waiting\r\n");
		}
	}
}
void task_eight()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 5000)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-8\r\n");
			// Give Semaphore back:
			xSemaphoreGive(xSemaphore);
			vTaskDelay(600);
		}
		else {
			printf("Car-8 is waiting\r\n");
		}
	}
}
void task_nine()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 1000)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-9\r\n");
			// Give Semaphore back:
			xSemaphoreGive(xSemaphore);
			vTaskDelay(delayTime);
		}
		else {
			printf("Car-9 is waiting\r\n");
		}
	}
}
void task_ten()
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 2000)) {
			// Use Guarded Resource
			printf("Parking Spot is Occupied By Car-10\r\n");
			// Give Semaphore back:
			vTaskDelay(500);
			xSemaphoreGive(xSemaphore);
			vTaskDelay(100);
		}
		else {
			printf("Car-10 is waiting\r\n");
		}
	}
}





/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_blinky( void )
{
		xTaskCreate(task_one, "task_one", 200, 0, tskIDLE_PRIORITY+1, NULL);
		xTaskCreate(task_two, "task_two", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(task_three, "task_three", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(task_fourth, "task_fourth", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(task_five, "task_five", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(task_six, "task_six", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(task_seven, "task_seven", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(task_eight, "task_eight", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(task_nine, "task_nine", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(task_ten, "task_ten", 200, 0, NULL, tskIDLE_PRIORITY, NULL);
		xSemaphore = xSemaphoreCreateCounting(5, 5);
		vTaskStartScheduler();
		for (;;);
	
	
}

/*-----------------------------------------------------------*/


