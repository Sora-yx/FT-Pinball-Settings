#pragma once

struct pinballwk
{
    char idk0[56];
    long totalScore;
    char idk1[628];
    long score;
    char ballCount;
};

FUNCTION_PTR(void, __fastcall, pinballInit, 0x140718B80, pinballwk* a1, char a2, char a3);
FUNCTION_PTR(size_t, __fastcall, sub_140b8a8d0, 0x140b8a8d0, size_t* a1);
