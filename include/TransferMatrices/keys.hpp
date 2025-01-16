#ifndef KEY_HPP
#define KEY_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>

template <unsigned int bits_per_site>
class key_64_bit_t {
private:
    static constexpr uint64_t MASK = (1ULL << bits_per_site) - 1;
    uint64_t bits;

public:
    key_64_bit_t (uint64_t a = 0) : bits(a) {}
    key_64_bit_t (std::initializer_list<int> a) : bits(0) {
        int i = 0;
        for (int el : a) set(i++, el); 
    }

    int operator[](int i) const {
        return (bits >> bits_per_site*i) & MASK;
    }

    void clear(int i) {
        bits &= ~(MASK << bits_per_site*i);
    }

    void set(int i, int value) {
        clear(i);
        bits |= (static_cast<uint64_t>(value) << bits_per_site*i);
    }

    bool operator==(const key_64_bit_t& other) const {
        return bits == other.bits;
    }
    
    void print(size_t size) const {
        std::cout << " (";
        for (size_t i = 0; i < size; i++) {
            std::cout << std::setw(3) << operator[](i);
        }
        std::cout << "  )";
    }
};

template <std::size_t size>
class key_int8_t : public std::array<int8_t, size> {
public:
    bool operator==(const key_int8_t& other) const {
        for (int i = 0; i < size; i++) {
            if (other[i] != (*this)[i]) return false;
        }
        return true;
    }

    // Print keys
    friend std::ostream& operator<<(std::ostream& os, const key_int8_t& k) {
        os << "(";
        for (const auto& e : k) {
            os << std::setw(3) << static_cast<int>(e);
        }
        os << "  )";
        return os;
    }
};

template <std::size_t size>
struct key_int8_hash_t {
    std::size_t operator()(const key_int8_t<size>& k) const {
        std::size_t hash = 5381;
        for (char c : k) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
        }
        return hash;
    }
};

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

#endif // KEY_HPP
#include "../../src/keys.hpp"
