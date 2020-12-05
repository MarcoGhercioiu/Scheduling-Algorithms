/*
 File: main_mjg170001.cpp
 Author: Vlad
 Procedures:
 main 				- creates a default service time and arrival time array, creates service time, arrival time, turn around time,
 				and finish time arrays for each scheduling algorithm, then calls each scheduling algorithm. Prints out
 				data collected.
 createProcesses 		- creates a randomly generated, normally distributed data set.
 copyArr 			- copy array
 printArray	 		- print formatted data from array
 printResults	 		- print out service time, arrival time, turn around time, and finish time arrays formatted
 process 			- simulates process by incrementing total time
 FCFS 				- run first-come-first-serve scheduling algorithm
 RR					- run round robin scheduling algorithm
 HRRN				- run highest response ratio next algorithm
 FB					- run feedback algorithm
 comparisonVec			- creates vector with metrics calculated

 */

#include <time.h>
#include <iostream>
#include <random>
#include <unistd.h>
#include <vector>

// total number of processes
#define numProc 1000
// total number of tests
#define numTest 1000

using namespace std;

/*
 void createProcesses(int processes[], int increment)
 Author: Vlad
 Date: 11/7/2020
 Description: generate random data set of normally distributed numbers

 Parameters:
 processes			I/P		int[]	array holding data set
 increment			I/P		int		increment for seed to produce different data each call
 createProcesses		O/P		void	status code
 */

void createProcesses(int processes[], int increment) {
	// generator needed to seed normal distribution
	default_random_engine generator;
	// create normal distribution with mean of 10 and standard deviation of 5
	normal_distribution<> distribution(10, 5);
	// seed generator
	generator.seed(time(NULL) + increment);
	// add bounds to distribution
	for (int i = 0; i < numProc; ++i) {
		int number = distribution(generator);
		if ((number < 1) || (number > 40)) {
			i--;
			continue;
		}
		processes[i] = number;
	}
}

/*
 void copyArr(int orig[], int dest[])
 Author: Vlad
 Date: 11/7/2020
 Description: copy original array into new array

 Parameters:
 orig				I/P		int[]	original array
 dest				I/P		int[]	new array
 copyArr			O/P		void	status code
 */

void copyArr(int orig[], int dest[]) {
	for (int i = 0; i < numProc; i++) {
		dest[i] = orig[i];
	}
}

/*
 void printArray(int arr[])
 Author: Vlad
 Date: 11/7/2020
 Description: print array formatted

 Parameters:
 arr			I/P		int[]	original array
 printArray		O/P		void	status code
 */

void printArray(int arr[]) {
	for (int i = 0; i < numProc; i++) {
		cout << "Index " << i << " = " << arr[i] << endl;
	}
}

/*
 void printResults(int servT[], int arrvT[], int finiT[], int turnT[])
 Author: Vlad
 Date: 11/7/2020
 Description: print out service time, arrival time, turn around time, and finish time arrays formatted

 Parameters:
 servT			I/P		int[]	service time array
 arrvT			I/P		int[]	arrival time array
 finiT			I/P		int[]	finish time array
 turnT			I/P		int[]	turnaround time array
 printResults		O/P		void	status code
 */

void printResults(int servT[], int arrvT[], int finiT[], int turnT[]) {
	cout
			<< "Process\t\tService Time\tArrival Time\tFinish Time\tTurnaround Time"
			<< endl;
	for (int i = 0; i < numProc; i++) {
		cout << "Process " << i << ":\t   " << servT[i] << "\t\t   " << arrvT[i]
				<< "\t\t   " << finiT[i] << "\t\t   " << turnT[i] << endl;
	}
}

/*
 int process(int time)
 Author: Vlad
 Date: 11/7/2020
 Description: print out service time, arrival time, turn around time, and finish time arrays formatted

 Parameters:
 time			I/P		int		total time
 process		O/P		int	status code
 */

int process(int time) {
	return (++time);
}

