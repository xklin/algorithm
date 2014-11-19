

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>


#ifdef	__FIND_KTH_SMALLEST_NUMBER__


void build_array (int elem_num, int A[])
{
	int elem ;
	struct timeval tv ;
	int n ;

	gettimeofday (&tv, NULL) ;
	srand (tv.tv_usec) ;
	for (elem=rand()%50, n=0; n<elem_num; ++n) {
		A[n] = elem ;
		elem += rand () % 200 ;
	}
}


int main (int argc, char *argv[])
{
	FILE *stream ;
	int len_a, len_b ;
	int *A, *B ;
	int k , n ;

	len_a = atoi (argv[1]) ;
	len_b = atoi (argv[2]) ;

	// Read the first array
	if (0 > len_a)
		exit (1) ;
	A = malloc (4*len_a) ;
	printf ("[Array A]\nLength :\t%d\nElements :\t", len_a) ;
	build_array (len_a, A) ;

	// Read the second array
	if (0 > len_b)
		exit (1) ;
	B = malloc (4*len_b) ;
	printf ("\n[Array B]\nLength :\t%d\nElements :\t", len_b) ;
	build_array (len_b, B) ;

	printf ("\nEnter the index\n") ;
	scanf ("%d", &k) ;

	printf ("======>The result by simple way : %d\n",
			simple_way (A, len_a, B, len_b, k)) ;
	
	printf ("======>The result by best way : %d\n",
			best_way (A, len_a, B, len_b, k)) ;
	return 0 ;
}

#endif
