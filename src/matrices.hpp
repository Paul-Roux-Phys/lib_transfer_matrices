#include <functional>

template <typename Vector>
using TransferMatrix = std::function<void(Vector&, Vector&)>;

template <typename Vector, typename Real>
class MatrixMul {
public:
    using Key = typename Vector::Key;
    using Value = typename Vector::Value;

private:
    Vector *_v[2]; // pair of pointers to vectors
    int _data_position;
    std::size_t _size;
    Key* _state_table;
    std::function<void(Vector&, Vector&)> _transfer_vv;

public:
    MatrixMul(Vector* v1, Vector* v2, TransferMatrix<Vector> t)
    : _data_position(0),
    _v({v1, v2}),
    _transfer_vv(t),
    _size(v1->get_size())
    {
        // generate_space();
        // _v = v;
        // _size = v->size();
        // make_state_table();
        // _v->clear();
        // _eig_prob.DefineParameters(size(), nev, this, &Matrix::MultMx, which);
    }

    // ~Matrix() {
    //     delete[] _state_table;
    // }

    void generate_space() {
        
    }

    void set_size() {
        _v[_data_position]->set_size();
        _size = _v[_data_position]->get_size();
    }

    std::size_t get_size() {
        return _size;
    }

    void transfer() {
        _transfer_vv(_v[1-_data_position], _v[_data_position]);
        _data_position = 1 - _data_position;
    }

    // void MultMx(Value* x, Value* y) {
    //     // place x in the hash table
    //     for (int i=0; i < _size; i++) {
    //         (*_v->getempty())[_state_table[i]] = x[i];
    //     }
    //     _v->swap_data_position();

    //     // multiply by the transfer matrix
    //     _v->transfer();

    //     // put the result back into y
    //     for (int i=0; i < _size; i++) {
    //         y[i] = (*_v)[_state_table[i]];
    //     }

    //     _v->clear();
    // }

    // void make_state_table() {
    //     _state_table = new Key[_size];
    //     int i=0;
    //     for (auto it = _v->get()->draining_begin(); it != _v->get()->draining_end(); it++) {
    //         BasisVector<W> b(*it);
    //         _state_table[i] = b.key;
    //         i++;
    //     }
    // }

    // void print_state_table() {
    //     for (int i=0; i < _size; i++) {
    //         cout << std::setw(3) << i+1 << ": " << _state_table[i] << endl;
    //     }
    // }

    // W operator()(int i, int j) {
    //     double* x = new W[_size];
    //     double* y = new W[_size];

    //     for (int k=0; k < _size; k++) { x[k] = 0; }
    //     x[i] = 1;

    //     MultMx(x, y);

    //     W res = y[j];
        
    //     delete[] x;
    //     delete[] y;
    //     return res;
    // }

    // void print(int column_width = 10) {
    //     cout << "Matrix of dimension " << _size << ":" << endl;
    //     cout << "[" << endl;;
    //     for (int i=0; i < _size; i++) {
    //         cout << std::setw(4) << " ";
    //         for (int j=0; j < _size; j++) {
    //             cout << std::left << std::setw(column_width) << (*this)(i, j);
    //         }
    //         if (i != _size-1) {
    //             cout << ";" << endl;
    //         }
    //     }
    //     cout << endl << "]" << endl;
    // }
};