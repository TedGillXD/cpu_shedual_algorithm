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
	cout << "������ ���� ���� ��ʼ ���� ��ת ��Ȩ��ת" << endl;
	for (auto Answer : Answers) {
		int TurnaroundTime = Answer.second.GetEndTime() - Answer.second.GetProcess().GetArrivalTime();
		cout << Answer.second.ToString() << " " << TurnaroundTime << " " << (double)TurnaroundTime / (double)Answer.second.GetProcess().GetNeedTime() << endl;
		SumOfTurnaroundTime += TurnaroundTime;
		SumOfTurnaroundTimeWithWeight += (double)TurnaroundTime / (double)Answer.second.GetProcess().GetNeedTime();
	}

	cout << "ƽ����תʱ��:" << SumOfTurnaroundTime / Answers.size() << endl;
	cout << "ƽ����Ȩ��תʱ��:" << SumOfTurnaroundTimeWithWeight / Answers.size() << endl;

	cout << "ִ��˳��:";
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
		cout << "1.�����ȷ���" << endl
			<< "2.����ҵ����" << endl
			<< "3.����Ȩ�㷨" << endl
			<< "4.ʱ��Ƭ��ת" << endl
			<< "5.�˳�" << endl;
		cout << "��ѡ��";
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
			cout << "0.����ռʽ" << endl
				 << "1.��ռʽ" << endl;
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
			cout << "������ʱ��Ƭ��С(Ϊ������):";
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