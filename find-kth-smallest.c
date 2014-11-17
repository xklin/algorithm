
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX(n, m) ({\
	typeof(n) __x_ = n , __y_ = m ;\
	__x_ > __y_ ? __x_ : __y_ ;})


/* name : best way
 * description :
 * 	The way to find the k-th smallest number in two
 * 	sorted array, this algorithm is of time complexity
 * 	of O (lgn + lgm)
 */
int best_way (int A[], int m, int B[], int n, int k)
{
	int x, y, x_l, y_l, x_h, y_h ;

	x_h = m ;
	y_h = n ;
	x_l = 0 ;
	y_l = 0 ;

	for (x=m, y=n; x+y!=k;) {

		if (x + y > k) {
			if (A[x-1] > B[y-1] && x_l != x_h ||
			A[x-1] < B[y-1] && y_l == y_h) {
				x_h = x-1 ;
				x = (x + x_l)/2 ;
			}else{
				y_h = y-1 ;
				y = (y + y_l)/2 ;
			}
		}else{
			if (A[x-1] > B[y-1] && y_l != y_h ||
			A[x-1] < B[y-1] && x_l == x_h) {
				y_l = y+1 ;
				y = (y + y_h)/2 ;
			}else{
				x_l = x+1 ;
				x = (x + x_h)/2 ;
			}
		}
	}
	return MAX (A[x-1], B[y-1]) ;
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
		return -1 ;

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

	return MAX (A[x-1], B[y-1]) ;
}



