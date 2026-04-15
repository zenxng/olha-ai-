#pragma once

#include <Windows.h>
#include <iostream>
#include <dwmapi.h>
#include <TlHelp32.h>
#include <D3DX11tex.h>
#include <D3dx9math.h>
#include <winternl.h>
#include <psapi.h>
#include <string>
#include <thread>
#include <WtsApi32.h>
#include <chrono>
#include <atomic>
#include <mutex>
#include <vector>
#include <cmath>
#include <map>
#include <winnt.h>
#include <werapi.h>
#include <regex>

#include "Security/LazyImporter.hpp"
#include "Security/XorStr.hpp"

#include "Math/Matrix4x4.hpp"
#include "Math/Vectors/Vector2D.hpp"
#include "Math/Vectors/Vector3D.hpp"
#include "Math/Vectors/Vector4D.hpp"

#include "Utils/Misc.hpp"
#include "Utils/Memory.hpp"

#include "Render/Overlay.hpp"
#include "Render/Interface.hpp"
#include "Render/Assets/Assets.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"

#define XorStr(str) xor (str)
#define SafeCall(str) lzimpLI_FN(str)