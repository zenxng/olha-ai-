#pragma once

#include "Offsets.hpp"
#include "GTADefines.hpp"
#include "Fivem.hpp"

#include "../FiveM-External.hpp"

class CPed;

namespace Cheat
{
	class CWeaponInfo
	{
	public:
		std::string GetWeaponName()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadProcessMemoryString(FrameWork::Memory::ReadMemory<uint64_t>(this + 0x05F0));
		}
	};

	class CWeaponManager
	{
	public:
		CWeaponInfo* GetWeaponInfo()
		{
			if (!this)
				return 0;

			return (CWeaponInfo*)FrameWork::Memory::ReadMemory<uint64_t>(this + 0x20);
		}
	};

	class CVehicle
	{
	public:
		uint64_t GetModelInfo()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<uint64_t>(this + 0x20);
		}

		void FuckVehicleEngine()
		{
			if (!this) return;

			Vector3D pos = GetCoordinate();

			FrameWork::Memory::WriteMemory<float>((uintptr_t)this + 0x844, -4000.f);
			this->SetHealth(-4000.f);
		}

		void FixEngine()
		{
			if (!this) return;

			Vector3D pos = GetCoordinate();

			FrameWork::Memory::WriteMemory<float>(this + 0x844, 1000.f);
			SetHealth(1000.f);
		}

		uint64_t GetNavigation()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<uint64_t>(this + 0x30);
		}

		void SetLockState(eCarLockState NewState)
		{
			if (!this)
				return;

			FrameWork::Memory::WriteMemory(this + Offsets::DoorLock, (unsigned int)NewState);
		}

		bool GetLockState()
		{
			if (!this)
				return false;

			return FrameWork::Memory::ReadMemory<eCarLockState>(this + Offsets::DoorLock);
		}

		void SetHealth(float NewHealth)
		{
			if (!this)
				return;

			FrameWork::Memory::WriteMemory(this + 0x280, NewHealth);
		}

		Vector3D GetCoordinate()
		{
			if (!this)
				return Vector3D{ 0,0,0 };

			return FrameWork::Memory::ReadMemory<Vector3D>(this + 0x90);
		}
	};

	class CPlayerInfo
	{
	public:
		int GetPlayerID()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<int>(this + Offsets::PlayerNetID);
		}
	};

	class CPed
	{
	public:
		CPlayerInfo* GetPlayerInfo()
		{
			if (!this)
				return 0;

			return (CPlayerInfo*)FrameWork::Memory::ReadMemory<uint64_t>(this + Offsets::PlayerInfo);
		}

		uint32_t GetPedType()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<uint32_t>(this + Offsets::EntityType);
		}

		uint64_t GetNavigation()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<uint64_t>(this + 0x30);
		}

		uint64_t GetModelInfo()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<uint64_t>(this + 0x20);
		}

		Vector3D GetCoordinate()
		{
			if (!this)
				return Vector3D{ 0,0,0 };

			return FrameWork::Memory::ReadMemory<Vector3D>(this + 0x90);
		}

		CWeaponManager* GetWeaponManager()
		{
			if (!this)
				return 0;

			return (CWeaponManager*)FrameWork::Memory::ReadMemory<uint64_t>(this + Offsets::WeaponManager);
		}

		bool IsVisible()
		{
			if (!this)
				return false;

			BYTE VisibilityFlag = FrameWork::Memory::ReadMemory<BYTE>(this + Offsets::VisibleFlag);

			if (VisibilityFlag == 36 || VisibilityFlag == 0 || VisibilityFlag == 4)
				return false;

			return true;
		}

		float GetArmor()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<float>(this + Offsets::Armor);
		}

		float GetHealth()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<float>(this + 0x280);
		}

		float GetMaxHealth()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<float>(this + Offsets::MaxHealth);
		}

		void SetHealth(float NewHealth)
		{
			if (!this)
				return;

			FrameWork::Memory::WriteMemory(this + 0x280, NewHealth);
		}

		bool IsNPC()
		{
			uint32_t PedType = GetPedType();

			if (!PedType)
				return false;

			PedType = PedType << 11 >> 25;

			if (PedType != 2)
				return true;

			return false;
		}

		void SetConfigFlag(ePedConfigFlags Flag, bool Value)
		{
			int v1 = (int)Flag;
			if (!this || v1 >= 0x1CA)
				return;

			auto v2 = 1 << (v1 & 0x1F);
			auto v3 = v1 >> 5;
			auto v4 = (uint64_t)this + 4 * v3 + Offsets::ConfigFlags;
			auto v5 = FrameWork::Memory::ReadMemory<long>(v4);

			if (Value != ((v2 & v5) != 0))
			{
				auto v6 = v2 & (v5 ^ -(uint8_t)(Value ? 1 : 0));
				v5 ^= v6;

				FrameWork::Memory::WriteMemory(v4, v5);
			}
		}

		bool HasConfigFlag(ePedConfigFlags Flag)
		{
			int v1 = (int)Flag;
			if (!this || v1 >= 0x1CA)
				return false;

			auto v2 = 1 << (v1 & 0x1F);
			auto v3 = v1 >> 5;
			auto v4 = (uint64_t)this + 4 * v3 + Offsets::ConfigFlags;
			auto v5 = FrameWork::Memory::ReadMemory<long>(v4);

			return (v2 & v5) != 0;
		}
	};

	class CVehicleList
	{
	public:
		CVehicle* Vehicle(int Index)
		{
			if (!this)
				return 0;

			return (CVehicle*)FrameWork::Memory::ReadMemory<uint64_t>(this + (Index * 0x10));
		}
	};

	class CVehicleInterface
	{
	public:

		CVehicleList* VehicleList()
		{
			if (!this)
				return 0;

			return (CVehicleList*)FrameWork::Memory::ReadMemory<uint64_t>(this + 0x180);
		}

		uint64_t VehicleMaximum()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<uint64_t>(this + 0x188);
		}

		uint64_t VehiclesAtList()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<uint64_t>(this + 0x190);
		}
	};

	class CPedList
	{
	public:
		CPed* Ped(int Index)
		{
			if (!this)
				return 0;

			return (CPed*)FrameWork::Memory::ReadMemory<uint64_t>(this + (Index * 0x10));
		}
	};

	class CPedInterface
	{
	public:
		CPedList* PedList()
		{
			if (!this)
				return 0;

			return (CPedList*)FrameWork::Memory::ReadMemory<uint64_t>(this + 0x100);
		}

		uint64_t PedMaximum()
		{
			if (!this)
				return 0;

			return FrameWork::Memory::ReadMemory<uint64_t>(this + 0x108);
		}
	};

	class CReplayInterface
	{
	public:
		CVehicleInterface* VehicleInterface()
		{
			if (!this)
				return 0;

			return (CVehicleInterface*)FrameWork::Memory::ReadMemory<uint64_t>(this + 0x10);
		}

		CPedInterface* PedInterface()
		{
			if (!this)
				return 0;

			return (CPedInterface*)FrameWork::Memory::ReadMemory<uint64_t>(this + 0x18);
		}
	};

	class CCamFollowPedCamera
	{
	public:
		Vector3D GetViewAngles()
		{
			if (!this)
				return Vector3D(0, 0, 0);

			return FrameWork::Memory::ReadMemory<Vector3D>(this + 0x40);
		}

		void SetViewAngles(Vector3D Angles)
		{
			if (!this)
				return;

			FrameWork::Memory::WriteMemory(this + 0x40, Angles);
		}

		Vector3D GetCrosshairPosition()
		{
			if (!this)
				return Vector3D(0, 0, 0);

			return FrameWork::Memory::ReadMemory<Vector3D>(this + 0x60);
		}

		void SetCrosshairPosition(Vector3D Position)
		{
			if (!this)
				return;

			FrameWork::Memory::WriteMemory(this + 0x60, Position);
		}

		Vector3D GetThirdpersonViewAngles()
		{
			if (!this)
				return Vector3D(0, 0, 0);

			return FrameWork::Memory::ReadMemory<Vector3D>(this + 0x3d0);
		}

		void SetThirdpersonViewAngles(Vector3D ViewAngles)
		{
			if (!this)
				return;

			FrameWork::Memory::WriteMemory(this + 0x3d0, ViewAngles);
		}

		Vector3D GetCameraPosition()
		{
			if (!this)
				return Vector3D(0, 0, 0);

			return FrameWork::Memory::ReadMemory<Vector3D>(this + 0x3F0);
		}

		void SetCameraPosition(Vector3D NewPosition)
		{
			if (!this)
				return;

			FrameWork::Memory::WriteMemory(this + 0x3F0, NewPosition);
		}
	};

	class CCamGameplayDirector
	{
	public:
		CCamFollowPedCamera* GetFollowPedCamera()
		{
			if (!this)
				return 0;

			return (CCamFollowPedCamera*)FrameWork::Memory::ReadMemory<uint64_t>(this + 0x3C0);
		}
	};

	class CWorld
	{
	public:
		CPed* LocalPlayer()
		{
			if (!this)
				return 0;

			return (CPed*)FrameWork::Memory::ReadMemory<uint64_t>(this + 0x8);
		}
	};
}