/*
 void FCFS(int serviceTime[], int arrivalTime[], int finishTime[], int turnaroundTime[])
 Author: Vlad
 Date: 11/7/2020
 Description: implement first-come-first-serve algorithm

 Parameters:
 serviceTime			I/P		int[]	service time array
 arrivalTime			I/P		int[]	arrival time array
 finishTime			I/P		int[]	finish time array
 turnaroundTime			I/P		int[]	turnaround time array
 FCFS				O/P		void	status code
 */

void FCFS(int serviceTime[], int arrivalTime[], int finishTime[],
		int turnaroundTime[]) {
// array to hold updated service times when processes execute
	int currServiceTime[numProc];
	for (int i = 0; i < numProc; i++)
		currServiceTime[i] = -1;
	// load in first process
	currServiceTime[0] = serviceTime[0];
	arrivalTime[0] = 0;

	bool procFinished[numProc] = { false };
	bool allProcFinished = false;

	int totalTime = 0;

	while (!allProcFinished) {
		// loop through all processes
		for (int i = 0; i < numProc; i++) {
			// compute process to completion
			while (currServiceTime[i] > 0) {
				currServiceTime[i]--;
				totalTime = process(totalTime);

				int newPrcIndex = 0;

				// find next available spot for new process
				while (currServiceTime[newPrcIndex] >= 0
						&& newPrcIndex < numProc) {
					newPrcIndex++;
				}
				// load in new process
				if (newPrcIndex < numProc) {
					currServiceTime[newPrcIndex] = serviceTime[newPrcIndex];
					arrivalTime[newPrcIndex] = totalTime;
				}
			}
			finishTime[i] = totalTime;
			procFinished[i] = true;
		}
		allProcFinished = true;
		for (int i = 0; i < numProc; i++) {
			if (procFinished[i] == false)
				allProcFinished = false;
		}

	}
	// calculate turn around time
	for (int i = 0; i < numProc; i++) {
		turnaroundTime[i] = finishTime[i] - arrivalTime[i];
	}

}

/*
 void RR(int serviceTime[], int arrivalTime[], int finishTime[], int turnaroundTime[])
 Author: Vlad
 Date: 11/7/2020
 Description: implement round robin algorithm

 Parameters:
 serviceTime			I/P		int[]	service time array
 arrivalTime			I/P		int[]	arrival time array
 finishTime			I/P		int[]	finish time array
 turnaroundTime			I/P		int[]	turnaround time array
 RR						O/P		void	status code
 */

void RR(int serviceTime[], int arrivalTime[], int finishTime[],
		int turnaroundTime[]) {

	int currServiceTime[numProc];
	for (int i = 0; i < numProc; i++)
		currServiceTime[i] = -1;
	currServiceTime[0] = serviceTime[0];
	arrivalTime[0] = 0;

	bool procFinished[numProc] = { false };
	bool allProcFinished = false;

	int totalTime = 0;

	while (!allProcFinished) {
		// loop through all processes
		for (int i = 0; i < numProc; i++) {
			// compute one time slice of process
			if (!procFinished[i]) {
				currServiceTime[i]--;
				totalTime = process(totalTime);

				int newPrcIndex = 0;
				// find next available location for process
				while (currServiceTime[newPrcIndex] >= 0
						&& newPrcIndex < numProc) {
					newPrcIndex++;
				}
				// load in new process
				if (newPrcIndex < numProc) {
					currServiceTime[newPrcIndex] = serviceTime[newPrcIndex];
					arrivalTime[newPrcIndex] = totalTime;
				}

				if (currServiceTime[i] == 0) {
					finishTime[i] = totalTime;
					procFinished[i] = true;
				}

			}

		}
		allProcFinished = true;
		for (int i = 0; i < numProc; i++) {
			if (procFinished[i] == false)
				allProcFinished = false;
		}

	}

	for (int i = 0; i < numProc; i++) {
		turnaroundTime[i] = finishTime[i] - arrivalTime[i];
	}

}

/*
 void HRRN(int serviceTime[], int arrivalTime[], int finishTime[], int turnaroundTime[])
 Author: Vlad
 Date: 11/7/2020
 Description: implement highest response ratio next algorithm

 Parameters:
 serviceTime			I/P		int[]	service time array
 arrivalTime			I/P		int[]	arrival time array
 finishTime			I/P		int[]	finish time array
 turnaroundTime			I/P		int[]	turnaround time array
 HRRN				O/P		void	status code
 */

