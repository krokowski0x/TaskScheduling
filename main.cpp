#include <cstdlib>
#include <iostream>

#include "RPQ.h"
#include "WITI.h"
#include "NEH.h"
#include "INSA.h"

using namespace std;

int GetInput() {
  int choice;    
  cin >> choice;
  return choice;
}

void DisplayMainMenu() {
  cout << "Main Menu\n";
  cout << "Please chose, which scheduling\n";
  cout << "algorithm you want to test:\n";
  cout << "-------------\n";
  cout << "1 - RPQ\n";
  cout << "2 - WITI\n";
  cout << "3 - NEH\n";
  cout << "4 - SCHRAGE\n";
  cout << "5 - CARLIER\n";
  cout << "6 - INSA\n";
  cout << "0 - Quit\n";
  cout << "-------------\n";
  cout << "Your choice: ";
}

int main(int argc, char *argv[]) {
	RPQ rpq;
	WITI witi;
	NEH neh;
	INSA insa;
  int choice = 9;

	do {
	    DisplayMainMenu();
	    choice = GetInput();
	
	    switch(choice) {
	      case 1:
	        cout << "-------------\n";
	        cout << "RPQ:\n";
	        cout << "-------------\n";
	        rpq.loadData("rpq.data.txt");
	        break;
	      case 2:
	        cout << "-------------\n";
	        cout << "WITI:\n";
	        cout << "-------------\n";
	        witi.loadData("witi.data.txt");
	        break;
	      case 3:
	        cout << "-------------\n";
	        cout << "NEH:\n";
	        cout << "-------------\n";
	        neh.loadData("neh.data.txt");
	        break;
	      case 4:
	        cout << "-------------\n";
	        cout << "SCHRAGE:\n";
	        cout << "-------------\n";
			rpq.loadSchrData("schr.data.txt");
	        break;
	      case 5:
	        cout << "-------------\n";
	        cout << "CARLIER:\n";
	        cout << "-------------\n";
			rpq.loadSchrData("carl.data.txt");
	        break;
	      case 6:
	        cout << "-------------\n";
	        cout << "INSA:\n";
	        cout << "-------------\n";
	        insa.loadData("insa.data.txt");
	        break;    
	      case 0: 
	        cout << "Goodbye!\n";
	        break;
	      default: 
	      	DisplayMainMenu();
	        break;
	    }
	} while (choice != 0);
	
	system("PAUSE");
	return EXIT_SUCCESS;
}

