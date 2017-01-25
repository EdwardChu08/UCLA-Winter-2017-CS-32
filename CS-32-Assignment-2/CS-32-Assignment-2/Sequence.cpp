//
//  Sequence.cpp
//  CS-32-Assignment-2
//
//  Created by Edward Chu on 25/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#include "Sequence.h"

#include <iostream>

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
        
        for(Node* temp = newNode->nextNode; temp != nullptr; temp = temp->nextNode){
            temp->pos++;
        }
        
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
    while(temp != nullptr && temp->pos != pos){
        temp = temp->nextNode;
    }
    
    Node* newNode = new Node(value, temp->prevNode, temp);
    temp->prevNode->nextNode = newNode;
    temp->prevNode = newNode;
    listSize++;
    
    for(Node* temp = newNode->nextNode; temp != nullptr; temp = temp->nextNode){
        temp->pos++;
    }
    
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


void Sequence::dump() const{
    std::cerr << "--- Starting printing ---" << std::endl;
    
    Node* temp = headNode;
    
    while(temp != nullptr){
        std::cerr << "Node " << temp->pos << ": " << temp->value << std::endl;
        temp = temp->nextNode;
    }
    
    std::cerr << "--- Printing ended ---" << std::endl;
}
