#include "Memory.hpp"

#include <winternl.h>

struct TGetWindowHandleData
{
	DWORD Pid;
	std::wstring WindowName;
	HWND hWnd;
};

BOOL CALLBACK EnumWindowsCallback(HWND Handle, LPARAM lParam)
{
	TGetWindowHandleData& Data = *(TGetWindowHandleData*)lParam;

	if (Data.Pid == 0)
	{
		int Length = SafeCall(GetWindowTextLength)(Handle);
		if (Length == 0)
			return true;

		std::wstring Buffer(Length + 1, L'\0');

		SafeCall(GetWindowText)(Handle, &Buffer[0], Length + 1);

		if (Data.WindowName != Buffer)
			return true;

		Data.hWnd = Handle;
		return false;
	}
	else
	{
		DWORD Pid;
		SafeCall(GetWindowThreadProcessId)(Handle, &Pid);

		if (Data.Pid != Pid)
			return true;

		Data.hWnd = Handle;
		return false;
	}

	return true;
}

namespace FrameWork
{
	namespace Memory
	{
		HWND Memory::GetWindowHandleByPID(DWORD Pid)
		{
			TGetWindowHandleData HandleData;
			HandleData.Pid = Pid;
			HandleData.WindowName = XorStr(L"WindowName001");
			HandleData.hWnd = NULL;

			SafeCall(EnumWindows)(EnumWindowsCallback, (LPARAM)&HandleData);
			return HandleData.hWnd;
		}

		bool Memory::IsProcessRunning(DWORD pid)
		{
			if (!pid)
				return false;
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
			if (!hProcess)
				return false;

			DWORD exitCode = 0;
			bool running = GetExitCodeProcess(hProcess, &exitCode) && exitCode == STILL_ACTIVE;
			CloseHandle(hProcess);
			return running;
		}

		DWORD Memory::GetProcessPidByName(std::wstring ProcessName)
		{
			HANDLE hSnapshot = SafeCall(CreateToolhelp32Snapshot)(TH32CS_SNAPPROCESS, 0);
			if (!hSnapshot || hSnapshot == INVALID_HANDLE_VALUE || hSnapshot == ((HANDLE)(LONG_PTR)ERROR_BAD_LENGTH))
			{
				return 0;
			}

			DWORD Pid;
			PROCESSENTRY32 ProcessEntry;
			ProcessEntry.dwSize = sizeof(ProcessEntry);
			if (SafeCall(Process32First)(hSnapshot, &ProcessEntry))
			{
				while (_wcsicmp(ProcessEntry.szExeFile, ProcessName.c_str()))
				{
					if (!SafeCall(Process32Next)(hSnapshot, &ProcessEntry))
					{
						SafeCall(CloseHandle)(hSnapshot);
						return 0;
					}
				}

				Pid = ProcessEntry.th32ProcessID;
			}
			else
			{
				SafeCall(CloseHandle)(hSnapshot);
				return 0;
			}

			SafeCall(CloseHandle)(hSnapshot);
			return Pid;
		}

		uint64_t Memory::GetModuleBaseByName(DWORD Pid, std::wstring ModuleName)
		{
			HANDLE hSnapshot = SafeCall(CreateToolhelp32Snapshot)(TH32CS_SNAPMODULE, Pid);
			if (!hSnapshot || hSnapshot == INVALID_HANDLE_VALUE || hSnapshot == ((HANDLE)(LONG_PTR)ERROR_BAD_LENGTH))
			{
				return 0;
			}

			uint64_t ModuleBase;
			MODULEENTRY32 ModuleEntry;
			ModuleEntry.dwSize = sizeof(ModuleEntry);
			if (SafeCall(Module32First)(hSnapshot, &ModuleEntry))
			{
				while (_wcsicmp(ModuleEntry.szModule, ModuleName.c_str()))
				{
					if (!SafeCall(Module32Next)(hSnapshot, &ModuleEntry))
					{
						SafeCall(CloseHandle)(hSnapshot);
						return 0;
					}
				}

				ModuleBase = (uint64_t)ModuleEntry.modBaseAddr;
			}
			else
			{
				SafeCall(CloseHandle)(hSnapshot);
				return 0;
			}

			SafeCall(CloseHandle)(hSnapshot);
			return ModuleBase;
		}

		void Memory::AttachProces(DWORD Pid)
		{
			AttachedProcessHandle = SafeCall(OpenProcess)(PROCESS_ALL_ACCESS, false, Pid);
			AttachedProcessPid = Pid;
		}

