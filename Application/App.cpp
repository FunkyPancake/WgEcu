//
// Created by PC on 23.05.2021.
//

#include "App.h"
#include "peripherals.h"
#include "FreeRTOS.h"
#include "LpSpi.h"
#include "LpUart.h"
#include "FlexCan.h"
#include "NeoM9N.h"
#include "Tle9461.h"
#include "ICanStream.h"
#include "SyvecsCan.h"


const UBaseType_t app_task_PRIORITY = (configMAX_PRIORITIES - 1);

[[noreturn]] static void app_task(void *pvParameters);

[[noreturn]] static void app_task(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 40;
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        gps.GetData();
        canStream.SendFrames();
        sbc.RefreshWatchdog();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void App()
{
    if (xTaskCreate(app_task, "app_task", configMINIMAL_STACK_SIZE + 256, nullptr, app_task_PRIORITY, nullptr)
        != pdPASS)
    {
        while (true);
    }
}