#include <iostream>
#include "FairPlay.h"

int main()
{
	FairPlay Interface;

	Interface.OpenProcess(PROCESS_ALL_ACCESS, 3552);

	uint64_t processBase = Interface.GetProcessBase();

	cout << std::hex << processBase << std::dec << "\n";
	
	cout << Interface.ReadMemory<uint16_t>(processBase);
}