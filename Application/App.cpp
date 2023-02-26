//
// Created by PC on 23.05.2021.
//

#include "App.h"
#include "peripherals.h"
#include "FreeRTOS.h"
#include "FlexCan.h"
#include "RealTimeData.h"

const UBaseType_t app_task_PRIORITY = (configMAX_PRIORITIES - 1);

[[noreturn]] static void app_task(void *pvParameters);

[[noreturn]] static void app_task(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 40;
    xLastWakeTime = xTaskGetTickCount();
    FlexCan can{16};
    ICan::Payload buf{{1,2,3,4,5}};
    for (;;)
    {
        can.Send(0x80001234,buf,5);
        can.Send(0x80001235,buf,2);
        can.Send(0x34,buf,4);
        can.Send(0x1,buf,5);
        realTimeData.analogRaw.ain1_raw++;
        realTimeData.analogRaw.ain2_raw--;
//        buf.b[0]++;

//        gps.GetData();
//        canStream.SendFrames();
//        sbc.RefreshWatchdog();
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