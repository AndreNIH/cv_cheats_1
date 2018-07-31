#include "HAPIH.h"
#include <iostream>
#include <Windows.h>
#include <string>


const long cheat = 0x833890000006D0E9;
const long original = 0x8338000006CF850F;

const short remove_anticheat = 0x05EB;
const short anticheat = 0x0575;

const short remove_second_anticheat = 0x5DEB;
const short second_anticheat = 0x840F;

const short remove_third_anticheat = 0x9090;

enum stat_code
{
	OK_DISABLED = 0,
	OK_ACTIVE = 1,
	ADDRESS_RECALCULATED = 2,
	RECALCULATE_FAILED = 3
};
void pause(HWND &wd)
{
	while (1)
	{
		if (GetAsyncKeyState(VK_RETURN) == -32767 && GetForegroundWindow() == wd) return;
		Sleep(2);
	}
}
template <typename Type>

int memcheck(HackIH &proc, PointerIH &ptr, size_t size, Type cheat, Type original)
{
	if (size == sizeof(float))
	{
		if (proc.Read<long>(ptr) == cheat) return OK_ACTIVE;
		if (proc.Read<long>(ptr) == original) return OK_DISABLED;

		for (int offset = 0; offset < 100; offset++)
		{
			if (proc.Read<short>(ptr - offset) == original)
			{
				PointerIH mex = { proc.BaseAddress, ptr[0] - offset };
				ptr = mex;
				return ADDRESS_RECALCULATED;
			}
		}

		for (int offset = 0; offset < 100; offset++)
		{
			if (proc.Read<long>(ptr + offset) == original)
			{
				PointerIH mex = { proc.BaseAddress, ptr[0] + offset };
				ptr = mex;
				return ADDRESS_RECALCULATED;
			}
		}
		return RECALCULATE_FAILED;
	}
	if (size == sizeof(short))
	{
		if (proc.Read<short>(ptr) == cheat) return OK_ACTIVE;
		if (proc.Read<short>(ptr) == original) return OK_DISABLED;

		for (int offset = 0; offset < 100; offset++)
		{
			if (proc.Read<short>(ptr - offset) == original)
			{
				PointerIH mex = { proc.BaseAddress, ptr[0] - offset };
				ptr = mex;
				return ADDRESS_RECALCULATED;
			}
		}

		for (int offset = 0; offset < 100; offset++)
		{
			if (proc.Read<short>(ptr + offset) == original)
			{
				PointerIH mex = { proc.BaseAddress, ptr[0] + offset };
				ptr = mex;
				return ADDRESS_RECALCULATED;
			}
		}
		return RECALCULATE_FAILED;
	}
}
int main()
{
	HWND consoleID = GetForegroundWindow();
	HackIH process;
	std::cout << "Waiting for Geometry Dash...";
	while (!process.bind("GeometryDash.exe")) { Sleep(200); }

	PointerIH noclip = { process.BaseAddress,0x0020A1E5 };
	PointerIH anticheataddress = { process.BaseAddress,0x0020D3B1 };
	PointerIH second_anticheataddress = { process.BaseAddress,0x001FD756 };
	PointerIH third_anticheataddress = { process.BaseAddress,0x001FF7A2 };
	std::string additionalinfo;
	bool status = false;
	auto visualStatus = [](bool &st) {return st ? "ENABLED" : "DISABLED"; };

	switch (memcheck(process, noclip, sizeof(long), cheat, original))
	{
	case OK_ACTIVE:
		status = true;
		break;
	case OK_DISABLED:
		status = false;
		break;
	case ADDRESS_RECALCULATED:
		status = false;
		MessageBox(NULL, "Memory addresses have been recalculated, You should be okay to continue but in case that automatic address recalculation is not working feel free to contact me at svcheatsreport@gmail.com", NULL, MB_ICONWARNING);
		break;
	case RECALCULATE_FAILED:
		MessageBox(NULL, "The corresponding memory addresses to be edited have not been found and address recalculation has been unuccessful,feel free to contact me at svcheatsreport@gmail.com in order to recieve instructions on how to manually recalculate the addresses", "Noclip loader", MB_ICONERROR);
		return 0;
	}

	for (;;)
	{
		system("cls");
		std::cout << "cl_cheats 1 by AndreNIH - Geometry Dash 2.113(Steam)\nPress ENTER to toggle\n\n";
		std::cout << "STATUS:" << visualStatus(status) << "\n\n\"cl_cheats 1\" and should automatically detect if you are using a pirated or incorrect version of GD and try to automatically correct memory offsets, however in case that \"cl_cheats 1\" fails to to recalibrate itself correctly a tutorial will be provided for you in order to manually recalibrate this cheat by yourself" << "\n\nAdditional information:\nGeometryDash.exe base address = " << process.BaseAddress;
		pause(consoleID);
		status = !status;
		if (status)
		{
			process.Write<long>(noclip, cheat);
			process.Write<short>(anticheataddress, remove_anticheat);
			process.Write<short>(second_anticheataddress, remove_second_anticheat);
			process.Write<short>(third_anticheataddress, remove_third_anticheat);
		}
		else
		{
			process.Write<long>(noclip, original);
			//process.Write<short>(anticheataddress, anticheat);
			//process.Write<short>(second_anticheataddress, second_anticheat);
		}

		if (!process.IsBound())
		{
			MessageBox(NULL, "Geometry Dash has been closed or cv_cheats 1 is unable to detect it\rTerminating process", NULL, MB_ICONINFORMATION);
			return 0;
		}
	}
}
