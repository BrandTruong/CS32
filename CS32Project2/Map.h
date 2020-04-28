#ifndef MAP_H
#define MAP_H

#include <string>

using KeyType = std::string;
using ValueType = double;


//Class Declaration
class Map
{
public:
    Map();
    Map(const Map& other);
    Map& operator=(const Map& other);
    ~Map();
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    //Testing function
    void dump() const;
private:
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* next;
        Node* prev;
    };
    Node* head;
    Node* tail;
    int m_size;
    //Helper Functions
    void add(const KeyType& key, const ValueType& value);
};
//NonMember Function Declaration
bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);
#endif
