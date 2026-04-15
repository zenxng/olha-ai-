#include "Cheat.hpp"
#include "../FiveM-External.hpp"
#include "../Render/CustomWidgets/Notify.hpp"

namespace Cheat
{
	void Initialize()
	{
		while (!g_Fivem.IsInitialized())
		{
			g_Fivem.Intialize();

			if (!g_Fivem.IsInitialized())
				std::this_thread::sleep_for(std::chrono::seconds(3));
		}

		FrameWork::Overlay::Setup(g_Fivem.GetPid());
		FrameWork::Overlay::Initialize();

		std::thread([]() { while (!g_Options.General.ShutDown) { if (!g_Fivem.IsInitialized() || !FrameWork::Memory::IsProcessRunning(g_Fivem.GetPid())) { ExitProcess(0); } std::this_thread::sleep_for(std::chrono::milliseconds(10)); } }).detach();

		std::thread([&]() { AimBot::RunThread(); }).detach();
		std::thread([&]() { SilentAim::RunThread(); }).detach();
		std::thread([&]() { TriggerBot::RunThread(); }).detach();
		std::thread([&]() { NotifyManager::Send(XorStr("Initialize Cheat!"), 4000); }).detach();

		std::thread([&]()  
			{
				while (!g_Options.General.ShutDown)
				{
					Cheat::Exploits::ToggleCarLock();

					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
			}).detach();

		std::thread([&]() { 
			while (!g_Options.General.ShutDown)
			{
				Exploits::RunThread();

				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
			}).detach();

		if (FrameWork::Overlay::IsInitialized())
		{
			FrameWork::Interface Interface(FrameWork::Overlay::GetOverlayWindow(), FrameWork::Overlay::GetTargetWindow(), FrameWork::Overlay::dxGetDevice(), FrameWork::Overlay::dxGetDeviceContext());
			Interface.UpdateStyle();
			FrameWork::Overlay::SetupWindowProcHook(std::bind(&FrameWork::Interface::WindowProc, &Interface, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

			auto TargetTP = std::chrono::steady_clock::now();

			MSG Message;
			ZeroMemory(&Message, sizeof(Message));
			while (Message.message != WM_QUIT)
			{
				TargetTP += std::chrono::microseconds(std::chrono::seconds(1)) / int(144.f); std::this_thread::sleep_until(TargetTP);

				if (PeekMessage(&Message, FrameWork::Overlay::GetOverlayWindow(), NULL, NULL, PM_REMOVE))
				{
					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}

				g_Fivem.UpdateEntities();
				g_Fivem.UpdateVehicles();

				ImGui::GetIO().MouseDrawCursor = Interface.GetMenuOpen();

				if (Interface.ResizeHeight != 0 || Interface.ResizeWidht != 0)
				{
					FrameWork::Overlay::dxCleanupRenderTarget();
					FrameWork::Overlay::dxGetSwapChain()->ResizeBuffers(0, Interface.ResizeWidht, Interface.ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
					Interface.ResizeHeight = Interface.ResizeWidht = 0;
					FrameWork::Overlay::dxCreateRenderTarget();
				}

				Interface.HandleMenuKey();

				FrameWork::Overlay::UpdateWindowPos();

				static bool CaptureBypassOn = false;
				if (g_Options.General.CaptureBypass != CaptureBypassOn)
				{
					CaptureBypassOn = g_Options.General.CaptureBypass;
					SafeCall(SetWindowDisplayAffinity)(FrameWork::Overlay::GetOverlayWindow(), CaptureBypassOn ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);
				}

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
				{
					if (g_Options.Visuals.Players.Toogle && GetAsyncKeyState(g_Options.Visuals.Players.ToggleKey) & 1)
					{
						g_Options.Visuals.Players.Enabled = !g_Options.Visuals.Players.Enabled;
					}

					if (g_Options.Visuals.Vehicles.Toogle && GetAsyncKeyState(g_Options.Visuals.Vehicles.ToggleKey) & 1)
					{
						g_Options.Visuals.Vehicles.Enabled = !g_Options.Visuals.Vehicles.Enabled;
					}

					if (g_Options.LegitBot.AimBot.ShowFov)
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), g_Options.LegitBot.AimBot.FOV, FrameWork::Misc::Float4ToImColor(g_Options.LegitBot.AimBot.FovColor));

					if (g_Options.LegitBot.SilentAim.ShowFOV)
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), g_Options.LegitBot.SilentAim.Fov, FrameWork::Misc::Float4ToImColor(g_Options.LegitBot.SilentAim.FovColor));

					if (g_Options.Visuals.Vehicles.Enabled)
						ESP::Vehicles();

					if (g_Options.Visuals.Players.Enabled)
						ESP::Players();

					Interface.RenderGui();

					NotifyManager::Render();
				}
				ImGui::EndFrame();
				ImGui::Render();

				FrameWork::Overlay::dxRefresh();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				FrameWork::Overlay::dxGetSwapChain()->Present(g_Options.General.Vsync, 0U);
			}
		}
	}

	void ShutDown()
	{
		if (!g_Options.General.ShutDown)
			return;

		FrameWork::Overlay::ShutDown();
		FrameWork::Overlay::dxShutDown();
	}
}