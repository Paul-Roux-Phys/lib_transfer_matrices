# Representation of vectors

The library provides a type template `Vector<BasisVector>` for representing vectors as dictionaries of (key, value) pairs.

## Basis vectors

The type `BasisVector<Key, Value>` is a template type for representing a basis state, as a (key, value) pair. The value is typically meant to be a scalar, but it could be a more complex type, containing a scalar representing the state's weight, as well as additional data.

The `Key` type must

* implement a copy constructor
* overload the `operator<<` (for printing)
* overload the == (comparison) operator.

The `Value` type must

* implement a copy constructor
* overload the operators `+=`, `*=`, `*`, `+`, `/`, `/=`. This is automatically verified if `Value` is a scalar type such as `double`.
* overload the `operator<<``.

## Vectors

A `Vector` object can be created with

```c++
Vector<BasisVector, KeyHash> v(size);
```

The size passed as argument corresponds to the number of buckets in the hash-table internal implementation of the Vector. It is important for performance to choose the size argument to be larger (ideally ~1.3x) than the expected dimension of the vector, to avoid dynamic resizing.
The `KeyHash` type is a `struct` than must contain a function

```c++
std::size_t operator()(const Key& key) const {
    std::size_t hash;
    // add some hash logic
    return hash;
};
```

A basis vector can be added to a vector with

```c++
BasisVector<char[4], double> b({1, 2, 3, 4}, 1.0); // create basis vector b with key {1, 2, 3, 4} and value 1.0
Vector v(size); // create an empty vector v
v += b; // add b to v
cout << v; // print v
```

The output is

```sh

```
