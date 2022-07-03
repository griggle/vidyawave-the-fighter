#pragma once

#include <string>

struct MatchPacket
{
    int frame_time;

    float x;
    float y;

    char state[100];
    int  counter;
};