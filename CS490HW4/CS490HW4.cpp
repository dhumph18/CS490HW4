// CS490HW4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//TODO List
// - Implement second quantum size
// - Round data to 2 decimal places
// - Document/Comment code
// - One page summary

#include <iostream>
#include <fstream>
#include <iomanip>
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
};

void calculateIndStats(vector<Process> &finishedList);
void readInputFile(vector<int> &quantumTimes, vector<int> &serviceTimes, queue<Process> &processes);
int simulateRoundRobin(queue<Process> &processQueue, int quantum, vector<Process> *finishedList);
void printIndStats(vector<Process> &finishedList, int quantum, bool firstOutput);
void printSystemStats(vector<Process>& finishedList, int quantum, int finalClock);

int main()
{
	void calculateTat(vector<Process> &p);
	vector<int> inputServiceTimes;
	vector<int> inputQuantumTimes;
	queue<Process> processQueue;
	bool firstOutput = true;

	readInputFile(inputQuantumTimes, inputServiceTimes, processQueue);

	//Need to later run this twice for each quantum size
	//for (int i = 0; i < (int)inputQuantumTimes.size(); i++) {
	vector<Process>* finishedList = new vector<Process>();

	
	//change to use i for quantum sizes
	int finalClock = simulateRoundRobin(processQueue, inputQuantumTimes.at(0), finishedList);

	calculateIndStats(*finishedList);
	printIndStats(*finishedList, inputQuantumTimes.at(0), firstOutput); //change to i
	printSystemStats(*finishedList, inputQuantumTimes.at(0), finalClock); //change to i
	firstOutput = false;
	//}
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
		struct Process p { id++, 0, serviceTimes.at(i), 0 };
		cout << "Process #" << p.processId << " Service Time = " << p.serviceTime << endl;
		processQueue.push(p);
	}
}

int simulateRoundRobin(queue<Process>& processQueue, int quantum, vector<Process>* finishedList) {
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
		p.rotations += 1;

		if (p.cpuTime == p.serviceTime) {
			p.completionTime = clock;
			finishedList->push_back(p);
		}
		else {
			processQueue.push(p);
		}
	}

	return clock;
}

void calculateIndStats(vector<Process> &finishedList) {
	for (int i = 0; i < (int)finishedList.size(); i++) {
		Process p = finishedList.at(i);
		p.tat = p.completionTime - p.arrivalTime;
		p.normTat = p.tat / (double)p.serviceTime;
		finishedList.at(i) = p;
		//cout << p.tat << endl;
	}
}

void printIndStats(vector<Process>& finishedList, int quantum, bool firstOutput) {
	ofstream output;
	if (firstOutput) {
		output.open("HWK4output.txt");
	}
	else {
		output.open("HWK4output.txt", ios_base::app);
	}
	
	output << "Results for first 15 processes to finish with quantum size = " << quantum << endl;
	output << setw(5) << "PID" << setw(15) << "Arrival Time" << setw(15) << "Service Time" << setw(20) << "Deptarture Time" << setw(10) << "TAT" << setw(13) << "Norm TAT" << endl;

	for (int i = 0; i < 15; i++) {
		Process p = finishedList.at(i);
		output << setw(5) << p.processId << setw(15) << p.arrivalTime << setw(15) << p.serviceTime << setw(20) << p.completionTime << setw(10) << p.tat << setw(13) << p.normTat << endl;

	}

	output.close();
}

void printSystemStats(vector<Process>& finishedList, int quantum, int finalClock) {

	int shortSum = 0;
	int medSum = 0;
	int longSum = 0;
	int shortTatSum = 0;
	int medTatSum = 0;
	int longTatSum = 0;
	int systemTatSum = 0;
	double shortNTatSum = 0;
	double medNTatSum = 0;
	double longNTatSum = 0;
	double systemNTatSum = 0;

	for (int i = 0; i < (int)finishedList.size(); i++) {
		Process p = finishedList.at(i);
		if (p.rotations == 1) {
			shortSum += 1;
			shortTatSum += p.tat;
			shortNTatSum += p.normTat;
		}
		else if (p.rotations == 2) {
			medSum += 1;
			medTatSum += p.tat;
			medNTatSum += p.normTat;
		}
		else {
			longSum += 1;
			longTatSum += p.tat;
			longNTatSum += p.normTat;
		}
		systemTatSum += p.tat;
		systemNTatSum += p.normTat;
	}

	double shortTatAvg = shortTatSum / (double)shortSum;
	double medTatAvg = medTatSum / (double)medSum;
	double longTatAvg = longTatSum / (double)longSum;
	double systemTatAvg = systemTatSum / (double)finishedList.size();

	double shortNTatAvg = shortNTatSum / (double)shortSum;
	double medNTatAvg = medNTatSum / (double)medSum;
	double longNTatAvg = longNTatSum / (double)longSum;
	double systemNTatAvg = systemNTatSum / (double)finishedList.size();

	ofstream output;
	output.open("HWK4output.txt", ios_base::app);

	output << "\nSummary Statistics for Quantum Size = " << quantum << endl;
	output << "Quantum = " << quantum << ", Number of clock ticks = " << finalClock << endl;
	output << setw(30) << "Short" << setw(15) << "Medium" << setw(15) << "Long" << setw(15) << "System" << endl;
	output << "Number of processes " << setw(10) << shortSum << setw(15) << medSum << setw(15) << longSum << setw(15) << finishedList.size() << endl;
	output << "Average TAT    " << setw(15) << shortTatAvg << setw(15) << medTatAvg << setw(15) << longTatAvg << setw(15) << systemTatAvg << endl;
	output << "Average NTAT   " << setw(15) << shortNTatAvg << setw(15) << medNTatAvg << setw(15) << longNTatAvg << setw(15) << systemNTatAvg << endl;
	output << "\n" << endl;
	output.close();
}

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
