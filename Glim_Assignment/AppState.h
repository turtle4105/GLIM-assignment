#pragma once
#include <Windows.h>

struct Pt { int x{ 0 }, y{ 0 }; };
struct Circ { double cx{ 0 }, cy{ 0 }, R{ 0 }; };
enum class Phase { Waiting1, Waiting2, Waiting3, Ready };

struct AppState {
    Phase phase{ Phase::Waiting1 };
    Pt P[3];
    bool hasCircle{ false };
    Circ circle;
    bool dragging{ false };
    int  dragHit{ -1 };
    Pt   prevMouse{};
    int markerR{ 10 };
    int thickness{ 2 };
    RECT canvas{ 0,0,0,0 };
    int bufW{ 800 }, bufH{ 600 }, bufBpp{ 8 };
};
