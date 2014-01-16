/* 
 * File:   miller_rabin.cpp
 * Author: Zosia Sobocinska
 * 
 * Created on December 20, 2013, 11:41 AM
 */

#include <stdlib.h>

#include "../headers/miller_rabin.h"

bool test_miller_rabin(int number, int k) {
  bool prime_maybe;

  if (number <= 5) {
    return true;
  } else if (number % 2 == 0) {
    return false;
  } else {
    int s = 0, s2 = 1;
    while ((s2 & (number - 1)) == 0) {
      s += 1;
      s2 <<= 1;
    }
    int d = number / s2;

    for (int i = 0; i < k; i++) {
      int a = rand() % (number - 1) + 1;
      if (fmp(a, d, number) != 1) {
        prime_maybe = false;
        for (int r = 0; r <= s - 1; r++) {
          if (fmp(a, (1 << r) * d, number) == number - 1) {
            prime_maybe = true;
            break;
          }
        }
        if (!prime_maybe) {
          return false;
        }
      }
    }
  }
  return true;
}

int fmp(int a, int b, int n) {
  int result = 1;
  long int x = a % n;

  do {
    x %= n;
    if (b & 1) {
      result *= x;
      result %= n;
    }
    x *= x;

    b >>= 1;
  } while (b >= 1);

  return result;
}
