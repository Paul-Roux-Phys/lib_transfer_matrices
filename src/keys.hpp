#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include "key_types/64bit.hpp"
#include "key_types/int8array.hpp"

template <typename Key>
struct KeyHash {
    size_t operator()(const Key& k) const {
        size_t hash = 5381;
        for (size_t i = 0; i < sizeof(k); i++) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(k[i]); // hash * 33 + c
        }
        return hash;
    }
};