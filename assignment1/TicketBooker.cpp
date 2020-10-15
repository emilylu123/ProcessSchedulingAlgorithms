/*
created by:Yingyao Lu
ID:a1784870
time:24.08.2020
Contact Email: a1784870@student.adelaide.edu.au

Include int main(int argc,char *argv[])
input: argv[1]
output: Screen

input sample:
ID arrival_time priority age total_tickets_required
for example: s1 3 1 0 50

output sample:
ID, arrival and termination times, ready time and durations of running and waiting
*/

#include <cctype>
#include <queue>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <utility>
#include <algorithm>
using namespace std;

//static bool debug = true;
static bool debug = false;

// create a a_customer information;
class Customer
{
public:
    int processId;
    float priority;   // range from 1 to 5
    int age;
    int ticketsRequire;
    int arrivalTime;
    int endTime;
    int readyTime;  // the a time the system processes request
    int runningTime;    // durations of running
    int waitingTime;    // durations of waiting
    int remainingTime;
    bool isReady;
    bool isDone;
    bool hasServed;
};

// this function will create a a_customer instance according to one line in the file.
Customer initialCustomer(string aLine)
{
    stringstream line(aLine);
    string str;
    vector<int> vec;

    // Traverse the string
    while(line >> str) {
        for (int i = 0; i< str.length(); i++){
            if(!isdigit(str[i]))
                str.erase(str.begin());
        }
        int temp = 0;
        for(int i =0; i<str.size(); i++){
            temp = temp*10 + (str[i]-'0');
        }
        vec.push_back(temp);
    }

    // create a Customer object
    Customer aCustomer;
    aCustomer.processId = vec [0];
    aCustomer.arrivalTime = vec [1];
    aCustomer.priority = vec [2];
    aCustomer.age = vec [3];
    aCustomer.ticketsRequire = vec [4];
    aCustomer.isReady = false;
    aCustomer.isDone = false;
    aCustomer.hasServed = false;
    aCustomer.readyTime = 0;
    aCustomer.runningTime = 0;
    aCustomer.waitingTime = 0;
    aCustomer.remainingTime = aCustomer.ticketsRequire * 5;

    return aCustomer;
}

// print out the result
void output(vector<Customer>& result, bool isProcess)
{
    if (!isProcess)
    {
        cout << "name   arrival   end   ready   running   waiting" << endl;
        for (int i = 0; i < result.size(); i++){
            cout << "a" << result.at(i).processId << "\t";
            cout << result.at(i).arrivalTime << "\t";
            cout << result.at(i).endTime << "\t";
            cout << result.at(i).readyTime << "\t";
            cout << result.at(i).runningTime << "\t";
            cout << result.at(i).waitingTime << endl;
        }
    } else{
//        cout << "name arrival tickets running priority age/runs waiting" << endl;
        for (int i = 0; i < result.size(); i++){
            cout << "a" << result.at(i).processId << "\t";
            cout << result.at(i).arrivalTime << "\t";
            cout << result.at(i).ticketsRequire << "\t";
            cout << result.at(i).runningTime << "\t";
            cout << result.at(i).priority << "\t";
            cout << result.at(i).age << "\t";
            cout << result.at(i).waitingTime << endl;
        }
    }
}

// show all the process
void printProcess(vector<Customer>& result, vector<Customer>& queue1, vector<Customer>& queue2, int& runningTime){
    if (!(queue1.empty() && queue2.empty()))
        cout <<endl<< "==========TIME  " << runningTime << "===========" << endl;
    cout << "name arrival tickets running priority age/runs waiting" << endl;
    if (!queue1.empty()) {
        cout << "Queue1" << endl;
        output(queue1,true);
    }
    if (!queue2.empty()) {
        cout << "Queue2" << endl;
        output(queue2,true);
    }

//    if (!result.empty()){
//        cout << endl << "===========RESULT===========" << endl;
//        output(result, false);
//    }
}

// if the arrival time is same, order will follow the increasing order of their customer IDs
bool compareID(const Customer& a, const Customer& b) {
    return a.processId < b.processId;
}

// the condition of sort function on arrival time;
bool compareArrivalTime(const Customer& a, const Customer& b) {
    if (a.arrivalTime < b.arrivalTime)
        return true;
    else if (a.arrivalTime == b.arrivalTime) {
        return a.processId < b.processId;
    }
    else
        return false;
//    return a.arrivalTime < b.arrivalTime;
}

// the condition of sorting function on priority;
bool comparePriority(const Customer& a, const Customer& b) {
    return a.priority < b.priority;
}

// the condition of end time.
bool compareRemainingTime(const Customer& a, const Customer& b) {
    return a.remainingTime < b.remainingTime;
}

void updateRunning (Customer aCustomer, int& ticket_dealing_time){
    aCustomer.runningTime += ticket_dealing_time;
}

// promote the customer from queue2 to queue1
void promoteQueue(vector<Customer>& queue1, vector<Customer>& queue2){
    // todo not so sure about this part
    for (int i = 0; i<queue2.size(); i++) {
        if (queue2[i].priority<=3){
//            queue2.front().age = 0; // todo whether change back to zero is not sure
            queue1.push_back(queue2[i]);
            queue2.erase(queue2.begin()+i);
        }
    }
}

