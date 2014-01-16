#include "../headers/mpi_tiny.h"

int send(long *buf, int dest_range) {
  return MPI_Send(buf, 1, MPI_LONG, dest_range, 0, MPI_COMM_WORLD);
}
int send(int *buf, int dest_range) {
  return MPI_Send(buf, 1, MPI_INT, dest_range, 0, MPI_COMM_WORLD);
}
int send_more(long *buf, int count, int dest_range) {
  return MPI_Send(buf, count, MPI_LONG, dest_range, 0, MPI_COMM_WORLD);
}

int isend(long *buf, int dest_range, MPI_Request *request) {
  return MPI_Isend(buf, 1, MPI_LONG, dest_range, 0, MPI_COMM_WORLD, request);
}

int irecv(long *buf, int src_range, MPI_Request *request) {
  return MPI_Irecv(buf, 1, MPI_LONG, src_range, 0, MPI_COMM_WORLD, request);
}
int irecv(int *buf, int src_range, MPI_Request *request) {
  return MPI_Irecv(buf, 1, MPI_INT, src_range, 0, MPI_COMM_WORLD, request);
}
int irecv_more(long *buf, int count, int src_range, MPI_Request *request) {
  return MPI_Irecv(buf, count, MPI_LONG, src_range, 0, MPI_COMM_WORLD, request);
}

int recv(long *buf, int src_range, MPI_Status *status) {
  return MPI_Recv(buf, 1, MPI_LONG, src_range, 0, MPI_COMM_WORLD, status);
}

int waitany(int requests_count, MPI_Request* requests_array, int *request_index, MPI_Status *statuses_array) {
  return MPI_Waitany(requests_count, requests_array, request_index, statuses_array);
}

int broadcast(long *buf) {
  return MPI_Bcast(buf, 1, MPI_LONG, 0, MPI_COMM_WORLD);
}

int waitall(int requests_count, MPI_Request* requests_array, MPI_Status *statuses_array) {
  return MPI_Waitall(requests_count, requests_array, statuses_array);
}
