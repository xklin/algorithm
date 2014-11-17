

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>


#ifdef	__FIND_KTH_SMALLEST_NUMBER__

#define FILENAME	"fksn.txt"


void build_array (int elem_num)
{
	FILE *stream ;
	int elem ;
	struct timeval tv ;
	stream = fopen (FILENAME, "a") ;
	if (NULL == stream)
		exit (1) ;

	gettimeofday (&tv, NULL) ;
	srand (tv.tv_usec) ;
	for (elem=rand()%50; elem_num>0; --elem_num) {
		fprintf (stream, "%d ", elem) ;
		elem += rand () % 200 ;
	}

	fclose (stream) ;
}


int main (int argc, char *argv[])
{
	FILE *stream ;
	int len_a, len_b ;
	int *A, *B ;
	int k , n ;

	len_a = atoi (argv[1]) ;
	len_b = atoi (argv[2]) ;
	build_array (len_a) ;
	usleep (1234567) ;
	build_array (len_b) ;

	stream = fopen (FILENAME, "r+") ;
	if (NULL == stream)
		exit (1) ;

	// Read the first array
	if (0 > len_a)
		exit (1) ;
	A = malloc (4*len_a) ;
	printf ("[Array A]\nLength :\t%d\nElements :\t", len_a) ;
	for (n=0; n<len_a; ++n) {
		fscanf (stream, "%d", &A[n]) ;
		printf ("%d ", A[n]) ;
	}

	// Read the second array
	if (0 > len_b)
		exit (1) ;
	B = malloc (4*len_b) ;
	printf ("\n[Array B]\nLength :\t%d\nElements :\t", len_b) ;
	for (n=0; n<len_b; ++n) {
		fscanf (stream, "%d", &B[n]) ;
		printf ("%d ", B[n]) ;
	}

	printf ("\nEnter the index\n") ;
	scanf ("%d", &k) ;

	printf ("======>The result by simple way : %d\n",
			simple_way (A, len_a, B, len_b, k)) ;
	
	printf ("======>The result by best way : %d\n",
			best_way (A, len_a, B, len_b, k)) ;
	return 0 ;
}

#endif
