# Home page {#mainpage}

## Purpose of this library

This small template library offers a few convenient data types and methods designed to simplify writing efficient transfer matrix programs.

Specifically, the program is designed to perform computations on transfer matrices written in the form

$$T = \prod R_i,$$

where the \f$R_i\f$ are sparse matrices, in the sense that there exists a basis \f$|e_i \rangle\f$ such that \f$R |e_i \rangle\f$ has only a few non-zero components in the basis \f$|e_i\rangle\f$.

## Installation

This library is a template library, meaning the library itself requires no compilation. Simply clone or download the 
[git repo](https://github.com/Paul-Roux-Phys/lib_transfer_matrices/) to `some/path/lib_transfter_matrices`
and include `TransferMatrices.hpp` in your project:

```cpp
#include "TransferMatrices.hpp"
```

Make sure to compile with the option `-I/path/to/lib_transfer_matrices/include` so that the compiler
finds the file `TransferMatrices.hpp`.

### Dependencies

The library depends on a few external libraries: 

 * [mpfr](https://www.mpfr.org/)
 * [mpc](https://www.multiprecision.org/mpc/)
 * [arpack](https://github.com/opencollab/arpack-ng)
 * [blas](http://www.openmathlib.org/OpenBLAS/docs/user_manual/)
 * [boost/multiprecision](https://www.boost.org/)

These libraries need to be installed on your system in order to use `lib_transfer_matrices`.
The easiest way to achieve this is in your console, via your system's package manager; here are a few examples for popular packages managers:

```bash
# on macOS
brew install arpack mpfr libmpc openblas boost

# on windows: via vcpkg (https://vcpkg.io/en/index.html)
vcpkg install arpack-ng mpfr mpc openblas boost

# on linux; depending on your distribution: 
# ubuntu/debian based
sudo apt install libarpack2-dev libmpfr-dev libmpc-dev libopenblas-dev libboost-all-dev
# Fedora, CentOS, or RHEL-based Systems
sudo dnf install arpack-devel mpfr-devel libmpc-devel openblas-devel boost-devel
# for older versions
sudo yum install arpack-devel mpfr-devel libmpc-devel openblas-devel boost-devel
# on arch / Manjaro
sudo pacman -S arpack mpfr libmpc openblas boost
# on openSUSE
sudo zypper install arpack-devel mpfr-devel libmpc-devel openblas-devel boost-devel
```
