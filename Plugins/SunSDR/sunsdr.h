#ifndef SUNSDR_H
#define SUNSDR_H

#define EXPORT extern "C" __declspec(dllexport) __cdecl

#include "windows.h"

enum StateChgReason {
    crPtt = 0,
    crDash = 1,
    crDot = 2,
    crAdc = 3
};

typedef __cdecl void (*CallbackFunc_SdrStateChanged)(UINT_PTR handle, StateChgReason reason, bool arg1, int arg2, int arg3);

void PttChanged(bool KeyPtt);
void DashChanged(bool KeyDash);
void DotChanged(bool KeyDot);
void AdcChanged(int Ufwd, int Uref);

#endif // SUNSDR_H
