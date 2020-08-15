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

int main (void) {
	unsigned long int i;

	double sum, *temp_ptr, *array;

	array = malloc ((ARRAY_LENGTH)*sizeof(double));

	temp_ptr=array;
	for (i = 0; i < (ARRAY_LENGTH); ++i) {
		(*temp_ptr) = sin(i);

		++temp_ptr;
	}

	printf ("Array a[i] = sin(i) is ready.\n");

	sum = 0.0;
	temp_ptr=array;
	for (i = 0; i < (ARRAY_LENGTH); ++i) {
		sum += (*temp_ptr);

		++temp_ptr;
	}

	printf ("Sum a[i] = %.15e\n", sum);

	sum = 0.0;
	temp_ptr=array;
	for (i = 0; i < (ARRAY_LENGTH); ++i) {
		sum += sin(*temp_ptr);

		++temp_ptr;
	}

	printf ("Sum sin(a[i]) = %.15e\n", sum);

	free (array);
	
	exit (0);
}