		void Memory::ReadProcessMemoryImpl(uint64_t ReadAddress, LPVOID Read, SIZE_T Size)
		{
			if (AttachedProcessHandle && AttachedProcessPid)
			{
				static pNtReadVirtualMemory NtReadVirtualMemory = (pNtReadVirtualMemory)SafeCall(GetProcAddress)(SafeCall(GetModuleHandleA)(XorStr("ntdll.dll")), XorStr("NtReadVirtualMemory"));

				NTSTATUS Status = NtReadVirtualMemory(AttachedProcessHandle, (PVOID)ReadAddress, Read, Size, nullptr);

				if (Status == 0)
					return;
			}
			return;
		}

		std::string Memory::ReadProcessMemoryString(uint64_t ReadAddress, SIZE_T StringSize)
		{
			const int BufferSize = 256;

			char Buffer[BufferSize];

			int BytesRead = 0;

			while (BytesRead < BufferSize && BytesRead < StringSize)
			{
				char Character;
				ReadProcessMemoryImpl((uint64_t)ReadAddress + BytesRead, &Character, sizeof(char));
				Buffer[BytesRead] = Character;

				if (Character == '\0')
					break;

				BytesRead++;
			}

			return std::string(Buffer);
		}

		bool Memory::WriteProcessMemoryImpl(uint64_t WriteAddress, LPVOID Value, SIZE_T Size)
		{
			if (AttachedProcessHandle && AttachedProcessPid)
			{
				static pNtWriteVirtualMemory NtWriteVirtualMemory = (pNtWriteVirtualMemory)SafeCall(GetProcAddress)(SafeCall(GetModuleHandleA)(XorStr("ntdll.dll")), XorStr("NtWriteVirtualMemory"));

				NTSTATUS Status = NtWriteVirtualMemory(AttachedProcessHandle, (PVOID)WriteAddress, Value, Size, nullptr);

				if (Status == 0)
					return true;
			}

			return false;
		}

		bool Memory::HookJump(uintptr_t HookAddress, uintptr_t JmpToAddress)
		{
			BYTE JumpPatch[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };

			for (int x = 0; x < sizeof(JumpPatch); x++)
			{
				WriteMemory<BYTE>(HookAddress + x, JumpPatch[x]);
			}

			WriteMemory<uintptr_t>(HookAddress + 6, JmpToAddress);

			return true;
		}

		std::vector<uint8_t> Memory::ReadBytes(uintptr_t Addr, size_t Size)
		{
			std::vector<uint8_t> Bytes(Size);
			size_t BytesRead = 0;
			ReadProcessMemory(AttachedProcessHandle, (LPCVOID)Addr, Bytes.data(), Size, &BytesRead);
			Bytes.resize(BytesRead);
			return Bytes;
		}

		bool Memory::WriteBytes(uintptr_t Addr, std::vector<uint8_t> Bytes)
		{
			NTSTATUS Status = WriteProcessMemory(AttachedProcessHandle, (LPVOID)Addr, Bytes.data(), Bytes.size(), NULL);
			return NT_SUCCESS(Status);
		}

		uint64_t Memory::FindSignature(std::vector<uint8_t> Signature, uintptr_t ModuleBase, uintptr_t ModuleBaseSize)
		{
			const size_t blockSize = 0x1000 * 4;
			std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(blockSize);

			size_t signatureSize = Signature.size();
			if (signatureSize == 0)
				return 0;

			static pNtReadVirtualMemory NtReadVirtualMemory = (pNtReadVirtualMemory)SafeCall(GetProcAddress)(SafeCall(GetModuleHandleA)(XorStr("ntdll.dll")), XorStr("NtReadVirtualMemory"));

			if (!NtReadVirtualMemory)
				return 0;

			for (uintptr_t address = ModuleBase; address < ModuleBase + ModuleBaseSize; address += blockSize)
			{
				SIZE_T bytesRead = 0;
				NTSTATUS status = NtReadVirtualMemory(AttachedProcessHandle, (PVOID)address, data.get(), blockSize, &bytesRead);

				if (status < 0 || bytesRead == 0)
					continue;

				for (uintptr_t i = 0; i < bytesRead; i++)
				{
					uintptr_t j = 0;
					for (; j < signatureSize; j++)
					{
						if (Signature[j] == 0x00)
							continue;

						if (i + j >= bytesRead)
							break;

						if (data[i + j] != Signature[j])
							break;
					}

					if (j == signatureSize)
						return address + i;
				}
			}

			return 0;
		}

		uint64_t Memory::CreateCodeCave(size_t Size)
		{
			LPVOID CaveAddress = VirtualAllocEx(AttachedProcessHandle, NULL, Size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			return (uint64_t)CaveAddress;
		}

		bool Memory::FreeCave(uintptr_t Addr)
		{
			return VirtualFreeEx(AttachedProcessHandle, (LPVOID)Addr, 0, MEM_RELEASE);
		}
	}
}