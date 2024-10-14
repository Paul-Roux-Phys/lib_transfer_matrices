#include "TransferMatrices.hpp"

using namespace transfer_matrices;

using K = int8_key_t<4>;
using V = double;
using BasisVector = BasisVector<K, V>;
using Vec = Vector<BasVector, KeyHash<K>>;
using R = RMatrix<Vec>;

// identity r matrix
void r_matrix(Vec* v, BV b) {
    *v += b;
}

void swap(Vec* &v1, Vec* &v2) {
    Vec* tmp = v2;
    v2 = v1;
    v1 = tmp;
}

int main() {
    BV b({1, -2, 3, 4}, 1.0);

    Vec v1, v2;
    v1 += b;

    multiply<Vec>(&v2, std::function<void(Vec*, BV)>(r_matrix), &v1);

    cout << "v1: " << endl;
    v1.print();
    cout << "v2: " << endl;
    v2.print();

    swap(v1, v2);
    cout << "v1: " << endl;
    v1.print();
    cout << "v2: " << endl;
    v2.print();



    return 0;
}