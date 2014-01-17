/* 
 * File:   master.cpp
 * Author: Zosia Sobocinska
 * 
 * Created on December 20, 2013, 11:32 AM
 */

#include "../headers/master.h"

using std::cout;
using std::endl;

void master_routine(const char* path) {

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int slaves_count = world_size - 1;

  Testrun testrun = get_testrun_params();

  ifstream ifs;
  ifs.open(path);

  if (ifs.is_open()) {


    List* list = List::create();

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
    long length = list->length();

    //////////////////////////////////////////////////////////////////////////
    // DISTRIBUTE TASKS

    double time = MPI_Wtime();
    MPI_Status status;

    for (int slave_index = 0; slave_index < slaves_count; slave_index++) {
      // let slaves get ready for maximum possible amount of data 
      send(&length, slave_index + 1);
    }

    MPI_Request* ready_requests = new MPI_Request[slaves_count];
    long ready_buff; // just flag buffer for MPI

    MPI_Request *send_requests = new MPI_Request[length];

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

    time = MPI_Wtime() - time;

    List::destroy(list);

    delete [] ready_requests;
    delete [] send_requests;


    if (testrun.running) {

      std::cout << time << std::endl;

    } else {
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


  } else {
    cerr << "Cannot open file " << path << endl;
  }

}
