//
//  linear.cpp
//  CS-32-Homework-3
//
//  Created by Edward Chu on 10/2/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#include <iostream>
#include <string>
//#include <cassert>

using namespace std;

//bool somePredicate(double x);


// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
    if(n < 0) n = 0;
    
    if(n == 0) return true;
    
    if(n == 1) return somePredicate(a[0]);
    
    return allTrue(a, n/2) && allTrue(a+(n/2), (n+1)/2);
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
    if(n < 0) n = 0;
    
    if(n == 0) return 0;
    
    if(n == 1) return somePredicate(a[0])? 0 : 1;
    
    return countFalse(a, n/2) + countFalse(a+(n/2), (n+1)/2);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
    
    if(n < 0) n = 0;
    if(n == 0) return -1;
    
    //if(n == 1) return somePredicate(a[0]) ? -1 : 0;
    
    if(!somePredicate(a[0])) return 0;
    
    int temp = firstFalse(a+1, n-1);
    if(temp != -1) return 1 + temp;
    
    return -1;
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if(n < 0) n = 0;
    
    if(n == 0) return -1;
    
    if(n == 1) return 0;
    
    int temp = indexOfMin(a+1, n-1);
    if(a[0] <= a[temp+1]) return 0;
    else return temp+1;
}


// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if(n1 < 0) n1 = 0;
    if(n2 < 0) n2 = 0;
    
    if(n2 == 0) return true;
    if(n1 == 0) return false;
    
    if(a1[0] == a2[0]){
        if(includes(a1+1, n1-1, a2+1, n2-1)){
            return true;
        }
    }
    else if(includes(a1+1, n1-1, a2, n2)){
        return true;
    }
    return false;
}


/*
bool somePredicate(double x)
{
    return x < 0;
}

int main(){
    
    double allTrueTest1[5] = {-1, -2, -3, -4, -5};
    assert(allTrue(allTrueTest1, 5));
    assert(countFalse(allTrueTest1, 5) == 0);
    assert(firstFalse(allTrueTest1, 5) == -1);
    assert(indexOfMin(allTrueTest1, 5) == 4);;
    
    double allTrueTest2[5] = {1, -2, -3, -4, -5};
    assert(!allTrue(allTrueTest2, 5));
    assert(countFalse(allTrueTest2, 5) == 1);
    assert(firstFalse(allTrueTest2, 5) == 0);
    
    double allTrueTest3[5] = {-1, -2, -3, -4, 5};
    assert(!allTrue(allTrueTest3, 5));
    assert(countFalse(allTrueTest3, 5) == 1);
    assert(firstFalse(allTrueTest3, 5) == 4);
    
    double allTrueTest4[5] = {1, 2, 3, 4, 5};
    assert(!allTrue(allTrueTest4, 5));
    assert(countFalse(allTrueTest4, 5) == 5);
    assert(firstFalse(allTrueTest4, 5) == 0);
    
    double allTrueTest5[5] = {-1, -2, 3, 4, -5};
    assert(!allTrue(allTrueTest5, 5));
    assert(countFalse(allTrueTest5, 5) == 2);
    assert(firstFalse(allTrueTest5, 5) == 2);
    
    assert(allTrue(allTrueTest4, 0));
    assert(countFalse(allTrueTest4, 0) == 0);
    assert(firstFalse(allTrueTest4, 0) == -1);
    assert(indexOfMin(allTrueTest4, 0) == -1);
    
    
    double minIndexTest1[5] = {0, 1, 2, 3, 4};
    assert(indexOfMin(minIndexTest1, 5) == 0);
    
    double minIndexTest2[5] = {0, 1, -2, 3, 4};
    assert(indexOfMin(minIndexTest2, 5) == 2);
    assert(indexOfMin(minIndexTest2, 1) == 0);
    
    
    double includeTestA1[7] = {10, 50, 40, 20, 50, 40, 30};
    double includeTestB1[3] = {50, 20, 30};
    double includeTestB2[3] = {50, 40, 40};
    double includeTestB3[3] = {50, 30, 20};
    double includeTestB4[3] = {10, 20, 20};
    double includeTestB5[1] = {50};
    double includeTestB6[1] = {-1};
    double includeTestB7[3] = {10, 40, 30};
    
    assert(includes(includeTestA1, 7, includeTestB1, 3));
    assert(includes(includeTestA1, 7, includeTestB2, 3));
    assert(includes(includeTestA1, 7, includeTestB2, 0));
    assert(includes(includeTestA1, 7, includeTestB5, 1));
    assert(includes(includeTestA1, 7, includeTestB7, 3));
    
    assert(!includes(includeTestA1, 7, includeTestB3, 3));
    assert(!includes(includeTestA1, 7, includeTestB4, 3));
    assert(!includes(includeTestA1, 0, includeTestB4, 3));
    assert(!includes(includeTestA1, 7, includeTestB6, 1));
    
    cerr << "All tests succeed" << endl;
    
}
*/
