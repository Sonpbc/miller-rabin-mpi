/* 
 * File:   slave.cpp
 * Author: Zosia Sobocinska
 * 
 * Created on December 20, 2013, 11:32 AM
 */

#define MASTER_RANGE 0
#define READY_STATE 0

#include "../headers/slave.h"

#include <stdlib.h>

/**
 * Serves as slave in Miler-Rabin prime numbers test. Waits for subsequent
 * numbers sent over MPI, collects results and after finish flag sends a
 * complete response.
 * 
 * @param k number of Miller-Rabin repetitions
 */
void slave_routine(int k) {


  Testrun testrun = get_testrun_params();

  long ready_state = READY_STATE;
  long length;
  long score;

  MPI_Status status;

  recv(&length, MASTER_RANGE, &status);

  long *results = new long[length + 1];

  long number;

  score = 0;
  while (true) {

    // non-blocking send and recv are pointless
    // since nothing master doesn't send tasks to busy thread
    send(&ready_state, MASTER_RANGE);
    recv(&number, MASTER_RANGE, &status);

    if (!number) break;



    while (testrun.iterations-- > 0) {
      if (test_miller_rabin(number, k)) {
        results[score] = number;
      } else {
        results[score] = -number;
      }
    }

    score++;
  }

  results[length] = score;

  send_more(results, length + 1, MASTER_RANGE);

  delete [] results;
}

