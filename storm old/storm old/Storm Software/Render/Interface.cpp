#include "Interface.hpp"
#include "../Definations/Cheat.hpp"
#include "Assets/Data/FontAwesome.hpp"
#include "CustomWidgets/Notify.hpp"

int CurrentTab = 0;
int CurrentSubTab = 0;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace FrameWork
{
	void Interface::Initialize(HWND Window, HWND TargetWindow, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
	{
		hWindow = Window;
		IDevice = Device;

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hWindow);
		ImGui_ImplDX11_Init(Device, DeviceContext);
	}

	void Interface::UpdateStyle()
	{
		ImGuiStyle* Style = &ImGui::GetStyle();
		{
			Style->WindowRounding = 7;
			Style->WindowBorderSize = 1;
			Style->WindowShadowSize = 0;
			Style->WindowPadding = ImVec2(0, 0);
			Style->ScrollbarSize = 8;

			Style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 0);
			Style->Colors[ImGuiCol_SeparatorActive] = ImColor(0, 0, 0, 0);
			Style->Colors[ImGuiCol_SeparatorHovered] = ImColor(0, 0, 0, 0);
			Style->Colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
			Style->Colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 0);
			Style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 0);

			Style->Colors[ImGuiCol_WindowBg] = ImColor(16, 16, 16);
			Style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);
			Style->Colors[ImGuiCol_Border] = ImColor(23, 24, 25);
			Style->Colors[ImGuiCol_Text] = ImColor(1.f, 1.f, 1.f, 0.8f);
			Style->Colors[ImGuiCol_TextSelectedBg] = ImColor(138, 43, 226, 100);
		}
		FrameWork::Assets::Initialize(FrameWork::Overlay::dxGetDevice());
	}

	void Interface::RenderGui()
	{
		if (!bIsMenuOpen)
			return;

		ImGui::SetNextWindowSize(ImVec2(650, 490));
		ImGui::Begin(XorStr("Storm"), &bIsMenuOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			ImDrawList* DrawList = ImGui::GetWindowDrawList();
			ImVec2 Pos = ImGui::GetWindowPos();
			ImVec2 Size = ImGui::GetWindowSize();
			ImGuiStyle* Style = &ImGui::GetStyle();

			DrawList->AddRectFilled(Pos, Pos + ImVec2(Size.x / 4.f, Size.y), ImColor(16, 16, 16, (int)(ImGui::GetStyle().Alpha * 255)), ImGui::GetStyle().WindowRounding, ImDrawFlags_RoundCornersLeft);
			DrawList->AddLine(Pos + ImVec2(Size.x / 4.f, 0), Pos + ImVec2(Size.x / 4.f, Size.y), ImGui::GetColorU32(ImGuiCol_Border));

			ImGui::BeginChild(XorStr("LeftChild"), ImVec2(Size.x / 3.7f, Size.y));
			{
				ImGui::SetCursorPos(ImVec2(15, Size.y / 7.0f + 40));
				ImGui::BeginGroup();
				{
					ImVec2 logoSize = ImVec2(125.f, 105.f);
					ImVec2 cursorPos = ImGui::GetCursorScreenPos();
					ImVec2 padding = ImVec2(10.f, 10.f);

					float windowWidth = ImGui::GetWindowSize().x;

					float offsetX = -20.f;
					float offsetY = -95.f;

					float centeredX = cursorPos.x + (windowWidth - logoSize.x) * 0.5f + offsetX;
					float posY = cursorPos.y + padding.y + offsetY;

					ImVec2 pos = ImVec2(centeredX, posY);

					ImGui::GetWindowDrawList()->AddImage(FrameWork::Assets::Logo, pos, pos + logoSize);

					ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, pos.y + logoSize.y + 10.f));

					ImVec2 tabStartPos = ImGui::GetCursorScreenPos();

					if (ImGui::Tab(XorStr("Aimbot"), ICON_FA_CROSSHAIRS, CurrentTab == 0)) {
						CurrentTab = 0;
						CurrentSubTab = 0;
					}
					ImGui::SetCursorScreenPos(ImVec2(tabStartPos.x, ImGui::GetCursorScreenPos().y + 3.f));
					if (ImGui::Tab(XorStr("Visuals"), ICON_FA_EYE, CurrentTab == 1)) {
						CurrentTab = 1;
						CurrentSubTab = 0;
					}
					ImGui::SetCursorScreenPos(ImVec2(tabStartPos.x, ImGui::GetCursorScreenPos().y + 3.f));
					if (ImGui::Tab(XorStr("Vehicles"), ICON_FA_CAR, CurrentTab == 2)) {
						CurrentTab = 2;
						CurrentSubTab = 0;
					}
					ImGui::SetCursorScreenPos(ImVec2(tabStartPos.x, ImGui::GetCursorScreenPos().y + 3.f));
					if (ImGui::Tab(XorStr("Players"), ICON_FA_USERS, CurrentTab == 3)) {
						CurrentTab = 3;
						CurrentSubTab = 0;
					}
					ImGui::SetCursorScreenPos(ImVec2(tabStartPos.x, ImGui::GetCursorScreenPos().y + 3.f));
					if (ImGui::Tab(XorStr("Exploits"), ICON_FA_BUG, CurrentTab == 4)) {
						CurrentTab = 4;
						CurrentSubTab = 0;
					}
					ImGui::SetCursorScreenPos(ImVec2(tabStartPos.x, ImGui::GetCursorScreenPos().y + 3.f));
					if (ImGui::Tab(XorStr("Configs"), ICON_FA_FOLDER, CurrentTab == 5)) {
						CurrentTab = 5;
						CurrentSubTab = 0;
					}
				}
				ImGui::EndGroup();
			}
			ImGui::EndChild();

			static float TabAlpha = 0.f; static float TabAdd; static int ActiveTab = 0;

			TabAlpha = ImClamp(TabAlpha + (4.f * ImGui::GetIO().DeltaTime * (CurrentTab == ActiveTab ? 1.f : -1.f)), 0.f, 1.f);

			if (TabAlpha == 0.f && TabAdd == 0.f) ActiveTab = CurrentTab;

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, TabAlpha * Style->Alpha);

			ImGui::SetCursorPos(ImVec2(Size.x / 3.7f, 0));
			ImGui::BeginChild(XorStr("MainChild"), ImVec2(Size.x - (Size.x / 3.7f), Size.y), ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			{
				if (ActiveTab == 0 )
				{
					ImGui::SetCursorPos(ImVec2(15, 20));
					ImGui::BeginGroup();
					{
						const char* tabLabels[] = { XorStr("Aimbot"), XorStr("Silent"), XorStr("Trigger") };
						const int tabCount = 3;
						float totalTabWidth = 0;
						float tabSpacing = 8.0f;

						float tabWidths[3];
						for (int i = 0; i < tabCount; i++) {
							auto textSize = FrameWork::Misc::CalcTextSize(FrameWork::Assets::InterSemiBold, FrameWork::Assets::InterSemiBold->FontSize, tabLabels[i]);
							tabWidths[i] = textSize.x + 40;
							totalTabWidth += tabWidths[i];
							if (i < tabCount - 1) totalTabWidth += tabSpacing;
						}

						float windowWidth = ImGui::GetContentRegionAvail().x;
						float startX = (windowWidth - totalTabWidth) * 0.5f;

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + startX);

						if (ImGui::SubTab(XorStr("Aimbot"), 0 == CurrentSubTab))
							CurrentSubTab = 0;
						ImGui::SameLine(0, tabSpacing);
						if (ImGui::SubTab(XorStr("Silent"), 1 == CurrentSubTab))
							CurrentSubTab = 1;
						ImGui::SameLine(0, tabSpacing);
						if (ImGui::SubTab(XorStr("Trigger"), 2 == CurrentSubTab))
							CurrentSubTab = 2;

						ImGui::SetCursorPos(ImVec2(0, 65));
						ImGui::BeginChild(XorStr("AimbotConfig"));
						{
							if (CurrentSubTab == 0)
							{
								ImGui::SetCursorPos(ImVec2(5, 0));
								ImGui::BeginGroup();
								{
									ImGui::CustomChild(XorStr("Aimbot"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
									{
										ImGui::Checkbox(XorStr("Enabled"), &g_Options.LegitBot.AimBot.Enabled);
										ImGui::SameLine();
										ImGui::KeyBind(XorStr(" "), &g_Options.LegitBot.AimBot.KeyBind, &g_Options.LegitBot.AimBot.KeyBindState);
										ImGui::Checkbox(XorStr("Visible Check"), &g_Options.LegitBot.AimBot.VisibleCheck);
										ImGui::Checkbox(XorStr("Target Deads"), &g_Options.Visuals.Players.ExcludeDeads);
										ImGui::Checkbox(XorStr("Target NPC"), &g_Options.LegitBot.AimBot.TargetNPC);
										ImGui::Checkbox(XorStr("Show Fov"), &g_Options.LegitBot.AimBot.ShowFov);
										ImGui::Combo(XorStr("Hitbox"), &g_Options.LegitBot.AimBot.HitBox, XorStr("Head\0Neck\0Chest\0"), -1);
									}
									ImGui::EndCustomChild();

									ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
									ImGui::BeginGroup();
									{
										ImGui::CustomChild(XorStr("Settings"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
										{
											ImGui::SliderInt(XorStr("FOV"), &g_Options.LegitBot.AimBot.FOV, 0, 300, "%d");
											ImGui::SliderInt(XorStr("Max Distance"), &g_Options.LegitBot.AimBot.MaxDistance, 0, 600, "%d");
											ImGui::SliderInt(XorStr("Smooth X"), &g_Options.LegitBot.AimBot.SmoothHorizontal, 0, 100, "%d");
											ImGui::SliderInt(XorStr("Smooth Y"), &g_Options.LegitBot.AimBot.SmoothVertical, 0, 100, "%d");
											ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Priority By"));
											ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
											ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 135);
											ImGui::Combo(XorStr(""), &g_Options.LegitBot.AimBot.ClosestFov, XorStr("FOV\0Distance\0Health\0"), -1);
										}
										ImGui::EndCustomChild();
									}
									ImGui::EndGroup();
								}
								ImGui::EndGroup();
							}
							if (CurrentSubTab == 1)
							{
								ImGui::SetCursorPos(ImVec2(5, 0));
								ImGui::BeginGroup();
								{
									ImGui::CustomChild(XorStr("Silent"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
									{
										ImGui::Checkbox(XorStr("Enabled"), &g_Options.LegitBot.SilentAim.Enabled);
										ImGui::SameLine();
										ImGui::KeyBind(XorStr(" "), &g_Options.LegitBot.SilentAim.KeyBind, &g_Options.LegitBot.SilentAim.KeyBindState);
										ImGui::Checkbox(XorStr("Magic Bullets"), &g_Options.LegitBot.SilentAim.MagicBullet);
										ImGui::Checkbox(XorStr("Humanize Bullet"), &g_Options.LegitBot.SilentAim.Humanize);
										ImGui::Checkbox(XorStr("Visible Check"), &g_Options.LegitBot.SilentAim.VisibleCheck);
										ImGui::Checkbox(XorStr("Target Deads"), &g_Options.Visuals.Players.ExcludeDeads);
										ImGui::Checkbox(XorStr("Target NPC"), &g_Options.LegitBot.SilentAim.ShotNPC);
										ImGui::Checkbox(XorStr("Show Fov"), &g_Options.LegitBot.SilentAim.ShowFOV);
										ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("HitBox"));
										ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
										ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 135);
										ImGui::Combo(XorStr(""), &g_Options.LegitBot.SilentAim.HitBox, XorStr("Head\0Neck\0Chest\0"), -1);
									}
									ImGui::EndCustomChild();

									ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
									ImGui::BeginGroup();
									{
										ImGui::CustomChild(XorStr("Settings"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
										{
											ImGui::SliderInt(XorStr("FOV"), &g_Options.LegitBot.SilentAim.Fov, 0, 300, "%d");
											ImGui::SliderInt(XorStr("Max Distance"), &g_Options.LegitBot.SilentAim.MaxDistance, 0, 600, "%d");
											ImGui::SliderInt(XorStr("Miss Chance"), &g_Options.LegitBot.SilentAim.MissChance, 0, 50, "%d");
											ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Priority By"));
											ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
											ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 135);
											ImGui::Combo(XorStr(""), &g_Options.LegitBot.SilentAim.ClosestFov, XorStr("FOV\0Distance\0Health\0"), -1);
										}
										ImGui::EndCustomChild();
									}
									ImGui::EndGroup();
								}
								ImGui::EndGroup();
							}
							if (CurrentSubTab == 2)
							{
								ImGui::SetCursorPos(ImVec2(5, 0));
								ImGui::BeginGroup();
								{
									ImGui::CustomChild(XorStr("Trigger"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
									{
										ImGui::Checkbox(XorStr("Enabled"), &g_Options.LegitBot.Trigger.Enabled);
										ImGui::SameLine();
										ImGui::KeyBind(XorStr(" "), &g_Options.LegitBot.Trigger.KeyBind, &g_Options.LegitBot.Trigger.KeyBindState);
										ImGui::Checkbox(XorStr("Allow NPC"), &g_Options.LegitBot.Trigger.ShotNPC);
										ImGui::Checkbox(XorStr("Visible Check"), &g_Options.LegitBot.Trigger.VisibleCheck);
									}
									ImGui::EndCustomChild();

									ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
									ImGui::BeginGroup();
									{
										ImGui::CustomChild(XorStr("Settings"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
										{
											ImGui::SliderInt(XorStr("Max Distance"), &g_Options.LegitBot.Trigger.MaxDistance, 0, 600, "%d");
											ImGui::SliderInt(XorStr("Reaction Time"), &g_Options.LegitBot.Trigger.ReactionTime, 0, 50, "%d");
										}
										ImGui::EndCustomChild();
									}
									ImGui::EndGroup();
								}
								ImGui::EndGroup();
							}
						}
						ImGui::EndChild();
					}
					ImGui::EndGroup();
				}

				if (ActiveTab == 1)
				{
					ImGui::SetCursorPos(ImVec2(15, 20));
					ImGui::BeginGroup();
					{
						ImGui::SetCursorPos(ImVec2(15, 20));
						ImGui::BeginGroup();
						{
							const char* tabLabels[] = { XorStr("Player"), XorStr("Vehicle"), XorStr("Color") };
							const int tabCount = 3;
							float totalTabWidth = 0;
							float tabSpacing = 8.0f;

							float tabWidths[3];
							for (int i = 0; i < tabCount; i++) {
								auto textSize = FrameWork::Misc::CalcTextSize(FrameWork::Assets::InterSemiBold, FrameWork::Assets::InterSemiBold->FontSize, tabLabels[i]);
								tabWidths[i] = textSize.x + 40;
								totalTabWidth += tabWidths[i];
								if (i < tabCount - 1) totalTabWidth += tabSpacing;
							}

							float windowWidth = ImGui::GetContentRegionAvail().x;
							float startX = (windowWidth - totalTabWidth) * 0.5f;

							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + startX);

							if (ImGui::SubTab(XorStr("Player"), 0 == CurrentSubTab))
								CurrentSubTab = 0;
							ImGui::SameLine(0, tabSpacing);
							if (ImGui::SubTab(XorStr("Vehicle"), 1 == CurrentSubTab))
								CurrentSubTab = 1;
							ImGui::SameLine(0, tabSpacing);
							if (ImGui::SubTab(XorStr("Color"), 2 == CurrentSubTab))
								CurrentSubTab = 2;

							ImGui::SetCursorPos(ImVec2(0, 65));
							ImGui::BeginChild(XorStr("Visuals"));
							{
								if (CurrentSubTab == 0)
								{
									ImGui::SetCursorPos(ImVec2(5, 0));
									ImGui::BeginGroup();
									{
										ImGui::CustomChild(XorStr("Player"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
										{
											ImGui::Checkbox(XorStr("Enabled"), &g_Options.Visuals.Players.Enabled);
											ImGui::SameLine();
											ImGui::KeyBind(XorStr(" "), &g_Options.Visuals.Players.ToggleKey, &g_Options.Visuals.Players.ToggleKeyState);
											ImGui::Checkbox(XorStr("Bind ESP"), &g_Options.Visuals.Players.Toogle);
											ImGui::Checkbox(XorStr("Show NPC"), &g_Options.Visuals.Players.ShowNPC);
											ImGui::Checkbox(XorStr("Show Deads"), &g_Options.Visuals.Players.ExcludeDeads);
											ImGui::Checkbox(XorStr("Show LocalPlayer"), &g_Options.Visuals.Players.ShowLocalPlayer);
											ImGui::Checkbox(XorStr("Visible Check"), &g_Options.Visuals.Players.VisibleCheck);
											ImGui::SliderInt(XorStr("Render Distance"), &g_Options.Visuals.Players.RenderDistance, 0, 500, "%d");
										}
										ImGui::EndCustomChild();

										ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
										ImGui::BeginGroup();
										{
											ImGui::CustomChild(XorStr("Settings"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
											{
												ImGui::Checkbox(XorStr("Enable Box"), &g_Options.Visuals.Players.EnableBox);
												if (g_Options.Visuals.Players.EnableBox)
												{
													ImGui::Combo(XorStr("Box"), &g_Options.Visuals.Players.BoxType, XorStr("Normal\0Corner\0"), -1);
												}
												ImGui::Checkbox(XorStr("Enable Distance"), &g_Options.Visuals.Players.EnableDistance);

												ImGui::Checkbox(XorStr("Head Circle"), &g_Options.Visuals.Players.EnableHeadBol);

												ImGui::Checkbox(XorStr("Radar"), &g_Options.Visuals.Players.RadarEnabled);

												ImGui::Checkbox(XorStr("Enable Skeleton"), &g_Options.Visuals.Players.Skeleton);

												ImGui::Checkbox(XorStr("Enable HealthBar"), &g_Options.Visuals.Players.HealthBar);
												if (g_Options.Visuals.Players.HealthBar)
												{
													ImGui::Combo(XorStr("HealthBar"), &g_Options.Visuals.Players.HealthBarType, XorStr("Left\0Right\0Bottom\0Top"), -1);
												}

												ImGui::Checkbox(XorStr("Enable ArmorBar"), &g_Options.Visuals.Players.AmorBar);
												if (g_Options.Visuals.Players.AmorBar)
												{
													ImGui::Combo(XorStr("ArmorBar"), &g_Options.Visuals.Players.AmorBarType, XorStr("Left\0Right\0Bottom\0Top"), -1);
												}

												ImGui::Checkbox(XorStr("Enable WeaponName"), &g_Options.Visuals.Players.WeaponName);

												ImGui::Checkbox(XorStr("Enable Names"), &g_Options.Visuals.Players.Name);
											}
											ImGui::EndCustomChild();
										}
										ImGui::EndGroup();
									}
									ImGui::EndGroup();
								}
								if (CurrentSubTab == 1)
								{
									ImGui::SetCursorPos(ImVec2(5, 0));
									ImGui::BeginGroup();
									{
										ImGui::CustomChild(XorStr("Vehicle"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
										{
											ImGui::Checkbox(XorStr("Enabled"), &g_Options.Visuals.Vehicles.Enabled);
											ImGui::SameLine();
											ImGui::KeyBind(XorStr(" "), &g_Options.Visuals.Vehicles.ToggleKey, &g_Options.Visuals.Vehicles.ToggleKeyState);
											ImGui::Checkbox(XorStr("Bind ESP"), &g_Options.Visuals.Vehicles.Toogle);
											ImGui::Checkbox(XorStr("Show Lock State"), &g_Options.Visuals.Vehicles.ShowLockState);
										}
										ImGui::EndCustomChild();

										ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
										ImGui::BeginGroup();
										{
											ImGui::CustomChild(XorStr("Settings"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
											{
												ImGui::Checkbox(XorStr("Enable Name"), &g_Options.Visuals.Vehicles.Name);
												ImGui::Checkbox(XorStr("Enable Distance"), &g_Options.Visuals.Vehicles.Distance);
												ImGui::Checkbox(XorStr("Enable Marker"), &g_Options.Visuals.Vehicles.Marker);
											}
											ImGui::EndCustomChild();
										}
										ImGui::EndGroup();
									}
									ImGui::EndGroup();
								}
								if (CurrentSubTab == 2)
								{
									ImGui::SetCursorPos(ImVec2(5, 0));
									ImGui::BeginGroup();
									{
										ImGui::CustomChild(XorStr("Player Color"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
										{
											ImGui::ColorEdit4(XorStr("Visible Box Color"), g_Options.Visuals.Players.BoxColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
											ImGui::ColorEdit4(XorStr("Invisible Box Color"), g_Options.Visuals.Players.InvisibleVisibleBoxColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
											ImGui::ColorEdit4(XorStr("Visible Skeleton Color"), g_Options.Visuals.Players.SkeletonColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
											ImGui::ColorEdit4(XorStr("Invisible Skeleton Color"), g_Options.Visuals.Players.InvisibleSkeletonColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
											ImGui::ColorEdit4(XorStr("HealthBar Color"), g_Options.Visuals.Players.HealthBarColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
											ImGui::ColorEdit4(XorStr("HeadCircle Color"), g_Options.Visuals.Players.HeadCircleColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
										}
										ImGui::EndCustomChild();

										ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
										ImGui::BeginGroup();
										{
											ImGui::CustomChild(XorStr("Vehicle Color"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
											{
												ImGui::ColorEdit4(XorStr("Marker Color"), g_Options.Visuals.Vehicles.MarkerColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
												ImGui::ColorEdit4(XorStr("Distance Color"), g_Options.Visuals.Vehicles.DistanceColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
												ImGui::ColorEdit4(XorStr("Text's Color"), g_Options.Visuals.Vehicles.NameColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);
											}
											ImGui::EndCustomChild();
										}
										ImGui::EndGroup();
									}
									ImGui::EndGroup();
								}
								ImGui::EndGroup();
							}
						}
					}
					ImGui::EndGroup();
				}

				if (ActiveTab == 2)
				{
					ImGui::SetCursorPos(ImVec2(15, 20));
					ImGui::BeginGroup();
					{
						ImGui::SetCursorPos(ImVec2(0, 65));
						ImGui::BeginChild(XorStr("RadarList"));
						{
							if (CurrentSubTab == 0)
							{
								ImGui::SetCursorPos(ImVec2(10, 0));
								ImGui::BeginGroup();
								{
									static ImGuiTextFilter mFilter;
									static Cheat::VehicleInfo SelectedVehicle = Cheat::VehicleInfo();

									if (SelectedVehicle.Vehicle->GetCoordinate().DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos) > 600)
										SelectedVehicle = Cheat::VehicleInfo();

									ImGui::CustomChild(XorStr("List"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
									{
										if (Cheat::g_Fivem.GetLocalPlayerInfo().Ped)
										{
											for (Cheat::VehicleInfo Current : Cheat::g_Fivem.GetVehicleList())
											{
												if (mFilter.PassFilter(Current.Name.c_str()))
												{
													std::string Text = Current.Name + (" (") + std::to_string((int)(Current.Vehicle->GetCoordinate().DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos))) + ("m)");

													bool Selected = (Current.Vehicle == SelectedVehicle.Vehicle);

													if (ImGui::Selectable(Text.c_str(), &Selected))
													{
														SelectedVehicle = Current;
													}
												}
											}
										}
									}
									ImGui::EndCustomChild();

									ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
									ImGui::BeginGroup();
									{
										Vector3D limbo = { 0.0f, 0.0f, 0.0f };

										ImGui::CustomChild(XorStr("Actions"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 250));
										{
											if (SelectedVehicle.iIndex > 0)
											{
												if (ImGui::Button(XorStr("Teleport to"), ImVec2(200, 30)))
												{
													Cheat::g_Fivem.TeleportToObject((uintptr_t)Cheat::g_Fivem.GetLocalPlayerInfo().Ped, Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetNavigation(), Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo(), SelectedVehicle.Vehicle->GetCoordinate(), SelectedVehicle.Vehicle->GetCoordinate(), true);
												}

												if (ImGui::Button(XorStr("Unlock door"), ImVec2(200, 30)))
												{
													SelectedVehicle.Vehicle->SetLockState(CARLOCK_UNLOCKED);
												}
												if (ImGui::Button(XorStr("Lock door"), ImVec2(200, 30)))
												{
													SelectedVehicle.Vehicle->SetLockState(CARLOCK_LOCKED);
												}
												if (ImGui::Button(XorStr("Fuck Engine"), ImVec2(200, 30)))
												{
													SelectedVehicle.Vehicle->FuckVehicleEngine();
												}
												if (ImGui::Button(XorStr("Fix Engine"), ImVec2(200, 30)))
												{
													SelectedVehicle.Vehicle->FixEngine();
												}
												if (ImGui::Button(XorStr("Teleport to Void"), ImVec2(200, 30)))
												{
													Cheat::g_Fivem.TeleportToObject((uintptr_t)SelectedVehicle.Vehicle, SelectedVehicle.Vehicle->GetNavigation(), SelectedVehicle.Vehicle->GetModelInfo(), SelectedVehicle.Vehicle->GetCoordinate(), limbo, true);
												}
											}
											else
											{
												ImGui::Text(XorStr("Select a vehicle first!"));
											}
										}
										ImGui::EndCustomChild();

										ImGui::CustomChild(XorStr("Info"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 150));
										{
											if (SelectedVehicle.iIndex > 0)
											{
												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Name: ")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), SelectedVehicle.Name.c_str());
												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Index: ")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), std::to_string(SelectedVehicle.iIndex).c_str());
												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Distance: ")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), (std::to_string((int)(SelectedVehicle.Vehicle->GetCoordinate().DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos))) + ("m")).c_str());

												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Driver: "));

												std::string DriverName;

												uint64_t Driver = 0;

												if (Driver)
												{
													DriverName = Cheat::g_Fivem.GetPlayerName(Driver, SelectedVehicle.iIndex);
												}
												else
												{
													DriverName = XorStr("None");
												}

												ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), DriverName.c_str());
											}
											else
											{
												ImGui::Text(XorStr("Select a vehicle first!"));
											}
										}
										ImGui::EndCustomChild();
									}
									ImGui::EndGroup();
								}
								ImGui::EndGroup();
							}
						}
						ImGui::EndChild();
					}
					ImGui::EndGroup();
				}

				if (ActiveTab == 3)
				{
					ImGui::SetCursorPos(ImVec2(15, 20));
					ImGui::BeginGroup();
					{
						ImGui::SetCursorPos(ImVec2(0, 65));
						ImGui::BeginChild(XorStr("RadarList"));
						{
							if (CurrentSubTab == 0)
							{
								ImGui::SetCursorPos(ImVec2(10, 0));
								ImGui::BeginGroup();
								{
									static Cheat::Entity SelectedEntity = Cheat::Entity();
									static ImGuiTextFilter mFilter;

									ImGui::CustomChild(XorStr("List"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
									{
										if (Cheat::g_Fivem.GetLocalPlayerInfo().Ped)
										{
											for (Cheat::Entity Current : Cheat::g_Fivem.GetEntitiyList())
											{
												if (Current.StaticInfo.bIsNPC || !Current.StaticInfo.Ped || Current.StaticInfo.bIsLocalPlayer)
													continue;

												if (mFilter.PassFilter(Current.StaticInfo.Name.c_str()))
												{
													ImGui::PushID(Current.StaticInfo.iIndex);
													{
														std::string Text = Current.StaticInfo.Name + XorStr(" (") + std::to_string((int)(Current.Cordinates.DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos))) + XorStr("m)");

														if (Current.StaticInfo.IsFriend)
															Text += XorStr(" (friend)");

														bool Selected = (Current.StaticInfo.Ped == SelectedEntity.StaticInfo.Ped);

														if (ImGui::Selectable(Text.c_str(), &Selected))
														{
															SelectedEntity = Current;
														}

														if (Selected)
															ImGui::SetItemDefaultFocus();
													}
													ImGui::PopID();
												}
											}
										}
									}
									ImGui::EndCustomChild();

									ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
									ImGui::BeginGroup();
									{
										Vector3D limbo = { 0.0f, 0.0f, 0.0f };

										ImGui::CustomChild(XorStr("Actions"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 250));
										{
											if (SelectedEntity.StaticInfo.iIndex > 0)
											{
												if (ImGui::Button(XorStr("Teleport to"), ImVec2(200, 30)))
												{
													Cheat::g_Fivem.TeleportToObject((uintptr_t)Cheat::g_Fivem.GetLocalPlayerInfo().Ped,
														Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetNavigation(),
														Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo(),
														SelectedEntity.Cordinates,
														SelectedEntity.Cordinates,
														true);
												}
												if (ImGui::Button(XorStr("Steal Outfit"), ImVec2(200, 30)))
												{
													Cheat::Exploits::SetLocalPlayerColthes(SelectedEntity.StaticInfo.Ped);
												}
												if (!SelectedEntity.StaticInfo.IsFriend)
												{
													if (ImGui::Button(XorStr("Add Friend"), ImVec2(200, 30)))
													{
														if (!Cheat::g_Fivem.AllEntitesList[SelectedEntity.StaticInfo.Ped].IsFriend || !SelectedEntity.StaticInfo.IsFriend)
															Cheat::g_Fivem.FriendList[SelectedEntity.StaticInfo.Ped->GetPlayerInfo()->GetPlayerID()] = SelectedEntity.StaticInfo;

														Cheat::g_Fivem.AllEntitesList[SelectedEntity.StaticInfo.Ped].IsFriend = true;
														SelectedEntity.StaticInfo.IsFriend = true;
													}
												}
												else
												{
													if (ImGui::Button(XorStr("Remove Friend"), ImVec2(200, 30)))
													{
														if (Cheat::g_Fivem.AllEntitesList[SelectedEntity.StaticInfo.Ped].IsFriend || SelectedEntity.StaticInfo.IsFriend)
															Cheat::g_Fivem.FriendList.erase(SelectedEntity.StaticInfo.Ped->GetPlayerInfo()->GetPlayerID());

														Cheat::g_Fivem.AllEntitesList[SelectedEntity.StaticInfo.Ped].IsFriend = false;
														SelectedEntity.StaticInfo.IsFriend = false;
													}
												}
											}
											else
											{
												ImGui::Text(XorStr("Select a player first!"));
											}
										}
										ImGui::EndCustomChild();

										ImGui::CustomChild(XorStr("Info"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 150));
										{
											if (SelectedEntity.StaticInfo.iIndex > 0)
											{
												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Name: ")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), SelectedEntity.StaticInfo.Name.c_str());
												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Index: ")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), std::to_string(SelectedEntity.StaticInfo.iIndex).c_str());
												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("NetID: ")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), std::to_string(SelectedEntity.StaticInfo.NetId).c_str());
												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Distance: ")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), (std::to_string((int)(SelectedEntity.Cordinates.DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos))) + ("m")).c_str());

												Cheat::CWeaponManager* WeaponManager = SelectedEntity.StaticInfo.Ped->GetWeaponManager();
												if (WeaponManager)
												{
													Cheat::CWeaponInfo* WeaponInfo = WeaponManager->GetWeaponInfo();
													if (WeaponInfo)
													{
														ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Weapon: ")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), WeaponInfo->GetWeaponName().c_str());
													}
												}
											}
											else
											{
												ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Select A Player First."));
											}
										}
										ImGui::EndCustomChild();
									}
									ImGui::EndGroup();
								}
								ImGui::EndGroup();
							}
						}
						ImGui::EndChild();
					}
					ImGui::EndGroup();
				}

				if (ActiveTab == 4)
				{
					ImGui::SetCursorPos(ImVec2(15, 20));
					ImGui::BeginGroup();
					{
						ImGui::SetCursorPos(ImVec2(0, 65));
						ImGui::BeginChild(XorStr("Misc"));
						{
							if (CurrentSubTab == 0)
							{
								ImGui::SetCursorPos(ImVec2(5, 0));
								ImGui::BeginGroup();
								{
									ImGui::CustomChild(XorStr("Local Player"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
									{
										if (ImGui::Button(XorStr("Revive"), ImVec2(200, 30)))
										{
											Cheat::Exploits::ReviveThread();
										}
										if (ImGui::Button(XorStr("Suicide"), ImVec2(200, 30)))
										{
											Cheat::Exploits::SuicideThread();
										}
										struct cLocations
										{
											std::string Name;
											Vector3D Coords;
										};

										std::vector<cLocations> Locations =
										{
											cLocations(XorStr("Waypoint"), Vector3D(0, 0, 0)),
										};

										static int SeletedIndex = 0;
										auto Location = Locations[SeletedIndex];
										if (ImGui::Button(XorStr("Teleport Waypoint"), ImVec2(200, 30)))
										{
											if (SeletedIndex == 0)
											{
												Cheat::Exploits::TeleportToWaypoint();
											}
											else
											{
												Cheat::g_Fivem.TeleportToObject((uintptr_t)Cheat::g_Fivem.GetLocalPlayerInfo().Ped, Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetNavigation(), Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo(), Location.Coords, Location.Coords, true);
											}
										}
										ImGui::Checkbox(XorStr("Invisible"), &g_Options.Exploits.Self.Invisible);
										ImGui::Checkbox(XorStr("Ant HeadShot"), &g_Options.Exploits.Self.AntHS);
										ImGui::Checkbox(XorStr("God Mode"), &g_Options.Exploits.Self.GodMode);
										if (g_Options.Exploits.Self.GodMode)
										{
											ImGui::SameLine();
											ImGui::KeyBind(XorStr(" "), &g_Options.Exploits.Self.GodKey, &g_Options.Exploits.Self.GodKeyState);
										}
										ImGui::Checkbox(XorStr("Replay"), &g_Options.Exploits.Self.ReplayTP);
										if (g_Options.Exploits.Self.ReplayTP)
										{
											ImGui::SameLine();
											ImGui::KeyBind(XorStr(" "), &g_Options.Exploits.Self.ReplayTPBind, &g_Options.Exploits.Self.ReplayTPBindState);
										}
										ImGui::Checkbox(XorStr("No RagDoll"), &g_Options.Exploits.Self.NoRagDoll);
										ImGui::Checkbox(XorStr("Seat Belt"), &g_Options.Exploits.Self.SeatBelt);
										ImGui::Checkbox(XorStr("Remove Spread"), &g_Options.Exploits.Self.RemoveSpread);
										ImGui::Checkbox(XorStr("Remove Recoil"), &g_Options.Exploits.Self.RemoveRecoil);
										ImGui::Checkbox(XorStr("UnLock All Vehicles"), &g_Options.Exploits.Self.UnLockAllVehicles);
										ImGui::Checkbox(XorStr("Lock All Vehicles"), &g_Options.Exploits.Self.LockAllVehicles);
										ImGui::Checkbox(XorStr("StealVehicle"), &g_Options.Exploits.Self.StealVehicle);
									}
									ImGui::EndCustomChild();

									ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
									ImGui::BeginGroup();
									ImGui::CustomChild(XorStr("Misc"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
									{
										ImGui::Checkbox(XorStr("No Clip"), &g_Options.Exploits.Self.NoClip);
										ImGui::SameLine();
										ImGui::KeyBind(XorStr(""), &g_Options.Exploits.Self.NoClipKey, &g_Options.Exploits.Self.NoClipKeyState);
										ImGui::Combo(XorStr("NoClip Modifier"), &g_Options.Exploits.Self.NoClipMode, XorStr("Normal\0Invisible\0"), -1);
										ImGui::SliderFloat(XorStr("NoClip Speed"), &g_Options.Exploits.Self.NoClipSpeed, 10.f, 500.f, "%.0f");
										ImGui::SliderFloat(XorStr("Modify Weapon Size"), &g_Options.Exploits.Self.WeaponSize, 0.5f, 50.f, "%.1f");
									}
									ImGui::EndCustomChild();


									ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
									ImGui::BeginGroup();
								}
								ImGui::EndGroup();
							}
						}
					}
					ImGui::EndGroup();
				}

				if (ActiveTab == 5)
				{
					ImGui::SetCursorPos(ImVec2(15, 20));
					ImGui::BeginGroup();
					{
						ImGui::SetCursorPos(ImVec2(0, 25));
						ImGui::BeginChild(XorStr("CloudConfig"));
						{
							if (CurrentSubTab == 0)
							{
								ImGui::SetCursorPos(ImVec2(5, 0));
								ImGui::BeginGroup();
								{
									ImGui::CustomChild(XorStr("Settings"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
									{
										ImGui::Checkbox(XorStr("Stream Mode"), &g_Options.General.CaptureBypass);
										ImGui::Checkbox(XorStr("Vsync"), &g_Options.General.Vsync);

										ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Menu Bind"));
										ImGui::SameLine();
										ImGui::KeyBind(XorStr(""), &g_Options.General.MenuKey, &g_Options.General.MenuKeyState);

										ImGui::Dummy(ImVec2(0, 5.f));
										if (ImGui::Button(XorStr("Unload"), ImVec2(200, 30))) {
											ExitProcess(0);
										}
									}
									ImGui::EndCustomChild();

									ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - 25 + 25, 0));
									ImGui::BeginGroup();
									{
										ImGui::CustomChild(XorStr("More"), ImVec2(ImGui::GetWindowSize().x / 2 - 20, 410));
										{
											ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Fivem Version:")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), std::to_string(Cheat::g_Fivem.GetGameVersion()).c_str());
											ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.8f), XorStr("Cheat Version:")); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.6f), XorStr("1.0"));
										}
										ImGui::EndCustomChild();
									}
									ImGui::EndGroup();
								}
								ImGui::EndGroup();
							}
						}
						ImGui::EndChild();
					}
					ImGui::EndGroup();
				}
			}
			ImGui::EndChild();

			ImGui::PopStyleVar();
		}
		ImGui::End();
	}

	void Interface::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED)
			{
				ResizeWidht = (UINT)LOWORD(lParam);
				ResizeHeight = (UINT)HIWORD(lParam);
			}
			break;
		}

		if (bIsMenuOpen)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		}
	}

	void Interface::HandleMenuKey()
	{
		static bool MenuKeyDown = false;
		if (GetAsyncKeyState(g_Options.General.MenuKey) & 0x8000)
		{
			if (!MenuKeyDown)
			{
				MenuKeyDown = true;
				bIsMenuOpen = !bIsMenuOpen;

				if (bIsMenuOpen)
				{
					SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT);
					SetForegroundWindow(hWindow);
				}
				else
				{
					SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_LAYERED);
					SetForegroundWindow(hTargetWindow);
				}
			}
		}
		else
		{
			MenuKeyDown = false;
		}
	}

	void Interface::ShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}