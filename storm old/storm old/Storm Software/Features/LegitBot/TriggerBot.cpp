#include "TriggerBot.hpp"
#include <thread>
#include <chrono>
#include <random>
#include <cstdlib>
#include <algorithm>
#include "../../Definations/Variables.hpp"

namespace Cheat
{
	void TriggerBot::RunThread()
	{
		while (!g_Options.General.ShutDown)
		{
			int randomDelay = 15 + (rand() % 31);
			std::this_thread::sleep_for(std::chrono::milliseconds(randomDelay));
			if (!g_Options.LegitBot.Trigger.Enabled)
				continue;

			if (!g_Fivem.GetLocalPlayerInfo().Ped)
				continue;

			static bool Shooting = false;
			bool CanShoot = false;

			if (!SafeCall(GetAsyncKeyState)(g_Options.LegitBot.Trigger.KeyBind))
			{
				if (Shooting)
				{
					INPUT input = { 0 };
					input.type = INPUT_MOUSE;
					input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
					SafeCall(SendInput)(1, &input, sizeof(INPUT));
					Shooting = false;
				}
				continue;
			}

			CPed* AimingEnity = g_Fivem.GetAimingEntity();
			if (AimingEnity)
			{
				if (g_Fivem.GetLocalPlayerInfo().WorldPos.DistTo(AimingEnity->GetCoordinate()) > g_Options.LegitBot.Trigger.MaxDistance)
					CanShoot = false;
				else
					CanShoot = true;

				if (AimingEnity->IsNPC() && !g_Options.LegitBot.Trigger.ShotNPC)
					CanShoot = false;

				if (!g_Fivem.PlayerAimingAt)
					CanShoot = false;
			}
			else
				CanShoot = false;

			if ((rand() % 100) < 5) continue; // 5% de erro humano

			if (CanShoot && !Shooting)
			{
				int variedReactionTime = g_Options.LegitBot.Trigger.ReactionTime + (-10 + (rand() % 21)); // ±10ms de variação
				int safeReactionTime = (variedReactionTime < 1) ? 1 : variedReactionTime;
				std::this_thread::sleep_for(std::chrono::milliseconds(safeReactionTime));
				INPUT input = { 0 };
				input.type = INPUT_MOUSE;
				input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				SafeCall(SendInput)(1, &input, sizeof(INPUT));
				Shooting = true;
			}
			else if (Shooting && !CanShoot)
			{
				INPUT input = { 0 };
				input.type = INPUT_MOUSE;
				input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				SafeCall(SendInput)(1, &input, sizeof(INPUT));
				Shooting = false;
			}
		}
	}
}