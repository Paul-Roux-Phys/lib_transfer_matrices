#ifndef ISING_HPP
#define ISING_HPP

#include <algorithm>
#include <boost/multiprecision/cpp_complex.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <iostream>

#include "TransferMatrices.hpp"

// Lattice size
const size_t SIZE = 3;

// Hash table size
size_t HASH_SIZE = (2 << SIZE);  // 2^{L+1}

// Store keys on 64 bit integers. Store each site value on 3 bits.
// key[i] accesses the i-th element, and key.set(i) sets the i-th element.
// key_64_bit_t is provided by the library.
/// [Defining a key type]
#define bits_per_site 3
class Key : public key_64_bit_t<bits_per_site> {
public:
using key_64_bit_t<bits_per_site>::key_64_bit_t;
// we must specify how our keys should be printed
  void print () const { key_64_bit_t<bits_per_site>::print (SIZE); }
  void shift_right () {
    int last = (*this)[SIZE-1];
    for (int i = SIZE-2; i >= 0; i--) {
      set (i + 1, (*this)[i]);
    }
    set (0, last);
  }
};
/// [Defining a key type]

/// [Operations on keys]
void test_key() {
Key k ({1, 0, 1, 0});
k.print (); // output: (  1, 0, 1, 0  ) 
printf ("k[2] = %d\n", k[2]); // output: 1
 k.set (4, 1); // set key[4] to 1.
}
/// [Operations on keys]

using Hash = KeyHash<Key>;
// Complex numbers with 100 digits
class IsingPM;
class IsingState;


/// [Defining a basis]
using Weight = double;
class IsingPM : public BasisVector<Key, Weight> {
public:
using BasisVector<Key, Weight>::BasisVector;  // use the base constructors
IsingPM (Key k, Weight v) : BasisVector<Key, Weight> (k, v) {}
IsingPM (Key k) : BasisVector<Key, Weight> (k, Weight (1)) {}
// ...
/// [Defining a basis]
/// [Operations on basis states]
// in class IsingPM {
public:
void add_vertical_edge(IsingState& v, int pos);
void add_horizontal_edge(IsingState& v, int pos1, int pos2);
void r_matrix(IsingState& v, int pos);
}; // end of class IsingPM
/// [Operations on basis states]

Weight J = log(1+sqrt(2))/2;
Weight expJ  = exp(J);
Weight expmJ = exp(-J);
/// [Defining a vector class]

class IsingState : public Vector<IsingPM, Hash> {
public:
  using Vector<IsingPM, Hash>::Vector; // inherit base constructors
using Vector<IsingPM, Hash>::operator+=;
void print () const { Vector<IsingPM, Hash>::print (); }
// ...
/// [Defining a vector class]

/// [draining_iterator]
// in class IsingState {
public: 
void r_matrix(IsingState& tmp, int i) {
for (auto it = draining_begin (); it != draining_end (); it++)
    IsingPM (*it).r_matrix (tmp, i);
// here tmp = R_i * v
swap (tmp); // pull the result back in v.
}
/// [draining_iterator]

/// [transfer matrix]
// in class IsingState {
public:
void transfer(IsingState& tmp) {
for (int pos = 0; pos < SIZE; pos++)
    r_matrix(tmp, 2*pos);
for (int pos = 0; pos < SIZE; pos++)
    r_matrix(tmp, 2*pos+1);
}
/// [transfer matrix]
}; // end of class IsingState

/// [test_vector]
void test_vector() {
  IsingState v = IsingState(10); // initialises a vector with a hash table with 10 slots
  v *= 100.0; // multiplies the norm of v by 100
  v.factorise_norm(); // rewrites v as |v| * v' where v' has norm 1.
}
/// [test_vector]

/// [test_transfer]
void test_multiplication () {
  IsingState v, tmp;
  v += IsingPM({1, 0, 1}, 2.0); // v = 2 * |+-+>
  v.transfer(tmp); // v -> T*v
}
/// [test_transfer]

#endif // ISING_HPP
