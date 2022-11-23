#include "pch.h"

static char ballCount = 6;
const static int RedSpawnTime = 1200;
static int scoreRequirement = 1000000;
static int redRingSpawnTimer = 0;

HOOK(void, __fastcall, pinballInit_r, 0x140718B80, pinballwk* a1, size_t a2, size_t a3)
{
    a1->ballCount = ballCount;
    originalpinballInit_r(a1, a2, a3);
}

HOOK(void, __fastcall, FUN_140718e10_r, 0x140718e10, pinballwk* a1, long score)
{
    auto pin = (pinballwk*)sub_140b8a8d0((size_t*)a1);

    if (pin)
        pin->totalScore = scoreRequirement;

    originalFUN_140718e10_r(a1, score);
}

HOOK(void, __fastcall, redRingSpawn_r, 0x140719fd0, pinballwk* a1, size_t a2, float* a3)
{
    bool balls = a1 && a1->ballCount >= 3;

    if (balls)
    {
        ++redRingSpawnTimer;

        if (redRingSpawnTimer == RedSpawnTime) //1200 = 20 seconds
        {
            return originalredRingSpawn_r(a1, a2, a3);
        }
        else if (redRingSpawnTimer >= RedSpawnTime + 10)
        {
            redRingSpawnTimer = 0;
        }
    }
    else
    {
        originalredRingSpawn_r(a1, a2, a3);
    }
}

extern "C" {

    __declspec(dllexport) void Init()
    {
        const INIReader reader("pinball.ini");

        if (reader.ParseError() != 0)
            PrintInfo(("Cannot load configuration file, mod won't work.\n"));

        int ball = reader.GetInteger("Mod", "ballCount", ballCount);
        long score = reader.GetInteger("Mod", "scoreRequierement", scoreRequirement);

        if (ball <= 0)
            ball = 1;

        if (ball > CHAR_MAX) //might be a int, but do you actually need more than 127?
            ball = CHAR_MAX;

        ballCount = ball;

        if (score <= 999)
            score = 1000;

        scoreRequirement = score;

        if (ballCount != 3)
        {
            WRITE_NOP(0x140718bb3, 6); //remove the code that set the number of balls, we will manually set a new one
            INSTALL_HOOK(pinballInit_r);
        }

        if (ballCount > 3) { //having more than 3 balls mess with red ring spawn timer for some reason... we manually repair that with a custom timer

            WRITE_MEMORY(0x14071a234, uint8_t, ballCount - 1); //check current ballCount instead of hardcoded "2"
            INSTALL_HOOK(redRingSpawn_r); //repair red ring timer
        }

        if (scoreRequirement != 5000000)
            INSTALL_HOOK(FUN_140718e10_r);

    }

    __declspec(dllexport) void __cdecl OnFrame()
    {

    }
}