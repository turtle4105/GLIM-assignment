#include "AppState.h"

namespace MathCirc {
    bool Collinear(const Pt& a, const Pt& b, const Pt& c, double eps = 1e-6);
    bool ComputeCircumcircle(const Pt& a, const Pt& b, const Pt& c, Circ& out);
}
