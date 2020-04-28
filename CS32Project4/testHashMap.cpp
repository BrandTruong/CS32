#include "ExpandableHashMap.h"
#include <string>
#include <iostream>
using namespace std;

unsigned int hasher(const string& g)
{
	return std::hash<string>()(g);
}

void foo() {
	// Define a hashmap that maps strings to doubles and has a maximum
	// load factor of 0.3.It will initially have 8 bucketswhen empty.
	ExpandableHashMap<string,double> nameToGPA(0.3);  
	// Add new items to the hashmap.  Inserting the third item will cause
	// the hashmap to increase the number of buckets (since the maximum
	// load factor is 0.3), forcing a rehashof all items.
	nameToGPA.associate("Carey", 3.5);  // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	nameToGPA.associate("Abe", 3.2);// Abe has a 3.2 GPA
	cout<<nameToGPA.size();
	double* davidsGPA = nameToGPA.find("David");
	if (davidsGPA != nullptr) {
		cout << "Found David's GPA"<<endl;
		*davidsGPA = 1.5; // after a re-grade of David’s exam
	}
	nameToGPA.associate("Carey", 4.0);
	// Carey deserves a 4.0
	// replaces old 3.5 GPA
	double* lindasGPA = nameToGPA.find("Linda");
	if (lindasGPA == nullptr)
		cout << "Linda is not in the roster!" << endl;
	else
		cout << "Linda’s GPA is: " << *lindasGPA << endl;  
	double* DavidGPA = nameToGPA.find("David");
	if (DavidGPA == nullptr)
		cout << "Something wrong" << endl;
	else
		cout << "David’s GPA is: " << *DavidGPA << endl;

}

int main() {
	foo();
}