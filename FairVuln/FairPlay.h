#pragma once
#include <Windows.h>
#include <iostream>
#include "Command.h"

using namespace std;

#define CONTROL_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

class FairPlay {
public:
	FairPlay();
	~FairPlay();

	void OpenProcess(uint32_t desiredAccess, uint32_t processId);
	uint64_t GetProcessBase();
	void ReadMemory(uint64_t address, uint64_t destination, size_t length);

	template<typename T>
	T ReadMemory(uint64_t address) {
		T destination{};

		ReadMemory(address, reinterpret_cast<uint64_t>(&destination), sizeof(T));

		return destination;
	}
private:
	HANDLE DriverHandle;
	uint32_t ProcessHandle;

	void GetVersion();
};