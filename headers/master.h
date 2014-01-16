/* 
 * File:   master.h
 * Author: Zosia Sobocinska
 *
 * Created on December 20, 2013, 11:32 AM
 */

#ifndef MASTER_H
#define	MASTER_H

#include <iostream>
#include <fstream>
#include <mpi.h>
#include <math.h>
#include <string.h>
#include "mpi_tiny.h"
#include "utils.h"
#include "list.h"

void master_routine(const char* path);


#endif	/* MASTER_H */

