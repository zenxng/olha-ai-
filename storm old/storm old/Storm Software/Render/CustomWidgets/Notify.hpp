#pragma once
#include "../../FiveM-External.hpp"
#include "../../Utils/Misc.hpp"
#include <chrono>
#include <map>

namespace NotifyManager
{
    enum eType {
        None,
        Info,
        Warning
    };

    enum eState {
        In,
        Current,
        Out,
        Expired
    };

    class NotifyClass {
    private:
        std::string Title;
        std::string Description;
        time_t ExpireTime = 0;
        time_t CreationTime = 0;
        eType Type = eType::None;
        eState CurrentState = eState::In;
    public:
        void SetTitle(std::string NewTitle) { this->Title = NewTitle; }
        void SetDescription(std::string NewDesc) { this->Description = NewDesc; }
        void SetType(eType NewType) { this->Type = NewType; }
        void SetState(eState NewState) { this->CurrentState = NewState; }
        void SetCreationTime(time_t NewCreationTime) { this->CreationTime = NewCreationTime; }
    public:
        std::string GetTitle() { return Title; }
        std::string GetDescription() { return Description; }
        time_t GetExpireTime() { return ExpireTime; }
        time_t GetCreationTime() { return CreationTime; }
        eType GetType() { return Type; }
        eState GetCurrentState() { return CurrentState; }
    public:
        time_t GetNowTime() {
            using namespace std::chrono;
            return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
        }

        time_t GetTimeDiff() {
            return (time_t)(GetNowTime() - CreationTime);
        }

        NotifyClass(eType Type, time_t ExpireTime = 4000)
        {
            this->Type = Type;
            this->ExpireTime = ExpireTime;
            this->CreationTime = GetNowTime();
        }
    };

    inline std::vector<NotifyClass> NotifyList;

    inline void DeleteNotify(int Index)
    {
        NotifyList.erase(NotifyList.begin() + Index);
    }

