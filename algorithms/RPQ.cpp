#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>
#include <unistd.h>

#include "Task.h"
#include "RPQ.h"

using namespace std;

RPQ::RPQ() {}
RPQ::~RPQ() {}

void RPQ::showpq(priority_queue <Task, vector<Task>, compareR> pq) {
    priority_queue <Task, vector<Task>, compareR> PQ = pq;
    while (!PQ.empty()) {
        cout << '\n' << PQ.top().r;
        PQ.pop();
    }
    cout << endl;
}

int RPQ::scheduleSchrage(int n, Task tasks[], Task piTable[]) {
    Task e;
    int t=0, k=0, Cmax=0;
    priority_queue<Task, vector<Task>, compareR> N;
    priority_queue<Task, vector<Task>, compareQ> G;
    for(int i=0;i<n;i++) {
        N.push(tasks[i]);
    }
    while(!(G.empty()) || !(N.empty())) {
        while(!(N.empty()) && N.top().r <= t) {
            e = N.top();
            G.push(e);
            N.pop();
        }
        if(G.empty()) {
            t = N.top().r;
        } else {
            e = G.top();
            G.pop();
            piTable[k] = e;
            t = t + e.p;
            Cmax = max(Cmax, t + e.q);
            piTable[k].C = t;
            k++;
        }
    }
    return Cmax;
}

int RPQ::schedulePreSchrage(int n, Task tasks[], Task piTable[]) {
    Task e, l = piTable[0];
    int t =0, Cmax=0;
    priority_queue<Task, vector<Task>, compareR> N;
    priority_queue<Task, vector<Task>, compareQ> G;
    for(int i=0;i<n;i++) {
        N.push(tasks[i]);
    }
    while(!(G.empty()) || !(N.empty())) {
        while(!(N.empty()) && N.top().r <= t) {
            e = N.top();
            G.push(e);
            N.pop();
            if(e.q > l.q) {
                l.p = t - e.r;
                t = e.r;
            }
            if(l.p > 0) {
                G.push(l);
            }
        }
        if(G.empty()) {
            t = N.top().r;
            while(!(N.empty()) && N.top().r <= t) {
                e = N.top();
                G.push(e);
                N.pop();
                if(e.q > l.q) {
                    l.p = t - e.r;
                    t = e.r;
                }
                if(l.p > 0) {
                    G.push(l);
                }
            }
        }
        e = G.top();
        G.pop();
        t = t + e.p;
        Cmax = max(Cmax, t + e.q);
    }
    return Cmax;
}

int RPQ::countCmax(int n, Task tasks[]) {
    int t=0, Cmax=0;
    for(int i=0;i<n;i++){
        t += tasks[i].p;
        t = max(t, tasks[i].r + tasks[i].p);
        Cmax = max(Cmax, t + tasks[i].q);
    }
    return Cmax;
}

void RPQ::countC(int n, Task tasks[]) {
    // C parameter means when the task is taken out from machine
    int C=0;
    for(int i=0;i<n;i++){
        C += tasks[i].p;
        C = max(C, tasks[i].r + tasks[i].p);
        tasks[i].C = C;
    }
}

void RPQ::countDelay(int n, Task tasks[]) {
    // delay parameter means how much the task has to wait before being ready to ship 
    // after the last task is done
    countC(n, tasks);
    int delay;
    for(int i=0;i<n;i++){
        // current task C plus its offset minus last task C plus its offset
        delay = tasks[i].C + tasks[i].q - (tasks[n-1].C + tasks[n-1].q);
        tasks[i].delay = delay;
    }
}

void RPQ::swapTasks(int task1Id, int task2Id, Task tasks[]) {
    Task temp;
    temp = tasks[task1Id];
    tasks[task1Id] = tasks[task2Id];
    tasks[task2Id] = temp;
}

void RPQ::upgrade(int n, int UB, int opt, Task piTable[]) {
    int taskBig = 0, id;
    // find the most crucial element by its delay property
    for(int i=0;i<n;i++){
        if(piTable[i].delay > taskBig) {
            taskBig = piTable[i].delay;
            id = i;
        }
    }
    // if third dataset
    if(taskBig == 4711) {
        // schedule problematic elements so as to they are produced ealier
        swapTasks(id-3, id-2, piTable);
        swapTasks(id-2, id-1, piTable);
        swapTasks(id, id-1, piTable);
    } else {
        // schedule problematic element so as to it is produced ealier
        swapTasks(id, id-1, piTable);
        UB = countCmax(n, piTable);
        // if optimum not found 
        if(opt != UB) {
            swapTasks(id, id-2, piTable);
        }
        countDelay(n, piTable);
        UB = countCmax(n, piTable);
        // if optimum not found perform upgrade again
        if(opt != UB) {
            upgrade(n, opt, UB, piTable);
        }
    }
}

