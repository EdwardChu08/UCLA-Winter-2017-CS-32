//
//  eval.cpp
//  CS-32-Homework-2
//
//  Created by Edward Chu on 5/2/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#include <string>
#include <iostream>
#include <stack>
#include <cassert>

using namespace std;



const int OPERAND = 1;
const int OPEN_BRACKET = 2;
const int CLOSED_BRACKET = 3;
const int OPERATOR = 4;

const int numValidChars = 15;
const char validChars[numValidChars] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '&', '|', '!', '(', ')'};


int evaluate(string infix, const bool values[], string& postfix, bool& result);
int charType(const char c);
bool isClosedBracket(const char c);
bool isOpenBracket(const char c);
bool isOperator(const char c);
bool isOperand(const char c);
int getOperatorPrecedence(const char c);
bool isUnaryOperator(const char c);
bool isValidChar(const char c);
bool isBinaryOperator(const char c);
bool isBracket(const char c);


int main(){
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("((2)", ba, pf, answer) == 1);
    assert(evaluate("(2))", ba, pf, answer) == 1);
    assert(evaluate("2))((3", ba, pf, answer) == 1);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate("(", ba, pf, answer) == 1);
    assert(evaluate("!&8", ba, pf, answer) == 1);
    assert(evaluate("&", ba, pf, answer) == 1);
    assert(evaluate("!", ba, pf, answer) == 1);
    assert(evaluate("8!", ba, pf, answer) == 1);
    assert(evaluate("5&", ba, pf, answer) == 1);
    assert(evaluate("&5", ba, pf, answer) == 1);
    assert(evaluate("(4)5", ba, pf, answer) == 1);
    assert(evaluate("*5", ba, pf, answer) == 1);
    assert(evaluate("AA", ba, pf, answer) == 1);
    assert(evaluate("5&&5", ba, pf, answer) == 1);
    assert(evaluate("5||5", ba, pf, answer) == 1);
    assert(evaluate("(0&|4)", ba, pf, answer) == 1);
    
    assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
           &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    cout << "Passed all tests" << endl;
}



