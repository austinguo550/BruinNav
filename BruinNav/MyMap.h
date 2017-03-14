//#ifndef MYMAP_INCLUDED
//#define MYMAP_INCLUDED
//
//#include <map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN
//#include <iostream>
//
//// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
//// since you are not allowed to use any STL associative containers, and
//// this implementation uses std::map.
//
//// This code is deliberately obfuscated.
//
//// If you can not get your own MyMap class template working, you may use
//// this one during development in order to let you proceed with implementing
//// the other classes for this project; you can then go back to working on
//// fixing your own MyMap class template.
//
//template <typename KeyType, typename ValueType>
//class MyMap
//{
//public:
//    MyMap(const MyMap &other) = delete;
//    MyMap &operator=(const MyMap &other) = delete;
//    typedef KeyType O;using O1=int;void clear(){l01.clear();}
//    using l0=ValueType;MyMap(){}using l10=O const;O1 size()
//    const{return l01.size();}using ll0=l0 const;using Const=
//    MyMap<O,l0>;void associate(l10&Using,ll0&first){l01[
//                                                        Using]=first;}using l1=std::map<O,l0>;using l00=Const
//    const;ll0*find(l10&l11)const{auto first=l01.find(l11);
//        return(first!=l01.end()?&first->second:0);}l0*find(l10&
//                                                           l01){return(l0*)(*(l00*)(this)).find(l01);}private:l1
//    l01;
//};
//
//#endif // MYMAP_INCLUDED









//
//  MyMap.h
//  BruinNav
//
//  Created by Austin Guo on 3/8/17.
//  Copyright © 2017 Austin Guo. All rights reserved.
//

#ifndef MyMap_h
#define MyMap_h

////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: REMOVE!!! ONLY FOR TESTING
//
#include <iostream>
#include "support.h"
////////////////////////////////////////////////////////////////////////////////////////////////

// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.



template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap();
    ~MyMap();
    void clear();
    int size() const;
    
    
    // The associate method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the tree with that key/value pair. If there is
    // already an association with that key in the tree, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the tree contains no duplicate keys.
    void associate(const KeyType& key, const ValueType& value);
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value, and if the map is allowed to be modified, to
    // modify that value directly within the map (the second overload enables
    // this). Using a little C++ magic, we have implemented it in terms of the
    // first overload, which you must implement.
        /////// for a map that can't be modified, return a pointer to const ValueType
    // TODO: first overload that we have to implement
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    // DONE FOR US
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    struct Node {
        KeyType key;
        ValueType value;
        Node* lessThan = nullptr; // TODO: make it automatically point to nullptr?
        Node* greaterThan = nullptr; // TODO: make it automatically point to nullptr?
    };
    
    void freeTree(Node* current);
    bool insertAfter(const KeyType& key, const ValueType& value, Node*& node, char c = '<');
    Node* root;
    int sizeOf;
    
};



template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap() {
    sizeOf = 0;
    root = nullptr;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap() {
    clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear() {
    freeTree(root);
    sizeOf = 0;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::freeTree(Node* current){
    if (current == nullptr || sizeOf == 0)
        return;
    
    freeTree(current->lessThan);
    freeTree(current->greaterThan);
    
//    std::cerr << current << std::endl;
    sizeOf--;
    delete current;
}

template<typename KeyType, typename ValueType> inline
int MyMap<KeyType, ValueType>::size() const {
    return sizeOf;
}


// O(log N)     // since it divides the list to search in half every time, such that you must double the size of the list for one more operation to be performed in searching the list, and insertion is constant time
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
    
    // if the tree is empty
    if (size() == 0 && root == nullptr) {
        bool inserted = insertAfter(key, value, root);
        return;
    }
    
    // attempt to insert or replace
    Node* p = root;
    while (p != nullptr) {                          // should always break beefore getting to a nullptr position
        if (key == p->key) {                        // cannot have duplicate keys, just replace the value
            p->value = value;
            return;
        }
        if (key < p->key) {
            if (p->lessThan != nullptr)
                p = p->lessThan;
            else {
                bool inserted = insertAfter(key, value, p, '<');
                return;
            }
            continue;
        }
        if (key > p->key) {
            if (p->greaterThan != nullptr)
                p = p->greaterThan;
            else {
                bool inserted = insertAfter(key, value, p, '>');
                return;
            }
            continue;
        }
    }
    // no need for size++ because it's incremented in insertAfter
    return;
}

template<typename KeyType, typename ValueType>
bool MyMap<KeyType, ValueType>::insertAfter(const KeyType& key, const ValueType& value, Node*& node, char c) {   // c can only be '<' or '>' (do you insert before or after)
    // doesn't matter what default char is because only called by default when inserting at root
    
    //make a new node
    Node* toAdd = new Node;
    toAdd->lessThan = nullptr;
    toAdd->greaterThan = nullptr;
    toAdd->key = key;
    toAdd->value = value;
    
    //link it to other nodes
    
    //if tree's empty
    if (node == nullptr) {
        //std::cerr << size();                // this should be empty
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        node = toAdd;                       // should set root to toAdd
        sizeOf++;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //std::cerr << " " << root->value;    // should be toAdd's value
        return true;
    }
    //if tree has anything in it
    switch(c) {
        case '<':
            node->lessThan = toAdd;
            break;
        case '>':
            node->greaterThan = toAdd;
            break;
    }
    sizeOf++;
    return true;                            // always
}


// O(log N)     // since it divides the list to search in half every time, such that you must double the size of the list for one more operation to be performed in searching the list
template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const {
    Node* p = root;
    while (p != nullptr) {                          // should always break beefore getting to a nullptr position
        if (key == p->key) {                        // cannot have duplicate keys, just replace the value
            return &p->value;
        }
        if (key < p->key) {
            p = p->lessThan;
            continue;
        }
        if (key > p->key) {
            p = p->greaterThan;
            continue;
        }
    }
    return nullptr;
}

#endif /* MyMap_h */
