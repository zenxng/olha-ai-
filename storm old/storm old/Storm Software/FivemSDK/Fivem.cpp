#include "Fivem.hpp"
#include "../Definations/Variables.hpp"
#include "../FiveM-External.hpp"

namespace Cheat
{
	void FivemSDK::Intialize()
	{
		if (bIsIntialized)
			return;

		static const std::vector<uint8_t> CheckTable1 = { 0x48, 0xB8, 0x01, 0x00, 0x00 };
		static const std::vector<uint8_t> CheckTable2 = { 0x48, 0x89, 0x5C, 0x24, 0x10 };

		std::vector<std::wstring> ProcessList =
		{
			XorStr(L"FiveM_b2802_GameProcess.exe"),
			XorStr(L"FiveM_b2802_GTAProcess.exe"),
			XorStr(L"FiveM_b2944_GameProcess.exe"),
			XorStr(L"FiveM_b2944_GTAProcess.exe"),
			XorStr(L"FiveM_b3095_GameProcess.exe"),
			XorStr(L"FiveM_b3095_GTAProcess.exe"),
			XorStr(L"FiveM_GameProcess.exe"),
			XorStr(L"FiveM_GTAProcess.exe"),
			XorStr(L"FiveM_b3323_GTAProcess.exe"),
			XorStr(L"FiveM_b3323_GameProcess.exe"),
			XorStr(L"FiveM_b3407_GTAProcess.exe"),
			XorStr(L"FiveM_b3407_GameProcess.exe"),
		};

		for (size_t i = 0; i < ProcessList.size(); i++)
		{
			Pid = FrameWork::Memory::GetProcessPidByName(ProcessList.at(i).c_str());
			if (Pid)
			{
				ModuleName = FrameWork::Misc::Wstring2String(ProcessList.at(i));
				ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, ProcessList.at(i).c_str());

				std::regex Regex(XorStr(R"_(FiveM_b(\d+))_"));
				std::smatch Match;

				if (std::regex_search(ModuleName, Match, Regex)) {
					GameVersion = std::stoi(Match[1].str());
				}
				else {
					GameVersion = 3258;
				}

				FrameWork::Memory::AttachProces(Pid);

				break;
			}
		}

		std::vector<std::pair<uint64_t, int>> VersionsMap =
		{
			{ 0x6FFE78, 2802 },
			{ 0x7014E0, 2944 },
			{ 0x7072C0, 3095 },
			{ 0x70A064, 3258 },
			{ 0x70A814, 3323 },
			{ 0x711AC0, 3407 },
		};

		for (size_t i = 0; i < VersionsMap.size(); i++)
		{
			std::vector<uint8_t> ReadBuffer(CheckTable1.size());

			int Checks1 = 0;
			int Checks2 = 0;

			for (size_t x = 0; x < CheckTable1.size(); x++)
			{
				uint8_t Byte = FrameWork::Memory::ReadMemory<uint8_t>(ModuleBase + VersionsMap.at(i).first + x);
				if (Byte == CheckTable1[x])
					Checks1++;

				if (Byte == CheckTable2[x])
					Checks2++;
			}

			if (Checks1 >= CheckTable1.size() - 1 || Checks2 >= CheckTable2.size() - 1)
			{
				RealGameVersion = VersionsMap.at(i).second;
				break;
			}
		}