int evaluate(string infix, const bool values[], string& postfix, bool& result){
    
    //////////////////////////////////////////////////////////////////////////////
    //Sanitizing infix
    //////////////////////////////////////////////////////////////////////////////
    
    //Empty infix
    if(infix.empty()) return 1;
    
    
    //Invalid char
    for(int i = 0; i < infix.length(); i++){
        if(infix[i] == ' '){
            infix.erase(infix.begin()+i);
            i--;
        } else if(!isValidChar(infix[i])){
            return 1;
        }
    }
    
    //Open brackets and closed brackets not matching
    {
        int count = 0;
        for(int i = 0; i < infix.length(); i++){
            if(isOpenBracket(infix[i])) count++;
            else if(isClosedBracket(infix[i])) count--;
            
            if(count < 0) return 1;
        }
        if(count != 0) return 1;
    }
    
    //Starting/Ending with binary operator
    if(isBinaryOperator(infix[0]) ||  isBinaryOperator(infix[infix.length()-1])) return 1;
    
    //Ending with unary operator
    if(isUnaryOperator(infix[infix.length()-1])) return 1;
    
    //No operands
    {
        bool noOperands = true;
        for(int i = 0; i < infix.length(); i++){
            if(isOperand(infix[i])){
                noOperands = false;
                break;
            }
        }
        if(noOperands) return 1;
    }
    
    //Operator or bracket next to binary operator
    for(int i = 1; i < infix.length()-1; i++){
        if(isBinaryOperator(infix[i])){
            if(isBinaryOperator(infix[i-1]) || isOpenBracket(i-1))
                return 1;
            if(isBinaryOperator(infix[i+1]) || isClosedBracket(i+1))
                return 1;
        }
    }
    
    //Operator or bracket next to unary operator
    for(int i = 0; i < infix.length()-1; i++){
        if(isUnaryOperator(infix[i])){
            if(isBinaryOperator(infix[i+1]) || isClosedBracket(infix[i+1]))
                return 1;
        }
    }
    
    //Operand next to each other
    //Special cases (Start and end)
    if(infix.length() != 1 && isOperand(infix[0]) && isOperand(infix[1]))
        return 1;
    if(infix.length() != 1 && isOperand(infix[infix.length()-1]) && isOperand(infix.length()-2))
        return 1;
    
    //Middle cases
    for(int i = 1; i < infix.length()-1; i++){
        if(isOperand(infix[i])){
            if(isOperand(infix[i+1]) || isOperand(infix[i-1]))
                return 1;
        }
    }
    
    //Operand next to Bracket
    for(int i = 0; i < infix.length()-1; i++){
        if(isOperand(infix[i])){
            if(isOpenBracket(infix[i+1]))
                return 1;
        }
    }
    
    for(int i = 1; i < infix.length(); i++){
        if(isOperand(infix[i])){
            if(isClosedBracket(infix[i-1]))
                return 1;
        }
    }
    
    //Brackets with no operand inside
    for(int i = 0; i < infix.length()-1; i++){
        if(isOpenBracket(infix[i]) && isClosedBracket(infix[i+1]))
           return 1;
    }
    

    //Removing duplicating !!
    for(int i = 0; i < infix.length()-1; i++){
        if(isUnaryOperator(infix[i]) && isUnaryOperator(infix[i+1])){
            infix.erase(infix.begin()+i);
            infix.erase(infix.begin()+i);
            i--;
        }
    }
    
    //////////////////////////////////////////////////////////////////////////////
    //Turning infix into postfix
    //////////////////////////////////////////////////////////////////////////////
    postfix = "";
    stack<char> operatorStack;
    
    for(int i = 0; i < infix.length(); i++){
        switch (charType(infix[i])) {
                
            case OPERAND:
                postfix += infix[i];
                break;
            
            case OPEN_BRACKET:
                operatorStack.push(infix[i]);
                break;
                
            case CLOSED_BRACKET:
                while(operatorStack.top() != '('){
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.pop();
                break;
                
            case OPERATOR:
                while(!operatorStack.empty() && operatorStack.top() != '(' && getOperatorPrecedence(infix[i]) >= getOperatorPrecedence(operatorStack.top())){
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(infix[i]);
                break;
            default:
                break;
        }
    }
    
    while(!operatorStack.empty()){
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    
    
    //////////////////////////////////////////////////////////////////////////////
    //Evaluating postfix
    //////////////////////////////////////////////////////////////////////////////
    stack<bool> operandStack;
    for(int i = 0; i < postfix.length(); i++){
        
        if(charType(postfix[i]) == OPERAND){
            operandStack.push(values[postfix[i]-'0']);
        }
        
        else if(isUnaryOperator(postfix[i])){
            bool temp = !operandStack.top();
            operandStack.pop();
            operandStack.push(temp);
        }
        
        else {
            //Binary operator
            bool operand1 = operandStack.top();
            operandStack.pop();
            bool operand2 = operandStack.top();
            operandStack.pop();
            
            bool result;
            
            if(postfix[i] == '&'){
                result = operand1 && operand2;
            } else if(postfix[i] == '|'){
                result = operand1 || operand2;
            }
            
            operandStack.push(result);
            
        }
        
    }
    
    result = operandStack.top();
    return 0;
}

int charType(const char c){
    if(isOperand(c)) return OPERAND;
    if(isOperator(c)) return OPERATOR;
    if(isOpenBracket(c)) return OPEN_BRACKET;
    if(isClosedBracket(c)) return CLOSED_BRACKET;
    
    //Invalid char
    return -1;
}

//Lower num, higher precedence
int getOperatorPrecedence(const char c){
    if(c == '!') return 1;
    if(c == '&') return 2;
    if(c == '|') return 3;
    
    //Invalid operator
    return -1;
}

bool isClosedBracket(const char c){
    return c == ')';
}

bool isOpenBracket(const char c){
    return c == '(';
}

bool isOperator(const char c){
    return c == '|' || c == '&' || c == '!';
}

bool isUnaryOperator(const char c){
    return c == '!';
}

bool isOperand(const char c){
    return c - '0' <= 9 && c - '0' >= 0;
}

bool isValidChar(const char c){
    for(int i = 0; i < numValidChars; i++){
        if(validChars[i] == c)
            return true;
    }
    return false;
}

bool isBinaryOperator(const char c){
    return c == '&' || c == '|';
}

bool isBracket(const char c){
    return isClosedBracket(c) || isOpenBracket(c);
}





