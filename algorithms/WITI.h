#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Task.h"

class WITI {
public:
    WITI();
    virtual ~WITI();
    int loadData(string fileName);
    
private:
    int OPT;
    int N;
    int* schedule(int n, WITITask tasks[]);
    int log2(int number);
};