void HRRN(int serviceTime[], int arrivalTime[], int finishTime[],
		int turnaroundTime[]) {

	int currServiceTime[numProc];
	for (int i = 0; i < numProc; i++)
		currServiceTime[i] = -1;
	currServiceTime[0] = serviceTime[0];
	arrivalTime[0] = 0;

	bool procFinished[numProc] = { false };
	bool allProcFinished = false;

	int totalTime = 0;

	while (!allProcFinished) {

		double biggestRR = -1;
		int indexBiggestRR = -1;
		// calculate response ratio and save the highest ratio location
		for (int i = 0; i < numProc; i++) {
			if (!procFinished[i] && currServiceTime[i] > 0) {
				double currentRR = ((totalTime - arrivalTime[i])
						+ currServiceTime[i]) / currServiceTime[i];

				if (currentRR > biggestRR) {
					biggestRR = currentRR;
					indexBiggestRR = i;
				}
			}
		}

		// compute process with highest response ratio
		while (currServiceTime[indexBiggestRR] > 0) {
			currServiceTime[indexBiggestRR]--;
			totalTime = process(totalTime);

			int newPrcIndex = 0;
			// find next available location for process
			while (currServiceTime[newPrcIndex] >= 0 && newPrcIndex < numProc) {
				newPrcIndex++;
			}
			if (newPrcIndex < numProc) {
				currServiceTime[newPrcIndex] = serviceTime[newPrcIndex];
				arrivalTime[newPrcIndex] = totalTime;
			}
		}
		// update total time
		finishTime[indexBiggestRR] = totalTime;
		procFinished[indexBiggestRR] = true;

		allProcFinished = true;
		for (int i = 0; i < numProc; i++) {
			if (procFinished[i] == false)
				allProcFinished = false;
		}

	}

	for (int i = 0; i < numProc; i++) {
		turnaroundTime[i] = finishTime[i] - arrivalTime[i];
	}

}

/*
 void FB(int serviceTime[], int arrivalTime[], int finishTime[], int turnaroundTime[])
 Author: Vlad
 Date: 11/7/2020
 Description: implement feedback algorithm

 Parameters:
 serviceTime			I/P		int[]	service time array
 arrivalTime			I/P		int[]	arrival time array
 finishTime			I/P		int[]	finish time array
 turnaroundTime			I/P		int[]	turnaround time array
 FB				O/P		void	status code
 */

