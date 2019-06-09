#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
#include <chrono>

#include "WITI.h"

using namespace std;

WITI::WITI() {}
WITI::~WITI() {}

int WITI::log2(int number) {
    // Int has 32 bits so we can safely go as far as 2**31
    int tmp = 0, id;
    for(int i=0;i<30;i++) {
        tmp = pow(2, i);
        if (tmp == number) {
            id = i;
            break;
        }
    }
    return id;
}

int* WITI::schedule(int n, WITITask tasks[]) {
    int* order = new int [n-1];
    int initialSumNum = n, initialId = 0, elem;
    int C, N = 1 << n, stepsCounter = 0, current;
    Subtotal * steps = new Subtotal[N];
    steps[0].sumMin = 0;
    steps[0].id = 0;
    for (int i = 0, b = 1; i < n; i++, b *= 2) {
        initialId += b;
    }
    for (int set = 1; set < N; set++) {
        C = 0;
        for (int i = 0, b = 1; i < n; i++, b *= 2) {
            if (b&set) {
                C += tasks[i].p;
            }
        }
        steps[set].sumMin = 999999999;
        stepsCounter = 0;
        for (int i = 0, b = 1; i < n; i++, b *= 2) {
            if (b&set) {
                steps[set].id += b; 
                steps[set].sums[stepsCounter].id = set-b;
                steps[set].sums[stepsCounter].sum = steps[set-b].sumMin + tasks[i].w * max(0, C - tasks[i].d);
                steps[set].sumMin = min(steps[set].sumMin, steps[set-b].sumMin + tasks[i].w * max(0, C - tasks[i].d));
                stepsCounter++;
            }
        }
        steps[set].sumNum = stepsCounter; 
    }  
    int parsedOrder = steps[N-1].id, tmp = 0;
    for (int i = n-1; i >= 0; i--) {
        for (int x = N-1; x >= 0; x--) {
            current = 999999999;
            if (initialId == steps[x].id && initialSumNum == steps[x].sumNum) {
                for (int id = 0; id < steps[x].sumNum; id++) {
                    if (steps[x].sums[id].sum <= current) {
                        current = steps[x].sums[id].sum;
                        elem = steps[x].sums[id].id;
                    }
                }
                tmp = parsedOrder - elem;
                parsedOrder = elem;
                order[i] = log2(tmp) + 1;
                initialSumNum--;
                initialId = elem;
                break;
            }
        }
    }
    OPT = steps[N-1].sumMin;
    delete [] steps;
    return order;
}

int WITI::loadData(string fileName) {   
    int N, expectedCmax;
    string str, buf;
    char stop;
    ifstream data(fileName);
    
    while(!data.eof()) {
    	//auto start = chrono::steady_clock::now();
    	
        // data.000
		getline(data,buf);
		cout << buf << endl;

        // 	4
        data >> N;
        WITITask *tasks = new WITITask[N];
        int* expectedOrder = new int[N];
        
        // 	Data
        for(int i = 0; i < N; i++) {
            data >> tasks[i].p >> tasks[i].w >> tasks[i].d;
            tasks[i].id = i+1;
        } 

        // neh:
		getline(data, buf);
		getline(data, buf);
		getline(data, buf);

        // 32
		data >> expectedCmax;

        // 1 4 3 2
		for (int i = 0; i < N; ++i)
			data >> expectedOrder[i];
		
        getline(data, buf);
		
		cout << "Expected task order: ";
		for (int j = 0; j < N; ++j)
			cout << expectedOrder[j] << ", ";
		cout << endl;

		cout << "Calculated task order:";
		int* order = schedule(N, tasks);;
		for (int i = 0; i < N; ++i)
			cout << order[i] << ", ";
		cout << endl;
        
        cout << "Expected Cmax = "<< expectedCmax << endl;
        cout << "Cmax = "<< OPT << endl << endl;
        
        //auto end = chrono::steady_clock::now();
		//cout << "Elapsed time: "  << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl << endl;
		
        getline(data, buf);
        delete[] tasks;
        delete[] expectedOrder;
        cout << "Enter - next dataset" << endl << "q - back to menu" << endl;
        stop = getchar();
        if (stop == 'q') break;
	}
	
	data.close();
	return 0;
}
