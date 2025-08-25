#include "pch.h"
#include "RasterCircle.h"
#include <cstring>

static inline bool InImage(int x, int y, int w, int h) {
    return (0 <= x && x < w && 0 <= y && y < h);
}

// 8bpp 이미지 한 픽셀 찍기
void Raster::PutPixel8(CImage& img, int x, int y, unsigned char g) {
    int w = img.GetWidth(), h = img.GetHeight();
    if (!InImage(x, y, w, h)) return;
    int pitch = img.GetPitch();
    auto* fm = (unsigned char*)img.GetBits();
    fm[y * pitch + x] = g;
}

// 중점 원 알고리즘(외곽선 한 겹)
void Raster::DrawCircleMidpoint(CImage& img, int cx, int cy, int R, unsigned char gray) {
    if (R <= 0) return;
    int x = 0, y = R;
    int p = 1 - R;

    auto draw8 = [&](int ox, int oy) {
        PutPixel8(img, cx + ox, cy + oy, gray);
        PutPixel8(img, cx - ox, cy + oy, gray);
        PutPixel8(img, cx + ox, cy - oy, gray);
        PutPixel8(img, cx - ox, cy - oy, gray);
        PutPixel8(img, cx + oy, cy + ox, gray);
        PutPixel8(img, cx - oy, cy + ox, gray);
        PutPixel8(img, cx + oy, cy - ox, gray);
        PutPixel8(img, cx - oy, cy - ox, gray);
        };

    while (x <= y) {
        draw8(x, y);
        ++x;
        if (p < 0) p += 2 * x + 1;
        else { --y; p += 2 * (x - y) + 1; }
    }
}

// 두께 t 적용(둘레를 겹쳐 그리기)
void Raster::DrawCircleThick(CImage& img, int cx, int cy, int R, int t, unsigned char gray) {
    if (t < 1) t = 1;
    int half = t / 2;
    for (int k = -half; k <= half; ++k) {
        int r = R + k;
        if (r > 0) DrawCircleMidpoint(img, cx, cy, r, gray);
    }
}

// 마커(작은 채움 원)
void Raster::FillMarker(CImage& img, int cx, int cy, int r, unsigned char gray) {
    if (r <= 0) return;
    int w = img.GetWidth(), h = img.GetHeight();
    int pitch = img.GetPitch();
    auto* fm = (unsigned char*)img.GetBits();
    int r2 = r * r;

    for (int y = cy - r; y <= cy + r; ++y) {
        for (int x = cx - r; x <= cx + r; ++x) {
            int dx = x - cx, dy = y - cy;
            if (dx * dx + dy * dy <= r2) {
                if (InImage(x, y, w, h)) fm[y * pitch + x] = gray;
            }
        }
    }
}
