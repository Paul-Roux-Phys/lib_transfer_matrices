#pragma once

#include <functional>

template <typename Vector, typename... Params>
using RMatrix = std::function<void(Vector*, typename Vector::BasisVector, Params...)>;

template <typename Vector, typename... Params>
void multiply(Vector* v1, RMatrix<Vector, Params...> r, Vector* v2, Params... params) {
    for (auto it = v2->draining_begin(); it != v2->draining_end(); ++it) {
        typename Vector::BasisVector b(*it);
        r(v1, b, params...);
    }
    v1->set_norm_sq(v2->get_norm_sq());
}