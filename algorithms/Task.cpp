#include <cstdlib>
#include <iostream>

#include "Task.h"

Task& Task::operator=(const Task& task) {
    this->r = task.r;
    this->p = task.p;
    this->q = task.q;
    this->C = task.C;
    this->id = task.id;
    this->delay = task.delay;
    return * this;
}

Sum::Sum() {
    sum = 0;
    id = 0;
}

Sum::Sum(int _id, int _sum) {
    id = _id;
    sum = _sum;
}
    
Subtotal::Subtotal() {
    id = 0;
}
