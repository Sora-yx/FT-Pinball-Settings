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

SIG_SCAN
(
    SigredRingGrab,
    0x14072A7B0, //LMAO
    "\x48\x89\x7C\x24\x20\x55\x48\x8D\x6C\x24\xA9\x48\x81\xEC\xF0\x00\x00\x00\x48\x8B\xF9\xC6\x81\x58\x02\x00\x00\x00\xE8\xCC\xCC\xCC\xCC\x48\x8B\xD0\x48\x8B\xCF\xE8\xCC\xCC\xCC\xCC\x33\xD2\x48\x8B\xC8\xE8\xCC\xCC\xCC\xCC\xE8\xCC\xCC\xCC\xCC\x48\x8B\xD0\x48\x8B\xCF\xE8\xCC\xCC\xCC\xCC\x33\xD2\x48\x8B\xC8\xE8\xCC\xCC\xCC\xCC\x48\x8D\x8F\x68\x02\x00\x00\x33\xD2\xE8\xCC\xCC\xCC\xCC\x48\x8D\x8F\x78\x02\x00\x00\x33\xD2\xE8\xCC\xCC\xCC\xCC\xF3\x0F\x10\x05\xCC\xCC\xCC\xCC\x48\x8D\x4D\xB7\x33\xC0\xF3\x0F\x11\x45\x97\x48\x89\x45\x8F\x48\x89\x45\x9F\x48\x89\x45\xA7\x89\x45\xAF\x66\xC7\x45\x87\x01\x00\xC6\x45\x89\x00\xC7\x45\x9B\xFF\xFF\xFF\xFF\xE8\xCC\xCC\xCC\xCC\x48\x8D\x05\xCC\xCC\xCC\xCC\xC6\x45\xD7\x00\x48\x89\x45\x8F\x48\x8B\x87\x50\x02\x00\x00\x48\x89\x45\xE7\xC6\x45\xEF\x01\xC6\x45\x87\x00\xE8\xCC\xCC\xCC\xCC\x48\x8B\xD0\x48\x8B\xCF\xE8\xCC\xCC\xCC\xCC\x45\x33\xC0\x48\x8D\x55\x87\x48\x8B\xC8\xE8\xCC\xCC\xCC\xCC\xE8\xCC\xCC\xCC\xCC\x48\x8B\xD0\x48\x8B\xCF\xE8\xCC\xCC\xCC\xCC\x4C\x8B\x05\xCC\xCC\xCC\xCC\x48\x8D\x55\x67\x0F\x57\xDB\x48\x8B\xC8\xE8\xCC\xCC\xCC\xCC\xE8\xCC\xCC\xCC\xCC\x48\x8B\xD0\x48\x8B\xCF\xE8\xCC\xCC\xCC\xCC\x4C\x8B\x05\xCC\xCC\xCC\xCC\x48\x8D\x55\x67\x0F\x57\xDB\x48\x8B\xC8\xE8\xCC\xCC\xCC\xCC\x48\x8B\x15", 
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxx????x????xxxxxxx????xxxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxxxxxxx????x????xxxxxxx????xxx????xxxxxxxxxxx????x????xxxxxxx????xxx????xxxxxxxxxxx????xxx"
)

HOOK(void, __fastcall, redRingGrab_r, SigredRingGrab(), size_t a1)
{
    redRingSpawnTimer = 0;
    originalredRingGrab_r(a1);
}

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
            INSTALL_HOOK(pinballInit_r); //set new ball count
        }

        if (ballCount > 3) { //having more than 3 balls mess with red ring spawn timer for some reason... we manually repair that with a custom timer

            if (ballCount < 10)
                WRITE_MEMORY(sigRedRingBall(), uint8_t, ballCount - 1); //check current ballCount instead of hardcoded "2"
            else
                WRITE_MEMORY(sigRedRingBall(), uint8_t, 0x8); //fix crash 

            //repair red ring timer
            INSTALL_HOOK(redRingSpawn_r);
            INSTALL_HOOK(redRingGrab_r);
        }

        if (scoreRequirement != 5000000)
            INSTALL_HOOK(FUN_140718e10_r);
    }

    __declspec(dllexport) void __cdecl OnFrame()
    {

    }
}