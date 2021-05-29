#pragma once
#include <string>
#include "Process.h"

class ResultSet{
private:
	int StartTime;
	int EndTime;
	Process process;

public:
	ResultSet() {}

	ResultSet(Process ProcessItem, int StartTime, int EndTime) {
		this->process = ProcessItem;
		this->EndTime = EndTime;
		this->StartTime = StartTime;
	}

	Process GetProcess() {
		return process;
	}

	int GetEndTime() {
		return EndTime;
	}

	int GetStartTime() {
		return StartTime;
	}

	void SetEndTime(int EndTime) {
		this->EndTime = EndTime;
	}

	void SetStartTime(int StartTime) {
		this->StartTime = StartTime;
	}

	string ToString() {
		string ret = "";
		ret.append(process.ToString() + " " + to_string(StartTime) + " " + to_string(EndTime));
		return ret;
	}

};