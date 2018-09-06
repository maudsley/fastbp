#ifndef __BINARY_ELEMENT_H__
#define __BINARY_ELEMENT_H__

#include "vector.h"

class binary_element {
public:

  binary_element() {
  }

  binary_element(const int value) : v_(value) {
  }

  binary_element(const binary_element& rhs) {
    *this = rhs;
  }

  uint8_t value() const {
    return v_;
  }

  binary_element& operator =(const binary_element& rhs) {
    v_ = rhs.v_;
    return *this;
  }

  binary_element operator +(const binary_element& rhs) const {
    return binary_element(v_ ^ rhs.v_);
  }

  binary_element& operator +=(const binary_element& rhs) {
    *this = *this + rhs;
    return *this;
  }

  binary_element operator *(const binary_element& rhs) const {
    return binary_element(v_ & rhs.v_);
  }

  binary_element& operator *=(const binary_element& rhs) {
    *this = *this * rhs;
    return *this;
  }

  uint8_t v_;

};

inline std::ostream& operator <<(std::ostream& stream, const binary_element& rhs) {
  if (rhs.v_) {
    stream << "1";
  } else {
    stream << ".";
  }
  return stream;
}

typedef vector<binary_element> binary_vector;
typedef matrix<binary_element> binary_matrix;

inline binary_vector binary_vector_from_int(const uint64_t x, const size_t n) {
  binary_vector v(n);
  for (size_t i = 0; i < n; ++i) {
    v[i] = (x >> i) & 1;
  }
  return v;
}

inline uint64_t binary_vector_to_int(const binary_vector& v) {
  uint64_t x = 0;
  for (size_t i = 0; i < v.size(); ++i) {
    x |= uint64_t(v[i].value()) << i;
  }
  return x;
}

inline real_vector binary_to_real_vector(const binary_vector& v) {
  real_vector r(v.size());
  for (size_t i = 0; i < v.size(); ++i) {
    r[i] = v[i].value();
  }
  return r;
}

inline binary_vector binary_from_real_vector(const real_vector& v) {
  binary_vector r(v.size());
  for (size_t i = 0; i < v.size(); ++i) {
    r[i] = (v[i] > 0.5 ? 1 : 0); /* hard decision */
  }
  return r;
}

#endif /* __BINARY_ELEMENT_H__ */
