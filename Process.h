#pragma once
#include <string>
#define NULLPROCESS Process("NONE", -1, -1, INT_MAX)

using namespace std;

class Process {
protected:
	string Name;
	int ArrivalTime;
	int NeedTime;
	int Priority;

public:
	Process() {}

	Process(string Name, int ArrivalTime, int NeedTime, int Priority) {
		this->Name = Name;
		this->ArrivalTime = ArrivalTime;
		this->NeedTime = NeedTime;
		this->Priority = Priority;
	}

	string GetName() {
		return Name;
	}

	int GetArrivalTime() {
		return ArrivalTime;
	}

	int GetNeedTime() {
		return NeedTime;
	}

	int GetPriority() {
		return Priority;
	}

	friend bool operator!= (Process a, Process b) {
		return a.GetName() != b.GetName();
	}

	friend bool operator== (Process a, Process b) {
		return a.GetName() == b.GetName();
	}

	friend bool operator< (Process a, Process b) {
		return a.GetPriority() < b.GetPriority();
	}

	friend bool operator> (Process a, Process b) {
		return a.GetPriority() > b.GetPriority();
	}

	string ToString() {
		string ret = "";
		ret.append(Name + " " + to_string(ArrivalTime) + " " + to_string(NeedTime) + " " + to_string(Priority));
		return ret;
	}
};