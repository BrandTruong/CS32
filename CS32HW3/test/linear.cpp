bool somePredicate(double x);
// Return true if the somePredicate function returns false for at
// least one of the array elements; return false otherwise.
bool anyFalse(const double a[], int n)
{
	if (n <= 0) {
		return false; //base case
	}
	if (!somePredicate(a[0])) { //base case
		return true;
	}
	return anyFalse(a + 1, n - 1); //recursion to last case
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
	if (n <= 0) {
		return 0; //base case
	}
	if (n == 1) {
		if (somePredicate(a[0])) { //counts for when somePredicate is true
			return 1;
		}
		else {
			return 0;
		}
	}
	int first = countTrue(a, n / 2);
	int last = countTrue(a + n / 2, n - n / 2); //sums by dividing into halves
	return first + last;
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
	if (n <= 0) { //base case, reached end without finding element
		return -1;
	}
	if (somePredicate(a[0])) { //base case
		return 0;
	}
	int pos = firstTrue(a + 1, n - 1); //recursion
	if (pos == -1) {
		return -1; //stays as -1, does not increment
	}
	else {
		return pos + 1; //increment pos by 1
	}
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int positionOfSmallest(const double a[], int n)
{
	if (n <= 0) {
		return -1;
	}
	if (n == 1) {
		return 0;
	}
	int pos = positionOfSmallest(a, n - 1); //first recursion, looks at n=1,then n=2, then ... until n is equal to original parameter
	if (a[n - 1] < a[pos]) { //first case that is smallest
		pos = n - 1;
	}
	return pos;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not contain
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool contains(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0) {
		return true;
	}
	if (n2 > n1) {
		return false;
	}
	if (a1[0] == a2[0]) {
		return contains(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	}
	else {
		return contains(a1 + 1, n1 - 1, a2, n2);
	}
}
bool somePredicate(double x)
{
	return x < 0;
}

#include <cassert>
#include <string>
#include <iostream>
using namespace std;

int main() {
	double arr1[7] = { 10, 50, 40, -20, 50, 40, 30 };
	double arr2[3] = { 50, 40, 30 }; //3
	double arr3[3] = { -1,-2,-3 };
	assert(anyFalse(arr1, 7));
	assert(!anyFalse(arr3, 3));
	assert(countTrue(arr1, 7) == 1);
	assert(firstTrue(arr1, 7) == 3);
	assert(positionOfSmallest(arr1, 7) == 3);
	assert(contains(arr1, 7, arr2, 3)); 
	cout << "Passed all tests" << endl;
}