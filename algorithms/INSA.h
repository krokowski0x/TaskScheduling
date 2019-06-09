#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>
#include <unistd.h>

#include "Task.h"

struct compareP {
    bool operator()(const INSATask& INSATask1, const INSATask& INSATask2)
    {
        if (INSATask1.p > INSATask2.p || INSATask1.p == INSATask2.p && INSATask1.INSATaskNum < INSATask2.INSATaskNum) {
            return false;
        } else {
            return true;
        }
    }
};

class INSA {
public:
    INSA();
    virtual ~INSA();
    int loadData(string fileName);

private:
    INSATask permutationTable[100][100];
    priority_queue<INSATask, vector<INSATask>, compareP> INSAtasksSorted;
    
	void fillRQZeros(int m, int n, int machine, int INSATask);
	int findPreOpNum(int opNum, int m, int n, int machine, int INSATask, int orderNum, int p);
	void establishRQ(int m, int n, int machine, int INSATask);
	int insertNextINSATask(int whichINSATask, int m, int n);
	int countCmax(int n, int m);
	void swapINSATasks(int id1, int id2, int m);
	void checkAllPositions(int m, int n, int machine);
    void showpq(priority_queue <INSATask, vector<INSATask>, compareP> pq);
	int schedule(int n, int m);
};
