// CircleDrawer.h
#pragma once
#include <atlimage.h>  // CImage
#include <cmath>

class CircleDrawer
{
public:
    CircleDrawer();
    void AttachImage(CImage* pImg); // 대화상자에서 만든 m_image를 연결

    // 기본 설정
    void SetPointRadius(int r) { m_pointRadius = r; }
    void SetEdgeThickness(int t) { m_edgeThickness = t; }

    // 점/원 그리기
    void DrawFilledDisk(int cx, int cy, int r, BYTE gray = 0);
    void DrawRing(int cx, int cy, int r, int thickness, BYTE gray = 0);

    // 3점 원 (수직이등분선)
    bool CircumcircleFrom3Pts(const POINT& A, const POINT& B, const POINT& C,
        double& cx, double& cy, double& r);

private:
    CImage* m_pImg = nullptr;  // 외부 이미지 버퍼
    int m_pointRadius = 10;
    int m_edgeThickness = 4;
};
