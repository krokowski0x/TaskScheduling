#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <algorithm>
#include "NEH.h"

using namespace std;

NEH::NEH() {}
NEH::~NEH() {}

int NEH::Cmax(int N, int M, int*tab, int *order, int *tabsort, int size) {
	int *x = new int[M+1];
	for (int i = 0; i < M + 1; ++i){
		x[i] = 0;
	}
	
	int k = 0;
	for (int j = 0; j < N; ++j){
		if (order[j] == 0) {
			for (int i=0; i < M; ++i) {
				tabsort[k*M+i]=0;
			}
		} else {
			for (int i = 0; i < M; ++i){
				tabsort[k*M+i] = tab[(order[j]-1)*M+i];	
			}
			k++;	
		}
	}

	for (int t = 0; t < size + 1; ++t){
		for (int i = 1 ; i< M + 1; ++i){
			x[i] = max( x[i], max(x[i-1] , x[i]) + tabsort[(i-1) + t*M]) ;
		}	 
	}
	return x[M] ;
}

void NEH::Swap(int &a, int &b) {
	int i = a;
	a = b;
	b = i;
}

void NEH::Sort(int N, int M, int *tab, int *order) {
	int *Suma = new int [N];
	for (int i = 0; i < N; ++i){
		Suma[i]=0;
	}
	int max = 0, ind = -1;
	for (int i = 0; i< N; i++) {
		for (int j = 0; j < M; j++) {
			Suma[i] = Suma[i] + tab[j+i*M];
		}
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (Suma[j] > max){
				max = Suma[j];
				ind = j;
			}
		}
		order[i] = ind + 1;
		Suma[ind] = -1;
		max = 0;
		ind = 0;
	}
}

int* NEH::schedule(int N, int M, int*tab) {
	int*order = new int[N];
	int*k1 = new int[N]; 
	int*k2 = new int[N]; 
	int size=1;  
	int m = 9999999;               
	for (int i = 0; i < N; i++){
		order[i]=0;
		k1[i] = 0;
		k2[i] = 0;
	}

	Sort(N,M,tab,order);  
	int*tabsort=new int [N*M];
	for (int i = 0; i < N*M; i++){
		tabsort[i]=0;
	}

	for (int i = 0; i < N; ++i) {
		if (i == 0) {
			k1[0] = order[0];
			k2[0] = order[0];
		} else {
			for (int z = size-1; z >= 0; --z){
				k1[z+1] = k1[z];
			}
			cmax = 999999999;
			k1[0] = order[i];

			for (int z = 0; z <= size; ++z){
				if (z == 0) {
					m = Cmax(N,M,tab,k1,tabsort,i);
					if (cmax > m) {
						cmax = m;
						for(int z = 0; z <= size; ++z)
							k2[z] = k1[z];
					}
				} else {
					Swap(k1[z-1],k1[z]);
					m = Cmax(N,M,tab,k1,tabsort,i);
					if (cmax > m) {
						cmax = m;
						for(int z = 0; z <= size; ++z)
							k2[z] = k1[z];
					}
				}
			}
			for (int z = 0; z <= size; ++z)
				k1[z] = k2[z];

			++size;
		}
	}
	return k1;
}

int NEH::loadData(string fileName) {
    int N,M;
    char stop;
    string buf;
    int expectedCmax;
    ifstream data(fileName);
    

    while(!data.eof()) {
    	//auto start = chrono::steady_clock::now();
		// data.000
		getline(data,buf);
		cout << buf << endl;

		// 	4 3
		data >> N >> M;
		int *tab = new int [N*M];
 		int *expectedOrder = new int[N];
		// 	Data
		for (int ind = 0 ; ind < N*M ; ++ind )
			data >> tab[ind];
		
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
		int* k1 = schedule(N, M, tab);
		for (int i = 0; i < N; ++i)
			cout << k1[i] << ", ";
		cout << endl;
		
		cout << "Expected Cmax = "<< expectedCmax << endl;
		cout << "Cmax = "<< cmax << endl << endl;
		
		//auto end = chrono::steady_clock::now();
		//cout << "Elapsed time: "  << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl << endl;
		
		getline(data, buf);
		delete[] tab;
		delete[] k1;
		delete[] expectedOrder;
		cout << "Enter - next dataset" << endl << "q - back to menu" << endl;
        stop = getchar();
        if (stop == 'q') break;
	}
	
	data.close();
	return 0;
}

