#include "CarMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    CarMap m;
    assert(m.addCar("nice")); //return true
    assert(!m.addCar("nice")); //return false->true since !
    assert(m.miles("nice")==0 && m.drive("nice", 12.3) && m.miles("nice")==12.3); //tests miles and drive
    assert(m.addCar("gg") && m.fleetSize() == 2);
    assert(m.addCar("hh"));
    m.print(); //prints nice 12.3 \n gg 0 \n hh 0
    cout << "Passed all test POG" << endl;
}
