//  HashMap.h
//  CS32Project4

#ifndef HashMap_h
#define HashMap_h

#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;

template <typename T>
class HashMap
{
public:
    HashMap(double max_load = 0.75); // constructor
    ~HashMap(); // destructor; deletes all of the items in the hashmap
    int size() const; // return the number of associations in the hashmap
    // The insert method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void insert(const std::string& key, const T& value);
    // Defines the bracket operator for HashMap, so you can use your map like this:
    // your_map["david"] = 2.99;
    // If the key does not exist in the hashmap, this will create a new entry in
    // the hashmap and map it to the default value of type T (0 for builtin types).
    // It returns a reference to the newly created value in the map.
    T& operator[](const std::string& key);
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value within the map.
    const T* find(const std::string& key) const;
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value or modify it directly within the map.
    T* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }
    
    // TOOOOO DELETE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void print() const {
        for (size_t i = 0; i < buckets.size(); ++i) {
            std::cout << "Bucket " << i << ": ";
            for (const auto& node : buckets[i]) {
                std::cout << "(" << node.m_key << ", " << node.m_value.to_string() << ") ";
            }
            std::cout << std::endl;
        }
    }

private:
    struct Node
    {
        string  m_key;
        T       m_value;
    };
    
    // member variables
    double max_load_factor;
    int num_associations;
    
    vector<list<Node>> buckets; // vector of linked lists of nodes (pairs)
    
    // helper functions
    void rehashMap();
    size_t hashFunction(string str) const;
};

/*
 Notes:
 Load factor = Max # values insert / total buckets in array
 More buckets than # of values
 */

// constructor
template <typename T>
HashMap<T>::HashMap(double max_load) : num_associations(0) {
    if (max_load <= 0)
        max_load = 0.75;
    max_load_factor = max_load;
    buckets.resize(10);
}

// destructor
template <typename T>
HashMap<T>::~HashMap() {
    // deletes all of the items in the hashmap
}

template <typename T>
int HashMap<T>::size() const {
    return num_associations;
}

template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value) {
    // first run through hash function
    size_t h = hashFunction(key);
    
    // loop through linked list of pairs
    for (auto it = buckets[h].begin(); it != buckets[h].end(); it++) {
        if ((*it).m_key == key) { // matching association, reassign
            (*it).m_value = value;
            return;
        }
    }
    
    // no matching associations, so insert a new association
    Node newNode;
    newNode.m_key = key;
    newNode.m_value = value;
    buckets[h].push_back(newNode);
    num_associations++;
    rehashMap(); // will only rehash if exceeds max_load_factor

}

// If no association exists with the given key, return nullptr; otherwise,
// return a pointer to the value associated with that key. This pointer can be
// used to examine that value or modify it directly within the map.
template <typename T>
const T* HashMap<T>::find(const std::string& key) const {
    size_t h = hashFunction(key);
    for (auto it = buckets[h].begin(); it != buckets[h].end(); it++) {
        if ((*it).m_key == key) {
            return (&(*it).m_value);
        }
    }
    return nullptr;
}

// bracket operator
// If the key does not exist in the hashmap, this will create a new entry in
// the hashmap and map it to the default value of type T. Then it will return a
// reference to the newly created value in the map.
template <typename T>
T& HashMap<T>::operator[](const std::string& key) {
    if (find(key) != nullptr) { // key exists
        return *find(key);
    }
    else { // key does not exist
        T* value = new T{}; // default value of T
        insert(key, *value);
        return *value;
    }
}

/* ///////// HELPER FUNCTIONS //////////*/

// rehashes map when exceeds max_load_factor
template <typename T>
void HashMap<T>::rehashMap() {
    
    double curr_load_factor = static_cast<double>(num_associations) / buckets.size();
    if (curr_load_factor <= max_load_factor)
        return;
    
    size_t bucket_size = buckets.size();
    bucket_size *= 2;
    
    vector<list<Node>> newBuckets(bucket_size);
    // cerr << "bucket_size!!!!" << bucket_size;
    
    for (int i = 0; i < bucket_size / 2; i++) {
        for (auto it = buckets[i].begin(); it != buckets[i].end(); it++) {
            
            size_t h = hash<string>()(it->m_key) % bucket_size; // hash function to new size
            
            // **************** USE SPLICE??????????????????
            Node newNode;
            newNode.m_key = (*it).m_key;
            newNode.m_value = (*it).m_value;
            //size_t h = hashFunction((*it).m_key);
            newBuckets[h].push_back(newNode);
            
//            auto next = std::next(it);
//            // format for splice - list1_name.splice (iterator position, list2)
//            newBuckets[h].splice(newBuckets[h].begin(), buckets[i], it);
//            it = next;
        }
    }
    buckets.clear();
    buckets = newBuckets;
}

// hash function using string's hash
template <typename T>
size_t HashMap<T>::hashFunction(string str) const {
    size_t h = hash<string>()(str) % buckets.size();
    return h;
}

#endif /* HashMap_h */
