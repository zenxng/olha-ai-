#pragma once

#include <Windows.h>

enum class NoClipState
{
	Disabled,
	Hold,
	Toggle
};

namespace Cheat
{
	class Options
	{
	public:
		struct LegitBot
		{
			struct AimBot
			{
				bool Enabled;
				bool ShowFov;
				bool TargetNPC;
				bool VisibleCheck;

				int ClosestFov = 0;
				int KeyBind;
				int KeyBindState;
				int HitBox = 0;
				int MaxDistance = 250;
				int FOV = 100;
				int SmoothHorizontal = 0;
				int SmoothVertical = 0;

				float FovColor[4] = { 0.55f, 0.f, 0.85f, 1.f };
			}AimBot;

			struct SilentAim
			{
				bool Enabled;
				bool ShowFOV;
				bool ShotNPC;
				bool VisibleCheck;
				bool MagicBullet;
				bool Humanize;

				int Fov = 100;
				int KeyBind;
				int KeyBindState;
				int MissChance = 0;
				int ClosestFov = 0;
				int MaxDistance = 250;
				int HitBox = 0;

				float FovColor[4] = { 0.55f, 0.f, 0.85f, 1.f };
			}SilentAim;

			struct TriggerBot
			{
				bool Enabled;
				bool ShotNPC;
				bool VisibleCheck;

				int KeyBind = 0;
				int KeyBindState = 0;
				int MaxDistance = 250;
				int ReactionTime;
			}Trigger;

		}LegitBot;

		struct Visuals
		{
			struct Players
			{
				bool Enabled;
				bool Toogle;
				bool ShowLocalPlayer;
				bool ShowNPC;
				bool VisibleCheck;
				bool ExcludeDeads;

				int ToggleKey = 0;
				int ToggleKeyState = 0;
				int RenderDistance = 250;
				int EspOptions = 6;
				int BoxType = 0;
				int HealthBarType = 1;
				int AmorBarType = 1;

				bool EnableBox;
				bool EnableDistance;
				bool EnableHeadBol;
				bool Skeleton;
				bool HealthBar;
				bool AmorBar;
				bool WeaponName;
				bool Name;
				bool RadarEnabled;

				float BoxColor[4] = { 1.f, 1.f, 1.f, 1.f };
				float InvisibleVisibleBoxColor[4] = { 1.f, 0.f, 0.f, 1.f };
				float DistanceColor[4] = { 1.f, 1.f, 1.f, 1.f };
				float SkeletonColor[4] = { 1.f, 1.f, 1.f, 1.f };
				float InvisibleSkeletonColor[4] = { 1.f, 0.5f, 0.5f, 1.f };
				float FriendSkeletonColor[4] = { 0.f, 1.f, 1.f, 0.7f };
				float HealthBarColor[4] = { 0.f, 1.f, 0.f, 0.7f };
				float ArmorBarColor[4] = { 0.f, 0.f, 1.f, 0.7f };
				float TextColor[4] = { 1.f, 1.f, 1.f, 1.f };
				float HeadCircleColor[4] = { 0.8f, 0.8f, 0.8f, 0.12f };
			}Players;

			struct Vehicles
			{
				bool Enabled;
				bool Toogle;
				bool Distance;
				bool Name;
				bool ShowLockState;

				int ToggleKey = 0;
				int ToggleKeyState = 0;
				int EspOptions = 0;

				bool Marker;
				float MarkerColor[4] = { 0.55f, 0.f, 0.85f, 0.7f };
				float DistanceColor[4] = { 0.55f, 0.3f, 0.9f, 0.7f };
				float NameColor[4] = { 0.9f, 0.8f, 1.f, 0.7f };
			}Vehicles;

		}Visuals;

		struct Exploits
		{
			struct Self
			{
				bool Invisible;
				bool GodMode;
				bool NoClip;
				bool NoRagDoll;
				bool RemoveSpread;
				bool RemoveRecoil;
				bool SeatBelt;
				bool StealVehicle;
				bool UnLockAllVehicles;
				bool LockAllVehicles;
				bool ReplayTP;
				bool AntHS;

				float WeaponSize = 1.f;
				float NoClipSpeed = 100.f;

				int NoClipKey = 0;
				int NoClipKeyState = 0;
				int GodKey = 0;
				int GodKeyState = 0;
				int ReplayTPBind = 0;
				int ReplayTPBindState = 0;
				int NoClipMode = 0;
			}Self;

		}Exploits;

		struct General
		{
			bool ShutDown = false;
			bool CaptureBypass = true;
			bool Vsync = true;
			bool SecondMonitor = false;

			int MenuKey = 0x34;
			int MenuKeyState = 0;
		}General;
	};
}

inline Cheat::Options g_Options;