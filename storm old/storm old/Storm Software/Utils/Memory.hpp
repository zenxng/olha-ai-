#pragma once

#include "../FiveM-External.hpp"
#include <vector>

#include <winternl.h>

typedef NTSTATUS(NTAPI* pNtWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, SIZE_T BufferSize, PSIZE_T NumberOfBytesWritten);
typedef NTSTATUS(NTAPI* pNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, SIZE_T BufferSize, PSIZE_T NumberOfBytesRead);

namespace FrameWork
{
	namespace Memory
	{
		HWND GetWindowHandleByPID(DWORD Pid);

		DWORD GetProcessPidByName(std::wstring ProcessName);
		bool IsProcessRunning(DWORD pid);

		uint64_t GetModuleBaseByName(DWORD Pid, std::wstring ModuleName);

		uint64_t CreateCodeCave(size_t Size);
		bool FreeCave(uintptr_t Addr);

		std::vector<uint8_t> ReadBytes(uintptr_t Addr, size_t Size);
		bool WriteBytes(uintptr_t Addr, std::vector<uint8_t> Bytes);

		bool HookJump(uintptr_t HookAddress, uintptr_t JmpToAddress);
		uint64_t FindSignature(std::vector<uint8_t> Signature, uintptr_t ModuleBase, uintptr_t ModuleBaseSize);

		void AttachProces(DWORD Pid);
		std::string ReadProcessMemoryString(uint64_t ReadAddress, SIZE_T StringSize = 256);
		void ReadProcessMemoryImpl(uint64_t ReadAddress, LPVOID Read, SIZE_T Size);
		bool WriteProcessMemoryImpl(uint64_t WriteAddress, LPVOID Write, SIZE_T Size);

		template <typename T, typename B>
		T ReadMemory(B ReadAddress)
		{
			T Read;
			ReadProcessMemoryImpl((uint64_t)ReadAddress, &Read, sizeof(T));
			return Read;
		}

		template <typename T, typename B>
		bool WriteMemory(B WriteAddress, T Value)
		{
			return WriteProcessMemoryImpl((uint64_t)WriteAddress, &Value, sizeof(T));
		}

		inline HANDLE AttachedProcessHandle;
		inline DWORD AttachedProcessPid;
	}
}