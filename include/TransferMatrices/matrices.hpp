#ifndef MATRICES_HPP
#define MATRICES_HPP

#include <functional>

template <typename Vector>
class Matrix {
public:
  using Key = typename Vector::Key;
  using Value = typename Vector::Value;

private:
  Vector *_v; // vector that holds the states
  Vector *_tmp; // vector used in intermediary computations
  std::size_t _size;
  Key* _state_table; // index the keys
  void (Vector::*_multiply)(Vector&);

public:
  Matrix(Vector* v, Vector* tmp, void (Vector::*M)(Vector&))
    : _v(v),
      _tmp(tmp),  
      _multiply(M),
      _size(v->get_size()) {
    generate_space();
    make_state_table();
  }

  ~Matrix() {
      delete[] _state_table;
  }

  void multiply() {
    (_v->*_multiply)(*_tmp);
  }

  void generate_space() {
    std::size_t old_dim = 0, new_dim = 1;
    while (new_dim > old_dim) {
      old_dim = new_dim;
      multiply();
      new_dim = _v->get_size();
    }
    _size = new_dim;
  }

  void set_size() {
    _size = _v->get_size();
  }

  std::size_t get_size() {
    return _size;
  }

  void MultMx(Value* x, Value* y) {
    // place x in the hash table
    for (int i=0; i < _size; i++) {
      (*_v)[_state_table[i]] = x[i];
    }
    // multiply be the matrix
    multiply();
    // put the result in y
    for (int i=0; i < _size; i++) {
      y[i] = (*_v)[_state_table[i]];
    }
    _v->clear();
  }

  void make_state_table() {
    _state_table = new Key[_size];
    int i=0;
    for (auto it = _v->draining_begin(); it != _v->draining_end(); it++) {
      BasisVector<Key, Value> b(*it);
      _state_table[i] = b.key;
      i++;
    }
  }

  void print_state_table() {
      for (int i=0; i < _size; i++) {
        cout << std::setw(3) << i << ": "; _state_table[i].print(); cout << endl;
      }
  }

  Value operator()(int i, int j) {
      Value* x = new Value[_size];
      Value* y = new Value[_size];

      for (int k=0; k < _size; k++) { x[k] = 0; }
      x[i] = 1;

      MultMx(x, y);

      Value res = y[j];
        
      delete[] x;
      delete[] y;
      return res;
  }

  void print(int column_width = 10) {
      cout << "Matrix of dimension " << _size << ":" << endl;
      cout << "[" << endl;;
      for (int i=0; i < _size; i++) {
          cout << std::setw(4) << " ";
          for (int j=0; j < _size; j++) {
              cout << std::left << std::setw(column_width) << (*this)(i, j);
          }
          if (i != _size-1) {
              cout << ";" << endl;
          }
      }
      cout << endl << "]" << endl;
  }
};

#endif // MATRICES_HPP
