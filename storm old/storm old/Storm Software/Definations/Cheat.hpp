#pragma once

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_internal.h"

#include "../FivemSDK/Fivem.hpp"
#include "../FivemSDK/Offsets.hpp"

#include "../Features/Visuals/Player/PlayerESP.hpp"
#include "../Features/Visuals/Vehicle/VehicleESP.hpp"

#include "../Features/Misc/Exploits.hpp"
#include "../Features/LegitBot/AimBot.hpp"
#include "../Features/LegitBot/SilentAim.hpp"
#include "../Features/LegitBot/TriggerBot.hpp"

#include "Variables.hpp"

namespace Cheat
{
	void Initialize();
	void ShutDown();
}