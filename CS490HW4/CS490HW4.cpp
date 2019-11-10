// CS490HW4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <queue>
//#include <deque>
#include <vector>
#include <sstream>


struct Process
{
	int processId;
	int arrivalTime;
	int serviceTime;
	int cpuTime;
};

//void calculateNormTat(vector<Process> &q);

using namespace std;

int main()
{
	void calculateTat(vector<Process> &p);
	vector<int>* quantumTimes = new vector<int>();
	vector<int> inputServiceTimes;
	vector<int> inputQuantumTimes;


	fstream data("HWK4input.txt", ios_base::in);

	//int time;
	//char test[] = "5 7";
	//stringstream ss;
	//ss << test;
	//while (ss >> time) {
	//	inputTimes.push_back(time);
	//}

	char* quantums = new char(10);
	
	
	data.getline(quantums, 10);
	int num;
	stringstream ss2;
	ss2 << quantums;
	while (ss2 >> num) {
		inputQuantumTimes.push_back(num);
		cout << num << endl;
	}
	

	while (!data.eof()) {
		char* serviceTimes = new char(5);
		data.getline(serviceTimes, 5);
		int time;
		stringstream ss;
		ss << serviceTimes;
		while (ss >> time) {
			inputServiceTimes.push_back(time);
		}
	}

	

	/*cout << "Input data:" << endl;
	for (int i = 0; i < (int)inputServiceTimes.size(); i++) {
		cout << inputServiceTimes.at(i) << " ";
	}
	cout << endl;*/

	int id = 0;
	int clock = 0;
	int quantum = 7; //Change to be able to take in input from file
	queue<Process> processQueue;
	vector<Process>* finishedList = new vector<Process>();

	//Read in the process service times from the file
	//and add the process to the end of the queue
	for (int i = 0; i < (int)inputServiceTimes.size(); i++) {
		//Arrival time is??
		struct Process p { id++, 0, inputServiceTimes.at(i), 0 };
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
			finishedList->push_back(p);
		}
		else {
			processQueue.push(p);
		}
	}

	vector<Process>* sortedVec = new vector<Process>();
	for (int i = 0; i < (int)finishedList->size(); i++) {
		sortedVec->at(finishedList->at(i).processId - 1) = finishedList->at(i);
	}

	calculateTat(*sortedVec);
	//calculateNormTat(*finishedList);

}

void calculateTat(vector<Process> &sortedVec) {
	vector<int> tats;
	for (int i = 0; i < (int)sortedVec.size(); i++) {
		Process p = sortedVec.at(i);
		tats.at(i) = p.cpuTime - p.arrivalTime;
		cout << tats.at(i) << endl;
	}
}

//void calculateNormTat(vector<Process> &finishedQueue) {
//
//}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
