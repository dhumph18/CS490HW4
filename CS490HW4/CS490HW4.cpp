// CS490HW4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>
//#include <deque>
#include <vector>

using namespace std;

int main()
{
	struct Process
	{
		int processId;
		int arrivalTime;
		int serviceTime;
		int cpuTime;
	};

	int temp[] = { 2,3,4,7,7,6,13,16,11,10,3,5,4,2,6,7 }; // Remove, read in from file

	int id = 0;
	int clock = 0;
	int quantum = 7; //Change to be able to take in input from file
	queue<Process> processQueue;
	queue<Process> finishedQueue;

	//Read in the process service times from the file
	//and add the process to the end of the queue
	for (int i = 0; i < 15; i++) {
		//Arrival time is??
		struct Process p { id++, 0, temp[i], 0 };
		cout << "Process #" << p.processId << " Service Time = " << p.serviceTime << endl;
		processQueue.push(p);
	}

	while (!processQueue.empty()) {
		Process p = processQueue.front();
		processQueue.pop();
		int neededQuantum;
		int timeRemaining = p.serviceTime - p.cpuTime;

		if (timeRemaining >= quantum) {
			neededQuantum = quantum;
		}
		else {
			neededQuantum = timeRemaining;
		}

		p.cpuTime += neededQuantum;
		clock += neededQuantum;

		if (p.cpuTime == p.serviceTime) {
			finishedQueue.push(p);
		}
		else {
			processQueue.push(p);
		}
	}

}










// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
