template <std::size_t size>
class int8_key_t : public std::array<int8_t, size> {
public:
    bool operator==(const int8_key_t& other) const {
        for (int i = 0; i < size; i++) {
            if (other[i] != (*this)[i]) return false;
        }
        return true;
    }

    // Print keys
    friend std::ostream& operator<<(std::ostream& os, const int8_key_t& k) {
        os << "(";
        for (const auto& e : k) {
            os << std::setw(3) << static_cast<int>(e);
        }
        os << "  )";
        return os;
    }
};

template <std::size_t size>
struct int8_key_hash_t {
    std::size_t operator()(const int8_key_t<size>& k) const {
        std::size_t hash = 5381;
        for (char c : k) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
        }
        return hash;
    }
};