#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <queue>
#include "Process.h"
#include "Result.h"

using namespace std;

enum class EWorkMode {
	no_interrupted,
	interrupted
};

class Processor {
private:
	vector<Process> Queue;
	uint32_t CurrentTime;
	vector<string> ProcessOrder;

public:
	Processor(vector<Process> Queue) {
		this->Queue = Queue;
		CurrentTime = 0;
	}

	vector<string> GetProcessOrder() {
		return ProcessOrder;
	}

	//先来先服务
	map<string, ResultSet> FCFS() {
		map<string, ResultSet> Ans;
		ProcessOrder.clear();
		for (auto process : Queue) {
			string ProcessName = process.GetName();
			int CurrentTime = std::max(CurrentTime, process.GetArrivalTime());
			Ans[ProcessName] = ResultSet(process, CurrentTime, CurrentTime + process.GetNeedTime());
			CurrentTime += process.GetNeedTime();
			for (int i = 0; i < process.GetNeedTime(); i++) {
				ProcessOrder.push_back(ProcessName);
			}
		}

		CurrentTime = 0;

		return Ans;
	}

	//短作业优先
	map<string, ResultSet> SJF() {
		ProcessOrder.clear();
		map<string, ResultSet> Ans;
		auto temp = Queue;
		vector<Process> tempQueue;
		while (!temp.empty()) {
			tempQueue.clear();
			for (auto it = temp.begin(); it != temp.end(); ) {
				if (it->GetArrivalTime() <= CurrentTime) {
					tempQueue.push_back(*it);
					it = temp.erase(it);
				} else {
					it++;
				}
			}

			//解决 CurrentTime < 剩余任务的最小到达时间 的问题
			if (tempQueue.empty()) {
				CurrentTime = INT_MAX;
				for (auto item : temp) {
					if (item.GetArrivalTime() < CurrentTime) {
						CurrentTime = item.GetArrivalTime();
					}
				}
			}

			sort(tempQueue.begin(), tempQueue.end(), [](Process a, Process b) -> bool {
				return a.GetNeedTime() < b.GetNeedTime();
			});

			//处理任务
			while (!tempQueue.empty()) {
				Ans[tempQueue.front().GetName()] = ResultSet(tempQueue.front(), CurrentTime, CurrentTime + tempQueue.front().GetNeedTime());
				CurrentTime += tempQueue.front().GetNeedTime();
				for (int i = 0; i < tempQueue.front().GetNeedTime(); i++) {
					ProcessOrder.push_back(tempQueue.front().GetName());
				}
				tempQueue.erase(tempQueue.begin());
			}
		}

		CurrentTime = 0;

		return Ans;
	}

	//优先权算法
	map<string, ResultSet> HPF(EWorkMode flag) {
		ProcessOrder.clear();
		map<string, ResultSet> Ans;
		auto tempQueue = Queue;
		//非抢占算法
		if (flag == EWorkMode::no_interrupted) {
			while (!tempQueue.empty()) {
				sort(tempQueue.begin(), tempQueue.end(), [](Process a, Process b) -> bool {
					return a.GetArrivalTime() < b.GetArrivalTime();
				});

				Process temp("TEMP", -1, -1, INT_MAX);
				auto eraseIt = tempQueue.end();
				for (auto it = tempQueue.begin(); it != tempQueue.end(); it++) {
					if ((uint32_t)it->GetArrivalTime() > CurrentTime) {
						continue;
					}
					if (it->GetPriority() < temp.GetPriority()) {
						temp = *it;
						eraseIt = it;
					}
				}

				Ans[temp.GetName()] = ResultSet(temp, CurrentTime, CurrentTime + temp.GetNeedTime());
				for (int i = 0; i < temp.GetNeedTime(); i++) {
					ProcessOrder.push_back(temp.GetName());
				}
				if (eraseIt != tempQueue.end()) {
					tempQueue.erase(eraseIt);
				}
				CurrentTime += temp.GetNeedTime();
			}
		} else { //抢占算法
			//记录所需时间
			map<string, int> Count;
			priority_queue<Process, vector<Process>, std::greater<Process>> pri_qu;
			for (auto item : Queue) {
				Count[item.GetName()] = item.GetNeedTime();
			}

			sort(tempQueue.begin(), tempQueue.end(), [](Process a, Process b) -> bool {
				return a.GetArrivalTime() < b.GetArrivalTime();
			});
			Process CurrentProcess = NULLPROCESS;
			for (; !pri_qu.empty() || !tempQueue.empty() || CurrentProcess != NULLPROCESS; CurrentTime++) {
				if (!tempQueue.empty() && CurrentTime == tempQueue.front().GetArrivalTime()) {
					if (tempQueue.front().GetPriority() < CurrentProcess.GetPriority()) {
						//记录还没有完成的进程
						if (CurrentProcess != NULLPROCESS) {
							pri_qu.push(CurrentProcess);
						}
						//将当前进程替换成优先级更高的进程
						CurrentProcess = tempQueue.front();
					} else {
						pri_qu.push(tempQueue.front());
					}
					tempQueue.erase(tempQueue.begin());
				} else if(CurrentProcess == NULLPROCESS){
					CurrentProcess = pri_qu.top();
					pri_qu.pop();
				}

				//处理
				auto Name = CurrentProcess.GetName();
				if (Count[Name] == CurrentProcess.GetNeedTime()) {
					Ans[Name] = ResultSet(CurrentProcess, CurrentTime, -1);
				}
				Count[Name]--;
				ProcessOrder.push_back(Name);

				if (Count[Name] == 0) {
					Ans[Name].SetEndTime(CurrentTime + 1);
					CurrentProcess = NULLPROCESS;
				}
			}
		}

		CurrentTime = 0;

		return Ans;
	}

	//时间片轮转，TimeSlice为时间片大小
	map<string, ResultSet> RR(int TimeSlice) {
		ProcessOrder.clear();
		queue<Process> qu;
		map<string, ResultSet> Ans;
		map<string, int> Count;
		for (auto item : Queue) {
			Count[item.GetName()] = item.GetNeedTime();
		}

		auto tempQueue = Queue;
		sort(tempQueue.begin(), tempQueue.end(), [](Process a, Process b) -> bool {
			return a.GetArrivalTime() < b.GetArrivalTime();
		});

		Process Processing = NULLPROCESS;
		int Minus = 0;
		do {
			while (!tempQueue.empty() && CurrentTime >= (uint32_t)tempQueue.front().GetArrivalTime()) {
				qu.push(tempQueue.front());
				Ans[tempQueue.front().GetName()] = ResultSet(tempQueue.front(), -1, -1);
				tempQueue.erase(tempQueue.begin());
			}
			if (Processing != NULLPROCESS && Count[Processing.GetName()] != 0) {
				qu.push(Processing);
			}

			Processing = qu.front();
			string ProcessName = Processing.GetName();
			qu.pop();
			if (Count[ProcessName] == Processing.GetNeedTime()) {
				Ans[ProcessName].SetStartTime(CurrentTime);
			}
			Minus = Count[ProcessName] - TimeSlice;
			Count[ProcessName] = std::max(0, Minus);
			for (int i = 0; i < TimeSlice; i++) {
				ProcessOrder.push_back(ProcessName);
			}

			if (Count[ProcessName] == 0) {
				Ans[ProcessName].SetEndTime(CurrentTime);
			}

			CurrentTime += std::min(TimeSlice, TimeSlice + Minus);
		} while (!tempQueue.empty() || !qu.empty());

		CurrentTime = 0;

		return Ans;
	}
};