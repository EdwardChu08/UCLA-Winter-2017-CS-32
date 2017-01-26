//
//  Sequence.cpp
//  CS-32-Assignment-2
//
//  Created by Edward Chu on 25/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#include "Sequence.h"

#include <iostream>

//////////////////////////////////////////////////////////
//CONSTRUCTORS AND DESTRUCTORS
//////////////////////////////////////////////////////////

Sequence::Sequence(){
    listSize = 0;
    headNode = tailNode = nullptr;
}

Sequence::~Sequence(){
    Node* temp = headNode;
    
    //Delete all the nodes!
    while(temp != nullptr){
        temp = temp->nextNode;
        
        if(temp != nullptr)
            delete temp->prevNode;
    }
}

//Copy Constructor

Sequence::Sequence(Sequence const &other){
    listSize = 0;
    Node* temp = other.headNode;
    
    for(int i = 0; temp != nullptr; i++, temp = temp->nextNode){
        insert(i, temp->value);
    }
}

//Overload Assignment Operator

Sequence& Sequence::operator= (const Sequence &other){
    if(&other != this){
        deleteSequence();
        
        listSize = 0;
        Node* temp = other.headNode;
        
        for(int i = 0; temp != nullptr; i++, temp = temp->nextNode){
            insert(i, temp->value);
        }
    }
    return *this;
}

//////////////////////////////////////////////////////////
//FUNCTIONS
//////////////////////////////////////////////////////////
bool Sequence::empty() const{
    return headNode == nullptr;
}


int Sequence::size() const{
    return listSize;
}


bool Sequence::insert(int pos, const ItemType& value){
    
    //Out of bounds
    if(pos > listSize || pos < 0)
        return false;
    
    //Empty List
    if(headNode == nullptr){
        headNode = new Node(value, nullptr, nullptr);
        tailNode = headNode;
        listSize++;
        return true;
    }
    
    //Add to start
    if(pos == 0){
        Node* newNode = new Node(value, nullptr, headNode);
        headNode->prevNode = newNode;
        headNode = newNode;
        listSize++;
        
        return true;
    }
    
    //Add to end
    if(pos == listSize){
        Node* newNode = new Node(value, tailNode, nullptr);
        tailNode->nextNode = newNode;
        tailNode = newNode;
        listSize++;
        return true;
    }
    
    //Middle cases
    Node* temp = headNode;
    int count = 0;
    while(temp != nullptr && count != pos){
        temp = temp->nextNode;
        count++;
    }
    
    Node* newNode = new Node(value, temp->prevNode, temp);
    temp->prevNode->nextNode = newNode;
    temp->prevNode = newNode;
    listSize++;
    
    return true;
}


int Sequence::insert(const ItemType& value){
    int countPos = 0;
    
    for(Node* temp = headNode; temp != nullptr; temp = temp->nextNode, countPos++){
        if(temp->value >= value){
            insert(countPos, value);
            return countPos;
        }
    }
    insert(listSize, value);
    return listSize-1;
}


bool Sequence::erase(int pos){
    
    if(outOfBounds(pos))
        return false;
    
    //Remove Head
    if(pos == 0){
        Node* temp = headNode->nextNode;
        delete headNode;
        headNode = temp;
        headNode->prevNode = nullptr;
        listSize--;
        return true;
    }
    
    //Remove Tail
    if(pos == listSize-1){
        Node* temp = tailNode->prevNode;
        delete tailNode;
        tailNode = temp;
        tailNode->nextNode = nullptr;
        listSize--;
        return true;
    }
    
    //Remove Middle
    int countPos = 0;
    for(Node* temp = headNode; temp != nullptr; temp = temp->nextNode, countPos++){
        if(pos == countPos){
            temp->prevNode->nextNode = temp->nextNode;
            temp->nextNode->prevNode = temp->prevNode;
            delete temp;
            listSize--;
            return true;
        }
    }
    
    //Should never be reached
    return false;
    
}

int Sequence::remove(const ItemType& value){
    int removeCount = 0;
    int countPos = 0;
    
    for(Node* temp = headNode; temp != nullptr; temp = temp->nextNode, countPos++){
        if(temp->value == value){
            erase(countPos);
            removeCount++;
            countPos--;
        }
    }
    
    return removeCount;
}

bool Sequence::get(int pos, ItemType& value) const {
    
    if(outOfBounds(pos))
        return false;
    
    int countPos = 0;
    
    for(Node* temp = headNode; temp != nullptr; temp = temp->nextNode, countPos++){
        if(countPos == pos){
            value = temp->value;
            return true;
        }
    }
    
    //This should never be reached
    return false;
}

bool Sequence::set(int pos, const std::string& value){
    
    if(outOfBounds(pos))
        return false;
    
    int countPos = 0;
    
    for(Node* temp = headNode; temp != nullptr; temp = temp->nextNode, countPos++){
        if(countPos == pos){
            temp->value = value;
            return true;
        }
    }
    
    //This should never be reached
    return false;
}

int Sequence::find(const ItemType& value) const{
    int countPos = 0;
    
    for(Node* temp = headNode; temp != nullptr; temp = temp->nextNode, countPos++){
        if(temp->value == value){
            return countPos;
        }
    }
    
    //Not found
    return -1;
}

void Sequence::swap(Sequence& other){
    
    if(&other == this)
        return; //Swapping with self
    
    Node* temp = other.headNode;
    other.headNode = headNode;
    headNode = temp;
    
    temp = other.tailNode;
    other.tailNode = tailNode;
    tailNode = temp;
    
    int tempInt = other.listSize;
    other.listSize = listSize;
    listSize = tempInt;
}

//////////////////////////////////////////////////////////
//NEW FUNCTIONS
//////////////////////////////////////////////////////////

int subsequence(const Sequence& seq1, const Sequence& seq2){
    int posCount1 = 0;
    
    for(posCount1 = 0; posCount1 + seq2.size() <= seq1.size(); posCount1++){
        
        ItemType value1, value2;
        seq1.get(posCount1, value1);
        seq2.get(0, value2);
        
        if(value1 == value2){
            int startPos = posCount1;
            for(int posCount2 = 0; value1 == value2; posCount1++, posCount2++){
                if(posCount2 == seq2.size())
                    return startPos; //End of Seq2 reached, subseq found
                
                seq1.get(posCount1, value1);
                seq2.get(posCount2, value2);
            }
                
        }
    }
    
    return -1;
}


//////////////////////////////////////////////////////////
//HELPER FUNCTIONS
//////////////////////////////////////////////////////////

bool Sequence::outOfBounds(int pos) const{
    return pos >= listSize || pos < 0;
}


void Sequence::deleteSequence(){
    Node* temp = headNode;
    
    //Delete all the nodes!
    while(temp != nullptr){
        temp = temp->nextNode;
        
        if(temp != nullptr)
            delete temp->prevNode;
    }
    
    listSize = 0;
    headNode = tailNode = nullptr;
}


void Sequence::dump() const{
    std::cerr << "--- Starting printing ---" << std::endl;
    
    std::cerr << "Size: " << listSize << std::endl;
    
    Node* temp = headNode;
    int count = 0;
    
    while(temp != nullptr){
        std::cerr << "Node " << count << ": " << temp->value << std::endl;
        temp = temp->nextNode;
        count++;
    }
    
    std::cerr << "--- Printing ended ---" << std::endl << std::endl;
}

