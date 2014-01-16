/* 
 * File:   utils.h
 * Author: Zosia Sobocinska
 *
 * Created on December 20, 2013, 11:05 AM
 */

#ifndef UTILS_H
#define	UTILS_H


#include <iostream>
#include <cstdlib>

using namespace std;

struct Testrun {
  bool running;
  int iterations;
};

Testrun get_testrun_params();

#endif	/* UTILS_H */
