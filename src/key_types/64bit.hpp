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