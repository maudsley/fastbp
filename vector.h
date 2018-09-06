#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <vector>
#include <cstring>
#include <cmath>
#include <ostream>

template <typename T> class vector {
public:

  vector() {
  }

  vector(const size_t n) {
    v_ = std::vector<T>(n);
  }

  vector(const size_t n, const T& initial_value) {
    v_ = std::vector<T>(n, initial_value);
  }

  size_t size() const {
    return v_.size();
  }

  const T& operator [](const size_t i) const {
    return v_[i];
  }

  T& operator [](const size_t i) {
    return v_[i];
  }

  vector operator -() const {
    vector r(size());
    for (size_t i = 0; i < r.size(); ++i) {
      r[i] = -v_[i];
    }
    return r;
  }

  vector operator +(const T& rhs) const {
    vector r(size());
    for (size_t i = 0; i < r.size(); ++i) {
      r[i] = v_[i] + rhs;
    }
    return r;
  }

  vector& operator +=(const T& rhs) {
    *this = *this + rhs;
    return *this;
  }

  vector operator +(const vector& rhs) const {
    if (size() == rhs.size()) {
      vector r(size());
      for (size_t i = 0; i < r.size(); ++i) {
        r[i] = v_[i] + rhs[i];
      }
      return r;
    }
    return vector();
  }

  vector& operator +=(const vector& rhs) {
    *this = *this + rhs;
    return *this;
  }

  vector operator -(const T& rhs) const {
    vector r(size());
    for (size_t i = 0; i < r.size(); ++i) {
      r[i] = v_[i] - rhs;
    }
    return r;
  }

  vector& operator -=(const T& rhs) {
    *this = *this - rhs;
    return *this;
  }

  vector operator -(const vector& rhs) const {
    if (size() == rhs.size()) {
      vector r(size());
      for (size_t i = 0; i < r.size(); ++i) {
        r[i] = v_[i] - rhs[i];
      }
      return r;
    }
    return vector();
  }

  vector& operator -=(const vector& rhs) {
    *this = *this - rhs;
    return *this;
  }

  vector operator *(const T& rhs) const {
    vector r(size());
    for (size_t i = 0; i < r.size(); ++i) {
      r[i] = v_[i] * rhs;
    }
    return r;
  }

  vector& operator *=(const T& rhs) {
    *this = *this * rhs;
    return *this;
  }

  vector operator *(const vector& rhs) const {
    if (size() == rhs.size()) {
      vector r(size());
      for (size_t i = 0; i < r.size(); ++i) {
        r[i] = v_[i] * rhs[i];
      }
      return r;
    }
    return vector();
  }

  vector& operator *=(const vector& rhs) {
    *this = *this * rhs;
    return *this;
  }

  vector operator /(const T& rhs) const {
    vector r(size());
    for (size_t i = 0; i < r.size(); ++i) {
      r[i] = v_[i] + rhs;
    }
    return r;
  }

  vector& operator /=(const T& rhs) {
    *this = *this / rhs;
    return *this;
  }

  vector operator /(const vector& rhs) const {
    if (size() == rhs.size()) {
      vector r(size());
      for (size_t i = 0; i < r.size(); ++i) {
        r[i] = v_[i] / rhs[i];
      }
      return r;
    }
    return vector();
  }

  vector& operator /=(const vector& rhs) {
    *this = *this / rhs;
    return *this;
  }

private:

  std::vector<T> v_;

};

template <typename T> class matrix {
public:

  matrix() {
  }

  matrix(const size_t rows, const size_t cols) {
    m_ = vector<vector<T>>(rows);
    for (size_t i = 0; i < rows; ++i) {
      m_[i] = vector<T>(cols);
    }
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        if (i == j) {
          m_[i][j] = 1;
        } else {
          m_[i][j] = 0;
        }
      }
    }
  }

  explicit matrix(const vector<T>& src) {
    m_ = vector<vector<T>>(src.size());
    for (size_t i = 0; i < src.size(); ++i) {
      m_[i] = vector<T>(src.size(), 0);
      m_[i][i] = src[i];
    }
  }

  size_t rows() const {
    return m_.size();
  }

  size_t cols() const {
    if (!rows()) {
      return 0;
    } else {
      return m_[0].size();
    }
  }

  const vector<T>& operator [](const size_t i) const {
    return m_[i];
  }

  vector<T>& operator [](const size_t i) {
    return m_[i];
  }

  matrix operator +(const matrix& rhs) const {
    if (rows() == rhs.rows() && cols() == rhs.cols()) {
      matrix r = *this;
      for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < cols(); ++j) {
          r[i][j] += rhs[i][j];
        }
      }
      return r;
    }
    return matrix();
  }

  matrix& operator +=(const matrix& rhs) {
    *this = *this + rhs;
    return *this;
  }

  matrix operator -(const matrix& rhs) const {
    if (rows() == rhs.rows() && cols() == rhs.cols()) {
      matrix r = *this;
      for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < cols(); ++j) {
          r[i][j] -= rhs[i][j];
        }
      }
      return r;
    }
    return matrix();
  }

  matrix& operator -=(const matrix& rhs) {
    *this = *this - rhs;
    return *this;
  }

  matrix operator *(const T& rhs) const {
    return *this * matrix(vector<T>(cols(), rhs));
  }

  matrix& operator *=(const matrix& rhs) {
    *this = *this * rhs;
    return *this;
  }