void FB(int serviceTime[], int arrivalTime[], int finishTime[],
		int turnaroundTime[]) {

	int currServiceTime[numProc];
	for (int i = 0; i < numProc; i++)
		currServiceTime[i] = -1;
	currServiceTime[0] = serviceTime[0];
	arrivalTime[0] = 0;
// create queues
	int q1[numProc];
	int q2[numProc];
	int q3[numProc];

	for (int i = 0; i < numProc; i++) {
		q1[i] = -1;
		q2[i] = -1;
		q3[i] = -1;
	}

	bool procFinished[numProc] = { false };
	bool allProcFinished = false;

	int totalTime = 0;

	q1[0] = 0;

	while (!allProcFinished) {
// boolean values to determine if queue is empty, used to find which queue saved index is in
		bool q1Empty = true;
		bool q2Empty = true;
		bool q3Empty = true;
		int indexToProcess = -1;
// index of process in queue
		int indexToQ = -1;
// find index of next process to execute
		for (int i = 0; i < numProc; i++) {
			if (q1[i] != -1) {
				indexToProcess = q1[i];
				indexToQ = i;
				q1Empty = false;
				i = numProc;
			}
		}
		if (q1Empty) {
			for (int i = 0; i < numProc; i++) {
				if (q2[i] != -1) {
					indexToProcess = q2[i];
					indexToQ = i;
					q2Empty = false;
					i = numProc;
				}
			}
			if (q2Empty) {
				for (int i = 0; i < numProc; i++) {
					if (q3[i] != -1) {
						indexToProcess = q3[i];
						indexToQ = i;
						q3Empty = false;
						i = numProc;
					}
				}
			}
		}

// compute process
		currServiceTime[indexToProcess]--;
		totalTime = process(totalTime);

		int newPrcIndex = 0;
// load in new process
		if (totalTime % 2 == 0) {
			while (currServiceTime[newPrcIndex] >= 0 && newPrcIndex < numProc) {
				newPrcIndex++;
			}
			// insert new process into queue 1
			if (newPrcIndex < numProc) {
				currServiceTime[newPrcIndex] = serviceTime[newPrcIndex];
				arrivalTime[newPrcIndex] = totalTime;
				for (int i = 0; i < numProc; i++) {
					if (q1[i] == -1) {
						q1[i] = newPrcIndex;
						i = numProc;
					}
				}
			}
		}
		// remove process from queue when completed
		if (currServiceTime[indexToProcess] == 0) {
			finishTime[indexToProcess] = totalTime;
			procFinished[indexToProcess] = true;
			if (q1Empty == false) {
				q1[indexToQ] = -1;
			} else if (q2Empty == false) {
				q2[indexToQ] = -1;

			} else {
				q3[indexToQ] = -1;
			}

		} else {
			// find next process to execute
			if (q1Empty == false) {
				int location = q1[indexToQ];
				q1[indexToQ] = -1;
				for (int i = 0; i < numProc; i++) {
					if (q2[i] == -1) {
						q2[i] = location;
						i = numProc;
					}
				}
			} else if (q2Empty == false) {
				int location = q2[indexToQ];
				q2[indexToQ] = -1;
				for (int i = 0; i < numProc; i++) {
					if (q3[i] == -1) {
						q3[i] = location;
						i = numProc;
					}
				}
			}
		}

		allProcFinished = true;
		for (int i = 0; i < numProc; i++) {
			if (procFinished[i] == false)
				allProcFinished = false;
		}

	}

	for (int i = 0; i < numProc; i++) {
		turnaroundTime[i] = finishTime[i] - arrivalTime[i];
	}

}

/*
 vector<double> comparisonVec(int turnaroundTime[], int serviceTime[])
 Author: Vlad
 Date: 11/7/2020
 Description: creates a vector containing the average turnaround time and the average of turn around time divided by service time

 Parameters:
 turnaroundTime			I/P		int[]			turnaround time array
 serviceTime			I/P		int[]			service time array
 comparisonVec			O/P		vector<double>		status code
 */

vector<double> comparisonVec(int turnaroundTime[], int serviceTime[]) {

	vector<double> temp;

	double FCFSttTotal = 0;
	double FCFSTrTs = 0;
	// calculate total turn around time and turn around/service time
	for (int j = 0; j < numProc; j++) {
		FCFSttTotal += turnaroundTime[j];
		FCFSTrTs += (turnaroundTime[j] / serviceTime[j]);
	}
	// add averages to vector
	temp.push_back((FCFSttTotal / numProc));
	temp.push_back((FCFSTrTs / numProc));

	return temp;
}

/*
 int main()
 Author: Vlad
 Date: 11/7/2020
 Description: creates a default service time and arrival time array, creates service time, arrival time, turn around time,
 and finish time arrays for each scheduling algorithm, then calls each scheduling algorithm. Prints out
 data collected.

 Parameters:
 main		O/P		int		status code
 */

