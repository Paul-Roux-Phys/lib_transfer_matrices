# Home page            {#mainpage}

## Purpose of this library

This small template library provides a few convenient data types and methods that aim to make it easy to write performant transfer matrix programs.

## Dependencies

To use this library, the user should download and install the [mpfr](https://www.mpfr.org/), 
[mpc](https://www.multiprecision.org/mpc/), and [arpack](https://github.com/opencollab/arpack-ng)
libraries.

## Getting started

This library is a template library, meaning it requires no compilation. Simply put the directory
to `some/path/libtransftermatrices` and include `TransferMatrices.hpp` in your project:

```cpp
#include "TransferMatrices.hpp"
```

Make sure to compile with the option `-I/path/to/libtransfermatrices/include` so that the compiler
sees the file `TransferMatrices.hpp`.

## Usage

The library provides some convenient template classes to cover common needs in transfer matrix computations.

### Basis Vectors

The class [BasisVector<K, V>](@ref BasisVector) holds a `(key, value)` pair of arbitrary types.

A good practice is to derive from this class in order to define a particular basis of a space of states:

```cpp
using BV = BasisVector<bool *, double>;
class IsingPM : public BV {
private:
    using BV::BV;
};
```

In this example, we create a class `IsingPM` which holds `(key, value)` pairs, with 

* keys of type `char *`, (C-style strings): sequences of 0s and 1s.
* values of type `double`.

The last `using` statement lets `IsingPM` inherit from the `BasisVector` constructor.

Inheriting gives us some flexibility to integrate more data into the BasisVector if we want, e.g.
we could add

```cpp
class IsingPMBasis : public BV {
// ...
private:
    uint nb_domain_walls;
}
```

to store some more information about the state, in this case the number of Ising domain walls.
Or we could define additional methods on the state:

```cpp
public:
    int nb_domain_walls() {
        int res = 0;
        for (int i = 0; i < sizeof(*this) - 1; i++)
            if ((*this)[i] != (*this)[i+1])
                res += 1;
        return res;
    }
```

For the complete documentation of this class click [here](@ref BasisVector)

### Vectors

To represent a general vector, which is a linear combination of basis vectors, the library exposes
a class [Vector<BasisVector>](@ref Vector) which stores several `(key, value)` pairs in a hash table (or dictionary),
using the hash table implementation of the C++ standard library `std::unordered_map`.
This data structure allows for fast insertion, lookup, and deletion.

Like for `BasisVector`s, it is a good practice to inherit from the `Vector` class:

```cpp
class IsingState : public Vector<BasisVector> {
public:
    using Vector<BasisVector>::Vector<BasisVector>;
};
```

This again gives some flexibility to add methods and data to the state.

One can multiply a vector by a scalar, add a basis vector to a vector, or add vectors together.
Vectors can be looped through, or looped through and drained at the same time, which is useful to 
limit memory usage. 
To prevent overflowing of vector components, one can factorize the norm of the vector:

```cpp
IsingState v = IsingState(10); # initialises a vector with a hash table of size 10
v *= 100.0;
v.factorise_norm();
```

The norm of the vector is stored as an arbitrary precision number, which cannot overflow.

One can easily iterate over all components of a vector `v`: iterate over the vector and
cast the iterator to a `BasisVector`, in our example of type `IsingPMbasis`.

```cpp
for(auto it = v.begin(); it = v.end(); it++) {
	IsingPMBasis p(*it);
	printf("key = "); p.print(); printf("\n");
}
```

For the complete documentation of this class click [here](@ref Vector).

### R-matrices


