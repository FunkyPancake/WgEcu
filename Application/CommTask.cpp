//
// Created by PC on 18.02.2023.
//

#include "CommTask.h"
#include "Calibration.h"
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
    uint8_t frameCounterTx = 0;
    uint8_t frameCounterRx = 0;

    Utils::Calibration cal = Utils::Calibration();
    std::vector<uint8_t> requestHeader(3);
    std::vector<uint8_t> responseHeader(3);
    for (;;)
    {
        requestHeader = uart.ReadBytes(3);
        len = (static_cast<uint16_t>(requestHeader[0]) << 8) + requestHeader[1];
        //        if (frameCounter != header[2])
        //        {
        //            auto payload = uart.ReadBytes(len);
        //            //            CommandProcessor(payload);
        //            uart.WriteBytes(std::vector<uint8_t>{0,1,frameCounter,0xff});
        //        }
        //        else
        {
            auto payload = uart.ReadBytes(len);
            std::vector<uint8_t> &response = cal.ProcessCommand(payload);
            responseHeader[0] = static_cast<uint8_t>(response.size() >> 8);
            responseHeader[1] = static_cast<uint8_t>(response.size());
            responseHeader[2] = frameCounterTx;
            uart.WriteBytes(responseHeader);
            uart.WriteBytes(response);
            frameCounterTx++;
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void CommTask(void *const parameters)
{
    if (xTaskCreate(Comm_Task, "comm_task", configMINIMAL_STACK_SIZE + 512, parameters, app_task_PRIORITY, nullptr)
        != pdPASS)
    {
        while (true)
            ;
    }
}