		if (RealGameVersion == 2802)
		{
			World = ModuleBase + 0x254D448;
			ReplayInterface = ModuleBase + 0x1F5B820;
			Camera = ModuleBase + 0x1FBCFA8;
			ViewPort = ModuleBase + 0x1FBC100;

			PlayerAimingAt = ModuleBase + 0x1FCA160;
			HandleBullet = ModuleBase + 0xFF716C;

			CanCombatRoll = ModuleBase + 0x6FFE78;
			GameplayCamHolder = ModuleBase + 0x288D72;
			GameplayCamTarget = ModuleBase + 0x1FBD298;
			BlipList = ModuleBase + 0x1FBD6E0;

			Offsets::EntityType = 0x1098;
			Offsets::FragInsNmGTA = 0x1430;
			Offsets::ConfigFlags = 0x1444;
			Offsets::PlayerInfo = 0x10A8;
			Offsets::PlayerNetID = 0x88;
			Offsets::WeaponManager = 0x10B8;
			Offsets::VisibleFlag = 0x145C;
			Offsets::MaxHealth = 0x284;
			Offsets::SeatBelt = 0x143C;
			Offsets::SeatBeltWindShield = Offsets::SeatBelt + 12;
			Offsets::Armor = 0x150C;
			Offsets::SpeedModifier = 0xCF0;
			Offsets::DoorLock = 0x13C0;

			bIsIntialized = true;
		}
		else if (RealGameVersion == 2944)
		{
			World = ModuleBase + 0x257BEA0;
			ReplayInterface = ModuleBase + 0x1F42068;
			Camera = ModuleBase + 0x1FEB968;
			ViewPort = ModuleBase + 0x1FEAAC0;

			CanCombatRoll = ModuleBase + 0x7014E0;
			GameplayCamHolder = ModuleBase + 0x28A916;
			GameplayCamTarget = ModuleBase + 0x1FEBC58;
			BlipList = ModuleBase + 0x1121F0;

			PlayerAimingAt = ModuleBase + 0x1FF8AF0;
			HandleBullet = ModuleBase + 0x1003F80;

			Offsets::EntityType = 0x1098;
			Offsets::FragInsNmGTA = 0x1430;
			Offsets::ConfigFlags = 0x1444;
			Offsets::SeatBelt = 0x143C;
			Offsets::SeatBeltWindShield = Offsets::SeatBelt + 12;
			Offsets::PlayerInfo = 0x10A8;
			Offsets::PlayerNetID = 0xE8;
			Offsets::WeaponManager = 0x10B8;
			Offsets::VisibleFlag = 0x145C;
			Offsets::MaxHealth = 0x284;
			Offsets::Armor = 0x150C;
			Offsets::DoorLock = 0x13C0;

			bIsIntialized = true;
		}
		else if (RealGameVersion == 3095)
		{
			World = ModuleBase + 0x2593320;
			ReplayInterface = ModuleBase + 0x1F58B58;
			Camera = ModuleBase + 0x2002888;
			ViewPort = ModuleBase + 0x20019E0;

			PlayerAimingAt = ModuleBase + 0x200FA10;
			HandleBullet = ModuleBase + 0x100F5A4;

			Invisible = ModuleBase + 0x1386426;
			CanCombatRoll = ModuleBase + 0x7072C0;
			GameplayCamHolder = ModuleBase + 0x28E5DA;
			GameplayCamTarget = ModuleBase + 0x2002B78;
			BlipList = ModuleBase + 0x2002FA0;

			Offsets::EntityType = 0x1098;
			Offsets::FragInsNmGTA = 0x1430;
			Offsets::ConfigFlags = 0x1444;
			Offsets::PlayerInfo = 0x10A8;
			Offsets::WeaponManager = 0x10B8;
			Offsets::PlayerNetID = 0xE8;
			Offsets::SeatBelt = 0x143C;
			Offsets::SeatBeltWindShield = Offsets::SeatBelt + 12;
			Offsets::VisibleFlag = 0x145C;
			Offsets::MaxHealth = 0x284;
			Offsets::Armor = 0x150C;
			Offsets::SpeedModifier = 0xD40;
			Offsets::DoorLock = 0x13C0;

			bIsIntialized = true;
		}
		else if (RealGameVersion == 3258)
		{
			World = ModuleBase + 0x25B14B0;
			ReplayInterface = ModuleBase + 0x1FBD4F0;
			Camera = ModuleBase + 0x201ED50;
			ViewPort = ModuleBase + 0x201DBA0;

			PlayerAimingAt = ModuleBase + 0X202C8D0;
			HandleBullet = ModuleBase + 0x101A660;

			Invisible = ModuleBase + 0x119345A;
			CanCombatRoll = ModuleBase + 0x70A064;
			GameplayCamHolder = ModuleBase + 0x29029A;
			GameplayCamTarget = ModuleBase + 0x201ED90;
			BlipList = ModuleBase + 0x2023400;

			Offsets::EntityType = 0x1098;
			Offsets::FragInsNmGTA = 0x1430;
			Offsets::ConfigFlags = 0x1444;
			Offsets::PlayerInfo = 0x10A8;
			Offsets::PlayerNetID = 0xE8;
			Offsets::WeaponManager = 0x10B8;
			Offsets::SeatBelt = 0x143C;
			Offsets::SeatBeltWindShield = Offsets::SeatBelt + 12;
			Offsets::VisibleFlag = 0x145C;
			Offsets::MaxHealth = 0x284;
			Offsets::Armor = 0x150C;
			Offsets::SpeedModifier = 0xD40;
			Offsets::DoorLock = 0x13C0;

			bIsIntialized = true;
		}
		else if (RealGameVersion == 3323)
		{
			World = ModuleBase + 0x25C15B0;
			ReplayInterface = ModuleBase + 0x1F85458;
			Camera = ModuleBase + 0x202EB48;
			ViewPort = ModuleBase + 0x202DC50;

			PlayerAimingAt = ModuleBase + 0X203C970;
			HandleBullet = ModuleBase + 0x1026CB0;

			Invisible = ModuleBase + 0x11A174C;
			CanCombatRoll = ModuleBase + 0x70A814;
			GameplayCamHolder = ModuleBase + 0x290274;
			GameplayCamTarget = ModuleBase + 0x202EE38;
			BlipList = ModuleBase + 0x2CEDFC;

			Offsets::EntityType = 0x1098;
			Offsets::FragInsNmGTA = 0x1430;
			Offsets::ConfigFlags = 0x1444;
			Offsets::PlayerInfo = 0x10A8;
			Offsets::PlayerNetID = 0xE8;
			Offsets::WeaponManager = 0x10B8;
			Offsets::SeatBelt = 0x143C;
			Offsets::SeatBeltWindShield = Offsets::SeatBelt + 12;
			Offsets::VisibleFlag = 0x145C;
			Offsets::MaxHealth = 0x284;
			Offsets::Armor = 0x150C;
			Offsets::SpeedModifier = 0xD40;
			Offsets::DoorLock = 0x13C0;

			bIsIntialized = true;
		}
		else if (RealGameVersion == 3407)
		{
			World = ModuleBase + 0x25D7108;
			ReplayInterface = ModuleBase + 0x1F9A9D8;
			Camera = ModuleBase + 0x20440C8;
			ViewPort = ModuleBase + 0x20431C0;

			PlayerAimingAt = ModuleBase + 0x2051EE0;
			HandleBullet = ModuleBase + 0x102FF8C;

			Invisible = ModuleBase + 0x11B1A2C;
			CanCombatRoll = ModuleBase + 0x711AC0;
			GameplayCamHolder = ModuleBase + 0x290804;
			GameplayCamTarget = ModuleBase + 0x20443B8;
			BlipList = ModuleBase + 0x2CF3E0;

			Offsets::EntityType = 0x1098;
			Offsets::FragInsNmGTA = 0x1430;
			Offsets::ConfigFlags = 0x1444;
			Offsets::PlayerInfo = 0x10A8;
			Offsets::PlayerNetID = 0xE8;
			Offsets::DoorLock = 0x13C0;
			Offsets::WeaponManager = 0x10B8;
			Offsets::VisibleFlag = 0x145C;
			Offsets::MaxHealth = 0x284;
			Offsets::SeatBelt = 0x143C;
			Offsets::SeatBeltWindShield = Offsets::SeatBelt + 12;
			Offsets::Armor = 0x150C;
			Offsets::SpeedModifier = 0xD40;
			Offsets::DoorLock = 0x13C0;

			bIsIntialized = true;
		}

