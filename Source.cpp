#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <map>
#include "Process.h"
#include "Processor.h"
#include "Result.h"

#define CLEAR system("cls")
#define PAUSE system("pause")

void PrintResult(map<string, ResultSet> Answers, Processor processor) {
	double SumOfTurnaroundTime = 0;
	double SumOfTurnaroundTimeWithWeight = 0;
	cout << "进程名 到达 服务 开始 结束 周转 带权周转" << endl;
	for (auto Answer : Answers) {
		int TurnaroundTime = Answer.second.GetEndTime() - Answer.second.GetProcess().GetArrivalTime();
		cout << Answer.second.ToString() << " " << TurnaroundTime << " " << (double)TurnaroundTime / (double)Answer.second.GetProcess().GetNeedTime() << endl;
		SumOfTurnaroundTime += TurnaroundTime;
		SumOfTurnaroundTimeWithWeight += (double)TurnaroundTime / (double)Answer.second.GetProcess().GetNeedTime();
	}

	cout << "平均周转时间:" << SumOfTurnaroundTime / Answers.size() << endl;
	cout << "平均带权周转时间:" << SumOfTurnaroundTimeWithWeight / Answers.size() << endl;

	cout << "执行顺序:";
	for (auto item : processor.GetProcessOrder()) {
		cout << item;
	}

	cout << endl;
}

int main() {
	vector<Process> test;
	test.push_back(Process("A", 0, 4, 4));
	test.push_back(Process("B", 1, 3, 2));
	test.push_back(Process("C", 2, 5, 3));
	test.push_back(Process("D", 3, 2, 5));
	test.push_back(Process("E", 4, 4, 1));
	Processor processor(test);

	while (true) {
		CLEAR;
		map<string, ResultSet> Ans;
		cout << "1.先来先服务" << endl
			<< "2.短作业优先" << endl
			<< "3.优先权算法" << endl
			<< "4.时间片轮转" << endl
			<< "5.退出" << endl;
		cout << "请选择：";
		int operation;
		cin >> operation;
		switch (operation)
		{
		case 1:
			Ans = processor.FCFS();
			PrintResult(Ans, processor);
			break;
		case 2:
			Ans = processor.SJF();
			PrintResult(Ans, processor);
			break;
		case 3:
			int WorkMode;
			cout << "0.非抢占式" << endl
				 << "1.抢占式" << endl;
			cin >> WorkMode;
			if (WorkMode == 0) {
				Ans = processor.HPF(EWorkMode::no_interrupted);
			} else if (WorkMode == 1) {
				Ans = processor.HPF(EWorkMode::interrupted);
			} else {
				cout << "illegal input" << endl;
			}
			PrintResult(Ans, processor);
			break;
		case 4:
			int TimeSlice;
			cout << "请输入时间片大小(为正整数):";
			cin >> TimeSlice;
			Ans = processor.RR(TimeSlice);
			PrintResult(Ans, processor);
			break;
		case 5:
			break;
		}
		PAUSE;
	}

	return 0;
}