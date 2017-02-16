//
//  tree.cpp
//  CS-32-Homework-3
//
//  Created by Edward Chu on 12/2/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//
/*
void separate(double a[], int n, double separator,
              int& firstNotGreater, int& firstLess);
void exchange(double& x, double& y);
*/

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a2     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if(n1 < 0) n1 = 0;
    if(n2 < 0) n2 = 0;
    
    if(n2 == 0) return 1;
    if(n1 == 0) return 0;
    
    int count = 0;
    if(a1[0] == a2[0]){
        count += countIncludes(a1+1, n1-1, a2+1, n2-1);
    }
    count += countIncludes(a1+1, n1-1, a2, n2);
    
    return count;
}

// Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > separator come before all the other elements,
// and all the elements whose value is < separator come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= separator, or n if there is no such element, and firstLess is
// set to the index of the first element that is < separator, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > separator
//   * for firstNotGreater <= i < firstLess, a[i] == separator
//   * for firstLess <= i < n, a[i] < separator
// All the elements > separator end up in no particular order.
// All the elements < separator end up in no particular order.
void separate(double a[], int n, double separator,
              int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;
    
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > separator
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == separator
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < separator
    
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < separator)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > separator)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}


// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
    if(n <= 1) return;
    
    int a1;
    int b1;
    separate(a, n, a[0], a1, b1);
    
    order(a, a1);
    order(a+b1, n-b1);
    
}

/*
#include <iostream>
#include <cassert>

using namespace std;

int main(){
    double countIncludeTestA1[7] = {10, 50, 40, 20, 50, 40, 30};
    double countIncludeTestB1[3] = {10, 20, 40};
    double countIncludeTestB2[3] = {10, 40, 30};
    double countIncludeTestB3[3] = {20, 10, 40};
    double countIncludeTestB4[3] = {50, 40, 30};
    double countIncludeTestB5[2] = {10, 50};
    
    
    assert(countIncludes(countIncludeTestA1, 7, countIncludeTestB1, 3) == 1);
    assert(countIncludes(countIncludeTestA1, 7, countIncludeTestB2, 3) == 2);
    assert(countIncludes(countIncludeTestA1, 7, countIncludeTestB3, 3) == 0);
    assert(countIncludes(countIncludeTestA1, 7, countIncludeTestB4, 3) == 3);
    assert(countIncludes(countIncludeTestA1, 7, countIncludeTestB5, 2) == 2);
    
    
    double orderTest1[7] = {10, 50, 40, 20, 50, 40, 30};
    order(orderTest1, 7);
    for(int i = 0; i < 7; i++){
        cout << orderTest1[i] << " ";
    }
    cout << endl;
    
    double orderTest2[3] = {50, 40, 30};
    order(orderTest2, 3);
    for(int i = 0; i < 3; i++){
        cout << orderTest2[i] << " ";
    }
    cout << endl;
    
    double orderTest3[2] = {30, 40};
    order(orderTest3, 2);
    for(int i = 0; i < 2; i++){
        cout << orderTest3[i] << " ";
    }
    cout << endl;
    
    double orderTest4[10] = {20, 20, 30, 50, 70, 90, 110, 0, -5, 20};
    order(orderTest4, 10);
    for(int i = 0; i < 10; i++){
        cout << orderTest4[i] << " ";
    }
    cout << endl;
    
}
*/
