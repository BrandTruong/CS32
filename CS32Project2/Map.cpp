#include "Map.h"
#include <iostream>

using namespace std;

//Non-Member Functions
bool combine(const Map& m1, const Map& m2, Map& result) //combines two maps to result, no two keys with two different values
{
	bool combine = true;
	KeyType tempKey;
	ValueType tempValue1;
	ValueType tempValue2;
	Map tempM1(m1); //Creates two temp maps for aliasing, copy constructors to m1 and m2
	Map tempM2(m2); //Allows function to work even if they have the same address

	if(!result.empty()) { //clears result by swapping with empty Map
		Map temp;
		result.swap(temp);
	}
	for (int j = 0; j < tempM1.size(); j++) { //adds everything in tempM1 first
		tempM1.get(j, tempKey, tempValue1);
		if (!tempM2.get(tempKey, tempValue2) || tempValue1 == tempValue2) { //Not a duplicate key or same value if duplicate key
				result.insert(tempKey,tempValue1);
		}
		else {
			combine = false; //Same key but different values, sets to false
		}
	}
	for (int i = 0; i < tempM2.size(); i++) { //adds tempM2 nodes unless already added by tempM1
		tempM2.get(i, tempKey, tempValue2);
		if ((!tempM1.get(tempKey, tempValue1) || tempValue1 == tempValue2) && !result.contains(tempKey)) { //Not a duplicate key or same value, and result does not contain already
			result.insert(tempKey, tempValue2); 
		}
	}
	return combine;
}
void reassign(const Map& m, Map& result)
{
	if (!result.empty() && &m != &result) { //clears result by swapping with empty Map unless same address
		Map temp;
		result.swap(temp);
	}
	result = m; //assignment to make result equal to m
	if (m.empty()) { //skips if empty list
		return;
	}
	ValueType head; //stores head value for later
	ValueType tail;
	KeyType Key;
	result.get(result.size()-1, Key, tail); //updates tail
	result.get(0, Key, head); //updates KeyType Key to head to call update func.
	result.update(Key, tail); //changes head value to have tail value
	for (int i = result.size() - 2; i >= 0; i--) { //shifts values one to the right, tail goes to head
		ValueType tempV1;
		ValueType tempV2;
		KeyType tempK;
		if (i != 0) {
			result.get(i, tempK, tempV1); //gets tempV1 value
			result.get(i + 1, tempK, tempV2); //updates tempK to Key to the right
			result.update(tempK, tempV1);
		}
		else { //special case for head
			result.get(i + 1, tempK, tempV2); //updates tempK to Key to the right
			result.update(tempK, head);
		}
	}
}
//Map Implementation
Map::Map() //default constructor
{
	head = new Node; //sets head and tail to nullptr, lets add function work
	head->prev = nullptr;
	head->next = nullptr;
	tail = head;
	m_size = 0;
}
Map::Map(const Map& other) //copy constructor
{
	head = new Node; //sets head and tail to nullptr, lets add function work
	head->prev = nullptr;
	head->next = nullptr;
	tail = head;
	m_size = 0;
	Node* ptr = other.head; //points to head of other Map and adds items starting from the head of this map
	while (ptr != nullptr) {
		add(ptr->m_key, ptr->m_value);
		ptr = ptr->next;
	}
}
Map& Map::operator=(const Map& other) //assignment operator
{
	if (this != &other) {
		Map temp(other); //uses scoped Map and swaps head and tail ptrs with swap func, temp deleted after out of scope
		swap(temp);
	}
	return *this;
}
Map::~Map() //destructor
{
	Node* ptr;
	ptr = head;
	while (ptr != nullptr) {
		Node* temp = ptr->next; //temp to destruct ptr and still have address
		delete ptr;
		ptr = temp;
	}
}
bool Map::empty() const //returns true if empty list
{
	return m_size == 0;
}
int Map::size() const //returns size of list
{
	return m_size;
}
bool Map::insert(const KeyType& key, const ValueType& value) //inserts key and value at end if key doesn't exist, false if key already exists
{
	if (!contains(key)) {
		add(key, value);
		return true;
	}
	return false;
}
bool Map::update(const KeyType& key, const ValueType& value) //changes value at key if key exists, true if value changed, else false
{
	Node* ptr = head;
	while (ptr != nullptr) { //finds key if exists, then updates value
		if (ptr->m_key == key) {
			ptr->m_value = value;
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) //updates value if key exists, otherwise inserts key and value
{
	if (!update(key, value)) { //updates if possible, else inserts (update does not change if key does not exist)
		insert(key, value);
	}
	return true; //always true
}
bool Map::erase(const KeyType& key) //deletes node with key if exists and unhook nodes, true if erased, else false
{
	Node* ptr = head;
	while (ptr != nullptr) { //loops through entire linked list unless it finds key
		if (ptr->m_key == key) {
			Node* temp = ptr;
			if (ptr->prev != nullptr) { //relink nodes, special conditions for head and tail
				ptr->prev->next = temp->next;
			}
			else {
				head = temp->next; //relink head if needed
				head->prev = nullptr;
			}
			if (ptr->next != nullptr) {
				ptr->next->prev = temp->prev;
			}
			else {
				tail = temp->prev; //relink tail if needed
				tail->next = nullptr;
			}
			delete temp;
			m_size--;
			return true;
			
		}
		ptr = ptr->next;
	}
	return false;
}
bool Map::contains(const KeyType& key) const //checks if key is in list, true if exists, else false
{
	Node* ptr = head;
	while (ptr != nullptr) { //loops until finds key or reaches end
		if (ptr->m_key == key) { 
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}
bool Map::get(const KeyType& key, ValueType& value) const //checks if key is in list, true if exists and updates value, else false and value unchanged
{
	Node* ptr = head;
	while (ptr != nullptr) { //loops until finds key or reaches end
		if (ptr->m_key == key) {
			value = ptr->m_value;
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}
bool Map::get(int i, KeyType& key, ValueType& value) const //if i is a valid parameter, updates key and value parameters and returns true, else false
{
	if (i < 0 || i >= size()) { //skips steps if invalid i
		return false;
	}
	if (i == (size() - 1)) { //skips for loop if at end, more efficient
		key = tail->m_key;
		value = tail->m_value;
		return true;
	}
	Node* ptr = head;
	for (int j = 0; j < i; j++) { //traverses through list until reaches i
		ptr = ptr->next;
	}
	key = ptr->m_key; //updates both key and value
	value = ptr->m_value;
	return true;
}
void Map::swap(Map& other) //Swaps head and tail pointers, then m_size
{
	Node* tempHead = head; //Swap head and tail ptrs
	Node* tempTail = tail;
	head = other.head;
	tail = other.tail;
	other.head = tempHead;
	other.tail = tempTail;
	int tempSize = m_size; //Swap size
	m_size = other.m_size;
	other.m_size = tempSize;
}

//Test Function
void Map::dump() const //prints list
{
	Node* ptr = head;
	while (ptr != nullptr) {
		if (ptr == head && ptr == tail) { //Cerrs head and tail is
			cerr << "Head and Tail is ";
		}
		else if (ptr == head) 
		{
			cerr << "Head is ";
		}
		else if (ptr == tail)
		{
			cerr << "Tail is ";
		}
		cerr << ptr->m_key <<": "<< ptr->m_value << endl; //Cerr entire linked list
		ptr = ptr->next;
	}
}
//Helper Function
void Map::add(const KeyType& key, const ValueType& value) //adds item to end
{	
	if (m_size == 0) { //Special condition if empty linked list, head points to first node
		head->m_key = key;
		head->m_value = value; 
		m_size++;
		return;
	}
	Node* temp = new Node; //New dynamically allocated node
	temp->m_key = key;
	temp->m_value = value;
	//If list is not empty, links the two nodes at the end
	tail->next = temp;
	temp->prev = tail;
	temp->next = nullptr;
	tail = temp; //Tail points to temp node do we even need a tail node bruh
	m_size++;
}