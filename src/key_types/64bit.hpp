constexpr uint64_t mask = 7;
template<std::size_t size>
class key_64_bit_t {
private:
    uint64_t bits;

public:
    key_64_bit_t (uint64_t a = 0) : bits(a) {}
    key_64_bit_t (std::initializer_list<int> a) {
        int i = 0;
        for (auto el : a) set(i++, el);
    }

    int operator[](int i) const {
        uint64_t m = mask << 3*i;
        return ((bits & m) >> 3*i);
    }

    void clear(int i) {
        uint64_t m = ~(mask << 3*i);
        bits &= m;
    }

    void set(int i, int value) {
        uint64_t v = static_cast<uint64_t>(value);
        clear(i);
        bits |= (v << 3*i);
    }

    bool operator==(const key_64_bit_t& other) const {
        for (int i = 0; i < size; i++) {
            if ((*this)[i] != other[i]) return false;
        }
        return true;
    }

    void shift_right() {
        int tmp = (*this)[size-1];
        for (int i = size - 2; i >= 0; i--) {
            set(i+1, (*this)[i]);
        }
        set(0, tmp);
    }
    
    friend std::ostream& operator<<(std::ostream& os, key_64_bit_t k) {
        os << "(";
        for (int i = 0; i < size; i++) {
            os << std::setw(3) << k[i];
        }
        os << "  )";
        return os;
    }
};

template <std::size_t size>
struct key_64_bit_hash_t {
    std::size_t operator()(const key_64_bit_t<size>& k) const {
        std::size_t hash = 5381;
        for (int i = 0; i < size; i++) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(k[i]); // hash * 33 + c
        }
        return hash;
    }
};