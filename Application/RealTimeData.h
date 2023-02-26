//
// Created by PC on 26.02.2023.
//

#ifndef REALTIMEDATA_H
#define REALTIMEDATA_H

#include <cstdio>

struct RealTimeData{
    struct AnalogRaw{
        uint16_t ain1_raw;
        uint16_t ain2_raw;
        uint16_t ain3_raw;
        uint16_t ain4_raw;
    };
    AnalogRaw analogRaw;
};
extern RealTimeData realTimeData;
#endif//REALTIMEDATA_H
