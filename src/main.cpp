/* 
 * File:   main.cpp
 * Author: Zosia Sobocinska
 *
 * Created on December 20, 2013, 11:04 AM
 * 
 * MPI CH2
 */

#include <mpi.h>
#include "../headers/master.h"
#include "../headers/slave.h"


void usage(const char* name, const char* arguments);

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc < 3) {
        const char* args = "<path> <k>";
        usage(argv[0], args);
    }

    MPI_Init(NULL, NULL);

    int tasks, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank) {
        int k = atoi(argv[2]);
        slave_routine(k);
    } else {
        const char* path = argv[1];
        master_routine(path);
    }

    MPI_Finalize();

    return 0;
}

void usage(const char* name, const char* arguments) {
    std::cout << "Usage: " << name << ' ' << arguments << std::endl;
    exit(64);
}

