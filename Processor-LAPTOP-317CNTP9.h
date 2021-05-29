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

	//先来先服务
	vector<ResultSet> FCFS() {
		vector<ResultSet> Ans;
		for (auto process : Queue) {
		}
		return vector<ResultSet>();
	}

	//短作业优先
	//有问题
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

	//优先权算法
	vector<ResultSet> HPF(WorkMode flag) {
		return vector<ResultSet>();
	}

	//时间片轮转
	vector<ResultSet> RR() {
		return vector<ResultSet>();
	}
};