/*
created by: Shaokang Ma
ID: a1790846
time: 17.08.2020
Contact Email: a1790846@student.adelaide.edu.au
Include int main(int argc,char *argv[])
input: argv[1]
output: Screen

input sample:
ID arrival_time priority age total_tickets_required
for example: s1 3 1 0 50

output sample:
ID, arrival and termination times, ready time and durations of running and waiting
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

class Customer
{  
    public:
    string id;
    int arrival;
    int priority;
    int age;
    int ticketrequired;
    int weight;
    int run;
    int end;
    int ready;
    int running;
    int waiting;
    Customer();
    void display( void );
};

Customer::Customer(void)
{
    this->age = 0;
    this->run = 0;
}

void Customer::display( void )
{
    cout << this->id << setw(10) << this->arrival << setw(10) << this->end << setw(10) << this->ready << setw(10) << this->running << setw(10) << this->waiting << endl;
}

Customer input(string line)
{
    Customer cus;
    int start = line.find_first_of(" ");
    cus.id = line.substr(0,start);
    line.replace(start, 1, "*");
    int end = line.find_first_of(" ");
    cus.arrival = stoi(line.substr(start + 1, end));
    start = end;
    line.replace(end, 1, "*");
    end = line.find_first_of(" ");
    cus.priority = stoi(line.substr(start + 1, end));
    cus.weight = (10 - cus.priority) * 10;
    start = end;
    line.replace(end, 1, "*");
    end = line.find_first_of(" ");
    cus.age = stoi(line.substr(start + 1, end));
    start = end;
    line.replace(end, 1, "*");
    end = line.find_first_of(" ");
    cus.ticketrequired = stoi(line.substr(start + 1, end)) * 5;
    cus.running = cus.ticketrequired;
    cus.run = 0;
    cus.end = 0;
    cus.ready = -1;
    cus.waiting = 0;
    return cus;
}

deque<Customer> sortArrival(deque<Customer> customers)
{
    vector<Customer> temp;
    while (!customers.empty())
    {
        temp.push_back(customers.front());
        customers.pop_front();
    }
    
    for (int i = 1; i < temp.size(); i++)
    {
        int j = i - 1;
        Customer key = temp[i];
        while (j >= 0 && key.arrival < temp[j].arrival)
        {
            swap(temp[j], temp[j + 1]);
            j -= 1;
        }
        swap(key, temp[j +1 ]);
    }

    for (int i = 0; i < temp.size(); i++)
    {
        customers.push_back(temp[i]);
    }
    
    return customers;  
}

deque<Customer> sortEnd(deque<Customer> customers)
{
    vector<Customer> temp;
    while (!customers.empty())
    {
        temp.push_back(customers.front());
        customers.pop_front();
    }
    
    for (int i = 1; i < temp.size(); i++)
    {
        int j = i - 1;
        Customer key = temp[i];
        while (j >= 0 && key.end < temp[j].end)
        {
            swap(temp[j], temp[j + 1]);
            j -= 1;
        }
        swap(key, temp[j +1 ]);
    }

    for (int i = 0; i < temp.size(); i++)
    {
        customers.push_back(temp[i]);
    }
    
    return customers;   
}

deque<Customer> sortPriority(deque<Customer> customers)
{
    vector<Customer> temp;
    while (!customers.empty())
    {
        temp.push_back(customers.front());
        customers.pop_front();
    }
    
    for (int i = 1; i < temp.size(); i++)
    {
        int j = i - 1;
        Customer key = temp[i];
        while (j >= 0 && key.priority < temp[j].priority)
        {
            swap(temp[j], temp[j + 1]);
            j -= 1;
        }
        swap(key, temp[j +1 ]);
    }

    for (int i = 0; i < temp.size(); i++)
    {
        customers.push_back(temp[i]);
    }
    
    return customers;  
}

deque<Customer> sortTicketsRequired(deque<Customer> customers)
{
    vector<Customer> temp;
    while (!customers.empty())
    {
        temp.push_back(customers.front());
        customers.pop_front();
    }
    
    for (int i = 1; i < temp.size(); i++)
    {
        int j = i - 1;
        Customer key = temp[i];
        while (j >= 0 && key.ticketrequired < temp[j].ticketrequired)
        {
            swap(temp[j], temp[j + 1]);
            j -= 1;
        }
        swap(key, temp[j +1 ]);
    }

    for (int i = 0; i < temp.size(); i++)
    {
        customers.push_back(temp[i]);
    }
    
    return customers;   
}

deque<Customer> sortRun(deque<Customer> customers)
{
    vector<Customer> temp;
    while (!customers.empty())
    {
        temp.push_back(customers.front());
        customers.pop_front();
    }
    
    for (int i = 1; i < temp.size(); i++)
    {
        int j = i - 1;
        Customer key = temp[i];
        while (j >= 0 && key.run < temp[j].run)
        {
            swap(temp[j], temp[j + 1]);
            j -= 1;
        }
        swap(key, temp[j +1 ]);
    }

    for (int i = 0; i < temp.size(); i++)
    {
        customers.push_back(temp[i]);
    }
    
    return customers;   
}

deque<Customer> sortAge(deque<Customer> customers)
{
    for (int i = 1; i < customers.size(); i++)
    {
        int j = i - 1;
        Customer key = customers[i];
        while (j >= 0 && key.ticketrequired < customers[j].ticketrequired)
        {
            swap(customers[j], customers[j + 1]);
            j -= 1;
        }
        swap(key, customers[j +1 ]);
    }
    return customers;  
}

deque<Customer> process(deque<Customer>customers)
{
    //set up 2 queues
    deque<Customer> queue1;
    deque<Customer> queue2;
    //  two attributes to control the process
    int time = 0;
    bool processQueue2 = true;
    bool sortQueue1 = false;
    bool ifAge = false;
    //  a vector to store the finished processes
    deque<Customer> output;
    Customer store;
    bool ifStore = false;
    while (!queue1.empty() || !queue2.empty() || !customers.empty() || ifStore)  // can not stop running if there is a movement
    {
        processQueue2 = true;  //  reset permission for queue2
        //  new arrivals in queue1
        //  choose which process to run
            

        while (!customers.empty() && customers.front().arrival == time && customers.front().priority <= 3)
        {
            queue1.push_back(customers.front());
            customers.pop_front();
            cout << time <<"s arrive"<< " "<<queue1.back().id << " in queue1"<< " left"<< queue1.back().ticketrequired<< endl;
        }

        if (ifStore)
        {
            queue1.push_back(store);
            ifStore = false;
        }

        //  aging mechanism
        deque<Customer> temp;  //  temporarily store remaining processes in queue2
        while (!queue2.empty())
        {
            //  check if increase priority
            if (queue2.front().age == 100)
            {
                queue2.front().age = 0;
                queue2.front().priority -= 1;
                queue2.front().weight = (10 - queue2.front().priority) * 10;
            }
            //  check if promoted
            if (queue2.front().priority <= 3)
            {
                queue1.push_back(queue2.front());
                queue2.pop_front();
            } else
            {
                temp.push_back(queue2.front());
                queue2.pop_front();
            }
        }
        queue2 = temp;
        
        if (sortQueue1)
            {
                queue1 = sortPriority(queue1);
                sortQueue1 = false;
                cout << "time " << time << endl;
                for (int i = 0; i < queue1.size(); i++)
                {
                    cout << queue1[i].id <<" " << queue1[i].priority << " " << queue1[i].run << " left"<< queue1[i].ticketrequired<< endl;
                }
                cout << "queue2" << endl;
                for (int i = 0; i < queue2.size(); i++)
                {
                    cout << queue2[i].id <<" " << queue2[i].priority << " " << queue2[i].age << " left"<< queue2[i].ticketrequired << endl;
                }      
            }

        if (!queue1.empty())
        {
            //  stop queue2 running
            processQueue2 = false;
            queue1.front().ticketrequired -= 5;
            ifAge =true;

            //  update the ready time
            if (queue1.front().ready == -1)
            {
                queue1.front().ready = time;
            }
            // update the waiting time of queue1
            for (int i = 1; i < queue1.size(); i++)
            {
                if (queue1[i].ready != -1)
                {
                    queue1[i].waiting += 5;
                } 
            }

            //  update the waiting time of queue2
            for (int i = 0; i < queue2.size(); i++)
            {
                if (queue2[i].ready != -1)
                {
                    queue2[i].waiting += 5;
                } 
            }
               
            cout << time <<"s process"<< " "<<queue1.front().id << " in queue1"<< " left"<< queue1.front().ticketrequired << endl;
            //  if not bought all the tickets
            if (queue1.front().ticketrequired != 0)
            {
                queue1.front().weight -= 5;
                if (queue1.front().weight == 0)
                {
                    queue1.front().run += 1;
                }
                //  check if decrease the priority
                if (queue1.front().run == 2)
                {
                    queue1.front().priority += 1;
                    queue1.front().run = 0;  //  reset the run for the new priority
                }
                //  check whether demoted or move to the back
                if (queue1.front().weight == 0)
                {
                    queue1.front().weight = (10 - queue1.front().priority) * 10;
                    if (queue1.front().priority > 3)
                    {
                        queue1.front().age -= 5;
                        queue2.push_back(queue1.front());
                        queue1.pop_front();
                        cout << "DEMOTE" << " "<<queue2.back().id << endl;
                    } else
                    {
                        store = queue1.front();
                        queue1.pop_front();
                        ifStore = true;
                    }
                    sortQueue1 = true;
                }
            } else  //  if already bought all tickets
            {
                queue1.front().end = time + 5;  //  remember to add 5 more units
                output.push_back(queue1.front());
                queue1.pop_front();
            } 
        }
        
        //  new arrivals in queue2
        while (!customers.empty() && customers.front().arrival == time && customers.front().priority > 3)
        {
            queue2.push_back(customers.front());
            customers.pop_front();
            cout << time <<"s arrive"<< " " <<queue2.back().id <<" in queue2"<< endl;
        }

        //  update the waiting time and age of queue2 because of processing queue1
        if (ifAge)
        {
            for (int i = 0; i < queue2.size(); i++)
            {
                queue2[i].age += 5;
            }
            ifAge = false;
        }

        queue2 = sortPriority(queue2);
        queue2 = sortTicketsRequired(queue2);
        if (!queue2.empty() && processQueue2)
        {
            queue2.front().ticketrequired -= 5;
            queue2.front().age = 0;

            cout << time <<"s process"<< " "<<queue2.back().id << " in queue2"<< endl;
            //  update the ready time
            if (queue2.front().ready == -1)
            {
                queue2.front().ready = time;
            }
            //  update waiting time and age of queue2
            for (int i = 1; i < queue2.size(); i++)
            {
                if (queue2[i].ready != -1)
                {
                    queue2[i].waiting += 5;
                }
                queue2[i].age += 5;
            }
            
            //  if bought all the tickets
            if (queue2.front().ticketrequired == 0)
            {
                queue2.front().end = time + 5;
                output.push_back(queue2.front());
                queue2.pop_front();
            }
        }
        
        time += 5;
    }
    return output;
}

int main(int argc, char const *argv[])
{
    /* initialise the input deque*/
    deque<Customer> customers;
    string line;
    freopen(argv[1],"r",stdin);
    while (getline(cin,line))
    {
        customers.push_back(input(line));
    }

    /* sort the vector by arrival time */
    customers = sortPriority(customers);
    customers = sortArrival(customers);

    //  process
    deque<Customer> output;
    output = process(customers);
    //  sort the output by termination time
    output = sortEnd(output);
    // display
    cout<<"name   arrival   end   ready   running   waiting"<<endl;
    while (!output.empty())
    {
        output.front().display();
        output.pop_front();
    }
    
    return 0;
}