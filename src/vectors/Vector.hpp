#include <boost/multiprecision/cpp_complex.hpp>

using LargeFloat = boost::multiprecision::cpp_complex_100;

/**
 * @class Vector<BasisVector, KeyHash>
 * @brief A class for storing large vectors.
 *
 * This class represents a vector. It stores the components of
 * the vector in a hash table implemented as a std::unordered_map.
 * (key, value) pairs have the type [BasisVector](@ref BasisVector).
 * This allows storing arbitrary key types and a O(1) time complexity
 * for insertion and lookup.
 */
template<typename B, typename KeyHash>
class Vector : public std::unordered_map<typename B::Key, typename B::Value, KeyHash> {
private:
  /**
   * @brief the norm squared of the vector.
   * This field store the squared norm of the object.
   * It is stored in a float type which cannot overflow.
  */
    LargeFloat _norm_sq; // norm_squared in the form (mantissa, exponent)
  /**
   * @brief the dimension of the vector
   * This fields stores the number of non-zero components
   * of the vector
   */
    std::size_t _size;

public:
    using BasisVector = B;
    using Key = typename BasisVector::Key;
    using Value = typename BasisVector::Value;
    // Using base class constructors
    using std::unordered_map<Key, Value, KeyHash>::unordered_map;
    using iterator = typename std::unordered_map<Key, Value, KeyHash>::iterator;

  /**
   * @brief default constructor
   *
   * Initializes a new `Vector`.
   * For optimization it is better to choose the size parameter
   * to be close to the expected number of non-zero components the
   * vector will need to store.
   */
    Vector(std::size_t size = 10000) : 
    std::unordered_map<Key, Value, KeyHash>(size),
    _norm_sq(1) 
    {}

  /**
   * @brief Copy constructor
   *
   * Initializes a new `Vector` which has the same components as
   * `v` and size `size`.
   */
    Vector(Vector& v, std::size_t size) :
    std::unordered_map<Key, Value, KeyHash>(size),
    _norm_sq(1)
    {
        for (auto it = v.begin(); it != v.end(); it++) {
            BasisVector b(*it);
            (*this)[b.key] = b.weight;
        }
    }

  /**
   * @brief Insertion operator
   *
   * Adds the basis vector `v` given as a (key, value) pair
   * to the `Vector` object.
   */
    Vector& operator+=(const std::pair<Key, Value>& v) {
        // Add a basis vector to the Vector
        (*this)[v.first] += v.second;
        return *this;
    }

  /**
   * @brief Insertion operator
   *
   * Adds the basis vector `v` to the `Vector` object.
   */
    Vector& operator+=(const BasisVector& v) {
        (*this)[v.key] += v.value;
        return *this;
    }

  /**
   * @brief add two vectors together
   */
    Vector& operator+=(const Vector& v) {
        for (auto it = v.begin(); it != v.end(); it++) {
            BasisVector b(*it);
            (*this)[b.key] += b.value;
        }
        return *this;
    }

  /**
   * @brief divide the vector by a scalar
   *
   * Divides the vector by a scalar. The scalar needs to be of the
   * same type as the values stored in the vector.
   */ 
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

  /**
   * @brief print a vector to stdout
   */
    void print() const {
        cout << "Norm: " << get_norm() << endl;
        cout << "Components:" << endl;
        for (const auto& pair : *this) {
            cout << "Key: "; pair.first.print();
            cout << std::setw(6) << " "
                 << "Value:  " << std::setprecision(15) << pair.second << endl;
        }
    }

  /**
   * @brief Draining iterator
   *
   * Iterate over the components of the vector, removing them from
   * the vector along the way. 
   */
    DrainingIterator<Vector> draining_begin() {
        return DrainingIterator<Vector>(this->begin(), this);
    }

    DrainingIterator<Vector> draining_end() {
        return DrainingIterator<Vector>(this->end(), this);
    }

    LargeFloat inner_product(const Vector& v) {
        LargeFloat res(0);
        for (auto it = v.begin(); it != v.end(); it++) {
            BasisVector b(*it);
            auto it2 = this->find(b.key);
            if (it2 != this->end()) {
                BasisVector b2(*it2);
                Value tmp = b.value * b2.value;
                res += LargeFloat(tmp.real(), tmp.imag());
            }
        }
        res *= get_norm();
        return res;
    }

    // Value inner_product(const VectorPair<T, BasisVector<T>, Vector<T>>& v);

  /**
   * returns the squared norm of the vector
   */
    LargeFloat get_norm_sq() const {
        return _norm_sq;
    }

  /**
   * returns the norm of the vector
   */
    LargeFloat get_norm() const {
        return sqrt(_norm_sq);
    }

  /**
   * Factorises the norm of the vector, i.e. performs
   * \( v \to |v| \frac{v}{|v|} \).
   * Since the overall norm is stored in a float type which
   * cannot overflow, this prevents overflow of the components
   * of the vector when using a standard float type for the values.
   */
    void factorise_norm() {
        Value tmp = 0;
        for (auto it = this->begin(); it != this->end(); it++) {
            BasisVector b(*it);
            tmp += abs(b.value*b.value);
        }
        *this /= sqrt(tmp);
        _norm_sq *= LargeFloat(tmp.real(), tmp.imag());
    }

  /**
   * Manually set the norm squared of the vector
   */
    void set_norm_sq(const LargeFloat &other) {
        _norm_sq = other;
    }

  /**
   * Normalise the vector
   */
    void normalise() {
        factorise_norm();
        set_norm_sq(1);
    }

    void set_size() {
        _size = 0;
        for (auto it = this->begin(); it != this->end(); it++) {
            _size++;
        }
    }

  /**
   * Returns the number of components of the vector.
   */
    std::size_t get_size() {
      _size = size();
      return _size;
    }

  /**
   * Remove all components of the vector. 
   */
    void clear() {
        std::unordered_map<Key, Value, KeyHash>::clear();
        _norm_sq = 1.0;
    }
};
