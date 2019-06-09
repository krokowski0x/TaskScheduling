#include <cstdlib>
#include <fstream>
#include <queue>
#include <iostream>
#include <sstream>

#include "Task.h"

struct compareR {
    bool operator()(const Task& Task1, const Task& Task2) {
        return Task1.r > Task2.r;
    }
};

struct compareQ {
    bool operator()(const Task& Task1, const Task& Task2) {
        return Task1.q < Task2.q;
    }
};

class RPQ {
public:
    RPQ();
    virtual ~RPQ();
    int loadData(string fileName);
    int loadSchrData(string fileName);

private:
	Task piTable[100];
	
	void showpq(priority_queue <Task, vector<Task>, compareR> pq);
	void countC(int n, Task tasks[]);
	void countDelay(int n, Task tasks[]);
	void manageZero(int n, Task taskTable[]);
	void swapTasks(int task1Id, int task2Id, Task tasks[]);
	void upgrade(int n, int UB, int opt, Task tasks[]);
    int scheduleSchrage(int n, Task tasks[], Task piTable[]);
	int schedulePreSchrage(int n, Task tasks[], Task piTable[]);
	int countCmax(int n, Task tasks[]);
	void scheduleCarlier(int n, Task tasks[], Task piTable[]);
};
