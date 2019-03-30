#include <Windows.h>
#include <TlHelp32.h>
#include <errno.h>
#include <cstdio>
#include <string>

#define PROCESS_NAME "Settlers IV Level Editor - [No Designation]"

namespace dh {
	int strcompare(const char* One, const char* Two, bool CaseSensitive)
	{
#if defined _WIN32 || defined _WIN64
		return CaseSensitive ? strcmp(One, Two) : _stricmp(One, Two);
#else
		return CaseSensitive ? strcmp(One, Two) : strcasecmp(One, Two);
#endif
	}

	PVOID getProcessHandle(const char* processName)
	{
		HWND windowHandle = FindWindow(nullptr, processName);
		DWORD processID;
		GetWindowThreadProcessId(windowHandle, &processID);
		PVOID processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, 0, processID);
		if (processHandle == nullptr) exit(1);
		//std::cout << windowHandle << std::endl;
		return processHandle;
	}

	DWORD getProcessID(const char* processName)
	{
		HWND windowHandle = FindWindow(nullptr, processName);
		DWORD processID;
		GetWindowThreadProcessId(windowHandle, &processID);
		//std::cout << "Window Handle: " << windowHandle << std::endl;
		//std::cout << "Process ID: " << processID << std::endl;
		return processID;
	}

	MODULEENTRY32 getModuleInfo(const char* ModuleName)
	{
		std::uint32_t processID = getProcessID(ModuleName);
		void* handleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
		MODULEENTRY32 Mod32 = { 0 };
		if (handleSnapshot == INVALID_HANDLE_VALUE) return Mod32;

		Mod32.dwSize = sizeof(MODULEENTRY32);
		while (Module32Next(handleSnapshot, &Mod32))
		{
			if (strcompare(ModuleName, Mod32.szModule, false))
			{
				CloseHandle(handleSnapshot);
				return Mod32;
			}
		}

		CloseHandle(handleSnapshot);
		return { 0 };

	}

	bool writeToProcessMemory(const char* processName, size_t addressOffset, const void* newValue, size_t sizeOfNewValue)
	{
		MODULEENTRY32 moduleInfo = getModuleInfo(processName);
		void* processHandle = getProcessHandle(processName);
		if (!WriteProcessMemory(processHandle, moduleInfo.modBaseAddr + addressOffset, newValue, sizeOfNewValue, 0)) return false;
		return true;
	}

	bool readFromProcessMemory(const char* processName, size_t addressOffset, void* result, size_t sizeOfNewValue)
	{
		MODULEENTRY32 moduleInfo = getModuleInfo(processName);
		void* processHandle = getProcessHandle(processName);
		if (!ReadProcessMemory(processHandle, moduleInfo.modBaseAddr + addressOffset, result, sizeOfNewValue, 0)) return false;
		return true;
	}

	int readEditorCoordinates(const char* outputfile)
	{
		//read from memory
		const size_t bytesToRead = 13;
		char* result = new char[bytesToRead];
		for (size_t i = 0; i < bytesToRead; ++i)
		{
			/*if (dh::readFromProcessMemory(PROCESS_NAME, 0x6CD48 + i, &result[i], sizeof(result[i]))) {
				std::cout << result[i];
			}*/
			dh::readFromProcessMemory(PROCESS_NAME, 0x6CD48 + i, &result[i], sizeof(result[i]));
		}
		//std::cout << std::endl;

		int x = 0;
		int y = 0;
		// get coordinates
		for (size_t i = 0; i < bytesToRead; ++i)
		{
			if (result[i] == ' ') {
				// x positioin
				char* xs = new char[i + 1];
				for (size_t j = 0; j < i; ++j) {
					xs[j] = result[j];
				}
				xs[i] = '\0';
				x = std::atoi(xs);
				delete[] xs; //123 Y: 234##
				// y position
				i += 4;
				char* ts = new char[bytesToRead - i + 1];
				size_t count = 0;
				for (size_t j = 0; result[i + j] != '\0'; ++j) {
					ts[j] = result[i + j];
					++count;
				}
				ts[count] = '\0';
				char* ys = new char[count + 1];
				for (size_t i = 0; i < count; ++i) {
					ys[i] = ts[i];
				}
				ys[count] = '\0';
				delete[] ts;
				y = std::atoi(ys);
				delete[] ys;
				break;
			}
		}
		//std::cout << "Point is (" << x << "," << y << ")" << std::endl;

		delete[] result;

		if (std::strlen(outputfile) > 0)
		{
			std::FILE* fp;
			errno_t err = fopen_s(&fp, outputfile, "w");
			std::fprintf(fp, "%d,%d", x, y);
			std::fflush(fp);
			std::fclose(fp);
		}


		// convert both ints into one
		int xa[4] = { 0 };
		xa[3] = x % 10;
		xa[2] = (x - xa[3]) % 100;
		xa[1] = (x - xa[2] - xa[3]) % 1000;
		xa[0] = (x - xa[1] - xa[2] - xa[3]);
		int ya[4] = { 0 };
		ya[3] = y % 10;
		ya[2] = (y - ya[3]) % 100;
		ya[1] = (y - ya[2] - ya[3]) % 1000;
		ya[0] = (y - ya[1] - ya[2] - ya[3]);
		return (xa[0] + xa[1] + xa[2] + xa[3])*10000 + ya[0] + ya[1] + ya[2] + ya[3];
	}
} // namespace dh

int main(void)
{
	return dh::readEditorCoordinates("");
}
