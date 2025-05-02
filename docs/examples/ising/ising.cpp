#include "ising.hpp"

void IsingPM::add_vertical_edge(IsingState& v, int pos) {
  IsingPM b2 = *this; // copy *this

  value *= expJ;

  b2.key.set(pos, 1-key[pos]); // turns 0 to 1 and vice-versa
  b2.value *= expmJ;

  v += *this;
  v += b2;
}

void IsingPM::add_horizontal_edge(IsingState& v, int pos1, int pos2) {
  if (key[pos1] == key[pos2])
    value *= expJ;
  else 
    value *= expmJ;
  v += *this;
}

void IsingPM::r_matrix(IsingState& v, int pos) {
  if (pos%2 == 0)
    add_vertical_edge (v, pos/2);
  else
    add_horizontal_edge (v, pos/2, (pos/2+1)%SIZE);
}

/// [draining_iterator]
void IsingState::r_matrix(IsingState& tmp, int i) {
for (auto it = draining_begin (); it != draining_end (); it++)
  IsingPM (*it).r_matrix (tmp, i);
// here tmp = R_i * v
 swap (tmp); // pull the result back in v.
}
/// [draining_iterator]

void test_matrix() {
/// [test_matrix]
  IsingPM ini({1, 0, 1}, 1);

  IsingState s(ini), tmp;

  Matrix T(&s, &tmp, &IsingState::transfer);
  T.print();
  /* outputs
  Matrix of dimension 8:
  [
      14.0711   1         1         0.414214  1         0.414214  0.414214  1         ;
      5.82843   2.41421   0.414214  1         0.414214  1         0.171573  2.41421   ;
      5.82843   0.414214  2.41421   1         0.414214  0.171573  1         2.41421   ;
      2.41421   1         1         2.41421   0.171573  0.414214  0.414214  5.82843   ;
      5.82843   0.414214  0.414214  0.171573  2.41421   1         1         2.41421   ;
      2.41421   1         0.171573  0.414214  1         2.41421   0.414214  5.82843   ;
      2.41421   0.171573  1         0.414214  1         0.414214  2.41421   5.82843   ;
      1         0.414214  0.414214  1         0.414214  1         1         14.0711   
  ]
*/
/// [test_matrix]
/// [print_state_table]
  T.print_state_table();
  /* outputs:
      0  :  (1  1  1    )
      1  :  (1  1  0    )
      2  :  (1  0  1    )
      3  :  (1  0  0    )
      4  :  (0  1  1    )
      5  :  (0  1  0    )
      6  :  (0  0  1    )
      7  :  (0  0  0    )
  */
/// [print_state_table]
}

/// [transfer matrix]
void IsingState::transfer(IsingState& tmp) {
for (int pos = 0; pos < SIZE; pos++)
    r_matrix(tmp, 2*pos);
for (int pos = 0; pos < SIZE; pos++)
    r_matrix(tmp, 2*pos+1);
}
/// [transfer matrix]


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

int main() {
  test_matrix();
  return 0;
}
