// CircleDrawer.cpp
#include "pch.h"
#undef min
#undef max
#include "CircleDrawer.h"
#include <algorithm>
#include <cmath>

CircleDrawer::CircleDrawer() {}

void CircleDrawer::AttachImage(CImage* pImg)
{
    m_pImg = pImg;
}

void CircleDrawer::DrawFilledDisk(int cx, int cy, int r, BYTE gray)
{
    if (!m_pImg) return;
    const int w = m_pImg->GetWidth();
    const int h = m_pImg->GetHeight();
    const int pitch = m_pImg->GetPitch();
    unsigned char* fm = (unsigned char*)m_pImg->GetBits();

    const int r2 = r * r;
    for (int y = std::max(0, cy - r); y <= std::min(h - 1, cy + r); ++y) {
        for (int x = std::max(0, cx - r); x <= std::min(w - 1, cx + r); ++x) {
            int dx = x - cx, dy = y - cy;
            if (dx * dx + dy * dy <= r2) fm[y * pitch + x] = gray;
        }
    }
}

void CircleDrawer::DrawRing(int cx, int cy, int r, int thickness, BYTE gray)
{
    if (!m_pImg) return;
    if (r <= 0 || thickness <= 0) return;
    int inner = std::max(0, r - thickness);

    const int w = m_pImg->GetWidth();
    const int h = m_pImg->GetHeight();
    const int pitch = m_pImg->GetPitch();
    unsigned char* fm = (unsigned char*)m_pImg->GetBits();

    const int r2 = r * r;
    const int in2 = inner * inner;
    for (int y = std::max(0, cy - r); y <= std::min(h - 1, cy + r); ++y) {
        for (int x = std::max(0, cx - r); x <= std::min(w - 1, cx + r); ++x) {
            int dx = x - cx, dy = y - cy;
            int d2 = dx * dx + dy * dy;
            if (d2 <= r2 && d2 >= in2) fm[y * pitch + x] = gray;
        }
    }
}

bool CircleDrawer::CircumcircleFrom3Pts(const POINT& A, const POINT& B, const POINT& C,
    double& cx, double& cy, double& r)
{
    double x1 = A.x, y1 = A.y, x2 = B.x, y2 = B.y, x3 = C.x, y3 = C.y;
    double d = 2.0 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    if (fabs(d) < 1e-6) return false;

    double x1s = x1 * x1 + y1 * y1;
    double x2s = x2 * x2 + y2 * y2;
    double x3s = x3 * x3 + y3 * y3;

    cx = (x1s * (y2 - y3) + x2s * (y3 - y1) + x3s * (y1 - y2)) / d;
    cy = (x1s * (x3 - x2) + x2s * (x1 - x3) + x3s * (x2 - x1)) / d;
    r = std::sqrt((cx - x1) * (cx - x1) + (cy - y1) * (cy - y1));
    return true;
}
