template <typename K, typename V>
class BasisVector {
public:
    using Key = K;
    using Value = V;
    Key key;
    Value value;

    BasisVector() : key(), value() {}
    BasisVector(Key k, Value v) : key(k), value(v) {}
    BasisVector(const BasisVector& other) : key(other.key), value(other.value) {}
    BasisVector(std::pair<Key, Value> v) : key(v.first), value(v.second) {}

    void print() const {
        std::cout << "Key: " << key 
             << std::setw(6) << " "
             << "Value:  " << value << std::endl;
    }
};
