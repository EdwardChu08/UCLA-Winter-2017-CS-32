//
//  Sequence.h
//  CS-32-Assignment-2
//
//  Created by Edward Chu on 25/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#ifndef Sequence_h
#define Sequence_h

#include <string>

typedef std::string ItemType;

class Sequence
{
public:
    //Constructors and Destructors
    Sequence();
    Sequence(Sequence const &other);

    ~Sequence();
    
    //Assignment Operator
    Sequence& operator= (const Sequence &other);
    
    //Functions
    bool empty() const;
    int size() const;
    bool insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    
    void dump() const;

private:
    
    //Node
    struct Node {
        Node(ItemType initValue, Node* prev, Node* next){
            value = initValue;
            prevNode = prev;
            nextNode = next;
            
            if(prevNode == nullptr)
                pos = 0;
            else
                pos = prevNode->pos + 1;
        };
    
        ItemType value;
        Node* prevNode;
        Node* nextNode;
        int pos;
    };

    Node* headNode;
    Node* tailNode;
    int listSize;
    
};


#endif /* Sequence_h */
