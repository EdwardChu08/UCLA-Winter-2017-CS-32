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
    int pos = 0;
    
    for(Node* temp = headNode; temp != nullptr; temp = temp->nextNode, pos++){
        if(temp->value >= value){
            insert(pos, value);
            return pos;
        }
    }
    insert(listSize, value);
    return listSize-1;
}


bool Sequence::erase(int pos){
    
    if(pos >= listSize || pos < 0)
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
    int count = 0;
    for(Node* temp = headNode; temp != nullptr; temp = temp->nextNode, count++){
        if(pos == count){
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


//////////////////////////////////////////////////////////
//HELPER FUNCTIONS
//////////////////////////////////////////////////////////

bool Sequence::outOfBounds(int pos){
    return pos > listSize || pos < 0;
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
    
    std::cerr << "--- Printing ended ---" << std::endl;
}
