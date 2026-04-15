#include "SilentAim.hpp"
#include "../../Definations/Variables.hpp"

namespace Cheat
{
	void RestoreSilent()
	{
		std::vector<uint8_t> ReWriteTable =
		{
			0xF3, 0x41, 0x0F, 0x10, 0x19,
			0xF3, 0x41, 0x0F, 0x10, 0x41, 0x04,
			0xF3, 0x41, 0x0F, 0x10, 0x51, 0x08
		};

		FrameWork::Memory::WriteProcessMemoryImpl(g_Fivem.HandleBullet, &ReWriteTable[0], ReWriteTable.size());


		std::vector<uint8_t> AngleReWriteTable =
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00
		};

		FrameWork::Memory::WriteProcessMemoryImpl(g_Fivem.GetModuleBase() + 0xEF8, &AngleReWriteTable[0], AngleReWriteTable.size());
	}

	void ApplySilent(Vector3D EndBulletPos)
	{
		static uint64_t HandleBulletAddress = g_Fivem.HandleBullet;
		static uint64_t AllocPtr = g_Fivem.GetModuleBase() + 0x3D8;

		auto CalculateRelativeOffset = [](uint64_t CurrentAddress, uint64_t TargetAddress, int Offset = 5)
			{
				intptr_t RelativeOffset = static_cast<intptr_t>(TargetAddress - (CurrentAddress + Offset));

				return static_cast<uint32_t>(RelativeOffset);
			};

		union
		{
			float f;
			uint32_t i;
		} EndPosX, EndPosY, EndPosZ;

		EndPosX.f = EndBulletPos.x;
		EndPosY.f = EndBulletPos.y;
		EndPosZ.f = EndBulletPos.z;

		{
			std::vector<uint8_t> ReWriteTable =
			{
				0xE9, 0x00, 0x00, 0x00, 0x00
			};

			uint32_t JmpOffset = CalculateRelativeOffset(HandleBulletAddress, AllocPtr);
			ReWriteTable[1] = static_cast<uint8_t>(JmpOffset & 0xFF);
			ReWriteTable[2] = static_cast<uint8_t>((JmpOffset >> 8) & 0xFF);
			ReWriteTable[3] = static_cast<uint8_t>((JmpOffset >> 16) & 0xFF);
			ReWriteTable[4] = static_cast<uint8_t>((JmpOffset >> 24) & 0xFF);

			FrameWork::Memory::WriteProcessMemoryImpl(HandleBulletAddress, &ReWriteTable[0], ReWriteTable.size());
		}

		{
			uintptr_t currentAddress = (uintptr_t)AllocPtr;

			uintptr_t targetAddress = (uintptr_t)(HandleBulletAddress);

			intptr_t relativeOffset = static_cast<intptr_t>(targetAddress - (currentAddress + 28));

			uint32_t jmpOffset = static_cast<uint32_t>(relativeOffset);

			std::vector<uint8_t> ReWriteTable =
			{
				0x41, 0xC7, 0x01, static_cast<uint8_t>(EndPosX.i), static_cast<uint8_t>(EndPosX.i >> 8), static_cast<uint8_t>(EndPosX.i >> 16), static_cast<uint8_t>(EndPosX.i >> 24),
				0x41, 0xC7, 0x41, 0x04, static_cast<uint8_t>(EndPosY.i), static_cast<uint8_t>(EndPosY.i >> 8), static_cast<uint8_t>(EndPosY.i >> 16), static_cast<uint8_t>(EndPosY.i >> 24),
				0x41, 0xC7, 0x41, 0x08, static_cast<uint8_t>(EndPosZ.i), static_cast<uint8_t>(EndPosZ.i >> 8), static_cast<uint8_t>(EndPosZ.i >> 16), static_cast<uint8_t>(EndPosZ.i >> 24),
				0xF3, 0x41, 0x0F, 0x10, 0x19,
				0xE9, 0x00, 0x00, 0x00, 0x00
			};

			ReWriteTable[29] = static_cast<uint8_t>(jmpOffset & 0xFF);
			ReWriteTable[30] = static_cast<uint8_t>((jmpOffset >> 8) & 0xFF);
			ReWriteTable[31] = static_cast<uint8_t>((jmpOffset >> 16) & 0xFF);
			ReWriteTable[32] = static_cast<uint8_t>((jmpOffset >> 24) & 0xFF);

			FrameWork::Memory::WriteProcessMemoryImpl(AllocPtr, &ReWriteTable[0], ReWriteTable.size());
		}
	}

	void SilentAim::RunThread() noexcept
	{
		while (!g_Options.General.ShutDown)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (!g_Options.LegitBot.SilentAim.Enabled)
				continue;

			if (!g_Fivem.GetLocalPlayerInfo().Ped)
				continue;

			static bool SilentAplied = false;

			Entity ClosestEntity;

			bool isFriend = ClosestEntity.StaticInfo.IsFriend;
			if (isFriend) continue;

			if (g_Fivem.FindClosestEntity(g_Options.LegitBot.SilentAim.Fov, g_Options.LegitBot.SilentAim.MaxDistance, g_Options.LegitBot.SilentAim.VisibleCheck, g_Options.LegitBot.SilentAim.ShotNPC, g_Options.LegitBot.SilentAim.ClosestFov, &ClosestEntity))
			{
				if ((uint64_t)ClosestEntity.StaticInfo.Ped >= 0xCCCCCCCCCCCCCC)
				{
					if (SilentAplied)
					{
						RestoreSilent();
						SilentAplied = false;
					}
					continue;
				}

				if (!SafeCall(GetAsyncKeyState)(g_Options.LegitBot.SilentAim.KeyBind))
				{
					if (SilentAplied)
					{
						RestoreSilent();
						SilentAplied = false;
					}
					continue;
				}

				if (g_Options.LegitBot.SilentAim.MissChance > 0 && (rand() % 100) < g_Options.LegitBot.SilentAim.MissChance)
				{
					if (SilentAplied)
					{
						RestoreSilent();
						SilentAplied = false;
					}
					continue;
				}

				Vector3D BonePos;

				switch (g_Options.LegitBot.SilentAim.HitBox)
				{
				case 0: // Head
					BonePos = g_Fivem.GetBonePosVec3(ClosestEntity, SKEL_Head);
					break;
				case 1: // Neck
					BonePos = g_Fivem.GetBonePosVec3(ClosestEntity, SKEL_Neck_1);
					break;
				case 2: // Chest
					BonePos = g_Fivem.GetBonePosVec3(ClosestEntity, SKEL_Spine3);
					break;
				default:
					break;
				}

				ApplySilent(BonePos);
				SilentAplied = true;

				if (g_Options.LegitBot.SilentAim.MagicBullet)
				{
					auto localPlayer = g_Fivem.GetLocalPlayerInfo().Ped;
					if (localPlayer)
					{
						uintptr_t weaponManager = (uintptr_t)localPlayer->GetWeaponManager();
						if (weaponManager)
						{
							uintptr_t weaponObj = FrameWork::Memory::ReadMemory<uintptr_t>(weaponManager + 0x78);
							if (weaponObj)
							{
								uintptr_t CWeapon = FrameWork::Memory::ReadMemory<uintptr_t>(weaponObj + 0x320);
								if (CWeapon)
								{
									Vector3D targetHeading = FrameWork::Memory::ReadMemory<Vector3D>((uintptr_t)ClosestEntity.StaticInfo.Ped + 0x70);
									float distanceAhead = 5.0f;

									Vector3D spawnPosition = {
										BonePos.x + targetHeading.x * distanceAhead,
										BonePos.y + targetHeading.y * distanceAhead,
										BonePos.z + targetHeading.z * distanceAhead
									};

									FrameWork::Memory::WriteMemory<Vector3D>(CWeapon + 0x20, spawnPosition);
								}
							}
						}
					}
				}
			}
			else
			{
				if (SilentAplied)
				{
					RestoreSilent();
					SilentAplied = false;
				}
			}
		}
	}
}