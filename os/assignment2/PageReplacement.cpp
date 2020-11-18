/*
 * @name: Shaokang Ma
 * @id: a1790846
 * @Date: 2020-10-02 20:01:13
 * @LastEditTime: 2020-10-05 16:12:53
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

// counters for event, disk reads, disk writes and page faults
static int events = 0, disk_reads = 0, disk_writes = 0, page_faults = 0 ;

// to store the information of each page
class Pages
{
public:
    char event ;
    string address ;
    bool dirty ;
    int section ;
    int arrival ;
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
        if ( frames[i].section == page.section ) index = i ;
    }
    
    return index ;
}

// sort the current frames according to the additional reference bits
deque<Pages> sortReferenceBits(deque<Pages> frames, map<int, string> reference_bits)
{
    for (int i = 1; i < frames.size(); i++)
    {
        int j = i - 1;
        Pages key = frames[i];
        while (j >= 0 && stoi(reference_bits[key.section], nullptr, 2) < stoi(reference_bits[frames[j].section], nullptr, 2))
        {
            swap(frames[j], frames[j + 1]);
            j -= 1;
        }
        swap(key, frames[j + 1]);
    }

    return frames ;
}

// sort the current frames according to the working set frequency
deque<Pages> sortFrequency(deque<Pages> frames, map<int, int> frequency)
{
    for (int i = 1; i < frames.size(); i++)
    {
        int j = i - 1;
        Pages key = frames[i];
        while (j >= 0 && frequency[key.section] < frequency[frames[j].section])
        {
            swap(frames[j], frames[j + 1]);
            j -= 1;
        }
        swap(key, frames[j + 1]);
    }

    return frames ;
}

// sort the current frames according to the working set frequency
deque<Pages> sortArrival(deque<Pages> frames)
{
    for (int i = 1; i < frames.size(); i++)
    {
        int j = i - 1;
        Pages key = frames[i];
        while (j >= 0 && key.arrival < frames[j].arrival)
        {
            swap(frames[j], frames[j + 1]);
            j -= 1;
        }
        swap(key, frames[j + 1]);
    }

    return frames ;
}

// first in first out paging scheme
void firstInFirstOut(int page_size, int frame_num, deque<Pages> pages)
{
    // to store the pages in frames
    deque<Pages> frames ;

    // to store the dirty pages
    map<int, bool> dirtyPages ;

    // paging starts
    while ( !pages.empty() )
    {
        // get the next page in
        Pages current = pages.front() ;
        pages.pop_front() ;
        current.section = stoi(current.address, nullptr, 16) / page_size ;

        // to update the dirty bit
        if ( current.dirty ) dirtyPages[current.section] = true ;

        // if the page is not in the frames
        if ( findPage(current, frames) == -1 )
        {
            // a page fault happens
            page_faults ++ ;

            // reading from the disk must happen
            disk_reads ++ ;
            
            // read the page directly in from the disk if there is still a free frame
            // need to find a victim which is the first one if there is not a free frame
            if ( frames.size() == frame_num )
            {
                // if it is dirty, need to write to disk
                // if it is not, nothing will happen
                if ( dirtyPages[frames.front().section] )
                {
                    disk_writes ++ ;

                    // reset the dirty bit to clean
                    dirtyPages[frames.front().section] = false ;
                }

                // swap the first one with the current page
                frames.pop_front() ;
            }
            frames.push_back(current) ;
        }
    }
}

// least recently used paging scheme
void leastRecentlyUsed(int page_size, int frame_num, deque<Pages> pages)
{
    // to store the pages in frames
    deque<Pages> frames ;

    // to store the dirty pages
    map<int, bool> dirtyPages ;

    // paging starts
    while ( !pages.empty() )
    {
        // get the next page in
        Pages current = pages.front() ;
        pages.pop_front() ;
        current.section = stoi(current.address, nullptr, 16) / page_size ;

        // to update the dirty bit
        if ( current.dirty ) dirtyPages[current.section] = true ;

        // if the page is not in the frames
        if ( findPage(current, frames) == -1 )
        {
            // a page fault happens
            page_faults ++ ;

            // reading from the disk must happen
            disk_reads ++ ;
            
            // read the page directly in from the disk if there is still a free frame
            // need to find a victim which is the first one if there is not a free frame
            if ( frames.size() == frame_num )
            {
                // if it is dirty, need to write to disk
                // if it is not, nothing will happen
                if ( dirtyPages[frames.front().section] )
                {
                    disk_writes ++ ;

                    // reset the dirty bit to clean
                    dirtyPages[frames.front().section] = false ;
                }

                // swap the first one with the current page
                frames.pop_front() ;
            }
            frames.push_back(current) ;
        }
        // if the page is in the frames, move it to the bottom, marked as referenced
        else
        {
            frames.erase(frames.begin() + findPage(current, frames)) ;
            frames.push_back(current) ;
        }
    }
}

// additional reference bits paging scheme
void additionalReferenceBits(int page_size, int frame_num, deque<Pages> pages, int shift_register, int regular_interval)
{
    // to count the time
    int timer = 1 ;

    // to store the pages in frames
    deque<Pages> frames ;

    // to store the dirty pages
    map<int, bool> dirtyPages ;

    // to store the additional reference bits
    map<int, string> reference_bits ;

    // to tell whether it is the first time to be referenced
    bool first_time = false ;

    // paging starts
    while ( !pages.empty() )
    {
        // get the next page in
        Pages current = pages.front() ;
        pages.pop_front() ;
        current.section = stoi(current.address, nullptr, 16) / page_size ;
        current.arrival = timer ;

        // initialise the map if the page is not in it or the reference bits is 0
        if ( reference_bits.count(current.section) == 0 )
        {
            first_time = true ;
            reference_bits[current.section] = "1" ;
            // contrasin the length
            for (int i = 0; i < shift_register - 1; i++)
            {
                reference_bits[current.section] += "0" ;
            }   
        }
        else if ( reference_bits[current.section][0] == '0')
        {
            reference_bits[current.section][0] = '1' ;
            first_time = true ;
        }

        // to update the dirty bit
        if ( current.dirty ) dirtyPages[current.section] = true ;

        // if the page is not in the frames
        if ( findPage(current, frames) == -1 )
        {
            // a page fault happens
            page_faults ++ ;

            // reading from the disk must happen
            disk_reads ++ ;
            
            // read the page directly in from the disk if there is still a free frame
            // need to find a victim which is the first one if there is not a free frame
            if ( frames.size() == frame_num )
            {
                // sort the dequeue by the arrival time
                frames = sortArrival(frames) ;
                
                // sort the deque to make the least recently used page at the beginning of the deque
                frames = sortReferenceBits(frames, reference_bits) ;

                // if it is dirty, need to write to disk
                // if it is not, nothing will happen
                if ( dirtyPages[frames.front().section] )
                {
                    disk_writes ++ ;

                    // reset the dirty bit to clean
                    dirtyPages[frames.front().section] = false ;
                }

                // remove the reference bits of the page
                reference_bits.erase(reference_bits.find(frames.front().section)) ;

                // swap the first one with the current page
                frames.pop_front() ;
            }
            frames.push_back(current) ;
        }

        // update the timer
        timer ++ ;

        // if it is the time
        if ( timer % regular_interval == 1)
        {
            // to update the current additional reference bits
            if ( !first_time )
            {
                reference_bits[current.section] = "1" + reference_bits[current.section] ;
                reference_bits[current.section] = reference_bits[current.section].substr(0, shift_register) ;
            }
            else
            {
                first_time = false ;
            }
            
            // add a 0 to other additional reference bits
            map<int, string>::iterator iter  = reference_bits.begin() ;
            
            while ( iter != reference_bits.end() )
            {
                reference_bits[iter->first] = "0" + reference_bits[iter->first] ;
                reference_bits[iter->first] = reference_bits[iter->first].substr(0, shift_register) ;
                iter ++ ;
            }
        }
    }
}

// working set additional reference bits paging scheme - 1
void workingSetARB_1(int page_size, int frame_num, deque<Pages> pages, int shift_register, int regular_interval, int working_set_window)
{
    // to store the working set
    deque<int> working_set ;

    // to count the time
    int timer = 1 ;

    // to store the pages in frames
    deque<Pages> frames ;

    // to store the dirty pages
    map<int, bool> dirtyPages ;

    // to store the additional reference bits
    map<int, string> reference_bits ;

    // to tell whether it is the first time to be referenced
    bool first_time = false ;

    // paging starts
    while ( !pages.empty() )
    {
        // get the next page in
        Pages current = pages.front() ;
        pages.pop_front() ;
        current.section = stoi(current.address, nullptr, 16) / page_size ;
        current.arrival = timer ;

        // update the working set
        if ( working_set.size() == working_set_window ) working_set.pop_front() ;
        working_set.push_back(current.section) ;

        // initialise the map if the page is not in it or the reference bits is 0
        if ( reference_bits.count(current.section) == 0 )
        {
            first_time = true ;
            reference_bits[current.section] = "1" ;
            // contrasin the length
            for (int i = 0; i < shift_register - 1; i++)
            {
                reference_bits[current.section] += "0" ;
            }   
        }
        else if ( reference_bits[current.section][0] == '0')
        {
            reference_bits[current.section][0] = '1' ;
            first_time = true ;
        }

        // to update the dirty bit
        if ( current.dirty ) dirtyPages[current.section] = true ;

        // if the page is not in the frames
        if ( findPage(current, frames) == -1 )
        {
            // a page fault happens
            page_faults ++ ;

            // reading from the disk must happen
            disk_reads ++ ;
            
            // read the page directly in from the disk if there is still a free frame
            // need to find a victim which is the first one if there is not a free frame
            if ( frames.size() == frame_num )
            {
                // sort the dequeue by the arrival time
                frames = sortArrival(frames) ;
                
                // sort the deque to make the least recently used page at the beginning of the deque
                frames = sortReferenceBits(frames, reference_bits) ;

                // sort the deque by working set frequency
                map<int, int> frequency ;
                for (int i = 0; i < working_set.size(); i++)
                {
                    if ( frequency.count(working_set[i]) == 0 ) frequency[working_set[i]] = 1 ; else
                    frequency[working_set[i]] ++ ;
                }

                frames = sortFrequency(frames, frequency) ;

                // if it is dirty, need to write to disk
                // if it is not, nothing will happen
                if ( dirtyPages[frames.front().section] )
                {
                    disk_writes ++ ;

                    // reset the dirty bit to clean
                    dirtyPages[frames.front().section] = false ;
                }

                // remove the reference bits of the page
                reference_bits.erase(reference_bits.find(frames.front().section)) ;

                // swap the first one with the current page
                frames.pop_front() ;
            }
            frames.push_back(current) ;
        }

        // update the timer
        timer ++ ;

        // if it is the time
        if ( timer % regular_interval == 1)
        {
            // to update the current additional reference bits
            if ( !first_time )
            {
                reference_bits[current.section] = "1" + reference_bits[current.section] ;
                reference_bits[current.section] = reference_bits[current.section].substr(0, shift_register) ;
            }
            else
            {
                first_time = false ;
            }
            
            // add a 0 to other additional reference bits
            map<int, string>::iterator iter  = reference_bits.begin() ;
            
            while ( iter != reference_bits.end() )
            {
                reference_bits[iter->first] = "0" + reference_bits[iter->first] ;
                reference_bits[iter->first] = reference_bits[iter->first].substr(0, shift_register) ;
                iter ++ ;
            }
        }
    }
}

// working set additional reference bits paging scheme - 2
void workingSetARB_2(int page_size, int frame_num, deque<Pages> pages, int shift_register, int regular_interval, int working_set_window)
{
    // to store the working set
    deque<int> working_set ;

    // to count the time
    int timer = 1 ;

    // to store the pages in frames
    deque<Pages> frames ;

    // to store the dirty pages
    map<int, bool> dirtyPages ;

    // to store the additional reference bits
    map<int, string> reference_bits ;

    // to tell whether it is the first time to be referenced
    bool first_time = false ;

    // paging starts
    while ( !pages.empty() )
    {
        // get the next page in
        Pages current = pages.front() ;
        pages.pop_front() ;
        current.section = stoi(current.address, nullptr, 16) / page_size ;
        current.arrival = timer ;

        // update the working set
        if ( working_set.size() == working_set_window ) working_set.pop_front() ;
        working_set.push_back(current.section) ;

        // initialise the map if the page is not in it or the reference bits is 0
        if ( reference_bits.count(current.section) == 0 )
        {
            first_time = true ;
            reference_bits[current.section] = "1" ;
            // contrasin the length
            for (int i = 0; i < shift_register - 1; i++)
            {
                reference_bits[current.section] += "0" ;
            }   
        }
        else if ( reference_bits[current.section][0] == '0')
        {
            reference_bits[current.section][0] = '1' ;
            first_time = true ;
        }

        // to update the dirty bit
        if ( current.dirty ) dirtyPages[current.section] = true ;

        // if the page is not in the frames
        if ( findPage(current, frames) == -1 )
        {
            // a page fault happens
            page_faults ++ ;

            // reading from the disk must happen
            disk_reads ++ ;
            
            // read the page directly in from the disk if there is still a free frame
            // need to find a victim which is the first one if there is not a free frame
            if ( frames.size() == frame_num )
            {
                // sort the dequeue by the arrival time
                frames = sortArrival(frames) ;

                // sort the deque by working set frequency
                map<int, int> frequency ;
                for (int i = 0; i < working_set.size(); i++)
                {
                    if ( frequency.count(working_set[i]) == 0 ) frequency[working_set[i]] = 1 ; else
                    frequency[working_set[i]] ++ ;
                }

                frames = sortFrequency(frames, frequency) ;

                // sort the deque to make the least recently used page at the beginning of the deque
                frames = sortReferenceBits(frames, reference_bits) ;

                // if it is dirty, need to write to disk
                // if it is not, nothing will happen
                if ( dirtyPages[frames.front().section] )
                {
                    disk_writes ++ ;

                    // reset the dirty bit to clean
                    dirtyPages[frames.front().section] = false ;
                }

                // remove the reference bits of the page
                reference_bits.erase(reference_bits.find(frames.front().section)) ;

                // swap the first one with the current page
                frames.pop_front() ;
            }
            frames.push_back(current) ;
        }

        // update the timer
        timer ++ ;

        // if it is the time
        if ( timer % regular_interval == 1)
        {
            // to update the current additional reference bits
            if ( !first_time )
            {
                reference_bits[current.section] = "1" + reference_bits[current.section] ;
                reference_bits[current.section] = reference_bits[current.section].substr(0, shift_register) ;
            }
            else
            {
                first_time = false ;
            }
            
            // add a 0 to other additional reference bits
            map<int, string>::iterator iter  = reference_bits.begin() ;
            
            while ( iter != reference_bits.end() )
            {
                reference_bits[iter->first] = "0" + reference_bits[iter->first] ;
                reference_bits[iter->first] = reference_bits[iter->first].substr(0, shift_register) ;
                iter ++ ;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    // get the page size, frame number and paging scheme
    int page_size = atoi(argv[2]) ;
    int frame_num = atoi(argv[3]) ;
    int shift_register, regular_interval, working_set_window ;
    string scheme = argv[4] ;
    if ( argv[5] != NULL ) shift_register = atoi(argv[5]) ;
    if ( argv[6] != NULL ) regular_interval = atoi(argv[6]) ;
    if ( argv[7] != NULL ) working_set_window = atoi(argv[7]) ;

    // a deque to store the inputs
    deque<Pages> pages ;

    // open the input file
    string line ;
    freopen(argv[1], "r", stdin) ;

    // initialise the input deque
    while (getline(cin,line))
    {
        // count the events
        events ++ ;

        pages.push_back(input(line)) ;
    }

    // get rid off the first line which is the trace name
    pages.pop_front() ;
    events -- ;

    // handle different page replacement algorithms
    if ( scheme == "FIFO" ) firstInFirstOut(page_size, frame_num, pages) ; else
    if ( scheme == "LRU" ) leastRecentlyUsed(page_size, frame_num, pages) ; else
    if ( scheme == "ARB" ) additionalReferenceBits(page_size, frame_num, pages, shift_register, regular_interval) ; else
    if ( scheme == "WSARB-1" ) workingSetARB_1(page_size, frame_num, pages, shift_register, regular_interval, working_set_window) ; else
    workingSetARB_2(page_size, frame_num, pages, shift_register, regular_interval, working_set_window) ;

    // make the outputs
    cout << "events in trace: " << events << endl ;
    cout << "total disk reads: " << disk_reads << endl ;
    cout << "total disk writes: " << disk_writes << endl ;
    cout << "page faults: " << page_faults << endl ;

    return 0;
}