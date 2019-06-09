#pragma once

#include <cstdlib>
#include <iostream>

using namespace std;

class Task {
public:
    int r, p, q, C, id, delay;
    Task& operator=(const Task& task);
};

class WITITask {
public:
    int p, w, d, C, T, id;
};

class INSATask {
public:
    int opNumber, machineNumber, p, r, q, INSATaskNum, orderNum;
};

class Sum {
public:
    int sum, id;
    Sum ();
    Sum (int _id, int _sum);
};

class Subtotal {
public:
    int  sumMin, sumNum, id;
    Sum sums[20];
    Subtotal();
};
