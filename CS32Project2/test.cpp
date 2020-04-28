#include "Map.h"
#include <type_traits>
#include <iostream>
#include <cassert>

using namespace std;

#define CHECKTYPE(f, t) { auto p = static_cast<t>(f); (void)p; }

static_assert(std::is_default_constructible<Map>::value,
    "Map must be default-constructible.");
static_assert(std::is_copy_constructible<Map>::value,
    "Map must be copy-constructible.");
static_assert(std::is_copy_assignable<Map>::value,
    "Map must be assignable.");

void ThisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Map::operator=, Map & (Map::*)(const Map&));
    CHECKTYPE(&Map::empty, bool (Map::*)() const);
    CHECKTYPE(&Map::size, int  (Map::*)() const);
    CHECKTYPE(&Map::insert, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::update, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::insertOrUpdate, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::erase, bool (Map::*)(const KeyType&));
    CHECKTYPE(&Map::contains, bool (Map::*)(const KeyType&) const);
    CHECKTYPE(&Map::get, bool (Map::*)(const KeyType&, ValueType&) const);
    CHECKTYPE(&Map::get, bool (Map::*)(int, KeyType&, ValueType&) const);
    CHECKTYPE(&Map::swap, void (Map::*)(Map&));

    CHECKTYPE(combine, bool (*)(const Map&, const Map&, Map&));
    CHECKTYPE(reassign, void (*)(const Map&, Map&));
}
void testPublic();
void testNonMember();

int main() //test cases
{
    cerr << "Testing Public\n" << endl;
    testPublic();
    cerr << "Testing Private\n" << endl;
    testNonMember();
    cerr << "Reached end, passed all tests POG" << endl;
}
void testPublic() //tests public functions and cerr results
{

    Map* m = new Map;
    assert(m->insert("first", 1.1));
    m->dump();
    assert(m->insert("second", 2.2));
    cerr << "\nm has new key \n" << endl;
    m->dump();
    Map* a = new Map(*m);
    cerr << "\nNew Map a is created with copy constructor\n" << endl;
    a->dump();
    assert(a->insert("third", 3.3));
    cerr << "\na has new key \n" << endl;
    a->dump();
    a->swap(*m);
    cerr << "\nm swapped with a \n" << endl;
    a->dump();
    cerr << "\nPrinting out a ^^^ | m vvv\n" << endl;
    m->dump();
    assert(!m->empty()); //m should not be empty
    assert(a->size() == 2); //a should be 2 nodes
    assert(!m->insert("second", 2.2)); //should not insert
    assert(m->insertOrUpdate("second", 4.5)); //second should be 4.5 value now
    cerr << "\nValue changed\n" << endl;
    m->dump();
    assert(m->insertOrUpdate("fourth", 4.5));
    cerr << "\ninserted new node\n" << endl;
    m->dump();
    KeyType test;
    ValueType test2;
    assert(!m->get(4, test, test2)); //return false
    cerr << "\nnode at pos 4 does not exist\n" << endl;
    assert(a->get("first", test2));
    cerr << "\nValue at key 'first' is " << test2 << endl << endl;
    assert(m->contains("fourth")); //should find fourth then erase, then check to see if it is erased properly
    assert(m->erase("fourth"));
    assert(!m->contains("fourth"));
    cerr << "\nno longer contains fourth\n" << endl;
    m->dump();
    assert(m->insert("fifth", 2.2));
    assert(m->erase("first"));
    cerr << "\nnew node fifth, deleted first\n" << endl;
    m->dump();
    delete m;
    delete a;
}
void testNonMember() //tests combine and reassign
{
    Map m;
    Map a;
    Map result;
    m.insert("Fred", 123.0);
    m.insert("Ethel", 456.0);
    m.insert("Lucy",  789.0);
    a.insert("Lucy" , 789.0);
    a.insert("Ricky" , 321.0);
    assert(combine(m, a, result)); //combine returns true
    result.dump();
    cerr << endl << endl;
    Map n(m);
    Map o(a);
    o.update("Lucy", 654.0);
    assert(!combine(n, o, result)); //combine returns false
    result.dump();
    cerr << endl << endl;
    assert(combine(n, n, result)); //returns true, copy of n
    cerr << "Prints map n == m" << endl;
    result.dump();
    cerr << endl << endl;
    assert(combine(m, a, m)); //return true, alias test
    m.dump();
    cerr << endl << endl;
    reassign(m, m); //checks aliasing and reassign
    m.dump();
    /*
    Head is Fred: 321
    Ethel: 123
    Lucy: 456
    Tail is Ricky: 789
    */
    reassign(m, o);
    o.dump();
}