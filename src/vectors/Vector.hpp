template<typename B, typename KeyHash>
class Vector : public std::unordered_map<typename B::Key, typename B::Value, KeyHash> {
private:
    LargeFloat _norm_sq; // norm_squared in the form (mantissa, exponent)
    std::size_t _size;

public:
    // Using base class constructors
    using BasisVector = B;
    using Key = typename BasisVector::Key;
    using Value = typename BasisVector::Value;
    using std::unordered_map<Key, Value, KeyHash>::unordered_map;
    using iterator = typename std::unordered_map<Key, Value, KeyHash>::iterator;
    
    Vector(std::size_t size = 10000) : 
    std::unordered_map<Key, Value, KeyHash>(size),
    _norm_sq(std::make_pair(1.0, 0)) 
    {}

    Vector(Vector& v, std::size_t size) :
    std::unordered_map<Key, Value, KeyHash>(size),
    _norm_sq(std::make_pair(1.0, 0))
    {
        for (auto it = v.begin(); it != v.end(); it++) {
            BasisVector b(*it);
            (*this)[b.key] = b.weight;
        }
    }

    // Vector(VectorPair<T, BasisVector<T>, Vector<T>>, std::size_t size=HASH_TABLE_SIZE);

    Vector& operator+=(const std::pair<Key, Value>& v) {
        // Add a basis vector to the Vector
        (*this)[v.first] += v.second;
        return *this;
    }

    Vector& operator+=(const BasisVector& v) {
        (*this)[v.key] += v.value;
        return *this;
    }

    Vector& operator+=(const Vector& v) {
        for (auto it = v.begin(); it != v.end(); it++) {
            BasisVector b(*it);
            (*this)[b.key] += b.value;
        }
        return *this;
    }

    Vector& operator/=(const Value v) {
        for (auto it = this->begin(); it != this->end(); it++) {
            BasisVector b(*it);
            (*this)[b.key] /= v;
        }
        return *this;
    }

    void add_if(const BasisVector& b, bool cond) {
        if (cond) {
            *this += b;
        }
    }

    void print() {
        cout << "Norm: " << get_norm() << endl;
        cout << "Components:" << endl;
        for (const auto& pair : *this) {
            cout << "Key: " << pair.first 
                 << std::setw(6) << " "
                 << "Value:  " << pair.second << endl;
        }
    }

    DrainingIterator<Vector> draining_begin() {
        return DrainingIterator<Vector>(this->begin(), this);
    }

    DrainingIterator<Vector> draining_end() {
        return DrainingIterator<Vector>(this->end(), this);
    }

    LargeFloat inner_product(const Vector& v) {
        LargeFloat res(0.0, 0);
        for (auto it = v.begin(); it != v.end(); it++) {
            BasisVector b(*it);
            auto it2 = this->find(b.key);
            if (it2 != this->end()) {
                BasisVector b2(*it2);
                res.first += b.value * b2.value;
            }
        }
        res.first *= get_norm().first;
        res.second += get_norm().second;
        standardise_largefloat(res);
        return res;
    }

    // Value inner_product(const VectorPair<T, BasisVector<T>, Vector<T>>& v);

    LargeFloat get_norm() {
        return sqrt(_norm_sq);
    }

    LargeFloat get_norm_sq() {
        standardise_largefloat(_norm_sq);
        return _norm_sq;
    }

    void set_norm_sq(const LargeFloat &other) {
        _norm_sq = other;
    }

    void factorise_norm() {
        Value tmp = 0.0;
        int tmp_exp;
        for (auto it = this->begin(); it != this->end(); it++) {
            BasisVector b(*it);
            tmp += abs(b.value*b.value);
        }
        *this /= std::sqrt(tmp);
        tmp = std::frexp(tmp, &tmp_exp);
        _norm_sq.first *= tmp;
        _norm_sq.second += tmp_exp;
    }

    void set_size() {
        _size = 0;
        for (auto it = this->begin(); it != this->end(); it++) {
            _size++;
        }
    }

    std::size_t get_size() {
        return _size;
    }
};