// CS490HW4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <sstream>

using namespace std;

struct Process
{
	int processId;
	int arrivalTime;
	int serviceTime;
	int cpuTime;
	int completionTime; //Time that the process completed.
	int tat; //Turn around time
	double normTat; //Normalized turn around time
	int rotations; //The number of times that the process had to be recycled into the queue
	string length; //Short (1), Medium (2), Long (>2) corresponding to the number of rotations
};

void calculateIndStats(vector<Process> &finishedList);
//void calculateNormTat(vector<Process> &q);
void readInputFile(vector<int> &quantumTimes, vector<int> &serviceTimes, queue<Process> &processes);
void simulateRoundRobin(queue<Process> &processQueue, int quantum, vector<Process> *finishedList);

int main()
{
	void calculateTat(vector<Process> &p);
	vector<int> inputServiceTimes;
	vector<int> inputQuantumTimes;
	queue<Process> processQueue;

	readInputFile(inputQuantumTimes, inputServiceTimes, processQueue);

	vector<Process>* finishedList = new vector<Process>();

	for (int i = 0; i < (int)inputQuantumTimes.size(); i++) {
		simulateRoundRobin(processQueue, inputQuantumTimes.at(i), finishedList);
	}

	//for (int i = 0; i < finishedList->size(); i++) {
	//	cout << "Finished # " << finishedList->at(i).processId << endl;
	//}
	
	vector<Process>* sortedVec = new vector<Process>();
	Process init;
	init.processId = -1;
	//Initialize the sorted vector of processes to processes
	for (int i = 0; i < (int)finishedList->size(); i++) {
		sortedVec->push_back(init);
	}

	for (int i = 0; i < (int)finishedList->size(); i++) {
		sortedVec->at(finishedList->at(i).processId) = finishedList->at(i);
	}

	calculateIndStats(*sortedVec);

}

//Read in the process service times from the file
//and add the process to the end of the queue
void readInputFile(vector<int> &quantumTimes, vector<int> &serviceTimes, queue<Process> &processQueue) {
	int id = 0;
	fstream data("HWK4input.txt", ios_base::in);
	char* quantums = new char(10);
	
	data.getline(quantums, 10);
	int num;
	stringstream ss2;
	ss2 << quantums;
	while (ss2 >> num) {
		quantumTimes.push_back(num);
		cout << num << endl;
	}

	while (!data.eof()) {
		char* inputTime = new char(5);
		data.getline(inputTime, 5);
		int time;
		stringstream ss;
		ss << inputTime;
		while (ss >> time) {
			serviceTimes.push_back(time);
		}
	}

	for (int i = 0; i < (int)serviceTimes.size(); i++) {
		//Arrival time is??
		struct Process p { id++, 0, serviceTimes.at(i), 0 };
		cout << "Process #" << p.processId << " Service Time = " << p.serviceTime << endl;
		processQueue.push(p);
	}
}

void simulateRoundRobin(queue<Process>& processQueue, int quantum, vector<Process>* finishedList) {
	int clock = 0;

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
			p.completionTime = clock;
			finishedList->push_back(p);
		}
		else {
			processQueue.push(p);
		}
	}
}

void calculateIndStats(vector<Process> &finishedList) {
	for (int i = 0; i < (int)finishedList.size(); i++) {
		Process p = finishedList.at(i);
		p.tat = p.completionTime - p.arrivalTime;
		p.normTat = p.tat / (double)p.serviceTime;
		//cout << p.tat << endl;
	}
}


// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
