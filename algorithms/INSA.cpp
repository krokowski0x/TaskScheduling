#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>
#include <unistd.h>

#include "INSA.h"
#include "Task.h"

using namespace std;

INSA::INSA() {}
INSA::~INSA() {}

void INSA::showpq(priority_queue <INSATask, vector<INSATask>, compareP> pq) {
    priority_queue <INSATask, vector<INSATask>, compareP> PQ = pq;
    while (!PQ.empty()) {
        cout << PQ.top().p << " " << PQ.top().machineNumber << " " << PQ.top().opNumber << " " << PQ.top().INSATaskNum << endl;
        PQ.pop();
    }
    cout << endl;
}

void INSA::fillRQZeros(int m, int n, int machine, int INSATask) {
    permutationTable[machine][INSATask].r = 0;
    permutationTable[machine][INSATask].q = 0;
}

int INSA::findPreOpNum(int opNum, int m, int n, int machine, int INSATask, int orderNum, int p) {
    int lastPreOp = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (permutationTable[i][j].opNumber == opNum && (machine != i || INSATask != j) && permutationTable[i][j].orderNum < orderNum && permutationTable[i][j].p) {
                if (lastPreOp < permutationTable[i][j].q && permutationTable[i][j].r >= p) {
                    lastPreOp =  permutationTable[i][j].q;
                }
            }
        }
    }
    return lastPreOp;
}

void INSA::establishRQ(int m, int n, int machine, int INSATask) {
    fillRQZeros(m, n, machine, INSATask);
    if (INSATask == 0 && permutationTable[machine][INSATask].p != 0) {
        permutationTable[machine][INSATask].r = findPreOpNum(permutationTable[machine][INSATask].opNumber, m, n, machine, INSATask, permutationTable[machine][INSATask].orderNum, permutationTable[machine][INSATask].p);
        permutationTable[machine][INSATask].q = permutationTable[machine][INSATask].r + permutationTable[machine][INSATask].p;
    } else if (permutationTable[machine][INSATask].p != 0) {
        permutationTable[machine][INSATask].r = max(permutationTable[machine][INSATask-1].q, findPreOpNum(permutationTable[machine][INSATask].opNumber, m, n, machine, INSATask, permutationTable[machine][INSATask].orderNum, permutationTable[machine][INSATask].p));
        permutationTable[machine][INSATask].q = permutationTable[machine][INSATask].r + permutationTable[machine][INSATask].p;
    }
}

int INSA::insertNextINSATask(int whichINSATask, int m, int n) {
    bool placeFound = false;
    int i = 0;
    INSATask current = INSAtasksSorted.top();
    INSAtasksSorted.pop();
    current.orderNum = whichINSATask;
    while (placeFound != true) {
        if (permutationTable[current.machineNumber-1][i].machineNumber == 0) {
            permutationTable[current.machineNumber-1][i] = current;
            establishRQ(m, n, current.machineNumber-1, i);
            placeFound = true;
        }
        i++;
    }
    return current.machineNumber;
}

int INSA::countCmax(int n, int m) {
    int current = 0;
    for (int i = 0; i < m + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            if (permutationTable[i][0].p != 0 && permutationTable[i][j].p == 0) {
                if (max(permutationTable[i][j-1].q,permutationTable[i][j-1].r) > current) {
                    current = max(permutationTable[i][j-1].q,permutationTable[i][j-1].r);
                }
            }
        }
    }
    return current;
}

void INSA::swapINSATasks(int id1, int id2, int m) {
    INSATask tmp;
    tmp = permutationTable[m][id1];
    permutationTable[m][id1] = permutationTable[m][id2];
    permutationTable[m][id2] = tmp;
}

void INSA::checkAllPositions(int m, int n, int machine) {
    int lastINSATask = -1, currentCmax, bestCmax = 9999999, bestPosition;
    bestCmax = countCmax(n, m);
    for (int i = n; i > 0; i--) {
        if (permutationTable[machine-1][i-1].p != 0) {
            lastINSATask = i-1;
            break;
        }
    }
    bestPosition = lastINSATask;
    if (lastINSATask != 0) {
        for (int k = lastINSATask; k > 0; k--) {
            swapINSATasks(k-1, k, machine-1);
            establishRQ(m,n, machine-1, k-1);
            establishRQ(m,n, machine-1, k);
            currentCmax = countCmax(n, m);
            if(currentCmax <= bestCmax) {
                bestCmax = currentCmax;
                bestPosition = k-1;
            }
        }
        for (int j = 0; j < bestPosition; j++) {
            swapINSATasks(j, j+1, machine-1);
            establishRQ(m, n, machine-1, j);
            establishRQ(m, n, machine-1, j+1);
        }
    }
}

int INSA::schedule(int n, int m) {
    int currentMachine, cmax;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            permutationTable[i][j].machineNumber = 0;
            permutationTable[i][j].p = 0;
            permutationTable[i][j].opNumber = 0;
            permutationTable[i][j].r = -1;
            permutationTable[i][j].q = -1;
        }
    }
    for (int i = 0 ; i < m * n; i++) {
        currentMachine = insertNextINSATask(i, m, n);
        checkAllPositions(m, n, currentMachine);
    }
    cmax = countCmax(n, m);
    return cmax;
}


int INSA::loadData(string fileName) {
    ifstream data;
    data.open(fileName);
    int n, m, INSATaskQuanity, Cmax, expectedCmax;
    char stop;
    string str, buf;
    string data1;
    string dataNames[81];
    int finalPermutation[100][100];
    
    for(int i = 0; i < 81; i++) {
    	auto start = chrono::steady_clock::now();
        // data.000
        getline(data,buf);
		cout << buf << endl;

        // 	4 3
        data >> n;
        data >> m;
        data >> INSATaskQuanity;
        
        INSATask* INSAtasks = new INSATask[INSATaskQuanity];
		
        int techNum = 0;
        // 	Data
        for (int j = 0; j < INSATaskQuanity; j++) {
            if (j % n == 0) {
                techNum++;
            }
            if (j % m == 0) {
                data >> str;
            }
            data >> INSAtasks[j].machineNumber >> INSAtasks[j].p;
            INSAtasks[j].opNumber = techNum;
            INSAtasks[j].INSATaskNum = j+1;
            INSAtasks[j].q = 0;
            INSAtasks[j].r = 0;
        }
        for (int j = 0; j < INSATaskQuanity; j++) {
            INSAtasksSorted.push(INSAtasks[j]);
        }

        // neh:
		getline(data, buf);
		getline(data, buf);
		getline(data, buf);

        // 32
		data >> expectedCmax;

        // 1 4 3 2
		for (int i = 0; i < m; ++i)
			getline(data, buf);	
		getline(data, buf);

        cout << "Calculated task order (Machine number first): " << endl;
        showpq(INSAtasksSorted);

        Cmax = schedule(n, m);
        cout << "Cmax: " << Cmax << endl;
        cout << "Expected Cmax: " << expectedCmax << endl;
        
    	auto end = chrono::steady_clock::now();
		cout << "Elapsed time: "  << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl << endl;
		
        getline(data, buf);
        delete[] INSAtasks;
        cout << "Enter - next dataset" << endl << "q - back to menu" << endl;
        stop = getchar();
        if (stop == 'q') break;
    }

	data.close();
	return 0;
}
