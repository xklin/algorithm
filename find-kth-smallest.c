
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>


#define MAX(n, m) ({\
	typeof(n) __x_ = n , __y_ = m ;\
	__x_ > __y_ ? __x_ : __y_ ;})

#ifndef INT_MAX
#define INT_MAX	(1 << 31) ^ INT_MIN
#endif
#ifndef INT_MIN
#define INT_MIN	((int)~0)
#endif

/* name : best way
 * description :
 * 	The way to find the k-th smallest number in two
 * 	sorted array, this algorithm is of time complexity
 * 	of O (lgn + lgm)
 */
int best_way (int A[], int m, int B[], int n, int k)
{
	int A_l, B_l, Ai, Bj, i, j ;

	// Deal with the boundary
	assert (k != 0 && k <= m + n) ;
	if (k == m + n)
		return MAX (A[m-1], B[n-1]) ;

	i = (int)((double)m/(m+n) * (k-1)) ;
	j = (k-1) - i ;

	Ai = A[i] ;
	Bj = B[j] ;
	A_l = (i==0) ? INT_MIN : A[i-1] ;
	B_l = (j==0) ? INT_MIN : B[j-1] ;

	if (Ai < Bj && Ai > B_l)
		return Ai ;
	if (Ai > Bj && Bj > A_l)
		return Bj ;

	if (Ai > Bj)
		return best_way (A, i, B+j+1, n-j-1, k-j-1) ;
	else
		return best_way (A+i+1, m-i-1, B, j, k-i-1) ;
}


/* name : simple way
 * description :
 * 	The way to find the k-th smallest number in two
 * 	sorted array, this algorithm is of time complexity
 * 	of O (k)
 */
int simple_way (int A[], int m, int B[], int n, int k)
{
	int x , y ;

	if (k > m+n)
		return INT_MIN ;

	for (x=0, y=0; x+y<k;) {

		if (y >= n)
			x ++ ;
		else if (x >= m)
			y ++ ;
		else if (A[x] > B[y])
			y ++ ;
		else
			x ++ ;
	}
	printf ("x = %d, y = %d\n", x, y) ;

	return MAX (A[x-1], B[y-1]) ;
}


#define __MAGIC(m, n)	({\
	int x = m > n ? n : m ;\
	2/3 * x ;})


/* name : fksntsa
 * description :
 * 	Find the k-th smallest number in two sorted array
 */
int fksntsa (int A[], int m, int B[], int n, int k)
{
	/* If elements of one array are all litter than ones of
 		the other's */
	if (A[m-1] < B[0] || B[n-1] < A[0])
		return ((k > m) ? B[k-m] : A[k-1]) ;

	/* If the k is a litter value */
	if (k < __MAGIC (m, n))
		return simple_way (A, m, B, n, k) ;

	return best_way (A, m, B, n, k) ;
}



