#pragma once

template <typename Vec>
class VectorPair {
public:
    using Key = typename Vec::Key;
    using Value = typename Vec::Value;
    using BasisVector = typename Vec::BasisVector;
    using iterator = typename Vec::iterator;
    
private:
    Vec* _v[2];
    std::size_t _size;
    int _data_position;

public:
    VectorPair(Vec* v1, Vec* v2) : _v{v1, v2}, _data_position(0) {}

    Vec* get() const { return _v[_data_position]; }
    Vec* getempty() const { return _v[1-_data_position]; }
    void print() const { get()->print(); }
    void set_size() { _size = get()->size();}
    std::size_t size() { set_size(); return _size;}
    void swap_data_position() { _data_position = 1-_data_position; }

    template<typename... Params>
    void mul(RMatrix<Vec, Params...> r, Params... params) {
        multiply<Vec, Params...>(_v[1-_data_position], r, _v[_data_position], params...);
        swap_data_position();
    }

    void transfer();

    Value operator[](Key k) const {
        return (*this->get())[k];
    }

    VectorPair& operator+=(const std::pair<Key, Value>& p) {
        *get() += p;
        return *this;
    }

    VectorPair& operator+=(const BasisVector& b) {
        *get() += b;
        return *this;
    }

    VectorPair& operator+=(const Vec& v) {
        *get() += v;
        return *this;
    }

    VectorPair& operator/=(const double d) {
        get()->operator/=(d);
        return *this;
    }

    void add_if(BasisVector b, bool cond) {
        get()->add_if(b, cond);
    }

    void generate_space() {
        int old_dim=0, new_dim=1;

        while(new_dim > old_dim) {
            old_dim = new_dim;
            transfer();
            new_dim = size();
        }
    }

    void clear() { get()->clear(); getempty()->clear(); }

    LargeFloat inner_product(Vec& v) {
        return get()->inner_product(v);
    }

    LargeFloat inner_product(VectorPair<Vec>& v) {
        return get()->inner_product(v->get());
    }

    LargeFloat get_norm() {
        return get()->get_norm();
    }

    void factorise_norm() {
        get()->factorise_norm();
    }

    void normalise() {
        get()->normalise();
    }

    iterator begin()
    {
        return get()->begin();
    }

    iterator end()
    {
        return get()->end();
    }
};

// template <typename T>
// T Vector<T>::inner_product(const VectorPair<Vec>& v)
// {
//     return inner_product(*(v.get()));
// }

// template<typename T>
// Vector<T>::Vector(VectorPair<T, BasisVector<T>, Vector<T>> v, std::size_t size)
// : Vector(*v.get(), size)
// {} 