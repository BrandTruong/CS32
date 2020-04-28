// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#include <iostream>
#include "provided.h"

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	struct HashNode { //Node to store keytype and valuetype
		HashNode(KeyType key, ValueType value): m_key(key), m_value(value) {
			m_next = nullptr;
		}
		KeyType m_key;
		ValueType m_value;
		HashNode* m_next;
	}; 
	unsigned int getBucketNumber(const KeyType& key) const;
	void deleteTable();
	void rehash();
	double m_loadFactor;
	unsigned int m_buckets;
	unsigned int m_count;
	HashNode** m_table;
};
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
	//Default values with nullptrs
	m_buckets = 8;
	m_loadFactor = maximumLoadFactor;
	m_count = 0;
	m_table = new HashNode*[m_buckets];
	for (unsigned int i = 0; i < m_buckets; i++) {
		m_table[i] = nullptr;
	}
}
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	deleteTable();
}
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	deleteTable(); 
	//Reset table with default values
	m_buckets = 8;
	m_count = 0;
	m_table = new HashNode*[m_buckets];
	for (unsigned int i = 0; i < m_buckets; i++) {
		m_table[i] = nullptr;
	}
}
template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return m_count;
}
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	unsigned int ID = getBucketNumber(key); //hashkey location
	HashNode* ptr = m_table[ID];
	while (ptr != nullptr) { //Checks entire linked list
		if (ptr->m_key == key) { //Duplicate key, replaces value
			ptr->m_value = value;
			return;
		}
		ptr = ptr->m_next;
	}
	//No duplicate, insert at front, rehash called if above max loadfactor
	HashNode* newNode = new HashNode(key, value);
	newNode->m_next = m_table[ID]; //Insertion at front
	m_table[ID] = newNode;
	m_count++;
	if ((double)(m_count * 1.0 / m_buckets) > m_loadFactor) { //Exceeded max loadfactor
		rehash();
	}
}
template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	unsigned int ID = getBucketNumber(key); //Same value if keys are same
	HashNode* ptr = m_table[ID];
	while (ptr != nullptr) {
		if (ptr->m_key == key) { //Found key, return address to value
			return &(ptr->m_value);
		}
		ptr = ptr->m_next;
	}
	return nullptr; //Not found
}

template<typename KeyType, typename ValueType>
unsigned int ExpandableHashMap<KeyType, ValueType>::getBucketNumber(const KeyType& key) const { //Helper function to make bucket ID
	unsigned int hasher(const KeyType & k);  // prototype function
	unsigned int h = hasher(key);
	return h % m_buckets;
}
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::deleteTable() {
	for (unsigned int i = 0; i < m_buckets; i++) {
		HashNode* ptr = m_table[i];
		while (ptr != nullptr) { //Deletes all linked HashNodes
			HashNode* tempPtr = ptr->m_next;
			delete ptr;
			ptr = tempPtr;
		}
	}
	delete[] m_table; //Deletes new table
}
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::rehash() {
	unsigned int hasher(const KeyType & k);  // Prototype function
	unsigned int m_newBuckets = m_buckets * 2; //Resize
	HashNode** m_tempTable = new HashNode* [m_newBuckets];
	for (unsigned int i = 0; i < m_newBuckets; i++) { //Sets to default nullptr in new table
		m_tempTable[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_buckets; i++) { //Copies to new array
		HashNode* ptr = m_table[i];
			while (ptr != nullptr) { //not empty
				HashNode* tempPtr = ptr->m_next; //Holds next HashNode in list
				unsigned int bucketID = hasher(ptr->m_key) % m_newBuckets; //new bucket size, rehash
				ptr->m_next = m_tempTable[bucketID]; //relinks to front of
				m_tempTable[bucketID] = ptr;
				ptr = tempPtr; //next in original linked list
			}
	}
	delete[] m_table; //deletes old array, keeps values
	m_buckets = m_newBuckets; //replaces old table with new
	m_table = m_tempTable;
}