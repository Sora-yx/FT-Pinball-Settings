#include "pch.h"

static char ballCount = 6;
const static int RedSpawnTime = 1200;
static int scoreRequirement = 1000000;
static int redRingSpawnTimer = 0;

SIG_SCAN
(
    sigsub_140b8a8d0,
    0x140b8a8d0,
    "\x48\x8B\x81\xD8\x01\x00\x00\x48\x85\xC0\x74\x11", 
    "xxxxxxxxxxxx"
)

FUNCTION_PTR(size_t, __fastcall, sub_140b8a8d0, sigsub_140b8a8d0(), size_t* a1);

SIG_SCAN
(
    sigpinBallInit,
    0x140718B80,
    "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x50\x41",
    "xxxxxxxxxxxxxxxxxxxxx"
)

HOOK(void, __fastcall, pinballInit_r, sigpinBallInit(), pinballwk* a1, size_t a2, size_t a3)
{
    a1->ballCount = ballCount;
    originalpinballInit_r(a1, a2, a3);
}

SIG_SCAN
(
    sigFUN_140718e10,
    0x140718e10,
    "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x50\x8B\xFA\x48\x8B\xF1", 
    "xxxxxxxxxxxxxxxxxxxx"
)

HOOK(void, __fastcall, FUN_140718e10_r, sigFUN_140718e10(), pinballwk* a1, long score)
{
    auto pin = (pinballwk*)sub_140b8a8d0((size_t*)a1);

    if (pin)
        pin->totalScore = scoreRequirement;

    originalFUN_140718e10_r(a1, score);
}

SIG_SCAN
(
    sigRedRingSpawn,
    0x140719fd0,
    "\x48\x89\x74\x24\x20\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\xF0", 
    "xxxxxxxxxxxxxxx"
)

HOOK(void, __fastcall, redRingSpawn_r, sigRedRingSpawn(), pinballwk* a1, size_t a2, float* a3)
{
    bool balls = a1 && a1->ballCount >= 3;

    if (balls)
    {
        ++redRingSpawnTimer;

        if (redRingSpawnTimer >= RedSpawnTime + 60)
        {
            redRingSpawnTimer = 0;
        }

        if (redRingSpawnTimer >= RedSpawnTime) //1200 = 20 seconds
        {
            return originalredRingSpawn_r(a1, a2, a3);
        }

    }
    else
    {
        originalredRingSpawn_r(a1, a2, a3);
    }
}

SIG_SCAN
(
    sigSetBall,
    0x140718bb3,
    "\x89\xB7\xB4\x02\x00\x00\x89\x87\xB0\x02\x00\x00", 
    "xxxxxxxxxxxx"
)

SIG_SCAN
(
    sigRedRingBall,
    0x14071a234,
    "\x83\xFA\x02\x0F\x87\x0D\x01\x00\x00", 
    "xxxxxxxxx"
)

extern "C" {

    __declspec(dllexport) void Init()
    {
        const INIReader reader("pinball.ini");

        if (reader.ParseError() != 0)
        {
            MessageBox(nullptr, TEXT("Cannot load configuration file, mod won't work.\n"), TEXT("Pinball Settings Mod"), MB_ICONERROR);
        }

        if (!sigValid)
            MessageBox(nullptr, TEXT("Unsupported game version\n"), TEXT("Pinball Settings Mod"), MB_ICONERROR);


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
            WRITE_NOP(sigSetBall(), 6); //remove the code that set the number of balls, we will manually set a new one
            INSTALL_HOOK(pinballInit_r);
        }

        if (ballCount > 3) { //having more than 3 balls mess with red ring spawn timer for some reason... we manually repair that with a custom timer

            if (ballCount < 10)
                WRITE_MEMORY(sigRedRingBall(), uint8_t, ballCount - 1); //check current ballCount instead of hardcoded "2"
            else
                WRITE_MEMORY(sigRedRingBall(), uint8_t, 0x8);

            INSTALL_HOOK(redRingSpawn_r); //repair red ring timer
        }

        if (scoreRequirement != 5000000)
            INSTALL_HOOK(FUN_140718e10_r);

    }

    __declspec(dllexport) void __cdecl OnFrame()
    {

    }
}