// demote the customer from queue1 into the queue2
void demoteQueue(vector<Customer>& queue1, vector<Customer>& queue2){
    if (queue1.front().priority>=4){
        queue1.front().age = 0;
        queue2.push_back(queue1.front());
        queue1.erase(queue1.begin());
    }
    // todo not sure if sort here
    sort(queue2.begin(),queue2.end(),comparePriority);
    sort(queue2.begin(),queue2.end(),compareRemainingTime);
}

// update waiting time for queue1 and waiting and aging for queue2
void waitingAgingQ1(vector<Customer>& queue1, int& ticket_dealing_time){
    // update Q1 waiting
    for (int i = 1; i<queue1.size(); i++) {
        if (queue1[i].isReady)
            queue1[i].waitingTime += ticket_dealing_time;
    }
}

// update queue2 waiting and aging time only
bool waitingAgingQ2(vector<Customer>& queue1, vector<Customer>& queue2, int& ticket_dealing_time){
    bool ifPromote = false;
    for (int i = 1; i<queue2.size(); i++)
    {
        if (queue2[i].isReady)
            queue2[i].waitingTime += ticket_dealing_time; //todo

        // aging mechanism
        //        if(queue2[i].isReady){
        queue2[i].age += ticket_dealing_time;
        //        }
    }
    for (int i = 1; i<queue2.size(); i++){
        if (queue2[i].age>=100){
            queue2[i].age -= 100;
            queue2[i].priority -= 1;
        }
    }
    for (int i = 0; i<queue2.size(); i++){
        if(queue2[i].priority<=3){
//            promoteQueue(queue1,queue2);
            queue2[i].age = 0;
            queue2[i].hasServed = false;
            queue1.push_back(queue2[i]);
            queue2.erase(queue2.begin()+i);
            i--;
            ifPromote = true;
//            break;
        }
    }
    return ifPromote;
}

// update queue2 waiting and aging time only
bool waitingAgingQ2all(vector<Customer>& queue1, vector<Customer>& queue2, int& ticket_dealing_time){
    bool ifPromote = false;
        // update Q2 waiting and aging
    for (int i = 0; i<queue2.size(); i++){
        if (queue2 [i].isReady)
            queue2 [i].waitingTime += ticket_dealing_time; // todo

        // todo aging mechanism
        queue2 [i].age += ticket_dealing_time;
        if (queue2[i].age>=100){
            queue2 [i].age -= 100;
            queue2 [i].priority -= 1;
        }
    }

    for (int i = 0; i<queue2.size(); i++){
        if(queue2[i].priority<=3){
//            promoteQueue(queue1,queue2);
            queue2[i].age =0;
            queue2[i].hasServed = false;
            queue1.push_back(queue2[i]);
            queue2.erase(queue2.begin()+i);
            i--;
            ifPromote = true;
        }
    }
    return ifPromote;

}

// push the task into the queues according to the arrive time.
void queueing(vector<Customer>& inputs, vector<Customer>& queue1, vector<Customer>& queue2, int& runningTime){
    while(!inputs.empty() && runningTime == inputs.front().arrivalTime){
        // send to queue 1
        if (inputs.front().priority <= 3)
        {
            queue1.push_back(inputs.front());
        }

        // send to queue 2
        else {
            queue2.push_back(inputs.front());
            sort(queue2.begin(), queue2.end(), comparePriority);
            sort(queue2.begin(), queue2.end(), compareRemainingTime);
        }
        // delete the first customer from the inputs
        inputs.erase(inputs.begin());
    }
}

// Apply the Weighted Round Robin (WRR) rule to queue 1
void WRR(vector<Customer>& inputs, vector<Customer>& queue1, vector<Customer>& queue2, vector<Customer>& result, int& runningTime){
//    sort(queue1.begin(),queue1.end(),compareID);
    sort(queue1.begin(),queue1.end(),comparePriority);
    int requiredTime = queue1.front().ticketsRequire * 5;
    int quota = ( 10 - queue1.front().priority ) * 10;
    int dealingTime = 5;
    // change current customer's state to "Ready"
    if (!queue1.front().isReady){
        queue1.front().readyTime = runningTime;
        queue1.front().isReady = true;
    }

    // finish current customer update and push to result vector
    if (requiredTime <= quota){
        // check to queueing other inputs every 5 time units
        for (int i = 0; i < requiredTime/5; i++){
            runningTime += dealingTime;
            waitingAgingQ1(queue1, dealingTime); // todo
            waitingAgingQ2all(queue1, queue2, dealingTime);

            if (runningTime == inputs.front().arrivalTime)
                queueing(inputs, queue1, queue2, runningTime);
        }
        // todo q1 coming in the first in the middle of a operation
        queue1.front().runningTime += requiredTime;
        queue1.front().endTime = runningTime;
        queue1.front().isDone = true;

        result.push_back(queue1.front());
        queue1.erase(queue1.begin());

    } else {
        // check to queueing other inputs every 5 time units
        for (int i = 0; i < quota/5; i++){
            runningTime += 5;
            waitingAgingQ1(queue1, dealingTime);
            waitingAgingQ2all(queue1, queue2, dealingTime);

            if (runningTime == inputs.front().arrivalTime){
                queueing(inputs, queue1, queue2, runningTime);
            }
        }
        queue1.front().runningTime += quota;
        queue1.front().ticketsRequire -= quota/5;
        queue1.front().remainingTime -= quota;

        // priority +1 every two round
        if (!queue1.front().hasServed) {
            queue1.front().hasServed = true;
        }
        else{
            queue1.front().hasServed = false;
            queue1.front().priority += 1;
        }

        // demote to queue2 if its priority >=4
        if (queue1.front().priority >=4){
            demoteQueue(queue1,queue2);

            if (debug) {
                cout << "----DEMOTE-Q1--->" ;
                printProcess(result,queue1,queue2, runningTime);
            }
        }
        // else sort Queue1 by their priority
        else{
            queue1.push_back(queue1.front());
            queue1.erase(queue1.begin());
        }
    }
//    sort(queue1.begin(),queue1.end(),comparePriority); // todo ?? sort here?
//    sort(queue1.begin(),queue1.end(),compareID);
    sort(queue1.begin(),queue1.end(),comparePriority);
}

