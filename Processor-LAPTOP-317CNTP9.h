#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include "Process.h"
#include "Result.h"

using namespace std;

enum class WorkMode {
	no_interrupted,
	interrupted
};

class Processor {
private:
	vector<Process> Queue;
	uint32_t CurrentTime;

public:
	Processor(vector<Process> Queue) {
		this->Queue = Queue;
		CurrentTime = 0;
	}

	//�����ȷ���
	vector<ResultSet> FCFS() {
		vector<ResultSet> Ans;
		for (auto process : Queue) {
		}
		return vector<ResultSet>();
	}

	//����ҵ����
	//������
	vector<ResultSet> SJF() {
		vector<ResultSet> Ans;
		auto tempQueue = Queue;
		while (!tempQueue.empty()) {
			sort(tempQueue.begin(), tempQueue.end(), [](Process a, Process b) -> bool {
				return a.GetNeedTime() < b.GetNeedTime();
			});
			sort(tempQueue.begin(), tempQueue.end(), [](Process a, Process b) -> bool {
				return a.GetArrivalTime() < b.GetArrivalTime();
			});
			CurrentTime += tempQueue.front().GetNeedTime();
			Ans.push_back(ResultSet(tempQueue.front(), tempQueue.front().GetArrivalTime(), CurrentTime));
			tempQueue.erase(tempQueue.begin());
		}
		return Ans;
	}

	//����Ȩ�㷨
	vector<ResultSet> HPF(WorkMode flag) {
		return vector<ResultSet>();
	}

	//ʱ��Ƭ��ת
	vector<ResultSet> RR() {
		return vector<ResultSet>();
	}
};