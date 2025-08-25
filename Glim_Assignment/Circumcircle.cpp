#include "pch.h"
#include "Circumcircle.h"
#include <cmath>

// 면적의 2배로 일직선 판정
bool MathCirc::Collinear(const Pt& A, const Pt& B, const Pt& C, double eps) {
    double D = A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y);
    return std::fabs(D) < eps;
}

// 두 수직 이등분선의 교점 → 중심
bool MathCirc::ComputeCircumcircle(const Pt& p1, const Pt& p2, const Pt& p3, Circ& out) {
    if (Collinear(p1, p2, p3)) return false;

    double a = p1.x - p2.x;
    double b = p1.y - p2.y;
    double c = p1.x - p3.x;
    double d = p1.y - p3.y;

    double e = (p1.x * p1.x - p2.x * p2.x + p1.y * p1.y - p2.y * p2.y) * 0.5;
    double f = (p1.x * p1.x - p3.x * p3.x + p1.y * p1.y - p3.y * p3.y) * 0.5;

    double det = a * d - b * c;
    if (std::fabs(det) < 1e-9) return false;

    out.cx = (d * e - b * f) / det;
    out.cy = (-c * e + a * f) / det;
    out.R = std::sqrt((out.cx - p1.x) * (out.cx - p1.x) + (out.cy - p1.y) * (out.cy - p1.y));
    return true;
}
