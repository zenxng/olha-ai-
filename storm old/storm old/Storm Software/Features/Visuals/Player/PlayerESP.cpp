#include "PlayerESP.hpp"

#include <cmath>
#include <minmax.h>

#include "../../../FivemSDK/Fivem.hpp"
#include "../../../Definations/Variables.hpp"

float clamp(float val, float minVal, float maxVal) {
	return (val < minVal) ? minVal : (val > maxVal ? maxVal : val);
}

namespace Cheat
{
	void ESP::Players()
	{
		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

		for (Entity Current : g_Fivem.GetEntitiyList())
		{
			if (Current.StaticInfo.bIsLocalPlayer && !g_Options.Visuals.Players.ShowLocalPlayer)
				continue;

			if (Current.StaticInfo.bIsNPC && !g_Options.Visuals.Players.ShowNPC)
				continue;

			Vector3D PedCoordinates = Current.StaticInfo.Ped->GetCoordinate();

			float Distance = PedCoordinates.DistTo(g_Fivem.GetLocalPlayerInfo().WorldPos);

			if (Distance > g_Options.Visuals.Players.RenderDistance)
				continue;

			ImVec2 PedLocation = g_Fivem.WorldToScreen(PedCoordinates);
			if (!g_Fivem.IsOnScreen(PedLocation))
				continue;

			ImVec2 Head = Current.HeadPos;
			if (!g_Fivem.IsOnScreen(Head))
				continue;

			ImVec2 LeftFoot = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_L_Foot));
			if (!g_Fivem.IsOnScreen(LeftFoot))
				continue;