int main() {
	vector<vector<double>> data;

	int increment = 0;

	// run multiple tests
	for (int i = 0; i < numTest; i++) {

		cout << increment << endl;

		int serviceTime[numProc];
		int arrivalTime[numProc];
		for (int i = 0; i < numProc; i++)
			arrivalTime[i] = -1;
		int finishTime[numProc];
		int turnaroundTime[numProc];

		// generate data set
		createProcesses(serviceTime, increment++);

		// create array copies for each scheduling algorithm
		int serviceTimeFCFS[numProc];
		int arrivalTimeFCFS[numProc];
		int finishTimeFCFS[numProc];
		int turnaroundTimeFCFS[numProc];

		copyArr(serviceTime, serviceTimeFCFS);
		copyArr(arrivalTime, arrivalTimeFCFS);

		int serviceTimeRR[numProc];
		int arrivalTimeRR[numProc];
		int finishTimeRR[numProc];
		int turnaroundTimeRR[numProc];

		copyArr(serviceTime, serviceTimeRR);
		copyArr(arrivalTime, arrivalTimeRR);

		int serviceTimeHRRN[numProc];
		int arrivalTimeHRRN[numProc] = { 0 };
		int finishTimeHRRN[numProc];
		int turnaroundTimeHRRN[numProc];

		copyArr(serviceTime, serviceTimeHRRN);
		copyArr(arrivalTime, arrivalTimeHRRN);

		int serviceTimeFB[numProc];
		int arrivalTimeFB[numProc];
		int finishTimeFB[numProc];
		int turnaroundTimeFB[numProc];

		copyArr(serviceTime, serviceTimeFB);
		copyArr(arrivalTime, arrivalTimeFB);

		vector<double> temp;

		// run scheduling algorithms on data set

		FCFS(serviceTimeFCFS, arrivalTimeFCFS, finishTimeFCFS,
				turnaroundTimeFCFS);
		//cout << "first in first out" << endl;
		//printResults(serviceTimeFCFS, arrivalTimeFCFS, finishTimeFCFS,
		//		turnaroundTimeFCFS);

		data.push_back(comparisonVec(turnaroundTimeFCFS, serviceTimeFCFS));

		RR(serviceTimeRR, arrivalTimeRR, finishTimeRR, turnaroundTimeRR);
		//cout << "Round robin" << endl;
		//printResults(serviceTimeRR, arrivalTimeRR, finishTimeRR,
		//		turnaroundTimeRR);

		data.push_back(comparisonVec(turnaroundTimeRR, serviceTimeRR));

		HRRN(serviceTimeHRRN, arrivalTimeHRRN, finishTimeHRRN,
				turnaroundTimeHRRN);
		//cout << "Highest ratio next" << endl;
		//printResults(serviceTimeHRRN, arrivalTimeHRRN, finishTimeHRRN,
		//		turnaroundTimeHRRN);

		data.push_back(comparisonVec(turnaroundTimeHRRN, serviceTimeHRRN));

		FB(serviceTimeFB, arrivalTimeFB, finishTimeFB, turnaroundTimeFB);
		//cout << "feedback" << endl;
		//printResults(serviceTimeFB, arrivalTimeFB, finishTimeFB,
		//		turnaroundTimeFB);

		// add statistics on performance to vector
		data.push_back(comparisonVec(turnaroundTimeFB, serviceTimeFB));

	}

	// create array to store final performance statistics
	double valueData[4][2];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			valueData[i][j] = 0;
		}
	}

// add data to total array
	for (int i = 0; i < (4 * numTest); i++) {
		for (int j = 0; j < 2; j++) {
			if (i % 4 == 0) {
				valueData[0][j] += data[i][j];
			} else if (i % 4 == 1) {
				valueData[1][j] += data[i][j];
			} else if (i % 4 == 2) {
				valueData[2][j] += data[i][j];
			} else if (i % 4 == 3) {
				valueData[3][j] += data[i][j];
			}
		}
	}

	cout << endl;
	for (int i = 0; i < 4; i++) {
		if (i == 0)
			cout << "FCFS" << endl;
		if (i == 1)
			cout << "RR" << endl;
		if (i == 2)
			cout << "HRRN" << endl;
		else if (i == 3)
			cout << "FB" << endl;

		for (int j = 0; j < 2; j++) {
			// calculate average across number of tests
			valueData[i][j] = valueData[i][j] / numTest;

			if (j == 0) {
				cout << "Turnaround time Mean: 			" << valueData[i][j]
						<< endl;
			} else {
				cout << "(Turnaround time / Service Time) Mean: "
						<< valueData[i][j] << endl;
			}
		}
		cout << endl;
	}

	return 0;
}

