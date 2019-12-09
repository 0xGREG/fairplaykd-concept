#include "FairPlay.h"

FairPlay::FairPlay() {
	DriverHandle = CreateFileA("\\\\.\\FairplayKD0", FILE_READ_ACCESS | FILE_WRITE_ACCESS, 0, NULL, OPEN_EXISTING, 0, NULL);

	GetVersion();
}

FairPlay::~FairPlay() {
	if (!DriverHandle || DriverHandle == INVALID_HANDLE_VALUE)
		return;

	CloseHandle(DriverHandle);
}

void FairPlay::GetVersion() {
	GetVersionCommand command;
	GetVersionCommand::Response response;

	DWORD bytesReturned = 0;

	DeviceIoControl(DriverHandle, CONTROL_CODE, &command, sizeof(command), &response, sizeof(response), &bytesReturned, NULL);
}

void FairPlay::OpenProcess(uint32_t desiredAccess, uint32_t processId) {
	OpenProcessCommand command(desiredAccess, processId);
	OpenProcessCommand::Response response;

	DWORD bytesReturned = 0;

	DeviceIoControl(DriverHandle, CONTROL_CODE, &command, sizeof(command), &response, sizeof(response), &bytesReturned, NULL);

	ProcessHandle = response.ProcessHandle;
}

uint64_t FairPlay::GetProcessBase() {
	GetProcessBaseCommand command(ProcessHandle);
	GetProcessBaseCommand::Response response;

	DWORD bytesReturned = 0;

	DeviceIoControl(DriverHandle, CONTROL_CODE, &command, sizeof(command), &response, sizeof(response), &bytesReturned, NULL);

	return response.ProcessBase;
}

void FairPlay::ReadMemory(uint64_t address, uint64_t destination, size_t length) {
	CopyProcessMemoryCommand command(ProcessHandle, address, static_cast<uint32_t>(destination), static_cast<uint32_t>(length));
	CopyProcessMemoryCommand::Response response;

	DWORD bytesReturned = 0;

	DeviceIoControl(DriverHandle, CONTROL_CODE, &command, sizeof(command), &response, sizeof(response), &bytesReturned, NULL);
}