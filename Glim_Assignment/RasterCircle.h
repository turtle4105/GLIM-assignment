#pragma once
#include <atlimage.h>
#include "AppState.h"

namespace Raster {
    void PutPixel8(CImage& img, int x, int y, unsigned char gray);
    void DrawCircleMidpoint(CImage& img, int cx, int cy, int R, unsigned char gray);
    void DrawCircleThick(CImage& img, int cx, int cy, int R, int t, unsigned char gray);
    void FillMarker(CImage& img, int cx, int cy, int r, unsigned char gray);
}