		if (bIsIntialized)
			FrameWork::Memory::AttachProces(Pid);

		CitizemPlayerNamesModule = FrameWork::Memory::GetModuleBaseByName(Pid, XorStr(L"citizen-playernames-five.dll"));
		NetIdToNamesPtr = CitizemPlayerNamesModule + 0x30D38;
		ModuleBaseSize = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
	}

	bool FivemSDK::UpdateEntities()
	{
		if (!pWorld)
		{
			pWorld = (CWorld*)FrameWork::Memory::ReadMemory<uint64_t>(World);

			if (!pWorld)
				return false;
		}

		pLocalPlayer = pWorld->LocalPlayer();

		if (pReplayInterface && !pLocalPlayer)
		{
			g_Options.General.ShutDown = true;
			return false;
		}

		if (!pLocalPlayer)
			return false;

		if (!pReplayInterface)
		{
			pReplayInterface = (CReplayInterface*)FrameWork::Memory::ReadMemory<uint64_t>(ReplayInterface);

			if (!pReplayInterface)
				return false;
		}

		if (!pPedInterface)
		{
			pPedInterface = pReplayInterface->PedInterface();

			if (!pPedInterface)
				return false;
		}

		if (!pCamGameplayDirector)
		{
			pCamGameplayDirector = (CCamGameplayDirector*)FrameWork::Memory::ReadMemory<uint64_t>(Camera);

			if (!pCamGameplayDirector)
				return false;
		}

		LockLists.lock();

		EntityList.clear();
		EntityList.shrink_to_fit();

		for (size_t i = 0; i < pPedInterface->PedMaximum(); i++)
		{
			CPed* Ped = pPedInterface->PedList()->Ped(i);

			if (g_Options.Visuals.Players.ExcludeDeads && Ped->GetHealth() <= 101)
				continue;

			PedStaticInfo StaticInfo;
			{
				StaticInfo.Ped = Ped;
				StaticInfo.iIndex = i;
				StaticInfo.bIsLocalPlayer = (Ped == pLocalPlayer);
				StaticInfo.bIsNPC = Ped->IsNPC();
				StaticInfo.VisibleCheck = Ped->IsVisible();

				if (FriendList.find(Ped->GetPlayerInfo()->GetPlayerID()) != FriendList.end())
					StaticInfo.IsFriend = true;
				else
					StaticInfo.IsFriend = false;

				if (!StaticInfo.bIsNPC)
				{
					StaticInfo.NetId = Ped->GetPlayerInfo()->GetPlayerID();
					StaticInfo.Name = g_Fivem.GetPlayerName((uint64_t)Ped->GetPlayerInfo() + 0x20, StaticInfo.NetId);
				}
				else
				{
					StaticInfo.NetId = -1;
					StaticInfo.Name = XorStr("NPC");
				}
			}

			Entity CurrentEntity;
			CurrentEntity.StaticInfo = StaticInfo;

			CurrentEntity.Cordinates = Ped->GetCoordinate();
			CurrentEntity.Visible = Ped->IsVisible();

			CurrentEntity.HeadPos = WorldToScreen(GetBonePosVec3(CurrentEntity, SKEL_Head));

			if (CurrentEntity.StaticInfo.bIsLocalPlayer)
			{
				LocalPlayerInfo.WorldPos = CurrentEntity.Cordinates;
				LocalPlayerInfo.Ped = Ped;
				LocalPlayerInfo.iIndex = i;
			}

			try
			{
				EntityList.push_back(CurrentEntity);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}

		}

		LockLists.unlock();
	}

	bool FivemSDK::UpdateVehicles()
	{
		if (!pCamGameplayDirector)
			return false;

		if (!pVehicleInterface)
		{
			pVehicleInterface = pReplayInterface->VehicleInterface();

			if (!pVehicleInterface)
				return false;
		}

		LockLists2.lock();

		VehicleList.clear();
		VehicleList.shrink_to_fit();

		for (size_t i = 0; i < pVehicleInterface->VehicleMaximum(); i++)
		{
			CVehicle* Vehicle = pVehicleInterface->VehicleList()->Vehicle(i);
			if (!Vehicle)
				continue;

			if (Vehicle->GetCoordinate().DistTo(GetLocalPlayerInfo().WorldPos) > 600)
				continue;

			VehicleInfo CurrentVeh;

			CurrentVeh.Vehicle = Vehicle;
			CurrentVeh.ModelInfo = Vehicle->GetModelInfo();
			if (CurrentVeh.ModelInfo)
			{
				CurrentVeh.Name = FrameWork::Memory::ReadProcessMemoryString(CurrentVeh.ModelInfo + 0x298, 24);
				if (CurrentVeh.Name.find('?') != std::string::npos)
					continue;
			}

			CurrentVeh.iIndex = i;

			try
			{
				VehicleList.push_back(CurrentVeh);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}

		LockLists2.unlock();
	}

	void FivemSDK::TeleportToObject(uintptr_t Object, uintptr_t Navigation, uintptr_t ModelInfo, Vector3D Position, Vector3D VisualPosition, bool Stop)
	{
		float BackupMagic = 0.f;
		if (Stop)
		{
			BackupMagic = FrameWork::Memory::ReadMemory<float>(ModelInfo + 0x2C);
			FrameWork::Memory::WriteMemory<float>(ModelInfo + 0x2C, 0.f);
		}

		FrameWork::Memory::WriteMemory<Vector3D>(Object + 0x90, VisualPosition);
		FrameWork::Memory::WriteMemory<Vector3D>(Navigation + 0x50, Position);

		if (Stop)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
			FrameWork::Memory::WriteMemory<float>(ModelInfo + 0x2C, BackupMagic);
		}
	}

	uint64_t FivemSDK::SyncTreeGetter(std::string SyncName)
	{
		uint64_t SyncTree = NetSync;
		uint64_t SyncTreeOffset;

		std::map<std::string, uint64_t> SyncTreeTypes =
		{
			{XorStr("CPedSyncTree"), SyncTree + 0x26},
			{XorStr("CObjectSyncTree"), SyncTree + 0x2E},
			{XorStr("CHeliSyncTree"), SyncTree + 0x36},
			{XorStr("CDoorSyncTree"), SyncTree + 0x3E},
			{XorStr("CBoatSyncTree"), SyncTree + 0x46},
			{XorStr("CBikeSyncTree"), SyncTree + 0x4E},
			{XorStr("CAutomobileSyncTree"), SyncTree + 0x56},
			{XorStr("CPickupSyncTree"), SyncTree + 0x5E},
			{XorStr("CTrainSyncTree"), SyncTree + 0x82},
			{XorStr("CPlayerSyncTree"), SyncTree + 0x8A},
			{XorStr("CSubmarineSyncTree"), SyncTree + 0x92},
			{XorStr("CPlaneSyncTree"), SyncTree + 0x9A},
			{XorStr("CPickupPlacementSyncTree"), SyncTree + 0xA2}
		};

		auto it = SyncTreeTypes.find(SyncName);
		if (it != SyncTreeTypes.end())
			SyncTreeOffset = it->second;

		return FrameWork::Memory::ReadMemory<uint64_t>(SyncTreeOffset + FrameWork::Memory::ReadMemory<int>(SyncTreeOffset + 3) + 7);
	}

	ImVec2 FivemSDK::WorldToScreen(Vector3D Pos)
	{
		if (!pViewPort)
		{
			pViewPort = FrameWork::Memory::ReadMemory<uint64_t>(ViewPort);
		}

		Matrix4x4 ViewMatrix = FrameWork::Memory::ReadMemory<Matrix4x4>(pViewPort + 0x24C);

		ViewMatrix.TransposeThisMatrix();

		Vector4D VecX(ViewMatrix._21, ViewMatrix._22, ViewMatrix._23, ViewMatrix._24);
		Vector4D VecY(ViewMatrix._31, ViewMatrix._32, ViewMatrix._33, ViewMatrix._34);
		Vector4D VecZ(ViewMatrix._41, ViewMatrix._42, ViewMatrix._43, ViewMatrix._44);

		Vector3D ScreenPos;
		ScreenPos.x = (VecX.x * Pos.x) + (VecX.y * Pos.y) + (VecX.z * Pos.z) + VecX.w;
		ScreenPos.y = (VecY.x * Pos.x) + (VecY.y * Pos.y) + (VecY.z * Pos.z) + VecY.w;
		ScreenPos.z = (VecZ.x * Pos.x) + (VecZ.y * Pos.y) + (VecZ.z * Pos.z) + VecZ.w;

		if (ScreenPos.z <= 0.1f)
			return ImVec2(0, 0);

		ScreenPos.z = 1.0f / ScreenPos.z;
		ScreenPos.x *= ScreenPos.z;
		ScreenPos.y *= ScreenPos.z;

		ScreenPos.x += ImGui::GetIO().DisplaySize.x / 2 + float(0.5f * ScreenPos.x * ImGui::GetIO().DisplaySize.x + 0.5f);
		ScreenPos.y = ImGui::GetIO().DisplaySize.y / 2 - float(0.5f * ScreenPos.y * ImGui::GetIO().DisplaySize.y + 0.5f);

		return ImVec2(ScreenPos.x, ScreenPos.y);
	}

	CPed* FivemSDK::GetAimingEntity()
	{
		return (CPed*)FrameWork::Memory::ReadMemory<uint64_t>(PlayerAimingAt);
	}

	Vector3D GetBonePosByInstFragAndID(uint64_t crSkeletonData, unsigned int BoneID)
	{
		Matrix4x4 v4 = FrameWork::Memory::ReadMemory<Matrix4x4>(FrameWork::Memory::ReadMemory<uint64_t>(crSkeletonData + 0x8));
		Matrix4x4 Result = FrameWork::Memory::ReadMemory<Matrix4x4>(FrameWork::Memory::ReadMemory<uint64_t>(crSkeletonData + 0x18) + (BoneID << 6));

		Vector3D vec1(v4._11, v4._12, v4._13);
		Vector3D vec2(v4._21, v4._22, v4._23);
		Vector3D vec3(v4._31, v4._32, v4._33);
		Vector3D vec4(v4._41, v4._42, v4._43);
		Vector3D vec5(Result._41, Result._42, Result._43);

		return Vector3D(
			vec1.x * vec5.x + vec4.x + vec2.x * vec5.y + vec3.x * vec5.z,
			vec1.y * vec5.x + vec4.y + vec2.y * vec5.y + vec3.y * vec5.z,
			vec1.z * vec5.x + vec4.z + vec2.z * vec5.y + vec3.z * vec5.z
		);
	}

	ImVec2 FivemSDK::GetClosestHitBox(Entity Ped)
	{
		ImVec2 Result = ImVec2(0, 0);

		ImVec2 Head = Ped.HeadPos;
		if (!g_Fivem.IsOnScreen(Head))
			return Result;

		ImVec2 Neck = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Ped, SKEL_Neck_1));
		if (!g_Fivem.IsOnScreen(Neck))
			return Result;

		ImVec2 Chest = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Ped, SKEL_Spine3));
		if (!g_Fivem.IsOnScreen(Chest))
			return Result;

		ImVec2 Center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);

		float HeadDistance = Head.DistTo(Center);
		float NeckDistance = Neck.DistTo(Center);
		float ChestDistance = Chest.DistTo(Center);

		float DistancesArray[] = { HeadDistance, NeckDistance, ChestDistance };

		float Closest = DistancesArray[0];
		int ClosestBone = 0;

		for (int i = 0; i < 5; ++i)
		{
			if (DistancesArray[i] < Closest)
			{
				Closest = DistancesArray[i];
				ClosestBone = i;
			}
		}

		switch (ClosestBone)
		{
		case 0:
			return Head;
			break;
		case 1:
			return Neck;
			break;
		case 2:
			return Chest;
			break;
		default:
			return Head;
			break;
		}

	}

	bool FivemSDK::FindClosestEntity(float Fov, int MaxDistance, bool VisibleCheck, bool NPC, int PriorityMode, Entity* Output)
	{
		Entity Closest;
		float ClosestWorldDistance = FLT_MAX;
		float ClosestScreenDistance = FLT_MAX;
		float BestHealth = FLT_MAX;

		bool Found = false;

		LockLists.lock();
		for (Entity Current : EntityList)
		{
			if (Current.StaticInfo.bIsLocalPlayer)
				continue;

			if (Current.StaticInfo.bIsNPC && !NPC)
				continue;

			if (!Current.StaticInfo.VisibleCheck && VisibleCheck)
				continue;

			float WorldDistance = Current.Cordinates.DistTo(GetLocalPlayerInfo().WorldPos);
			if (WorldDistance > MaxDistance)
				continue;

			ImVec2 ClosestBone = GetClosestHitBox(Current);
			if (ClosestBone.x == 0 && ClosestBone.y == 0)
				continue;

			ImVec2 Center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
			float ScreenDistance = ClosestBone.DistTo(Center);

			if (ScreenDistance > Fov)
				continue;

			float CurrentHealth = 99999.f;
			if (Current.StaticInfo.Ped)
				CurrentHealth = Current.StaticInfo.Ped->GetHealth();

			switch (PriorityMode)
			{
			case 0: // FOV
				if (ScreenDistance < ClosestScreenDistance)
				{
					ClosestScreenDistance = ScreenDistance;
					Closest = Current;
					Found = true;
				}
				break;

			case 1: // Distância
				if (WorldDistance < ClosestWorldDistance)
				{
					ClosestWorldDistance = WorldDistance;
					Closest = Current;
					Found = true;
				}
				break;

			case 2: // Health
				if (CurrentHealth > 0 && CurrentHealth < BestHealth)
				{
					BestHealth = CurrentHealth;
					Closest = Current;
					Found = true;
				}
				break;
			}
		}

		if (Found)
			*Output = Closest;

		LockLists.unlock();
		return Found;
	}

	void FivemSDK::ProcessCameraMovement(Vector3D WorldPosition, int SmoothHorizontal, int SmoothVertical)
	{
		if (!pCamGameplayDirector)
			return;

		auto FollowPedCamera = pCamGameplayDirector->GetFollowPedCamera();

		Vector3D CrosshairPosition = FollowPedCamera->GetCrosshairPosition();
		Vector3D ViewAngles = FollowPedCamera->GetViewAngles();

		float Distance = CrosshairPosition.DistTo(WorldPosition);

		Vector3D AimAngles = Vector3D((WorldPosition.x - CrosshairPosition.x) / Distance, (WorldPosition.y - CrosshairPosition.y) / Distance, (WorldPosition.z - CrosshairPosition.z) / Distance);

		Vector3D FinalAngles = AimAngles;

		Vector3D CameraDelta = Vector3D(AimAngles.x - ViewAngles.x, AimAngles.y - ViewAngles.y, AimAngles.z - ViewAngles.z);

		if (SmoothHorizontal > 1)
		{
			FinalAngles.x = ViewAngles.x + CameraDelta.x / (float)SmoothHorizontal;
			FinalAngles.y = ViewAngles.y + CameraDelta.y / (float)SmoothHorizontal;
		}

		if (SmoothVertical > 1)
			FinalAngles.z = ViewAngles.z + CameraDelta.z / (float)SmoothVertical;

		float AimbotFixZ = ViewAngles.z - FollowPedCamera->GetThirdpersonViewAngles().z;

		Vector3D ThirdPersonAngles = FinalAngles;
		ThirdPersonAngles.z = ThirdPersonAngles.z - AimbotFixZ;

		FollowPedCamera->SetThirdpersonViewAngles(ThirdPersonAngles);
		FollowPedCamera->SetViewAngles(FinalAngles);
	}

	Vector3D FivemSDK::GetBonePosVec3(Entity Ped, unsigned int Mask)
	{
		uint64_t FragInstNMGta = FrameWork::Memory::ReadMemory<uint64_t>(Ped.StaticInfo.Ped + Offsets::FragInsNmGTA);
		if (FragInstNMGta)
		{
			Ped.StaticInfo.crSkeletonData = FrameWork::Memory::ReadMemory<uint64_t>(FrameWork::Memory::ReadMemory<uint64_t>(FragInstNMGta + 0x68) + 0x178);

			auto it = Ped.StaticInfo.MaskToBoneId.find(Mask);
			if (it == Ped.StaticInfo.MaskToBoneId.end())
			{
				unsigned int BoneId = 0;
				if (GetPedBoneIndex(Ped, Mask, BoneId))
				{
					if (BoneId)
					{
						Ped.StaticInfo.MaskToBoneId[Mask] = BoneId;
						return GetBonePosByInstFragAndID(Ped.StaticInfo.crSkeletonData, BoneId);
					}
				}
			}
			else
			{
				return GetBonePosByInstFragAndID(Ped.StaticInfo.crSkeletonData, it->second);
			}
		}

		return Ped.Cordinates;
	}

	bool FivemSDK::GetPedBoneIndex(Entity Ped, unsigned int Mask, unsigned int& newIdx)
	{
		uint64_t crSkeletonData = FrameWork::Memory::ReadMemory<uint64_t>(Ped.StaticInfo.crSkeletonData);

		if (FrameWork::Memory::ReadMemory<int16_t>(crSkeletonData + 0x1A))
		{
			uint16_t v1 = FrameWork::Memory::ReadMemory<uint16_t>(crSkeletonData + 0x18);
			if (v1)
			{
				int64_t v2 = FrameWork::Memory::ReadMemory<int64_t>(crSkeletonData + 0x10);
				int Count = 0;
				for (int64_t i = FrameWork::Memory::ReadMemory<int64_t>(v2 + 0x8 * (Mask % v1)); ; i = FrameWork::Memory::ReadMemory<int64_t>(i + 0x8))
				{
					Count++;
					if (!i || i >= 0xCCCCCCCCCCCCCC || Count > 3)
						return false;

					int v5 = FrameWork::Memory::ReadMemory<int>(i);
					if (Mask == v5)
					{
						int v6 = FrameWork::Memory::ReadMemory<int>(i + 0x4);
						newIdx = v6;
						return true;
					}
				}
			}
		}
		else if (Mask < FrameWork::Memory::ReadMemory<uint64_t>(crSkeletonData + 0x5E))
		{
			newIdx = Mask;
			return true;
		}

		return false;
	}

	uint32_t GetRelayIpAddress(uint64_t PeerAddress)
	{
		if (g_Fivem.GetGameVersion() >= 2944)
			return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 72);

		if (g_Fivem.GetGameVersion() >= 2372)
			return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 60);

		return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 20);
	}

	uint32_t GetPublicIpAddress(uint64_t PeerAddress)
	{
		if (g_Fivem.GetGameVersion() >= 2944)
			return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 168);

		if (g_Fivem.GetGameVersion() >= 2372)
			return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 76);

		return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 28);
	}

	uint32_t GetLocalIpAddress(uint64_t PeerAddress)
	{
		if (g_Fivem.GetGameVersion() >= 2944)
			return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 176);

		if (g_Fivem.GetGameVersion() >= 2372)
			return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 84);

		return FrameWork::Memory::ReadMemory<uint32_t>(PeerAddress + 36);
	}

	std::string FivemSDK::GetPlayerName(uint64_t PeerAddress, int GameNetId)
	{
		static std::unordered_map<int, std::string> g_NetToIdNames;

		auto it = g_NetToIdNames.find(GameNetId);

		if (it != g_NetToIdNames.end())
		{
			return it->second.c_str();
		}

		std::string Result = XorStr("** Invalid **");

		if (PeerAddress == 0x20)
			return Result;

		uint32_t RelayIpAddr = GetRelayIpAddress(PeerAddress);
		uint32_t PublicIpAddr = GetPublicIpAddress(PeerAddress);
		uint32_t LocalAddr = GetLocalIpAddress(PeerAddress);

		int NetId = 0;
		uint32_t CurAddress = 0;

		if (g_Fivem.GetGameVersion() < 2944)
		{
			if (g_Fivem.GetGameVersion() < 2372)
			{
				if (g_Fivem.GetGameVersion() < 2060)
				{
					if (RelayIpAddr != LocalAddr || RelayIpAddr != PublicIpAddr)
						return FrameWork::Memory::ReadProcessMemoryString(PeerAddress + 84, 64).c_str();
				}
				else
				{
					if (RelayIpAddr != LocalAddr || RelayIpAddr != PublicIpAddr)
						return FrameWork::Memory::ReadProcessMemoryString(PeerAddress + 92, 64).c_str();
				}
			}
			else
			{
				if (RelayIpAddr != LocalAddr || RelayIpAddr != PublicIpAddr)
					return FrameWork::Memory::ReadProcessMemoryString(PeerAddress + 132, 64).c_str();
			}

			CurAddress = RelayIpAddr;
		}
		else
		{
			CurAddress = LocalAddr;
		}

		NetId = (CurAddress & 0xFFFF) ^ 0xFEED;

		uint64_t HashPart1 = static_cast<uint8_t>(NetId) ^ 0xCBF29CE484222325;
		uint64_t HashPart2 = static_cast<uint8_t>(NetId >> 8) ^ 0x100000001B3 * HashPart1;
		uint64_t HashPart3 = static_cast<uint8_t>(NetId >> 16) ^ 0x100000001B3 * HashPart2;
		uint64_t HashPart4 = static_cast<uint8_t>(NetId >> 24) ^ 0x100000001B3 * HashPart3;
		uint64_t Hash = 0x100000001B3 * HashPart4;

		uint64_t HashMask = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.NetIdToNamesPtr + 0x28);
		uint64_t HashPosition = Hash & HashMask;

		uint64_t TableBase = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.NetIdToNamesPtr + 0x10);
		uint64_t IndexAddress = TableBase + 0x10 * HashPosition;

		uint64_t Index = FrameWork::Memory::ReadMemory<uint64_t>(IndexAddress + sizeof(uint64_t));
		if (!Index)
			return Result;

		uint64_t EndAddress = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.NetIdToNamesPtr);
		if (Index == EndAddress)
			return Result;

		uint64_t InitialValue = FrameWork::Memory::ReadMemory<uint64_t>(Index);

		if (NetId != FrameWork::Memory::ReadMemory<DWORD>(Index + 0x10))
		{
			while (Index != InitialValue)
			{
				Index = FrameWork::Memory::ReadMemory<uint64_t>(Index + 8);
				if (NetId == FrameWork::Memory::ReadMemory<DWORD>(Index + 0x10))
					break;
			}
		}

		if (Index != EndAddress)
		{
			Result = FrameWork::Memory::ReadProcessMemoryString(Index + 0x18, 32).c_str();
			g_NetToIdNames[GameNetId] = Result;
		}

		return Result;
	}

	//std::string FivemSDK::GetPlayerName(uint64_t PeerAddress, int GameNetId)
	//{
	//	static std::unordered_map<int, std::string> g_NetToIdNames;

	//	auto it = g_NetToIdNames.find(GameNetId);

	//	if (it != g_NetToIdNames.end())
	//	{
	//		return it->second.c_str();
	//	}

	//	std::string Result = XorStr("** Invalid **");

	//	if (PeerAddress == 0x20)
	//		return Result;

	//	uint32_t RelayIpAddr = GetRelayIpAddress(PeerAddress);
	//	uint32_t PublicIpAddr = GetPublicIpAddress(PeerAddress);
	//	uint32_t LocalAddr = GetLocalIpAddress(PeerAddress);

	//	int NetId = 0;
	//	uint32_t CurAddress = 0;

	//	if (g_Fivem.GetGameVersion() < 2944)
	//	{
	//		if (g_Fivem.GetGameVersion() < 2372)
	//		{
	//			if (g_Fivem.GetGameVersion() < 2060)
	//			{
	//				if (RelayIpAddr != LocalAddr || RelayIpAddr != PublicIpAddr)
	//					return FrameWork::Memory::ReadProcessMemoryString(PeerAddress + 84, 64).c_str();
	//			}
	//			else
	//			{
	//				if (RelayIpAddr != LocalAddr || RelayIpAddr != PublicIpAddr)
	//					return FrameWork::Memory::ReadProcessMemoryString(PeerAddress + 92, 64).c_str();
	//			}
	//		}
	//		else
	//		{
	//			if (RelayIpAddr != LocalAddr || RelayIpAddr != PublicIpAddr)
	//				return FrameWork::Memory::ReadProcessMemoryString(PeerAddress + 132, 64).c_str();
	//		}

	//		CurAddress = RelayIpAddr;
	//	}
	//	else
	//	{
	//		CurAddress = LocalAddr;
	//	}

	//	NetId = (CurAddress & 0xFFFF) ^ 0xFEED;

	//	uint64_t HashPart1 = static_cast<uint8_t>(NetId) ^ 0xCBF29CE484222325;
	//	uint64_t HashPart2 = static_cast<uint8_t>(NetId >> 8) ^ 0x100000001B3 * HashPart1;
	//	uint64_t HashPart3 = static_cast<uint8_t>(NetId >> 16) ^ 0x100000001B3 * HashPart2;
	//	uint64_t HashPart4 = static_cast<uint8_t>(NetId >> 24) ^ 0x100000001B3 * HashPart3;
	//	uint64_t Hash = 0x100000001B3 * HashPart4;

	//	uint64_t HashMask = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.NetIdToNamesPtr + 0x28);
	//	uint64_t HashPosition = Hash & HashMask;

	//	uint64_t TableBase = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.NetIdToNamesPtr + 0x10);
	//	uint64_t IndexAddress = TableBase + 0x10 * HashPosition;

	//	uint64_t Index = FrameWork::Memory::ReadMemory<uint64_t>(IndexAddress + sizeof(uint64_t));
	//	if (!Index)
	//		return Result;

	//	uint64_t EndAddress = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.NetIdToNamesPtr);
	//	if (Index == EndAddress)
	//		return Result;

	//	uint64_t InitialValue = FrameWork::Memory::ReadMemory<uint64_t>(Index);

	//	if (NetId != FrameWork::Memory::ReadMemory<DWORD>(Index + 0x10))
	//	{
	//		while (Index != InitialValue)
	//		{
	//			Index = FrameWork::Memory::ReadMemory<uint64_t>(Index + 8);
	//			if (NetId == FrameWork::Memory::ReadMemory<DWORD>(Index + 0x10))
	//				break;
	//		}
	//	}

	//	if (Index != EndAddress)
	//	{
	//		Result = FrameWork::Memory::ReadProcessMemoryString(Index + 0x18, 128);
	//		g_NetToIdNames[GameNetId] = Result;
	//	}

	//	return Result;
	//}

	bool FivemSDK::IsOnScreen(ImVec2 Pos)
	{
		if (Pos.x < 0.1f || Pos.y < 0.1 || Pos.x > ImGui::GetIO().DisplaySize.x || Pos.y > ImGui::GetIO().DisplaySize.y)
			return false;

		return true;
	}
}