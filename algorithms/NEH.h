#include <cstdlib>
#include <fstream>
#include <queue>
#include <iostream>
#include <sstream>

class NEH {
public:
    NEH();
    virtual ~NEH();
    int loadData(std::string fileName);

private:
	int cmax;
	
	void Sort(int N, int M, int *tab, int *order);
	void Swap(int &a, int &b);
	int Cmax(int N,int M, int*tab,int *order, int *tabsort,int size);
	int* schedule(int N, int M, int*tab);
};
