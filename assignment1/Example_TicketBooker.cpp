/*
created by:Guanhua Wang, student2
ID:a1709743,   student2_Id(for undergraduate)
time:06.08.2020
Contact Email:
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
#include <queue>
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

void output()
{
    int i;
    cout<<"name   arrival   end   ready   running   waiting"<<endl;
    for(i=0; i<result.size(); i++) // every result
    {

    }
    cout<<endl;
}

int main(int argc,char *argv[])
{
    int i,j,k;
    freopen(argv[1],"r",stdin);

    initial();        // initial data
    input();          // input data
    works();          // process data
    output();         // output result
    return 0;
}
