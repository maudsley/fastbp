#ifndef __PROB_UTILS_H__
#define __PROB_UTILS_H__

/* utility methods for dealing with logarithmic probability */

#include <limits>
#include <cmath>
#include <algorithm>

inline double prob_h_log(double x) {
  if (x == 0) {
    return 0;
  } else {
    return log2(x);
  }
}

inline double prob_log(double x) {
  if (x == 0) {
    return -std::numeric_limits<double>::infinity();
  } else {
    return log2(x);
  }
}

inline double prob_exp(double x) {
  if (std::isinf(x)) {
    return 0;
  } else {
    return exp2(x);
  }
}

inline double prob_exp_approx(double x) {
  int n = int(x);
  double y = double(uint64_t(1) << -n);
  double t = (x - n) * 0.69314718056; /* ln(2); */
  //double c = 1 + t * (1 + t * (1.0/2 + t * (1.0/6 + t/24)));
  double c = 1 + t * (1 + t * (1.0/2 + t/6));
  return c / y;
}

inline double prob_sum_finite_approx(double a, double b) {
  return a + prob_exp_approx(b - a + 0.5);
}

inline double prob_sum_approx(double a, double b) {
  double min_ab = std::min(a, b);
  a = std::max(a, b);
  b = min_ab;
  if (std::isinf(b)) {
    return a;
  } else {
    return prob_sum_finite_approx(a, b);
  }
}

inline double prob_sum_finite_exact(double a, double b) {
  return a + log1p(exp2(b - a))/log(2);
}

inline double prob_sum(double a, double b) {
  double min_ab = std::min(a, b);
  a = std::max(a, b);
  b = min_ab;
  if (std::isinf(b)) {
    return a;
  } else {
    return prob_sum_finite_exact(a, b);
  }
}

inline double prob_not(double x) {
  return prob_log(1 - prob_exp(x));
}

inline bool prob_is_zero(double x) {
  return std::isinf(x);
}

#endif /* __PROB_UTILS_H__ */
