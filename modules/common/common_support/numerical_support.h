#ifndef PLANNING_NUMERICAL_SUPPORT_H_
#define PLANNING_NUMERICAL_SUPPORT_H_

#define SYSTEM_PRECISION 1e-7

#define NUMBER_BOUND_CHECK(num, lower, upper) \
  if (num < lower) {                          \
    num = lower;                              \
  } else if (num > upper) {                   \
    num = upper;                              \
  }

#endif