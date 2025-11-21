/**
 ****************************************************************************************************
 * @file        freertos.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 精英F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __FREERTOS_DEMO_H
#define __FREERTOS_DEMO_H

#include "usart.h"
#include "led.h"
#include "ADC.H"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  64                  /* 任务堆栈大小 */
static TaskHandle_t   StartTask_Handler;    /* 任务句柄 */
static void start_task(void *pvParameters); /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      5                   /* 任务优先级 */
#define TASK1_STK_SIZE  64                  /* 任务堆栈大小 */
static TaskHandle_t   Task1Task_Handler;    /* 任务句柄 */
static void task1(void *pvParameters);      /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      3                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
static TaskHandle_t    Task2Task_Handler;   /* 任务句柄 */
static void task2(void *pvParameters);      /* 任务函数 */


/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO      4                   /* 任务优先级 */
#define TASK3_STK_SIZE  128                 /* 任务堆栈大小 */
static TaskHandle_t    Task3Task_Handler;   /* 任务句柄 */
static void task3(void *pvParameters);      /* 任务函数 */

extern QueueHandle_t semphore_handle;   /* semphore_handle二值信号量句柄 */

void freeRTOS_adc(void);

#endif
