/* 
 * File:   main.cpp
 * Author: Zosia Sobocinska
 *
 * Created on December 20, 2013, 11:04 AM
 * 
 * MPI CH2
 */

#include <mpi.h>
#include "../headers/list.h"
#include "../headers/master.h"
#include "../headers/slave.h"

#define MASTER_RANGE 0
#define READY_STATE 0

void usage(const char* name, const char* arguments);

/*
 * 
 */
int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);

  long length;
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int slaves_count = world_size - 1;
  List * list;
  double duration;

  MPI_Status status;

  MPI_Request* ready_requests;
  MPI_Request *send_requests;

  Testrun testrun = get_testrun_params();

  if (argc < 3) {
    const char* args = "<path> <k>";
    usage(argv[0], args);
  }


  int tasks, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &tasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  if (!rank) { // MASTER
    const char* path = argv[1];


    ifstream ifs;
    ifs.open(path);

    if (!ifs.is_open()) {
      exit(1);
    }


    list = List::create();

    //////////////////////////////////////////////////////////////////////////
    // SCAN NUMBERS
    long number;
    list->first();
    while (ifs >> number) {

      if (number > 0) {
        list->append(number);
      }
    }
    ifs.close();
    length = list->length();
  }

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank) { // SLAVE
    int k = atoi(argv[2]);

    long ready_state = READY_STATE;
    long score;


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
  } else { // MASTER


    //////////////////////////////////////////////////////////////////////////
    // DISTRIBUTE TASKS

    duration = MPI_Wtime();
    MPI_Status status;

    for (int slave_index = 0; slave_index < slaves_count; slave_index++) {
      // let slaves get ready for maximum possible amount of data 
      send(&length, slave_index + 1);
    }
    ready_requests = new MPI_Request[slaves_count];
    long ready_buff; // just flag buffer for MPI

    send_requests = new MPI_Request[length];

    int ready_slave_index = -1;
    memset(ready_requests, MPI_REQUEST_NULL, slaves_count * sizeof (MPI_Request));
    memset(send_requests, MPI_REQUEST_NULL, length * sizeof (MPI_Request));

    list->first();
    Entry* entry;
    do {
      entry = list->current();
      for (int slave_index = 0; slave_index < slaves_count; slave_index++) {
        if (ready_slave_index == -1 || slave_index == ready_slave_index) {
          irecv(&ready_buff, slave_index + 1, &(ready_requests[slave_index]));
        }
      }

      waitany(slaves_count, ready_requests, &ready_slave_index, &status);

      isend(&(entry->val), ready_slave_index + 1, &(send_requests[list->index()]));
    } while (list->next());
    waitall(slaves_count, ready_requests, &status);
    waitall(slaves_count, send_requests, &status);

    long eot_buff = 0; // just buffer for MPI
    for (int slave_index = 0; slave_index < slaves_count; slave_index++) {
      send(&eot_buff, slave_index + 1);
    }


  }
  MPI_Barrier(MPI_COMM_WORLD);

  if (!rank) { // MASTER
    duration = MPI_Wtime() - duration;
    List::destroy(list);

    delete [] ready_requests;
    delete [] send_requests;


    //////////////////////////////////////////////////////////////////////////
    // COLLECT RESULTS
    long **results = new long*[slaves_count];
    MPI_Request *collect_requests = new MPI_Request[slaves_count];

    for (int slave_index = 0; slave_index < slaves_count; slave_index++) {
      results[slave_index] = new long[length + 1];

      irecv_more(results[slave_index], length + 1, slave_index + 1, &(collect_requests[slave_index]));
    }

    waitall(slaves_count, collect_requests, &status);
    delete [] collect_requests;

    //////////////////////////////////////////////////////////////////////////
    // DISPLAY RESULTS

    if (testrun.running) {

      std::cout << duration << std::endl;

    } else {

      for (int slave_index = 0; slave_index < slaves_count; slave_index++) {
        int slave_score = results[slave_index][length];

        long number;
        bool prime;
        for (int i = 0; i < slave_score; i++) {
          long number = results[slave_index][i];
          bool prime = (number > 0);
          std::cout << fabs(results[slave_index][i]) << ": " << (prime ? "pierwsza" : "zlozona") << std::endl;
        }
        delete [] results[slave_index];
      }
      delete [] results;
    }

  }


  MPI_Finalize();

  return 0;
}

void usage(const char* name, const char* arguments) {
  std::cout << "Usage: " << name << ' ' << arguments << std::endl;
  exit(64);
}

