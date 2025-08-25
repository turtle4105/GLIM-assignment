#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include "AppState.h"

#define WM_APP_AUTOTICK (WM_APP + 1)

class AutoMover {
public:
    void Start(HWND, AppState*, std::mutex*) {}
    void Stop() {}
    bool IsRunning() const { return false; }
};
