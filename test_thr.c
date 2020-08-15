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
#include "test.h"

void array_fill (void * input) {
	unsigned long int i;
	double *temp_int_ptr;
	
	struct array_fill_thread_input	*data;

	data = (struct array_fill_thread_input *) input;

	temp_int_ptr = data->start_element;
	for (i = 0; i < data->elements_number; ++i) {
		(*temp_int_ptr) = sin(data->start_index + i);
		++temp_int_ptr;
	}
}

void array_sum (void * input) {
	unsigned long int i;
	double *temp_int_ptr;
	double local_sum = 0.0;

	struct array_sum_thread_input	*data;

	data = (struct array_sum_thread_input *) input;

	temp_int_ptr = data->start_element;
	for (i = 0; i < data->elements_number; ++i) {
		local_sum += (*temp_int_ptr);
		++temp_int_ptr;
	}
	pthread_mutex_lock (data->sum_mutex);
	(*(data->sum)) += local_sum;
	pthread_mutex_unlock (data->sum_mutex);
}

void array_sum_sin (void * input) {
	unsigned long int i;
	double *temp_int_ptr;
	double local_sum = 0.0;

	struct array_sum_thread_input	*data;

	data = (struct array_sum_thread_input *) input;

	temp_int_ptr = data->start_element;
	for (i = 0; i < data->elements_number; ++i) {
		local_sum += sin(*temp_int_ptr);
		++temp_int_ptr;
	}
	pthread_mutex_lock (data->sum_mutex);
	(*(data->sum)) += local_sum;
	pthread_mutex_unlock (data->sum_mutex);
}

int main (void) {
	unsigned long int i;
	unsigned long int each_thread_elements, last_thread_elements;

	double sum, *temp_ptr, *array;

	pthread_mutex_t sum_access;

	struct array_sum_thread_input *array_sum_data;
	struct array_fill_thread_input *array_fill_data;

	my_thr_pool_init ((THREAD_NUMBER));

	pthread_mutex_init (&sum_access, NULL);

	array = malloc ((ARRAY_LENGTH)*sizeof(double));
	array_fill_data = malloc ((THREAD_NUMBER) * sizeof(struct array_fill_thread_input));

	each_thread_elements = (unsigned long int) (0.5 + (1.0*(ARRAY_LENGTH))/(THREAD_NUMBER));
	last_thread_elements = (ARRAY_LENGTH) - each_thread_elements*((THREAD_NUMBER)-1);

	for (i = 0; i < ((THREAD_NUMBER)-1); ++i) {
		array_fill_data [i].start_element = (array + i*each_thread_elements);
		array_fill_data [i].start_index = i*each_thread_elements;
		array_fill_data [i].elements_number = each_thread_elements;

		my_thr_data_assign (i,  (void *) &array_fill_data[i]);
	}

	array_fill_data [((THREAD_NUMBER)-1)].start_element = (array + (ARRAY_LENGTH) - last_thread_elements);
	array_fill_data [((THREAD_NUMBER)-1)].start_index = ((THREAD_NUMBER)-1)*each_thread_elements;
	array_fill_data [((THREAD_NUMBER)-1)].elements_number = last_thread_elements;

	my_thr_data_assign (((THREAD_NUMBER)-1),  (void *) &array_fill_data[((THREAD_NUMBER)-1)]);

	my_thr_manager (array_fill);
	printf ("Array a[i] = sin(i) is ready.\n");
	
	array_sum_data = malloc ((THREAD_NUMBER) * sizeof(struct array_sum_thread_input));

	for (i = 0; i < ((THREAD_NUMBER)-1); ++i) {
		array_sum_data [i].sum_mutex = &sum_access;
		array_sum_data [i].sum = &sum;
		array_sum_data [i].start_element = (array + i*each_thread_elements);
		array_sum_data [i].elements_number = each_thread_elements;

		my_thr_data_assign (i,  (void *) &array_sum_data[i]);
	}
	
	array_sum_data [((THREAD_NUMBER)-1)].sum_mutex = &sum_access;
	array_sum_data [((THREAD_NUMBER)-1)].sum = &sum;
	array_sum_data [((THREAD_NUMBER)-1)].start_element = (array + (ARRAY_LENGTH) - last_thread_elements);
	array_sum_data [((THREAD_NUMBER)-1)].elements_number = last_thread_elements;

	my_thr_data_assign (((THREAD_NUMBER)-1), (void *) &array_sum_data[((THREAD_NUMBER)-1)]);

	sum = 0.0;
	my_thr_manager (array_sum);
	printf ("Sum a[i] = %.15e\n", sum);

	sum = 0.0;
	my_thr_manager (array_sum_sin);

	printf ("Sum sin(a[i]) = %.15e\n", sum);

	my_thr_pool_clear ();
	
	free (array);
	free (array_sum_data);
	pthread_mutex_destroy (&sum_access);

	exit (0);
}