private:

  vector<vector<T>> m_;

};

template <typename T> matrix<T> operator *(const matrix<T>& lhs, const matrix<T>& rhs) {
  if (lhs.cols() == rhs.rows()) {
    matrix<T> r(lhs.rows(), rhs.cols());
    for (size_t i = 0; i < lhs.rows(); ++i) {
      for (size_t j = 0; j < rhs.cols(); ++j) {
        T s = 0;
        for (size_t k = 0; k < lhs.cols(); ++k) {
          s += lhs[i][k] * rhs[k][j];
        }
        r[i][j] = s;
      }
    }
    return r;
  }
  return matrix<T>();
}

template <typename T> vector<T> operator *(const matrix<T>& lhs, const vector<T>& rhs) {
  if (lhs.cols() == rhs.size()) {
    vector<T> r(lhs.rows());
    for (size_t i = 0; i < lhs.rows(); ++i) {
      T s = 0.0;
      for (size_t j = 0; j < lhs.cols(); ++j) {
        s += lhs[i][j] * rhs[j];
      }
      r[i] = s;
    }
    return r;
  }
  return vector<T>();
}

template <typename T> vector<T> operator *(const vector<T>& lhs, const matrix<T>& rhs) {
  if (lhs.size() == rhs.rows()) {
    vector<T> r(rhs.cols());
    for (size_t i = 0; i < rhs.cols(); ++i) {
      T s = 0.0;
      for (size_t j = 0; j < rhs.rows(); ++j) {
        s += lhs[j] * rhs[j][i];
      }
      r[i] = s;
    }
    return r;
  }
  return vector<T>();
}

template <typename T> T inner_product(const vector<T>& lhs, const vector<T>& rhs) {
  if (lhs.size() == rhs.size()) {
    T r = 0;
    for (size_t i = 0; i < lhs.size(); ++i) {
      r += lhs[i] * rhs[i];
    }
    return r;
  }
  return 0;
}

template <typename T> matrix<T> outer_product(const vector<T>& lhs, const vector<T>& rhs) {
  matrix<T> r(lhs.size(), rhs.size());
  for (size_t i = 0; i < lhs.size(); ++i) {
    for (size_t j = 0; j < rhs.size(); ++j) {
      r[i][j] = lhs[i] * rhs[j];
    }
  }
  return r;
}

template <typename T> matrix<T> transpose(const matrix<T>& src) {
  matrix<T> r(src.cols(), src.rows());
  for (size_t i = 0; i < src.rows(); ++i) {
    for (size_t j = 0; j < src.cols(); ++j) {
      r[j][i] = src[i][j];
    }
  }
  return r;
}

template <typename T> vector<T> tanh(const vector<T>& src) {
  vector<T> r(src.size());
  for (size_t i = 0; i < r.size(); ++i) {
    r[i] = tanh(src[i]);
  }
  return r;
}

template <typename T> vector<T> tanh_derivative(const vector<T>& src) {
  vector<T> r(src.size());
  for (size_t i = 0; i < r.size(); ++i) {
    double y = tanh(src[i]);
    r[i] = 1 - y * y;
  }
  return r;
}

template <typename T> std::ostream& operator <<(std::ostream& stream, const vector<T>& src) {
  stream << "{";
  for (size_t i = 0; i < src.size(); ++i) {
    stream << (i != 0 ? ", " : "") << src[i];
  }
  stream << "}";
  return stream;
}

template <typename T> std::ostream& operator <<(std::ostream& stream, const matrix<T>& src) {
  stream << "{" << std::endl;
  for (size_t i = 0; i < src.rows(); ++i) {
    stream << "  " << src[i] << std::endl;
  }
  stream << "}";
  return stream;
}

typedef vector<double> real_vector;
typedef matrix<double> real_matrix;

#endif /* __VECTOR_H__ */
