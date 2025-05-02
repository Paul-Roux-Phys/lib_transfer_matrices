#include <ostream>
template <typename Vector>
class Matrix {
   public:
    using Value = typename Vector::Value;

   private:
    using Key = typename Vector::Key;
    using BasisVector = typename Vector::BasisVector;
    Vector *_v1, *_v2;
    int _size;
    // Key* _state_table;
    void (Vector::*_mul_method) (Vector&);

   public:
    Key* _state_table;

    Matrix (Vector* v1, Vector* v2,
            void (Vector::*mul) (Vector&) = &Vector::transfer)
        : _v1 (v1), _v2 (v2), _mul_method (mul) {
        _v1->generate_space (*_v2);
        _size = _v1->get_size ();
        make_state_table ();
        _v1->clear ();
    }

    ~Matrix () { delete[] _state_table; }

    int size () { return _size; }

    void MultMx (Value* x, Value* y) {
        // place x in the hash table
        for (int i = 0; i < _size; i++) {
            (*_v1)[_state_table[i]] = x[i];
        }

        // multiply by the transfer matrix
        (_v1->*_mul_method) (*_v2);

        // put the result back into y
        for (int i = 0; i < _size; i++) {
            y[i] = (*_v1)[_state_table[i]];
        }

        _v1->clear ();
    }

    void make_state_table () {
        _state_table = new Key[_size];
        int i = 0;
        for (auto it = _v1->draining_begin (); it != _v1->draining_end ();
             it++) {
            BasisVector b (*it);
            _state_table[i] = b.key;
            i++;
        }
    }

    void print_state_table () {
        for (int i = 0; i < _size; i++) {
            std::cout << std::setw (3) << i + 1 << ": ";
            _state_table[i].print ();
            std::cout << std::endl;
        }
    }

    Value operator() (int i, int j) {
        Value* x = new Value[_size];
        Value* y = new Value[_size];

        for (int k = 0; k < _size; k++) {
            x[k] = 0;
        }
        x[i] = 1;

        MultMx (x, y);

        Value res = y[j];

        delete[] x;
        delete[] y;
        return res;
    }

    void print (int precision = 6) {
        std::cout << "Matrix of dimension " << _size << ":" << std::endl
                  << "States: " << std::endl;
        print_state_table ();
        std::cout << "[" << std::endl;

        // Save current formatting state
        std::ios old_state (nullptr);
        old_state.copyfmt (std::cout);

        // Set local formatting
        std::cout << std::fixed << std::setprecision (precision)
		  << std::setw(precision + 6);

        for (int i = 0; i < _size; i++) {
            std::cout << std::setw (4) << " ";
            for (int j = 0; j < _size; j++) {
                std::cout << std::left << std::setw (precision + 4)
                          << (*this) (i, j);
            }
            if (i != _size - 1) {
                std::cout << ";" << std::endl;
            }
        }
        std::cout << std::endl << "]" << std::endl;

        // Restore formatting
        std::cout.copyfmt (old_state);
    }

    bool isapprox (Value a, Value b, Value eps) {
        return (abs (a - b) < eps || abs (1 - a / b) < eps);
    }
};
