/*
 * @name: Shaokang Ma
 * @id: a1790846
 * @Date: 2020-10-02 20:01:13
 * @LastEditTime: 2020-10-05 13:36:19
 * @Description: for page replacement
 * @FilePath: /MCI/os/assignment2/PageReplacement.cpp
 */

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <fstream>
#include <numeric>
#include <iomanip>
#include <bitset>
#include <list>
#include <stdexcept>
#include <functional>
#include <utility>
#include <ctime>
using namespace std;

// to store the information of each page
class Pages
{
public:
    char event ;
    string address ;
    bool dirty ;
};

// a function to convert a string into a page input
Pages input(string line)
{
    Pages page ;

    // split the string into 2 parts from the white space
    page.event = line[0] ;
    page.address = line.substr(1, line.length()) ;

    // check the event
    if ( page.event == 'W') page.dirty = true ; else
    page.dirty = false ;

    return page ;
}

// to find the index of a page in the frame
// return -1 if it does not exist
int findPage(Pages page, deque<Pages> frames)
{
    int size = frames.size() ;
    int index = -1 ;

    // if the address is the same, it means the page has been found
    for ( int i = 0 ; i < size ; i ++ )
    {
        if ( frames[i].address == page.address ) index = i ;
    }
    
    return index ;
}

int main(int argc, char const *argv[])
{
    Pages p1, p2, p3, p4, p5 ;
    p1.address = "0000c806" ;
    p2.address = "00007749" ;
    p3.address = "000081a3" ;
    p4.address = "00008a79" ;
    p5.address = "0000d290" ;

    deque<Pages> frames ;
    frames.push_back(p1) ;
    frames.push_back(p2) ;
    frames.push_back(p3) ;
    frames.push_back(p4) ;

    map<int , string> a ;
    a[3] = "1" ;
    a[3] += "0" ;
    a[4] = "5" ;
    a[1] = "7" ;
    a.erase(a.find(3)) ;

    if ( a.count(3) == 0 ) cout << "yes" ; else
    cout << "no" ;
    
    return 0;
}