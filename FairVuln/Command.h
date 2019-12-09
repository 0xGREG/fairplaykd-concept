#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

class Command {
public:
	uint32_t GetChecksum(uint8_t* buffer, uint32_t length)
	{
		uint32_t checksum = 0x4661223;

		for (size_t i = 0; i < length; i++)
		{
			uint8_t currentByte = buffer[i];

			uint32_t part1 = currentByte + checksum;
			uint32_t part2 = currentByte << ((i & 7) + 8);
			checksum = part1 ^ part2;
		}

		return checksum;
	}

	uint32_t CommandId;
	uint32_t Version = 367;
};

class GetVersionCommand : Command {
public:
	GetVersionCommand() {
		CommandId = 1;
	}

	struct Response {
		uint32_t Response;
		char _pad0[4];
	};
};

class OpenProcessCommand : Command {
public:
	OpenProcessCommand(uint32_t desiredAccess, uint32_t processId) {
		CommandId = 135;

		DesiredAccess = desiredAccess;
		ProcessId = processId;
		Checksum = GetChecksum((uint8_t*)this, offsetof(OpenProcessCommand, Checksum));
	}
	
	uint32_t DesiredAccess;
	uint32_t ProcessId;
	uint32_t Checksum;

	struct Response {
		uint32_t ProcessHandle;
		char _pad0[4];
	};
};

class GetProcessBaseCommand : Command {
public:
	GetProcessBaseCommand(uint32_t processHandle) {
		CommandId = 145;

		ProcessHandle = processHandle;
		Checksum = GetChecksum((uint8_t*)this, offsetof(GetProcessBaseCommand, Checksum));
	}

	uint32_t ProcessHandle;
	uint32_t Checksum;

	struct Response {
		uint64_t ProcessBase;
		char _pad0[4];
	};
};

class CopyProcessMemoryCommand : Command {
public:
	CopyProcessMemoryCommand(uint32_t processHandle, uint64_t source, uint32_t destination, uint32_t size) {
		CommandId = 146;

		ProcessHandle = processHandle;
		Destination = destination;
		Source = source;
		Size = size;
		Checksum = GetChecksum((uint8_t*)this, offsetof(CopyProcessMemoryCommand, Checksum));
	}

	uint32_t ProcessHandle;
	char _pad0[4];
	uint64_t Source;
	uint32_t Destination;
	uint32_t Size;
	uint32_t Checksum;

	struct Response {
		uint32_t BytesRead;
		uint32_t Checksum;
	};
};