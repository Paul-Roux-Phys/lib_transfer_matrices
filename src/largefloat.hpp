#pragma once

using LargeFloat = std::pair<double, int>;

void standardise_largefloat(LargeFloat& f) {
    int res_exp;

    f.first = std::frexp(f.first, &res_exp);
    f.second += res_exp;
}

LargeFloat sqrt(LargeFloat f) {
    LargeFloat res;
    res = std::make_pair(std::sqrt(f.first), f.second/2);
    if (f.second%2 == 1) {
        res.first *= std::sqrt(2);
    }
    standardise_largefloat(res);
    return res;
}

std::ostream& operator<<(std::ostream& os, LargeFloat p) {
    os << std::setprecision(std::numeric_limits<double>::digits10);
    os << p.first << " * big\"2\"^" << p.second;
    return os;
}
