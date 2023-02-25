//
// Created by PC on 18.02.2023.
//

#include "CommTask.h"
#include "FlexCan.h"
#include "FreeRTOS.h"
#include "LpUart.h"
#include "peripherals.h"

const UBaseType_t app_task_PRIORITY = (configMAX_PRIORITIES - 1);

[[noreturn]] static void Comm_Task(void *pvParameters);

[[noreturn]] static void Comm_Task(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 1000;
    xLastWakeTime = xTaskGetTickCount();
    LpUart uart{&LPUART0_rtos_handle, &LPUART0_rtos_config};
    uint16_t len;
    uint8_t frameCounter = 0;
    std::vector<uint8_t> header{0,0,0};
    for (;;)
    {
        header = uart.ReadBytes(3);
        len = (static_cast<uint16_t>(header[0]) << 8) + header[1];
//        if (frameCounter != header[2])
//        {
//            auto payload = uart.ReadBytes(len);
//            //            CommandProcessor(payload);
//            uart.WriteBytes(std::vector<uint8_t>{0,1,frameCounter,0xff});
//        }
//        else
        {
            auto payload = uart.ReadBytes(len);
//            CommandProcessor(payload);
            uart.WriteBytes(std::vector<uint8_t>{0,1,frameCounter,header[2],0xff});
            frameCounter++;
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void CommTask(void *const parameters )
{
    if (xTaskCreate(Comm_Task, "comm_task", configMINIMAL_STACK_SIZE + 256, parameters, app_task_PRIORITY, nullptr)
        != pdPASS)
    {
        while (true)
            ;
    }
}