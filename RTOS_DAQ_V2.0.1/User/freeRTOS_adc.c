#include "freeRTOS_adc.h"
#include "display.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  64               /* 任务堆栈大小 */
static TaskHandle_t   StartTask_Handler;  /* 任务句柄 */
static void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      10                   /* 任务优先级 */
#define TASK1_STK_SIZE  64                /* 任务堆栈大小 */
static TaskHandle_t   Task1Task_Handler;  /* 任务句柄 */
static void task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      11                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
static TaskHandle_t    Task2Task_Handler;  /* 任务句柄 */
static void task2(void *pvParameters);             /* 任务函数 */


/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO      8                   /* 任务优先级 */
#define TASK3_STK_SIZE  128                /* 任务堆栈大小 */
static TaskHandle_t    Task3Task_Handler;  /* 任务句柄 */
static void task3(void *pvParameters);             /* 任务函数 */



/******************************************************************************************************/



/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freeRTOS_adc(void)
{
    BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值 pdPASS：成功 */
    
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 ，字节为单位*/
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级，数值越大越优先*/
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄，也可以说是任务控制块。删除任务之类的，对句柄进行操作 */
                
    if(xReturn == pdPASS) printf("StartTaskCreate任务创建成功!\r\n");
    else                  printf("StartTaskCreate任务创建失败!\r\n");
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值 pdPASS：成功 */
    taskENTER_CRITICAL();           /* 进入临界区 */
    
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
      
                
    if(xReturn == pdPASS) printf("Task1任务创建成功!\r\n");
    else                  printf("Task1任务创建失败!\r\n");
                
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
                
    if(xReturn == pdPASS) printf("Task2任务创建成功!\r\n");
    else                  printf("Task2任务创建失败!\r\n");
                
                /* 创建任务2 */
    xTaskCreate(task3,"task3",
                TASK3_STK_SIZE,
                NULL,
                TASK3_PRIO,
                &Task3Task_Handler);
                
    if(xReturn == pdPASS) printf("Task2任务创建成功!\r\n");
    else                  printf("Task2任务创建失败!\r\n");
                
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
static void task1(void *pvParameters)
{
    while(1)
    {
      Data_Calculate();
      Print_Data();
      
      vTaskDelay(10);   /* 延时500个tick */
    }
}

/**
 * @brief       task2
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
static void task2(void *pvParameters)
{
    
    while(1)
    {
        get_adc1_result();
        vTaskDelay(10);   /* 延时200个tick */
    }
}

/**
 * @brief       task3
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
static void task3(void *pvParameters)
{
    while(1)
    {
        PBout(3) ^=1;
        DynamicImage_Shows();
        vTaskDelay(100);   /* 延时200个tick */
    }
}
