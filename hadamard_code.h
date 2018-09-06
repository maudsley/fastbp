#ifndef __HADAMARD_CODE_H__
#define __HADAMARD_CODE_H__

#include "binary_element.h"

class hadamard_code {
public:

  hadamard_code(const size_t size) {
    std::vector<binary_vector> ident;
    std::vector<binary_vector> gen;
    size_t num_states = 1 << size;
    for (size_t i = 0; i < num_states; ++i) {
      if (i && (i & (i - 1)) == 0) {
        ident.push_back(binary_vector_from_int(i, size));
      } else if (i) { /* skip the all-zero vector */
        gen.push_back(binary_vector_from_int(i, size));
      }
    }

    const size_t num_g_rows = ident.size() + gen.size();
    g_ = binary_matrix(num_g_rows, size);
    for (size_t i = 0; i < ident.size(); ++i) {
      g_[i] = ident[i];
    }
    for (size_t i = 0; i < gen.size(); ++i) {
      g_[i + ident.size()] = gen[i];
    }

    size_t num_h_cols = size + gen.size();
    h_ = binary_matrix(gen.size(), num_h_cols);
    for (size_t i = 0; i < gen.size(); ++i) {
      for (size_t j = 0; j < size; ++j) {
        h_[i][j] = gen[i][j];
      }
      for (size_t j = 0; j < gen.size(); ++j) {
        if (i == j) {
          h_[i][size + j] = 1;
        } else {
          h_[i][size + j] = 0;
        }
      }
    }
  }

  const binary_matrix& generator_matrix() const {
    return g_;
  }

  const binary_matrix& parity_check_matrix() const {
    return h_;
  }

private:

  binary_matrix g_;
  binary_matrix h_;

};

#endif /* __HADAMARD_CODE_H__ */