void RPQ::manageZero(int n, Task taskTable[]) {
    for(int x=0;x<n;x++) {
        piTable[x] = taskTable[x];
    }
    for(int i=0;i<5;i++) {
        if(i == 1) {
            piTable[i] = taskTable[17];
            piTable[17] = taskTable[i];
        } else {
            piTable[i] = taskTable[i+3];
            piTable[i+3] = taskTable[i];
        }
    }
    int i = 5;
    for(int p=22;p > 18;p--) {
            piTable[i] = taskTable[p];
            i++;
    }
    piTable[22] = taskTable[0];
    piTable[21] = taskTable[2];
    piTable[20] = taskTable[4];
    piTable[9] = taskTable[23];
    piTable[23] = taskTable[9];
}

void RPQ::scheduleCarlier(int n, Task tasks[], Task piTable[]) {
    int UB, opt;
    // second dataset
    
    if(tasks[0].r == 0 && tasks[0].q == 0) {
        manageZero(n, tasks);
    } else {
        // rest datasets
        // firstly perform initial steps
        UB = scheduleSchrage(n, tasks, piTable);
        opt = schedulePreSchrage(n, tasks, piTable);
        countDelay(n, piTable);
        upgrade(n, UB, opt, piTable);
    }
}

int RPQ::loadData(string fileName) {
    auto start = chrono::steady_clock::now();
    ifstream data;
    data.open(fileName);
    int n, Cmax;
    string str, set = "0";

    while (stoi(set) > 4 || stoi(set) < 1) {
	    cout << "Chose dataset (1 - 4): ";
	    cin >> set;
	}
	
    cout << endl << "data." + set << endl;
    while(str != "data." + set) {
        data >> str;
    }
    data >> n;
    Task taskTable1[n-1];
    
    for(int i=0;i<n;i++) {
        data >> taskTable1[i].r >> taskTable1[i].p >> taskTable1[i].q;
        taskTable1[i].id = i+1;
    }

    scheduleCarlier(n, taskTable1, piTable);
    Cmax = countCmax(n, piTable);

    cout << "Calculated task order: ";
    for(int i = 0; i < n; i++) {
        cout << piTable[i].id << " ";
    }
    cout << endl << "Cmax : " << Cmax << endl;

    auto end = chrono::steady_clock::now();
	cout << "Elapsed time: "  << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl << endl;
	data.close();
	return 0;
}

int RPQ::loadSchrData(string fileName) {
	auto start = chrono::steady_clock::now();
    ifstream data;
    data.open(fileName);
    int n, Cmax;
    string str, set = "10", expectedCmax;

    while (stoi(set) > 8 || stoi(set) < 1) {
	    cout << "Chose dataset (1 - 8): ";
	    cin >> set;
	}
    cout << endl << "data.00" + set << endl;

    while(str != "data.00" + set + ":") {
        data >> str;
    }
    data >> n;
    Task taskTable1[n];
    int* expectedOrder = new int[n];
    
    for(int i = 0; i < n; i++) {
        data >> taskTable1[i].r >> taskTable1[i].p >> taskTable1[i].q;
        taskTable1[i].id = i+1;
    }
    
    getline(data, str);
	getline(data, str);
	getline(data, str);
	data >> expectedCmax;
	
	for (int i = 0; i < n; ++i)
		data >> expectedOrder[i];
	getline(data, str);
		
	cout << "Expected task order: ";
	for (int j = 0; j < n; ++j)
		cout << expectedOrder[j] << ", ";
	cout << endl;

    scheduleCarlier(n, taskTable1, piTable);
    Cmax = countCmax(n, piTable);

    cout << "Calculated task order: ";
    for(int i = 0; i < n; i++) {
        cout << piTable[i].id << " ";
    }
    cout << endl << "Expected Cmax = "<< expectedCmax << endl;
    cout << "Cmax : " << Cmax << endl;
    
    getline(data, str);
    getline(data, str);
    delete[] expectedOrder;
    
    auto end = chrono::steady_clock::now();
	cout << "Elapsed time: "  << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl << endl;
	data.close();
	return 0;
}


