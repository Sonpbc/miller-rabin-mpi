/* 
 * File:   mpi_tiny.h
 * Author: Zosia
 *
 * Created on January 16, 2014, 3:06 PM
 */

#ifndef MPI_TINY_H
#define	MPI_TINY_H

#include <mpi.h>

int send(long *buf, int src_range);
int send(int *buf, int src_range);
int send_more(long *buf, int count, int dest_range);
int isend(long* buf, int dest_range, MPI_Request *request);
int recv(long *buf, int src_range, MPI_Status *status);
int irecv(long* buf, int src_range, MPI_Request *request);
int irecv(int* buf, int src_range, MPI_Request *request);
int irecv_more(long* buf, int count, int src_range, MPI_Request *request);
int waitany(int requests_count, MPI_Request* requests_array, int *request_index, MPI_Status *statuses_array);
int waitall(int requests_count, MPI_Request* requests_array, MPI_Status *statuses_array);
int broadcast(long* buf);

#endif	/* MPI_TINY_H */