    inline void Render()
    {
        const auto DrawList = ImGui::GetForegroundDrawList();
        RECT rc;
        GetClientRect(FrameWork::Overlay::hTargetWindow, &rc);
        float WindowWidth = static_cast<float>(rc.right - rc.left);
        float WindowHeight = static_cast<float>(rc.bottom - rc.top);
        float NextHeight = 0.f;

        for (auto i = 0; i < NotifyList.size(); i++)
        {
            auto& Notify = NotifyList.at(i);

            struct NotifyWid_t {
                float SlideX = 0.f;
                float YPos = 0.f;
                float Alpha = 0.f;
            };

            static std::map<std::string, NotifyWid_t> anim;
            const auto Id = Notify.GetDescription() + std::to_string(Notify.GetCreationTime());
            auto NotifyAnim = anim.find(Id);
            if (NotifyAnim == anim.end())
            {
                anim.insert({ Id, NotifyWid_t() });
                NotifyAnim = anim.find(Id);
            }

            if (Notify.GetCurrentState() == eState::Expired)
            {
                Notify.SetState(eState::In);
                DeleteNotify(i);
                anim.erase(Id);
                i--;
                continue;
            }

            auto TitleTxtSize = FrameWork::Misc::CalcTextSize(FrameWork::Assets::InterBold, FrameWork::Assets::InterBold->FontSize, Notify.GetTitle().c_str());
            auto DescTxtSize = FrameWork::Misc::CalcTextSize(FrameWork::Assets::InterBold, FrameWork::Assets::InterBold->FontSize, Notify.GetDescription().c_str());

            const float Padding = 8;

            static ImVec2 NotifySize;

            if (TitleTxtSize.x > DescTxtSize.x) {
                NotifySize = ImVec2(TitleTxtSize.x + (Padding * 2), DescTxtSize.y + TitleTxtSize.y + (Padding * 2) + 4);
            }
            else {
                NotifySize = ImVec2(DescTxtSize.x + (Padding * 2), DescTxtSize.y + TitleTxtSize.y + (Padding * 2) + 4);
            }

            if (Notify.GetCurrentState() == eState::In || Notify.GetCurrentState() == eState::Current)
            {
                NotifyAnim->second.SlideX = ImLerp(NotifyAnim->second.SlideX, NotifySize.x, ImGui::GetIO().DeltaTime * 8);
                NotifyAnim->second.Alpha = ImLerp(NotifyAnim->second.Alpha, 1.f, ImGui::GetIO().DeltaTime * 8);
            }

            if (NotifyAnim->second.SlideX >= NotifySize.x - 1.f) {
                Notify.SetState(eState::Current);
            }

            if (Notify.GetCurrentState() == eState::Current && Notify.GetTimeDiff() > Notify.GetExpireTime()) {
                Notify.SetState(eState::Out);
            }

            if (Notify.GetCurrentState() == eState::Out)
            {
                NotifyAnim->second.SlideX = ImLerp(NotifyAnim->second.SlideX, 0.f, ImGui::GetIO().DeltaTime * 8);
                NotifyAnim->second.Alpha = ImLerp(NotifyAnim->second.Alpha, 0.f, ImGui::GetIO().DeltaTime * 6);
                if (NotifyAnim->second.SlideX <= 2.f)
                {
                    Notify.SetState(eState::Expired);
                }
            }

            NotifyAnim->second.YPos = ImLerp(NotifyAnim->second.YPos, NextHeight, ImGui::GetIO().DeltaTime * 8);

            float NotifyStartX = WindowWidth - NotifySize.x;
            float NotifyStartY = WindowHeight - NotifySize.y - Padding - NotifyAnim->second.YPos;

            ImRect NotifyPos(ImVec2(NotifyStartX + NotifySize.x - NotifyAnim->second.SlideX, NotifyStartY), ImVec2(NotifyStartX + NotifySize.x - NotifyAnim->second.SlideX + NotifySize.x, NotifyStartY + NotifySize.y));

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, NotifyAnim->second.Alpha);

            DrawList->AddRectFilled(NotifyPos.Min, NotifyPos.Max, ImGui::GetColorU32(ImVec4(ImColor(14, 14, 14, 255))), 0);

            float Progress = 1.0f - ((float)Notify.GetTimeDiff() / (float)Notify.GetExpireTime());
            ImVec2 ProgressBarMin = ImVec2(NotifyPos.Min.x, NotifyPos.Max.y - 3);
            ImVec2 ProgressBarMax = ImVec2(NotifyPos.Min.x + (NotifySize.x * Progress), NotifyPos.Max.y - 1);
            ImVec2 ProgressBarShadowMax = ImVec2(NotifyPos.Min.x + (NotifySize.x * (Progress * 1.1)), NotifyPos.Max.y);
            DrawList->AddRectFilled(ProgressBarMin, ProgressBarShadowMax, ImGui::GetColorU32(ImVec4(ImColor(138, 43, 226))), 0.3);
            DrawList->AddRectFilled(ProgressBarMin, ProgressBarMax, ImGui::GetColorU32(ImVec4(ImColor(138, 43, 226))));

            auto TitlePos = ImVec2(NotifyPos.Min.x + Padding, NotifyPos.Min.y + Padding - 1);
            DrawList->AddText(FrameWork::Assets::InterBold, FrameWork::Assets::InterBold->FontSize, TitlePos, ImGui::GetColorU32(ImVec4(ImColor(200, 200, 200))), Notify.GetTitle().c_str());
            DrawList->AddText(FrameWork::Assets::InterBold, FrameWork::Assets::InterBold->FontSize, TitlePos + ImVec2(0, TitleTxtSize.y + 2.f), ImGui::GetColorU32(ImVec4(ImColor(80, 80, 80))), Notify.GetDescription().c_str());

            ImGui::PopStyleVar();

            NextHeight += NotifySize.y + 10.f;
        }
    }

    inline void Send(std::string Description, time_t ExpireTime)
    {
        NotifyManager::NotifyClass Notify(NotifyManager::eType::Info, ExpireTime);
        Notify.SetTitle(XorStr("Storm Bypass"));
        Notify.SetDescription(Description);
        NotifyList.push_back(Notify);
    }

}