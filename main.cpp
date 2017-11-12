#include <iostream>

#include "listLib.h"
#include "eventLib.h"
#include "dbLib.h"

using namespace std;


/// This function displays the content of database
void display(L1List<NinjaInfo_t>& nList) {
    cout.flush();
    nList.traverse(printNinjaInfo);
}

int main(int narg, char** argv) {
	
	clock_t start = clock();
   
	L1List<ninjaEvent_t>  eventList;
    L1List<NinjaInfo_t>   db;// database of NinjaInfo
    loadEvents(argv[1], eventList);
    loadNinjaDB(argv[2], db);

    cout << fixed << setprecision(12);// preset for floating point numbers
    /// Process events
    process(eventList, db);

    cout << resetiosflags(ios::showbase) << setprecision(-1);
	
	// caculate executing time (second)
	clock_t stop = clock();
	double elapsed{ static_cast<double>(stop - start) * 1000.0 / CLOCKS_PER_SEC };
	cout << elapsed << '\n';
    return 0;
}