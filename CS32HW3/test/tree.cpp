// Return the number of ways that all n2 elements of a2 appear
	 // in the n1 element array a1 in the same order (though not
	 // necessarily consecutively).  The empty sequence appears in a
	 // sequence of length n1 in 1 way, even if n1 is 0.
	 // For example, if a1 is the 7 element array
	 //	10 50 40 20 50 40 30
	 // then for this value of a2     the function must return
	 //	10 20 40			1
	 //	10 40 30			2
	 //	20 10 40			0
	 //	50 40 30			3
int countContains(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0) { //base case
		return 1;
	}
	if (n2 > n1) {
		return 0;
	}
	int total = 0;
	if (a1[0] == a2[0]) {
		total += countContains(a1 + 1, n1 - 1, a2 + 1, n2 - 1); //recursion next value in n2
	}
	total += countContains(a1 + 1, n1 - 1, a2, n2); //recursion other ways
	return total;
}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > separator come before all the other elements,
// and all the elements whose value is < separator come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= separator, or n if there is no such element, and firstLess is
// set to the index of the first element that is < separator, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > separator
//   * for firstNotGreater <= i < firstLess, a[i] == separator
//   * for firstLess <= i < n, a[i] < separator
// All the elements > separator end up in no particular order.
// All the elements < separator end up in no particular order.
void separate(double a[], int n, double separator,
	int& firstNotGreater, int& firstLess)
{
	if (n < 0)
		n = 0;

	// It will always be the case that just before evaluating the loop
	// condition:
	//  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
	//  Every element earlier than position firstNotGreater is > separator
	//  Every element from position firstNotGreater to firstUnknown-1 is
	//    == separator
	//  Every element from firstUnknown to firstLess-1 is not known yet
	//  Every element at position firstLess or later is < separator

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < separator)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > separator)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
	if (n <= 1) {
		return;
	}
	int before;
	int after;
	separate(a, n, a[n/2], before, after); //separates into two parts
	order(a, before); //sorts first part
	order(a + after, n - after); //sorts last part
}
#include <cassert>
#include <iostream>
using namespace std;

int factorial(int n) {
	int j = 1;
	for (int i = 1; i <= n; i++) {
		j *= i;
	}
	return j;
}
int main()
{
	int n1 = 3, n2 = 3;
	double arr1[7] = { 10, 50, 40, 20, 50, 40, 30 };
	double arr2[3] = { 50, 40, 30 }; //3
	assert(countContains(arr2, n1, arr2, n2) == 1);
	int n = countContains(arr2, n1, arr2, n2);
	cout << endl << n;
	cout << endl << factorial(n1 + 1) / (factorial(n2) * factorial(n1 + 1 - n2));
	n1 = 7;
	assert(countContains(arr1, n1, arr2, n2) == 3);
	n = countContains(arr1, n1, arr2, n2);
	cout << endl << n;
	cout << endl << factorial(n1 + 1) / (factorial(n2) * factorial(n1 + 1 - n2));
	double arr3[3] = { 20, 10, 40 }; //0
	assert(countContains(arr1, n1, arr3, n2) == 0);
	n = countContains(arr1, n1, arr3, n2);
	cout << endl << n;
	cout << endl << factorial(n1 + 1) / (factorial(n2) * factorial(n1 + 1 - n2));
	double arr4[3] = { 10, 40, 30 }; //2
	assert(countContains(arr1, n1, arr4, n2) == 2);
	n = countContains(arr1, n1, arr4, n2);
	cout << endl << n;
	cout << endl << factorial(n1 + 1) / (factorial(n2) * factorial(n1 + 1 - n2));
	double arr5[3] = { 10, 20, 40 }; //1
	assert(countContains(arr1, n1, arr5, n2) == 1);
	n = countContains(arr1, n1, arr5, n2);
	cout << endl << n;
	cout << endl << factorial(n1 + 1) / (factorial(n2) * factorial(n1 + 1 - n2)) << endl;
	double a1[7] = { 10, 50, 40, 20, -50, 40, 30 };
	double a2[3] = { 50, 40, 40 };
	order(a1, 7);
	order(a2, 3);
	for (int i = 0; i < 7; i++)
		cout << a1[i] << " ";
	cout << endl;
	for (int j = 0; j < 3; j++)
		cout << a2[j] << " ";
	cout << endl;
	cout << "Passed all tests" << endl;
}