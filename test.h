/* v 0.1 Copyright (c) 2009 Alexander O. Korotkevich, w5kao@yahoo.com */
/***********************************************************************
 * This file is part of my_thr_lib.

    my_thr_lib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License or Lesser General Public License
    as published by the Free Software Foundation, either version 3 of the License, or
    any later version.

    my_thr_lib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with my_thr_lib.  If not, see <https://www.gnu.org/licenses/>. 
*************************************************************************/
#ifndef _TEST_H
#define _TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "my_thr_lib.h"

#define THREAD_NUMBER 8
#define ARRAY_LENGTH 256*4096*64

struct array_fill_thread_input {
	double *start_element;
	unsigned long int start_index;
	unsigned long int elements_number;
};

struct array_sum_thread_input {
	pthread_mutex_t *sum_mutex;
	double *sum;
	double *start_element;
	unsigned long int elements_number;
};

void array_fill (void * input);

void array_sum (void * input);

void array_sum_sin (void * input);

#endif /* _TEST_H */
