#include "AimBot.hpp"
#include "../../Definations/Variables.hpp"

namespace Cheat
{
    namespace
    {
        Vector3D GetTargetBonePosition(const Entity& entity, int hitbox)
        {
            switch (hitbox)
            {
            case 0: return g_Fivem.GetBonePosVec3(entity, FB_Brow_Centre_000);
            case 1: return g_Fivem.GetBonePosVec3(entity, SKEL_Neck_1);
            case 2: return g_Fivem.GetBonePosVec3(entity, SKEL_Spine3);
            default: return {};
            }
        }
    }

    void AimBot::RunThread()
    {
        while (!g_Options.General.ShutDown)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));

            if (!g_Options.LegitBot.AimBot.Enabled || !SafeCall(GetAsyncKeyState)(g_Options.LegitBot.AimBot.KeyBind))
                continue;

            const auto& localPlayer = g_Fivem.GetLocalPlayerInfo();
            if (!localPlayer.Ped)
                continue;

            Entity closestEntity{};
            bool found = g_Fivem.FindClosestEntity(g_Options.LegitBot.AimBot.FOV, g_Options.LegitBot.AimBot.MaxDistance, g_Options.LegitBot.AimBot.VisibleCheck, g_Options.LegitBot.AimBot.TargetNPC, g_Options.LegitBot.AimBot.ClosestFov, &closestEntity);

            bool isFriend = closestEntity.StaticInfo.IsFriend;
            if (isFriend) continue;

            if (!found || !closestEntity.StaticInfo.Ped)
                continue;

            Vector3D targetPos = GetTargetBonePosition(closestEntity, g_Options.LegitBot.AimBot.HitBox);
            if (targetPos.IsZero())
                continue;

            g_Fivem.ProcessCameraMovement(targetPos, g_Options.LegitBot.AimBot.SmoothHorizontal, g_Options.LegitBot.AimBot.SmoothVertical);
        }
    }
}