			ImVec2 RightFoot = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_R_Foot));
			if (!g_Fivem.IsOnScreen(RightFoot))
				continue;

			float Height = -Head.y;

			if (LeftFoot.y > RightFoot.y)
				Height += LeftFoot.y;
			else
				Height += RightFoot.y;

			float Width = Height / 1.8f;
			float PedCenterY = Head.y + Height / 2.f;
			Height *= 1.2f;


			ImVec2 Padding[4] = { ImVec2(0, 0), ImVec2(0, 0), ImVec2(0, 0), ImVec2(0, 0) };

			ImColor CheckBoxColor = Current.StaticInfo.Ped->IsVisible() && g_Options.Visuals.Players.VisibleCheck ? FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.BoxColor) : FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.InvisibleVisibleBoxColor);

			if (g_Options.Visuals.Players.EnableBox)
			{
				if (g_Options.Visuals.Players.BoxType == 0)
				{
					ImGui::GetBackgroundDrawList()->AddRect(ImVec2(PedLocation.x - Width / 2, PedCenterY - Height / 2.f), ImVec2(PedLocation.x + Width / 2, PedCenterY + Height / 2.f), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.BoxColor[3]), 0, ImDrawFlags_None, 3);
					ImGui::GetBackgroundDrawList()->AddRect(ImVec2(PedLocation.x - Width / 2, PedCenterY - Height / 2.f), ImVec2(PedLocation.x + Width / 2, PedCenterY + Height / 2.f), CheckBoxColor, 0, ImDrawFlags_None, 2);

					Padding[0].y += 3;
					Padding[1].x += 3;
					Padding[2].y += 3;
					Padding[3].x += 3;
				}
				else if (g_Options.Visuals.Players.BoxType == 1)
				{
					float CornerSize = ((((Height / 2.f) / 100.f) + ((Width / 2.f) / 100.f)) / 2.f) * 30.f;

					// Top Left
					ImVec2 TopLeftPoints[] = { { PedLocation.x - Width / 2.f, PedLocation.y - Height / 2.f + CornerSize }, { PedLocation.x - Width / 2.f, PedLocation.y - Height / 2.f }, { PedLocation.x - Width / 2.f + CornerSize, PedLocation.y - Height / 2.f } };
					ImGui::GetBackgroundDrawList()->AddPolyline(TopLeftPoints, 3, ImColor(0.f, 0.f, 0.f, 1.f), ImDrawFlags_None, 3);
					ImGui::GetBackgroundDrawList()->AddPolyline(TopLeftPoints, 3, CheckBoxColor, ImDrawFlags_None, 2);

					// Top Right
					ImVec2 TopRightPoints[] = { { PedLocation.x + Width / 2.f, PedLocation.y - Height / 2.f + CornerSize }, { PedLocation.x + Width / 2.f, PedLocation.y - Height / 2.f }, { PedLocation.x + Width / 2.f - CornerSize, PedLocation.y - Height / 2.f } };
					ImGui::GetBackgroundDrawList()->AddPolyline(TopRightPoints, 3, ImColor(0.f, 0.f, 0.f, 1.f), ImDrawFlags_None, 3);
					ImGui::GetBackgroundDrawList()->AddPolyline(TopRightPoints, 3, CheckBoxColor, ImDrawFlags_None, 2);

					// Bottom Left
					ImVec2 BottomLeftPoints[] = { { PedLocation.x - Width / 2.f, PedLocation.y + Height / 2.f - CornerSize }, { PedLocation.x - Width / 2.f, PedLocation.y + Height / 2.f }, { PedLocation.x - Width / 2.f + CornerSize, PedLocation.y + Height / 2.f } };
					ImGui::GetBackgroundDrawList()->AddPolyline(BottomLeftPoints, 3, ImColor(0.f, 0.f, 0.f, 1.f), ImDrawFlags_None, 3);
					ImGui::GetBackgroundDrawList()->AddPolyline(BottomLeftPoints, 3, CheckBoxColor, ImDrawFlags_None, 2);

					// Bottom Right
					ImVec2 BottomRightPoints[] = { { PedLocation.x + Width / 2.f, PedLocation.y + Height / 2.f - CornerSize }, { PedLocation.x + Width / 2.f, PedLocation.y + Height / 2.f }, { PedLocation.x + Width / 2.f - CornerSize, PedLocation.y + Height / 2.f } };
					ImGui::GetBackgroundDrawList()->AddPolyline(BottomRightPoints, 3, ImColor(0.f, 0.f, 0.f, 1.f), ImDrawFlags_None, 3);
					ImGui::GetBackgroundDrawList()->AddPolyline(BottomRightPoints, 3, CheckBoxColor, ImDrawFlags_None, 2);

					Padding[0].y += 3;
					Padding[1].x += 3;
					Padding[2].y += 3;
					Padding[3].x += 3;
				}
			}

			if (g_Options.Visuals.Players.RadarEnabled)
			{
				const float radarSize = 200.0f;
				const ImVec2 radarPos(50.0f, 50.0f);
				const float radarRange = g_Options.Visuals.Players.RenderDistance;
				const float scale = radarSize / (2.0f * radarRange);

				DrawList->AddRectFilled(radarPos, ImVec2(radarPos.x + radarSize, radarPos.y + radarSize), ImColor(0, 0, 0, 150), 5.0f);
				DrawList->AddRect(radarPos, ImVec2(radarPos.x + radarSize, radarPos.y + radarSize), ImColor(255, 255, 255, 200), 5.0f, 0, 1.5f);

				ImVec2 radarCenter(radarPos.x + radarSize * 0.5f, radarPos.y + radarSize * 0.5f);
				DrawList->AddCircleFilled(radarCenter, 3.0f, ImColor(255, 255, 0, 255), 12);

				Vector3D localPos = g_Fivem.GetLocalPlayerInfo().WorldPos;

				for (auto& ent : g_Fivem.GetEntitiyList())
				{
					CPed* ped = ent.StaticInfo.Ped;
					bool isLocalPlayer = ent.StaticInfo.bIsLocalPlayer;
					bool isNPC = ent.StaticInfo.bIsNPC;

					bool isVisible = ped->IsVisible();

					if (!g_Options.Visuals.Players.ShowLocalPlayer && isLocalPlayer)
						continue;

					if (!g_Options.Visuals.Players.ShowNPC && isNPC && !isLocalPlayer)
						continue;

					if (g_Options.Visuals.Players.VisibleCheck && !isVisible)
						continue;

					Vector3D entityPos = ped->GetCoordinate();
					float distance = entityPos.DistTo(localPos);
					if (distance > g_Options.Visuals.Players.RenderDistance)
						continue;

					ImVec2 radarEntityPos(radarCenter.x + (entityPos.x - localPos.x) * scale, radarCenter.y - (entityPos.y - localPos.y) * scale);

					radarEntityPos.x = clamp(radarEntityPos.x, radarPos.x, radarPos.x + radarSize);
					radarEntityPos.y = clamp(radarEntityPos.y, radarPos.y, radarPos.y + radarSize);

					DrawList->AddCircleFilled(radarEntityPos, 3.0f, ImColor(255, 0, 0, 255), 12);
				}
			}

			if (g_Options.Visuals.Players.Skeleton)
			{
				do
				{
					ImVec2 Pelvis = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_Pelvis));
					if (!g_Fivem.IsOnScreen(Pelvis))
						continue;

					ImVec2 Neck = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_Neck_1));
					if (!g_Fivem.IsOnScreen(Neck))
						continue;

					ImVec2 LeftClavicle = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_L_Clavicle));
					if (!g_Fivem.IsOnScreen(LeftClavicle))
						continue;

					ImVec2 RightClavicle = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_R_Clavicle));
					if (!g_Fivem.IsOnScreen(RightClavicle))
						continue;

					ImVec2 LeftUperarm = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_L_UpperArm));
					if (!g_Fivem.IsOnScreen(LeftUperarm))
						continue;

					ImVec2 RightUperarm = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_R_UpperArm));
					if (!g_Fivem.IsOnScreen(RightUperarm))
						continue;

					ImVec2 RightFormArm = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_R_Forearm));
					if (!g_Fivem.IsOnScreen(RightFormArm))
						continue;

					ImVec2 LeftFormArm = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_L_Forearm));
					if (!g_Fivem.IsOnScreen(LeftFormArm))
						continue;

					ImVec2 RightHand = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_R_Hand));
					if (!g_Fivem.IsOnScreen(RightHand))
						continue;

					ImVec2 LeftHand = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_L_Hand));
					if (!g_Fivem.IsOnScreen(LeftHand))
						continue;

					ImVec2 LeftThigh = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_L_Thigh));
					if (!g_Fivem.IsOnScreen(LeftThigh))
						continue;

					ImVec2 LeftCalf = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_L_Calf));
					if (!g_Fivem.IsOnScreen(LeftCalf))
						continue;

					ImVec2 RightThigh = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_R_Thigh));
					if (!g_Fivem.IsOnScreen(RightThigh))
						continue;

					ImVec2 RightCalf = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_R_Calf));
					if (!g_Fivem.IsOnScreen(RightCalf))
						continue;

					ImColor Color = Current.StaticInfo.IsFriend ? Color = FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.FriendSkeletonColor) : FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.SkeletonColor);

					if (g_Options.Visuals.Players.VisibleCheck)
					{
						if (!Current.Visible)
							Color = FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.InvisibleSkeletonColor);
					}

					ImGui::GetBackgroundDrawList()->AddLine(Neck, RightClavicle, Color, 1.f);
					ImGui::GetBackgroundDrawList()->AddLine(Neck, LeftClavicle, Color, 1.f);

					ImGui::GetBackgroundDrawList()->AddLine(RightClavicle, RightUperarm, Color, 1.f);
					ImGui::GetBackgroundDrawList()->AddLine(LeftClavicle, LeftUperarm, Color, 1.f);

					ImGui::GetBackgroundDrawList()->AddLine(RightUperarm, RightFormArm, Color, 1.f);
					ImGui::GetBackgroundDrawList()->AddLine(LeftUperarm, LeftFormArm, Color, 1.f);

					ImGui::GetBackgroundDrawList()->AddLine(RightFormArm, RightHand, Color, 1.f);
					ImGui::GetBackgroundDrawList()->AddLine(LeftFormArm, LeftHand, Color, 1.f);

					ImGui::GetBackgroundDrawList()->AddLine(Neck, Pelvis, Color, 1.f);

					ImGui::GetBackgroundDrawList()->AddLine(Pelvis, LeftThigh, Color, 1.f);
					ImGui::GetBackgroundDrawList()->AddLine(Pelvis, RightThigh, Color, 1.f);

					ImGui::GetBackgroundDrawList()->AddLine(LeftThigh, LeftCalf, Color, 1.f);
					ImGui::GetBackgroundDrawList()->AddLine(RightThigh, RightCalf, Color, 1.f);

					ImGui::GetBackgroundDrawList()->AddLine(LeftCalf, LeftFoot, Color, 1.f);
					ImGui::GetBackgroundDrawList()->AddLine(RightCalf, RightFoot, Color, 1.f);
				} while (false);
			}

			if (g_Options.Visuals.Players.EnableHeadBol)
			{
				Vector3D HeadPos3D = g_Fivem.GetBonePosVec3(Current, SKEL_Head);
				if (HeadPos3D.x != 0.0f || HeadPos3D.y != 0.0f || HeadPos3D.z != 0.0f) {
					ImVec2 head_screen_pos = g_Fivem.WorldToScreen(HeadPos3D);
					if (head_screen_pos.x != 0 && head_screen_pos.y != 0) {
						int CircleValue = Height / 30;
						if (CircleValue < 3)
							CircleValue = 3;
						ImVec2 center(head_screen_pos.x, head_screen_pos.y);

						ImU32 color = FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.HeadCircleColor);

						DrawList->AddCircle(center, CircleValue, color, 32, 1.5f);
					}
				}
			}

			if (g_Options.Visuals.Players.EnableDistance)
			{
				std::string PlayerDistance = std::to_string((int)Distance) + ("m");
				ImVec2 TextSize = ImGui::CalcTextSize(PlayerDistance.c_str());

				ImVec2 DrawPos = ImVec2(PedLocation.x, PedCenterY + Height / 2);

				DrawPos = ImVec2(DrawPos.x - TextSize.x / 2, DrawPos.y + Padding[2].y - 3);
				Padding[2].y += TextSize.y;

				ImGui::PushFont(FrameWork::Assets::InterBold12);

				ImGui::GetBackgroundDrawList()->AddText(DrawPos + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.DistanceColor[3]), PlayerDistance.c_str());
				ImGui::GetBackgroundDrawList()->AddText(DrawPos, FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.DistanceColor), PlayerDistance.c_str());

				ImGui::PopFont();
			}

			if (g_Options.Visuals.Players.HealthBar)
			{
				float Health = Current.StaticInfo.Ped->GetHealth();
				float MaxHealth = Current.StaticInfo.Ped->GetMaxHealth();

				if (g_Options.Visuals.Players.HealthBarType == 0)
				{
					ImVec2 DrawPos = ImVec2(PedLocation.x - Width / 2, PedCenterY - Height / 2) - Padding[3] - ImVec2(4, 0);
					ImVec2 Size = ImVec2(2, Height);
					ImVec2 DrawSize = ImVec2(0, (Height / MaxHealth) * Health);

					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.HealthBarColor[3]));
					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + ImVec2(2, Height), FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.HealthBarColor));

					Padding[3].x += 3;
				}

				if (g_Options.Visuals.Players.HealthBarType == 1)
				{
					ImVec2 DrawPos = ImVec2(PedLocation.x + Width / 2, PedCenterY - Height / 2) + Padding[1] + ImVec2(2, 0);
					ImVec2 Size = ImVec2(2, Height);
					ImVec2 DrawSize = ImVec2(0, (Height / MaxHealth) * Health);

					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.HealthBarColor[3]));
					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + ImVec2(2, Height), FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.HealthBarColor));

					Padding[1].x += 3;
				}

				if (g_Options.Visuals.Players.HealthBarType == 2)
				{
					ImVec2 DrawPos = ImVec2(PedLocation.x - Width / 2, PedCenterY + Height / 2) + Padding[2];
					ImVec2 Size = ImVec2(Width, 2);
					ImVec2 DrawSize = ImVec2((Width / MaxHealth) * Health, 2);

					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.HealthBarColor[3]));
					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + DrawSize, FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.HealthBarColor));

					Padding[2].y += 3;
				}

				if (g_Options.Visuals.Players.HealthBarType == 3)
				{
					ImVec2 DrawPos = ImVec2(PedLocation.x, PedCenterY - Height / 2);
					DrawPos = ImVec2(DrawPos.x - Width / 2, DrawPos.y + Padding[2].y);
					ImVec2 Size = ImVec2(Width, 2);
					ImVec2 DrawSize = ImVec2((Width / MaxHealth) * Health, 2);

					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.HealthBarColor[3]));
					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + Size + ImVec2(3, 0), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.HealthBarColor[3]));
					ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos + Size, DrawPos + Size - DrawSize, FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.HealthBarColor));

					Padding[2].y += 5;
				}
			}
			if (g_Options.Visuals.Players.AmorBar)
			{
				float Armor = Current.StaticInfo.Ped->GetArmor();
				float MaxArmor = 100;

				if (g_Options.Visuals.Players.AmorBarType == 0)
				{
					ImVec2 DrawPos = ImVec2(PedLocation.x - Width / 2, PedCenterY - Height / 2) - Padding[3] - ImVec2(8, 0);
					ImVec2 Size = ImVec2(2, Height);
					ImVec2 DrawSize = ImVec2(0, (Height / MaxArmor) * Armor);

					if (Armor > 0)
					{
						ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.ArmorBarColor[3]));
						ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + ImVec2(2, Height), FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.ArmorBarColor));
					}

					Padding[3].x += 3;
				}

				if (g_Options.Visuals.Players.AmorBarType == 1)
				{
					ImVec2 DrawPos = ImVec2(PedLocation.x + Width / 2, PedCenterY - Height / 2) + Padding[1] + ImVec2(6, 0);
					ImVec2 Size = ImVec2(2, Height);
					ImVec2 DrawSize = ImVec2(0, (Height / MaxArmor) * Armor);

					if (Armor > 0)
					{
						ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.ArmorBarColor[3]));
						ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + ImVec2(2, Height), FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.ArmorBarColor));
					}
					Padding[1].x += 3;
				}

				if (g_Options.Visuals.Players.AmorBarType == 2)
				{
					ImVec2 DrawPos = ImVec2(PedLocation.x - Width / 2, PedCenterY + Height / 2) + Padding[2];
					ImVec2 Size = ImVec2(Width, 2);
					ImVec2 DrawSize = ImVec2((Width / MaxArmor) * Armor, 2);

					if (Armor > 0)
					{
						ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.ArmorBarColor[3]));
						ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + DrawSize, FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.ArmorBarColor));
					}
					Padding[2].y += 3;
				}

				if (g_Options.Visuals.Players.AmorBarType == 3)
				{
					ImVec2 DrawPos = ImVec2(PedLocation.x - Width / 2, PedCenterY - Height / 2) - Padding[0];
					ImVec2 Size = ImVec2(Width, 2);
					ImVec2 DrawSize = ImVec2((Width / MaxArmor) * Armor, 2);

					if (Armor > 0)
					{
						ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 3), DrawPos + Size + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.ArmorBarColor[3]));
						ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + DrawSize, FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.ArmorBarColor));
					}
					Padding[0].y += 3;
				}
			}

			if (g_Options.Visuals.Players.Name)
			{
				std::string Name = Current.StaticInfo.Name;
				if (Current.StaticInfo.bIsNPC)
					Name = XorStr("NPC");

				ImVec2 TextSize = ImGui::CalcTextSize(Name.c_str());

				ImVec2 DrawPos = ImVec2(PedLocation.x, PedCenterY + Height / 2);
				DrawPos = ImVec2(DrawPos.x - TextSize.x / 2, DrawPos.y + Padding[2].y);
				Padding[2].y += TextSize.y;

				ImGui::PushFont(FrameWork::Assets::InterBold12);

				ImGui::GetBackgroundDrawList()->AddText(DrawPos + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, 1.f), Name.c_str());
				ImGui::GetBackgroundDrawList()->AddText(DrawPos, ImColor(255, 255, 255), Name.c_str());

				ImGui::PopFont();
			}

			if (g_Options.Visuals.Players.WeaponName)
			{
				CWeaponManager* WeaponManager = Current.StaticInfo.Ped->GetWeaponManager();
				if (WeaponManager)
				{
					CWeaponInfo* WeaponInfo = WeaponManager->GetWeaponInfo();
					if (WeaponInfo)
					{
						std::string WeaponName = WeaponInfo->GetWeaponName();
						if (WeaponName.size() > 0)
						{
							ImVec2 TextSize = ImGui::CalcTextSize(WeaponName.c_str());

							ImVec2 DrawPos = ImVec2(PedLocation.x, PedCenterY + Height / 2);
							DrawPos = ImVec2(DrawPos.x - TextSize.x / 2, DrawPos.y + Padding[2].y - 3);
							Padding[2].y += TextSize.y;

							ImGui::PushFont(FrameWork::Assets::InterBold12);

							ImGui::GetBackgroundDrawList()->AddText(DrawPos + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_Options.Visuals.Players.TextColor[3]), WeaponName.c_str());
							ImGui::GetBackgroundDrawList()->AddText(DrawPos, FrameWork::Misc::Float4ToImColor(g_Options.Visuals.Players.TextColor), WeaponName.c_str());

							ImGui::PopFont();
						}
					}
				}
			}

		}
	}
}