// Apply Shortest Remaining Time First to queue 2
void SRTF(vector<Customer>& queue1, vector<Customer>& queue2, vector<Customer>& result, int& runningTime){
    sort(queue2.begin(),queue2.end(),comparePriority);
    sort(queue2.begin(),queue2.end(),compareRemainingTime);

    if (!queue2.front().isReady){
        queue2.front().readyTime = runningTime;
        queue2.front().isReady = true;
    }

    int dealingTime = 5;
    runningTime += dealingTime;
    queue2.front().age = 0;
    queue2.front().ticketsRequire -= 1;
    queue2.front().runningTime += dealingTime;
    queue2.front().remainingTime -= dealingTime;

    bool ifP = waitingAgingQ2(queue1, queue2, dealingTime);
    if (ifP && debug) printProcess(result,queue1,queue2, runningTime);


    // if current customer has finish all the tickets required
    if (queue2.front().ticketsRequire == 0) {
        queue2.front().isDone = true;
        queue2.front().endTime = runningTime;
        result.push_back(queue2.front());
        queue2.erase(queue2.begin());
        if (debug) printProcess(result,queue1,queue2, runningTime);
    }
}

// the function dealing with the ticket requirement
void works(vector<Customer>& inputs, vector<Customer>& queue1, vector<Customer>& queue2, vector<Customer>& result, int& runningTime)
{

    // add new customers to correct queue by their arrival time
    if (runningTime == inputs.front().arrivalTime){
        queueing(inputs, queue1, queue2, runningTime);
        if (debug) printProcess(result,queue1,queue2, runningTime);
    }

    // Deal with High Priority First Queue 1
    while (!queue1.empty())
    {
        WRR(inputs, queue1, queue2, result, runningTime);
        if (debug) printProcess(result,queue1,queue2, runningTime);
    }

    while (!queue2.empty() && queue1.empty())
    {
        SRTF(queue1, queue2 , result, runningTime);
        if (runningTime == inputs.front().arrivalTime){
            queueing(inputs, queue1, queue2, runningTime);
//            if (!queue1.empty()) WRR(inputs, queue1, queue2, result, runningTime);
            if (debug) printProcess(result,queue1,queue2, runningTime);
        }
    }

    while (queue1.empty() && queue2.empty() && !inputs.empty()) {
        runningTime += 5;
        if (runningTime == inputs.front().arrivalTime){
            queueing(inputs, queue1, queue2, runningTime);
//            if (!queue1.empty()) WRR(inputs, queue1, queue2, result, runningTime);
            if (debug) printProcess(result,queue1,queue2, runningTime);
        }
    }
//    if (!queue1.empty()) WRR(inputs, queue1, queue2, result, runningTime);

}

int main(int argc,char *argv[]){

    int running_time = 0;
    vector<Customer> inputs; // the whole task is stored here
    vector<Customer> queue1 ; // High-Priority queue (priority <=3) using Highest Priority First & Weighted Round Robin
    vector<Customer> queue2 ; // Low-Priority queue (>=4) using Shortest remaining time
    vector<Customer> result; // the task finished is stored here

    string aLine;
    fstream read_inputs(argv[1]);

    // initial data from inputs file to inputs queue
    while(getline(read_inputs,aLine))
    {
        Customer a_customer = initialCustomer(aLine);
        inputs.push_back(a_customer);
    }
    read_inputs.close();

    // sort the inputs tasks by their arrive time
    sort(inputs.begin(),inputs.end(),compareArrivalTime);

    // start to work when there is customer left in the inputs queue
    while(!inputs.empty() || !(queue1.empty() && queue2.empty())) {
        works(inputs, queue1, queue2, result, running_time);
    }

    // print out the final results (result queue)
    if (debug) cout << endl << "========Outputs========="<< endl;
    output(result, false);

    return 0;
}
