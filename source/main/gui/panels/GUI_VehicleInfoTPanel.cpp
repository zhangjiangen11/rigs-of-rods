/*
    This source file is part of Rigs of Rods
    Copyright 2005-2012 Pierre-Michel Ricordel
    Copyright 2007-2012 Thomas Fischer
    Copyright 2013-2024 Petr Ohlidal

    For more information, see http://www.rigsofrods.org/

    Rigs of Rods is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3, as
    published by the Free Software Foundation.

    Rigs of Rods is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rigs of Rods. If not, see <http://www.gnu.org/licenses/>.
*/

#include "GUI_VehicleInfoTPanel.h"

#include "Application.h"
#include "Actor.h"
#include "SimData.h"
#include "Language.h"
#include "EngineSim.h"
#include "GameContext.h"
#include "GfxActor.h"
#include "GUIManager.h"
#include "Utils.h"
#include "GUIUtils.h"

using namespace RoR;
using namespace GUI;

const float HELP_TEXTURE_WIDTH = 512.f;
const float HELP_TEXTURE_HEIGHT = 128.f;
const ImVec2 MAX_PREVIEW_SIZE(100.f, 100.f);
const float MIN_PANEL_WIDTH = 325.f;

void VehicleInfoTPanel::Draw(RoR::GfxActor* actorx)
{
    // === DETERMINE VISIBILITY ===

    // Show only once for 5 sec, with a notice
    bool show_translucent = false;
    if (App::ui_show_vehicle_buttons->getBool() && actorx && !m_startupdemo_init)
    {
        m_startupdemo_timer.reset();
        App::GetConsole()->putMessage(Console::CONSOLE_MSGTYPE_INFO, Console::CONSOLE_SYSTEM_NOTICE,
                                      fmt::format(_LC("VehicleButtons", "Hover the mouse on the left to see controls")), "lightbulb.png");
        m_startupdemo_init = true;
    }
    if (App::ui_show_vehicle_buttons->getBool() && m_startupdemo_timer.getMilliseconds() < 5000)
    {
        show_translucent = true;
    }

    // Show when mouse is on the left of screen
    if (m_visibility_mode != TPANELMODE_OPAQUE 
        && App::ui_show_vehicle_buttons->getBool()
        && App::GetGuiManager()->AreStaticMenusAllowed()
        && (ImGui::GetIO().MousePos.x <= MIN_PANEL_WIDTH + ImGui::GetStyle().WindowPadding.x*2))
    {
        show_translucent = true;
    }

    if (show_translucent && m_visibility_mode != TPANELMODE_OPAQUE)
    {
        m_visibility_mode = TPANELMODE_TRANSLUCENT;
    }
    else if (!show_translucent && m_visibility_mode != TPANELMODE_OPAQUE)
    {
        m_visibility_mode = TPANELMODE_HIDDEN;
    }

    if (m_visibility_mode == TPANELMODE_HIDDEN)
    {
        return;
    }

    // === OPEN IMGUI WINDOW ===

    GUIManager::GuiTheme& theme = App::GetGuiManager()->GetTheme();
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowPos(ImVec2(theme.screen_edge_padding.x, (theme.screen_edge_padding.y + 150)));
    switch (m_visibility_mode)
    {
        case TPANELMODE_OPAQUE:
            ImGui::PushStyleColor(ImGuiCol_WindowBg, theme.semitransparent_window_bg);
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            break;

        case TPANELMODE_TRANSLUCENT:
            ImGui::PushStyleColor(ImGuiCol_WindowBg, m_panel_translucent_color);
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, m_transluc_textdis_color);
            break;

        default:
            break;
    }
    ImGui::Begin("VehicleInfoTPanel", nullptr, flags);

    // === DECIDE WHAT THE WINDOW WILL DISPLAY ===

    int tabflags_basics = ImGuiTabItemFlags_None;
    int tabflags_stats = ImGuiTabItemFlags_None;
    int tabflags_commands = ImGuiTabItemFlags_None;
    int tabflags_diag = ImGuiTabItemFlags_None;
    if (m_requested_focus != TPANELFOCUS_NONE)
    {
        switch (m_requested_focus)
        {
            case TPANELFOCUS_BASICS: tabflags_basics = ImGuiTabItemFlags_SetSelected; break;
            case TPANELFOCUS_STATS:  tabflags_stats = ImGuiTabItemFlags_SetSelected; break;
            case TPANELFOCUS_DIAG:   tabflags_diag = ImGuiTabItemFlags_SetSelected; break;
            case TPANELFOCUS_COMMANDS: tabflags_commands = ImGuiTabItemFlags_SetSelected; break;
            default:;
        }
        
        // Reset the request
        m_requested_focus = TPANELFOCUS_NONE;
    }

    // === DRAW THE WINDOW HEADER - MINI IMAGE (if available) AND VEHICLE NAME ===

    ImVec2 name_pos = ImGui::GetCursorPos();
    ImVec2 content_pos;
    if (actorx->GetActor()->getUsedActorEntry()->filecachename != "")
    {
        Ogre::TexturePtr preview_tex = Ogre::TextureManager::getSingleton().load(
            actorx->GetActor()->getUsedActorEntry()->filecachename, RGN_CACHE);
        // Scale the image
        ImVec2 MAX_PREVIEW_SIZE(100.f, 100.f);
        ImVec2 size(preview_tex->getWidth(), preview_tex->getHeight());
        size *= MAX_PREVIEW_SIZE.x / size.x; // Fit size along X
        if (size.y > MAX_PREVIEW_SIZE.y) // Reduce size along Y if needed
        {
            size *= MAX_PREVIEW_SIZE.y / size.y;
        }
        // Draw the image
        ImGui::Image(reinterpret_cast<ImTextureID>(preview_tex->getHandle()), size);
        content_pos = ImGui::GetCursorPos();
        // Move name to the right
        name_pos.x += size.x + ImGui::GetStyle().ItemSpacing.x;
    }
    
    ImGui::SetCursorPos(name_pos);
    RoR::ImTextWrappedColorMarked(actorx->GetActor()->getTruckName());
    ImGui::Dummy(ImVec2(MIN_PANEL_WIDTH, 20));

    // === DRAW TAB BAR ===

    if (actorx->GetActor()->getUsedActorEntry()->filecachename != "")
    {
        ImGui::SetCursorPos(ImVec2(name_pos.x, content_pos.y - 21));
    }
    ImGui::BeginTabBar("VehicleInfoTPanelTabs", ImGuiTabBarFlags_None);
    if (ImGui::BeginTabItem(_LC("TPanel", "Basics"), nullptr, tabflags_basics))
    {
        // If the tab bar is drawn next to the image, we need to reset the cursor position
        if (actorx->GetActor()->getUsedActorEntry()->filecachename != "")
        {
            ImGui::SetCursorPos(content_pos);
            ImGui::Separator();
        }
    
        m_current_focus = TPANELFOCUS_BASICS;
        this->DrawVehicleBasicsUI(actorx);
    
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem(_LC("TPanel", "Stats"), nullptr, tabflags_stats))
    {
        // If the tab bar is drawn next to the image, we need to reset the cursor position
        if (actorx->GetActor()->getUsedActorEntry()->filecachename != "")
        {
            ImGui::SetCursorPos(content_pos);
            ImGui::Separator();
        }

        m_current_focus = TPANELFOCUS_STATS;
        this->DrawVehicleStatsUI(actorx);

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem(_LC("TPanel", "Commands"), nullptr, tabflags_commands))
    {
        // If the tab bar is drawn next to the image, we need to reset the cursor position
        if (actorx->GetActor()->getUsedActorEntry()->filecachename != "")
        {
            ImGui::SetCursorPos(content_pos);
            ImGui::Separator();
        }

        m_current_focus = TPANELFOCUS_COMMANDS;
        this->DrawVehicleCommandsUI(actorx);

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem(_LC("TPanel", "Diag"), nullptr, tabflags_diag))
    {
        // If the tab bar is drawn next to the image, we need to reset the cursor position
        if (actorx->GetActor()->getUsedActorEntry()->filecachename != "")
        {
            ImGui::SetCursorPos(content_pos);
            ImGui::Separator();
        }        

        m_current_focus = TPANELFOCUS_DIAG;
        this->DrawVehicleDiagUI(actorx);

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();

    if (actorx->GetActor()->getUsedActorEntry()->filecachename != "")
    {
        ImGui::SetCursorPos(content_pos);
    }
    ImGui::Separator();
    
    ImGui::End();
    ImGui::PopStyleColor(2); // WindowBg, TextDisabled

    this->DrawVehicleCommandHighlights(actorx);
}

void VehicleInfoTPanel::DrawVehicleCommandsUI(RoR::GfxActor* actorx)
{
    // === DRAW DESCRIPTION (if available) ===

    if (!actorx->GetActor()->getDescription().empty())
    {
        ImGui::TextDisabled("%s", _LC("VehicleDescription", "Description text:"));
        for (auto line : actorx->GetActor()->getDescription())
        {
            ImGui::TextWrapped("%s", line.c_str());
        }     
    }

    // === DRAW HELP TEXTURE (if available) ===

    if (actorx->GetHelpTex())
    {
        ImGui::TextDisabled("%s", _LC("VehicleDescription", "Help image:"));
        ImGui::SameLine();
        ImGui::SetCursorPosX(MIN_PANEL_WIDTH - (ImGui::CalcTextSize(_LC("VehicleDescription", "Full size")).x + 25.f));
        ImGui::Checkbox(_LC("VehicleDescription", "Full size"), &m_helptext_fullsize);
        
        ImTextureID im_tex = reinterpret_cast<ImTextureID>(actorx->GetHelpTex()->getHandle());
        if (m_helptext_fullsize)
        {
            ImGui::Image(im_tex, ImVec2(HELP_TEXTURE_WIDTH, HELP_TEXTURE_HEIGHT));
        }
        else
        {
            ImGui::Image(im_tex, ImVec2(MIN_PANEL_WIDTH, HELP_TEXTURE_HEIGHT));
        }
    }

    // === DRAW COMMAND KEYS, WITH HIGHLIGHT ===

    m_active_commandkey = COMMANDKEYID_INVALID;
    m_hovered_commandkey = COMMANDKEYID_INVALID;

    if (actorx->GetActor()->ar_unique_commandkey_pairs.size() > 0)
    {
        ImGui::TextDisabled("%s", _LC("VehicleDescription", "Command controls:"));
        ImGui::PushStyleColor(ImGuiCol_Text, m_cmdbeam_highlight_color);
        ImGui::Text("%s", _LC("VehicleDescription", "Hover controls for on-vehicle highlight"));
        ImGui::PopStyleColor(1); // Text
        ImGui::Columns(3, /*id=*/nullptr, /*border=*/true);
        // Apply the calculated column widths
        ImGui::SetColumnWidth(0, m_command_column_calc_width[0]);
        ImGui::SetColumnWidth(1, m_command_column_calc_width[1]);
        ImGui::SetColumnWidth(2, m_command_column_calc_width[2]);
        // Reset the values for new calculation
        m_command_column_calc_width[0] = 0.f;
        m_command_column_calc_width[1] = 0.f;
        m_command_column_calc_width[2] = 0.f;
        for (const UniqueCommandKeyPair& qpair: actorx->GetActor()->ar_unique_commandkey_pairs)
        {
            // Description comes first
            std::string desc = qpair.uckp_description;
            if (qpair.uckp_description == "")
            {
                desc = _LC("VehicleDescription", "unknown function");
            }
            bool selected_dummy = false;
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, m_cmdbeam_highlight_color);
            ImVec2 desc_cursor = ImGui::GetCursorScreenPos();
            ImGui::Selectable(desc.c_str(), &selected_dummy, ImGuiSelectableFlags_SpanAllColumns);
            ImGui::PopStyleColor(1); // FrameBgHovered
            if (ImGui::IsItemHovered())
            {
                m_hovered_commandkey = qpair.uckp_key1; // key1/key2 both point to the same command beams.
                // Draw the description text again in the high-contrast color.
                ImGui::GetWindowDrawList()->AddText(desc_cursor, ImColor(m_command_hovered_text_color), desc.c_str());
            }
            m_command_column_calc_width[0] = std::max(m_command_column_calc_width[0], ImGui::CalcTextSize(desc.c_str()).x);
            ImGui::NextColumn();

            // Key 1
            const RoR::events event1 = (RoR::events)RoR::InputEngine::resolveEventName(fmt::format("COMMANDS_{:02d}", qpair.uckp_key1));
            bool key1_hovered = false;
            bool key1_active = false;
            ImDrawEventHighlightedButton(event1, &key1_hovered, &key1_active);
            if (key1_active)
            {
                m_active_commandkey = qpair.uckp_key1;
            }
            if (key1_hovered)
            {
                m_hovered_commandkey = qpair.uckp_key1;
            }
            m_command_column_calc_width[1] = std::max(m_command_column_calc_width[1], ImGui::CalcTextSize(App::GetInputEngine()->getEventCommandTrimmed(event1).c_str()).x);
            ImGui::NextColumn();

            // Key 2
            const RoR::events event2 = (RoR::events)RoR::InputEngine::resolveEventName(fmt::format("COMMANDS_{:02d}", qpair.uckp_key2));
            bool key2_hovered = false;
            bool key2_active = false;
            ImDrawEventHighlightedButton(event2, &key2_hovered, &key2_active);
            if (key2_active)
            {
                m_active_commandkey = qpair.uckp_key2;
            }
            if (key2_hovered)
            {
                m_hovered_commandkey = qpair.uckp_key2;
            }
            m_command_column_calc_width[2] = std::max(m_command_column_calc_width[2], ImGui::CalcTextSize(App::GetInputEngine()->getEventCommandTrimmed(event2).c_str()).x);
            ImGui::NextColumn();
        }
        ImGui::Columns(1);

        // Fix up the calculated column widths
        m_command_column_calc_width[0] += 10.f;
        m_command_column_calc_width[1] += 10.f;
        m_command_column_calc_width[2] += 10.f;
        if (m_command_column_calc_width[0] + m_command_column_calc_width[1] + m_command_column_calc_width[2] > MIN_PANEL_WIDTH)
        {
            m_command_column_calc_width[0] = MIN_PANEL_WIDTH - m_command_column_calc_width[1] - m_command_column_calc_width[2];
        }
    }
}

void VehicleInfoTPanel::DrawVehicleStatsUI(RoR::GfxActor* actorx)
{
    GUIManager::GuiTheme& theme = App::GetGuiManager()->GetTheme();

    if (m_stat_health < 1.0f)
    {
        const float value = static_cast<float>( Round((1.0f - m_stat_health) * 100.0f, 2) );
        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Vehicle health: "));
        ImGui::SameLine();
        ImGui::Text("%.2f%%", value);
    }
    else if (m_stat_health >= 1.0f) //When this condition is true, it means that health is at 0% which means 100% of destruction.
    {
        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Vehicle destruction: "));
        ImGui::SameLine();
        ImGui::Text("100%%");
    }

    const int num_beams_i = actorx->FetchNumBeams();
    const float num_beams_f = static_cast<float>(num_beams_i);
    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Beam count: "));
    ImGui::SameLine();
    ImGui::Text("%d", num_beams_i);

    const float broken_pct = static_cast<float>( Round((float)m_stat_broken_beams / num_beams_f, 2) * 100.0f );
    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Broken beams count: "));
    ImGui::SameLine();
    ImGui::Text("%d (%.0f%%)", m_stat_broken_beams, broken_pct);

    const float deform_pct = static_cast<float>( Round((float)m_stat_deformed_beams / num_beams_f * 100.0f) );
    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Deformed beams count: "));
    ImGui::SameLine();
    ImGui::Text("%d (%.0f%%)", m_stat_deformed_beams, deform_pct);

    const float avg_deform = static_cast<float>( Round((float)m_stat_avg_deform / num_beams_f, 4) * 100.0f );
    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Average deformation: "));
    ImGui::SameLine();
    ImGui::Text("%.2f", avg_deform);

    const float avg_stress = 1.f - (float)m_stat_beam_stress / num_beams_f;
    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Average stress: "));
    ImGui::SameLine();
    ImGui::Text("%+08.0f", avg_stress);

    ImGui::NewLine();

    const int num_nodes = actorx->FetchNumNodes();
    const int num_wheelnodes = actorx->FetchNumWheelNodes();
    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Node count: "));
    ImGui::SameLine();
    ImGui::Text("%d (%s%d)", num_nodes, "wheels: ", num_wheelnodes);

    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Total mass: "));
    ImGui::SameLine();
    ImGui::Text("%8.2f Kg (%.2f tons)", m_stat_mass_Kg, m_stat_mass_Kg / 1000.0f);

    ImGui::NewLine();

    const float n0_velo_len   = actorx->GetSimDataBuffer().simbuf_node0_velo.length();
    if ((actorx->GetSimDataBuffer().simbuf_has_engine) && (actorx->GetSimDataBuffer().simbuf_driveable == TRUCK))
    {
        const double PI = 3.14159265358979323846;

        const float max_rpm       = actorx->GetSimDataBuffer().simbuf_engine_max_rpm;
        const float torque        = actorx->GetSimDataBuffer().simbuf_engine_torque;
        const float turbo_psi     = actorx->GetSimDataBuffer().simbuf_engine_turbo_psi;
        const float cur_rpm       = actorx->GetSimDataBuffer().simbuf_engine_rpm;
        const float wheel_speed   = actorx->GetSimDataBuffer().simbuf_wheel_speed;

        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Engine RPM: "));
        ImGui::SameLine();
        ImVec4 rpm_color = (cur_rpm > max_rpm) ? theme.value_red_text_color : ImGui::GetStyle().Colors[ImGuiCol_Text];
        ImGui::TextColored(rpm_color, "%.2f / %.2f", cur_rpm, max_rpm);

        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Input shaft RPM: "));
        ImGui::SameLine();
        const float inputshaft_rpm = Round(std::max(0.0f, actorx->GetSimDataBuffer().simbuf_inputshaft_rpm));
        ImGui::TextColored(rpm_color, "%.0f", inputshaft_rpm);

        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Current torque: "));
        ImGui::SameLine();
        ImGui::Text("%.0f Nm", Round(torque));

        const float currentKw = (((cur_rpm * (torque + ((turbo_psi * 6.8) * torque) / 100) * ( PI / 30)) / 1000));
        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Current power: "));
        ImGui::SameLine();
        ImGui::Text("%.0fhp (%.0fKw)", static_cast<float>(Round(currentKw *1.34102209)), static_cast<float>(Round(currentKw)));

        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Current gear: "));
        ImGui::SameLine();
        ImGui::Text("%d", actorx->GetSimDataBuffer().simbuf_gear);

        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Drive ratio: "));
        ImGui::SameLine();
        ImGui::Text("%.2f:1", actorx->GetSimDataBuffer().simbuf_drive_ratio);

        float velocityKPH = wheel_speed * 3.6f;
        float velocityMPH = wheel_speed * 2.23693629f;
        float carSpeedKPH = n0_velo_len * 3.6f;
        float carSpeedMPH = n0_velo_len * 2.23693629f;

        // apply a deadzone ==> no flickering +/-
        if (fabs(wheel_speed) < 1.0f)
        {
            velocityKPH = velocityMPH = 0.0f;
        }
        if (fabs(n0_velo_len) < 1.0f)
        {
            carSpeedKPH = carSpeedMPH = 0.0f;
        }

        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Wheel speed: "));
        ImGui::SameLine();
        ImGui::Text("%.0fKm/h (%.0f mph)", Round(velocityKPH), Round(velocityMPH));

        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Vehicle speed: "));
        ImGui::SameLine();
        ImGui::Text("%.0fKm/h (%.0f mph)", Round(carSpeedKPH), Round(carSpeedMPH));
    }
    else // Aircraft or boat
    {
        float speedKN = n0_velo_len * 1.94384449f;
        ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Current speed: "));
        ImGui::SameLine();
        ImGui::Text("%.0f kn (%.0f Km/h; %.0f mph)", Round(speedKN), Round(speedKN * 1.852), Round(speedKN * 1.151));

        if (actorx->GetSimDataBuffer().simbuf_driveable == AIRPLANE)
        {
            const float altitude = actorx->GetSimNodeBuffer()[0].AbsPosition.y / 30.48 * 100;
            ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Altitude: "));
            ImGui::SameLine();
            ImGui::Text("%.0f feet (%.0f meters)", Round(altitude), Round(altitude * 0.30480));

            int engine_num = 1; // UI; count from 1
            for (AeroEngineSB& ae: actorx->GetSimDataBuffer().simbuf_aeroengines)
            {
                ImGui::TextColored(theme.value_blue_text_color, "%s #%d:", _LC("SimActorStats", "Engine "), engine_num);
                ImGui::SameLine();
                if (ae.simbuf_ae_type == AeroEngineType::AE_XPROP)
                {
                    ImGui::Text("%.2f RPM", ae.simbuf_ae_rpm);
                }
                else // Turbojet
                {
                    ImGui::Text("%.2f", ae.simbuf_ae_rpm);
                }
                ++engine_num;
            }
        }
        else if (actorx->GetSimDataBuffer().simbuf_driveable == BOAT)
        {
            int engine_num = 1; // UI; count from 1
            for (ScrewpropSB& screw: actorx->GetSimDataBuffer().simbuf_screwprops)
            {
                ImGui::TextColored(theme.value_blue_text_color, "%s #%d:", _LC("SimActorStats", "Engine "), engine_num);
                ImGui::SameLine();
                ImGui::Text("%f%", screw.simbuf_sp_throttle);
                ++engine_num;
            }
        }
    }

    ImGui::NewLine();

    const float speedKPH = actorx->GetSimDataBuffer().simbuf_top_speed * 3.6f;
    const float speedMPH = actorx->GetSimDataBuffer().simbuf_top_speed * 2.23693629f;
    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "Top speed: "));
    ImGui::SameLine();
    ImGui::Text("%.0f km/h (%.0f mph)", Round(speedKPH), Round(speedMPH));

    ImGui::NewLine();

    ImGui::TextColored(theme.value_blue_text_color,"%s", _LC("SimActorStats", "G-Forces:"));
    ImGui::Text("Vertical: % 6.2fg  (%1.2fg)", m_stat_gcur_x, m_stat_gmax_x);
    ImGui::Text("Sagittal: % 6.2fg  (%1.2fg)", m_stat_gcur_y, m_stat_gmax_y);
    ImGui::Text("Lateral:  % 6.2fg  (%1.2fg)", m_stat_gcur_z, m_stat_gmax_z);

}

void VehicleInfoTPanel::DrawVehicleDiagUI(RoR::GfxActor* actorx)
{
    ImGui::TextDisabled("%s", _LC("TopMenubar", "Live diagnostic views:"));
    ImGui::TextDisabled("%s", fmt::format(_LC("TopMenubar", "(Toggle with {})"), App::GetInputEngine()->getEventCommandTrimmed(EV_COMMON_TOGGLE_DEBUG_VIEW)).c_str());
    ImGui::TextDisabled("%s", fmt::format(_LC("TopMenubar", "(Cycle with {})"), App::GetInputEngine()->getEventCommandTrimmed(EV_COMMON_CYCLE_DEBUG_VIEWS)).c_str());

    int debug_view_type = static_cast<int>(actorx->GetDebugView());
    ImGui::RadioButton(_LC("TopMenubar", "Normal view"),     &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_NONE));
    ImGui::RadioButton(_LC("TopMenubar", "Skeleton view"),   &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_SKELETON));
    ImGui::RadioButton(_LC("TopMenubar", "Node details"),    &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_NODES));
    ImGui::RadioButton(_LC("TopMenubar", "Beam details"),    &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_BEAMS));
    ActorPtr current_actor = actorx->GetActor();
    if (current_actor->ar_num_wheels > 0)
    {
        ImGui::RadioButton(_LC("TopMenubar", "Wheel details"),   &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_WHEELS));
    }
    if (current_actor->ar_num_shocks > 0)
    {
        ImGui::RadioButton(_LC("TopMenubar", "Shock details"),   &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_SHOCKS));
    }
    if (current_actor->ar_num_rotators > 0)
    {
        ImGui::RadioButton(_LC("TopMenubar", "Rotator details"), &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_ROTATORS));
    }
    if (current_actor->hasSlidenodes())
    {
        ImGui::RadioButton(_LC("TopMenubar", "Slidenode details"), &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_SLIDENODES));
    }
    if (current_actor->ar_num_cabs > 0)
    {
        ImGui::RadioButton(_LC("TopMenubar", "Submesh details"), &debug_view_type,  static_cast<int>(DebugViewType::DEBUGVIEW_SUBMESH));
    }

    if ((current_actor != nullptr) && (debug_view_type != static_cast<int>(current_actor->GetGfxActor()->GetDebugView())))
    {
        current_actor->GetGfxActor()->SetDebugView(static_cast<DebugViewType>(debug_view_type));
    }

    if (debug_view_type >= 1 && debug_view_type <= static_cast<int>(DebugViewType::DEBUGVIEW_BEAMS)) 
    {
        ImGui::Separator();
        ImGui::TextDisabled("%s",     _LC("TopMenubar", "Settings:"));
        DrawGCheckbox(App::diag_hide_broken_beams,   _LC("TopMenubar", "Hide broken beams"));
        DrawGCheckbox(App::diag_hide_beam_stress,    _LC("TopMenubar", "Hide beam stress"));
        DrawGCheckbox(App::diag_hide_wheels,         _LC("TopMenubar", "Hide wheels"));
        DrawGCheckbox(App::diag_hide_nodes,          _LC("TopMenubar", "Hide nodes"));
        if (debug_view_type >= 2)
        {
            DrawGCheckbox(App::diag_hide_wheel_info, _LC("TopMenubar", "Hide wheel info"));
        }
    }
             
}

void VehicleInfoTPanel::SetVisible(TPanelMode mode, TPanelFocus focus)
{
    m_visibility_mode = mode;
    m_requested_focus = focus; // Cannot be handled here, must be handled in Draw() while window is open.
}

void VehicleInfoTPanel::UpdateStats(float dt, ActorPtr actor)
{
    //taken from TruckHUD.cpp (now removed)
    beam_t* beam = actor->ar_beams;
    float average_deformation = 0.0f;
    float beamstress = 0.0f;
    float mass = actor->getTotalMass();
    int beambroken = 0;
    int beamdeformed = 0;
    Ogre::Vector3 gcur = actor->getGForces();
    Ogre::Vector3 gmax = actor->getMaxGForces();

    for (int i = 0; i < actor->ar_num_beams; i++ , beam++)
    {
        if (beam->bm_broken != 0)
        {
            beambroken++;
        }
        beamstress += std::abs(beam->stress);
        float current_deformation = fabs(beam->L - beam->refL);
        if (fabs(current_deformation) > 0.0001f && beam->bm_type != BEAM_HYDRO)
        {
            beamdeformed++;
        }
        average_deformation += current_deformation;
    }

    m_stat_health = ((float)beambroken / (float)actor->ar_num_beams) * 10.0f + ((float)beamdeformed / (float)actor->ar_num_beams);
    m_stat_broken_beams = beambroken;
    m_stat_deformed_beams = beamdeformed;
    m_stat_beam_stress = beamstress;
    m_stat_mass_Kg = mass;
    m_stat_avg_deform = average_deformation;
    m_stat_gcur_x = gcur.x;
    m_stat_gcur_y = gcur.y;
    m_stat_gcur_z = gcur.z;
    m_stat_gmax_x = gmax.x;
    m_stat_gmax_y = gmax.y;
    m_stat_gmax_z = gmax.z;
}

// --------------------------------
// class VehicleInfoTPanel

const ImVec2 BUTTON_SIZE(18, 18);
const ImVec2 BUTTON_OFFSET(0, 3.f);
const float BUTTON_Y_OFFSET = 0.f;
const ImVec2 BUTTONDUMMY_SIZE(18, 1);

void DrawSingleBulletRow(const char* name, RoR::events ev)
{
    ImGui::Dummy(BUTTONDUMMY_SIZE); ImGui::SameLine(); ImGui::Bullet(); ImGui::Text("%s", name);
    ImGui::NextColumn();
    ImDrawEventHighlighted(ev);
    ImGui::NextColumn();
}

void VehicleInfoTPanel::DrawVehicleBasicsUI(RoR::GfxActor* actorx)
{
    GUIManager::GuiTheme& theme = App::GetGuiManager()->GetTheme();

    if (!m_icons_cached)
    {
        this->CacheIcons();
    }

    ImGui::Columns(2, "TPanelMainControls");

    ImGui::TextDisabled("Simulation:"); ImGui::NextColumn(); ImGui::NextColumn();
    this->DrawRepairButton(actorx);
    
    this->DrawActorPhysicsButton(actorx);

    ImGui::TextDisabled("Lights and signals:"); ImGui::NextColumn(); ImGui::NextColumn();
    this->DrawHeadLightButton(actorx);
    
    this->DrawLeftBlinkerButton(actorx);
    
    this->DrawRightBlinkerButton(actorx);
    
    this->DrawWarnBlinkerButton(actorx);
    
    this->DrawBeaconButton(actorx);
    
    this->DrawHornButton(actorx);

    ImGui::TextDisabled("Engine:"); ImGui::NextColumn(); ImGui::NextColumn();
    this->DrawEngineButton(actorx);
    if (actorx->GetActor()->ar_engine && !actorx->GetActor()->ar_engine->isRunning())
    {
        DrawSingleBulletRow("Starter", EV_TRUCK_STARTER);
    }
    
    this->DrawTransferCaseModeButton(actorx);
    
    this->DrawTransferCaseGearRatioButton(actorx);

    this->DrawShiftModeButton(actorx);
    if (actorx->GetActor()->ar_engine)
    {
        switch (actorx->GetActor()->ar_engine->GetAutoShiftMode())
        {
        case SimGearboxMode::AUTO:
            DrawSingleBulletRow("Shift Up", EV_TRUCK_AUTOSHIFT_UP);
            DrawSingleBulletRow("Shift Down", EV_TRUCK_AUTOSHIFT_DOWN);
            break;
        case SimGearboxMode::SEMI_AUTO:
            DrawSingleBulletRow("Shift Up", EV_TRUCK_AUTOSHIFT_UP);
            DrawSingleBulletRow("Shift Down", EV_TRUCK_AUTOSHIFT_DOWN);
            DrawSingleBulletRow("Shift Neutral", EV_TRUCK_SHIFT_NEUTRAL);
            break;
        case SimGearboxMode::MANUAL:
            DrawSingleBulletRow("Shift Up", EV_TRUCK_SHIFT_UP);
            DrawSingleBulletRow("Shift Down", EV_TRUCK_SHIFT_DOWN);
            DrawSingleBulletRow("Shift Neutral", EV_TRUCK_SHIFT_NEUTRAL);
            DrawSingleBulletRow("Clutch", EV_TRUCK_MANUAL_CLUTCH);
            break;
        case SimGearboxMode::MANUAL_STICK:
            break;
        case SimGearboxMode::MANUAL_RANGES:
            break;
        }
    }
        
    ImGui::TextDisabled("Traction:");  ImGui::NextColumn(); ImGui::NextColumn();

    this->DrawAxleDiffButton(actorx);
    
    this->DrawWheelDiffButton(actorx);

    this->DrawTractionControlButton(actorx);
    
    this->DrawAbsButton(actorx);
    
    this->DrawParkingBrakeButton(actorx);
    
    this->DrawCruiseControlButton(actorx);

    ImGui::TextDisabled("Loading:");  ImGui::NextColumn(); ImGui::NextColumn();
    this->DrawLockButton(actorx);
    
    this->DrawSecureButton(actorx);

    ImGui::TextDisabled("View:");  ImGui::NextColumn(); ImGui::NextColumn();
    this->DrawParticlesButton(actorx);
    
    this->DrawMirrorButton(actorx);
    
    this->DrawCameraButton();

    ImGui::Columns(1);
}

void VehicleInfoTPanel::DrawVehicleCommandHighlights(RoR::GfxActor* actorx)
{
    if (m_hovered_commandkey == COMMANDKEYID_INVALID)
    {
        return;
    }

    ImDrawList* draw_list = GetImDummyFullscreenWindow("RoR_VehicleCommandHighlights");
    for (const commandbeam_t& cmdbeam: actorx->GetActor()->ar_command_key[m_hovered_commandkey].beams)
    {
        const beam_t& beam = actorx->GetActor()->ar_beams[cmdbeam.cmb_beam_index];
        ImVec2 p1_pos, p2_pos;
        if (GetScreenPosFromWorldPos(beam.p1->AbsPosition, p1_pos) && GetScreenPosFromWorldPos(beam.p2->AbsPosition, p2_pos))
        {
            draw_list->AddLine(p1_pos, p2_pos, ImColor(m_cmdbeam_highlight_color), m_cmdbeam_highlight_thickness);
        }    
    }
}

bool DrawSingleButtonRow(bool active, const Ogre::TexturePtr& icon, const char* name, RoR::events ev, bool* btn_active = nullptr)
{
    if (active)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
    }

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetCursorScreenPos() - BUTTON_OFFSET, ImGui::GetCursorScreenPos() + BUTTON_SIZE, 
        ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]), ImGui::GetStyle().FrameRounding);
    ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(icon->getHandle()),
        ImGui::GetCursorScreenPos() - BUTTON_OFFSET, ImGui::GetCursorScreenPos() + BUTTON_SIZE);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + BUTTON_SIZE.x + 2*ImGui::GetStyle().ItemSpacing.x);
    ImGui::PopStyleColor();
    
    ImGui::Text("%s", name);
    ImGui::NextColumn();
    const bool retval = ImDrawEventHighlightedButton(ev, nullptr, btn_active);
    ImGui::NextColumn();
    return retval;
}

void VehicleInfoTPanel::DrawHeadLightButton(RoR::GfxActor* actorx)
{
    bool has_headlight = false;
    for (int i = 0; i < actorx->GetActor()->ar_flares.size(); i++)
    {
        if (actorx->GetActor()->ar_flares[i].fl_type == FlareType::HEADLIGHT || actorx->GetActor()->ar_flares[i].fl_type == FlareType::TAIL_LIGHT)
        {
            has_headlight = true;
        }
    }

    if (!has_headlight)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->getHeadlightsVisible(), m_headlight_icon, "Head Lights", EV_COMMON_TOGGLE_TRUCK_LOW_BEAMS))
    {
        actorx->GetActor()->toggleHeadlights();
    }
}

void VehicleInfoTPanel::DrawLeftBlinkerButton(RoR::GfxActor* actorx)
{
    bool has_blink = false;
    for (int i = 0; i < actorx->GetActor()->ar_flares.size(); i++)
    {
        if (actorx->GetActor()->ar_flares[i].fl_type == FlareType::BLINKER_LEFT)
        {
            has_blink = true;
        }
    }

    if (!has_blink)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->getBlinkType() == BLINK_LEFT, m_left_blinker_icon, "Left Blinker", EV_TRUCK_BLINK_LEFT))
    {
        actorx->GetActor()->toggleBlinkType(BLINK_LEFT);
    }
}

void VehicleInfoTPanel::DrawRightBlinkerButton(RoR::GfxActor* actorx)
{
    bool has_blink = false;
    for (int i = 0; i < actorx->GetActor()->ar_flares.size(); i++)
    {
        if (actorx->GetActor()->ar_flares[i].fl_type == FlareType::BLINKER_RIGHT)
        {
            has_blink = true;
        }
    }

    if (!has_blink)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->getBlinkType() == BLINK_RIGHT, m_right_blinker_icon, "Right Blinker", EV_TRUCK_BLINK_RIGHT))
    {
        actorx->GetActor()->toggleBlinkType(BLINK_RIGHT);
    }
}

void VehicleInfoTPanel::DrawWarnBlinkerButton(RoR::GfxActor* actorx)
{
    bool has_blink = false;
    for (int i = 0; i < actorx->GetActor()->ar_flares.size(); i++)
    {
        if (actorx->GetActor()->ar_flares[i].fl_type == FlareType::BLINKER_LEFT)
        {
            has_blink = true;
        }
    }

    if (!has_blink)
    {
        return;
    }
    
    if (DrawSingleButtonRow(actorx->GetActor()->getBlinkType() == BLINK_WARN, m_warning_light_icon, "Warning Lights", EV_TRUCK_BLINK_WARN))
    {
        actorx->GetActor()->toggleBlinkType(BLINK_WARN);
    }
}

void VehicleInfoTPanel::DrawHornButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->getTruckType() != TRUCK)
    {
        return;
    }

    if (actorx->GetActor()->ar_is_police) // Police siren
    {
        if (DrawSingleButtonRow(SOUND_GET_STATE(actorx->GetActor()->ar_instance_id, SS_TRIG_HORN), m_horn_icon, "Horn", EV_TRUCK_HORN))
        {
            SOUND_TOGGLE(actorx->GetActor(), SS_TRIG_HORN);
        }
    }
    else
    {
        // Triggering continuous command every frame is sloppy
        // Set state and read it in GameContext via GetHornButtonState()
        DrawSingleButtonRow(SOUND_GET_STATE(actorx->GetActor()->ar_instance_id, SS_TRIG_HORN), m_horn_icon, "Horn", EV_TRUCK_HORN, &m_horn_btn_active);
    }
}

void VehicleInfoTPanel::DrawMirrorButton(RoR::GfxActor* actorx)
{
    if (!actorx->hasCamera())
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetVideoCamState() == VideoCamState::VCSTATE_ENABLED_ONLINE, m_mirror_icon, "Mirrors", EV_TRUCK_TOGGLE_VIDEOCAMERA))
    {
        if (actorx->GetVideoCamState() == VideoCamState::VCSTATE_DISABLED)
        {
            actorx->SetVideoCamState(VideoCamState::VCSTATE_ENABLED_ONLINE);
        }
        else
        {
            actorx->SetVideoCamState(VideoCamState::VCSTATE_DISABLED);
        }
    }
}

void VehicleInfoTPanel::DrawRepairButton(RoR::GfxActor* actorx)
{
    if (DrawSingleButtonRow(App::GetInputEngine()->getEventBoolValue(EV_COMMON_REPAIR_TRUCK), m_repair_icon, "Repair", EV_COMMON_REPAIR_TRUCK))
    {
        ActorModifyRequest* rq = new ActorModifyRequest;
        rq->amr_actor = actorx->GetActor()->ar_instance_id;
        rq->amr_type  = ActorModifyRequest::Type::RESET_ON_SPOT;
        App::GetGameContext()->PushMessage(Message(MSG_SIM_MODIFY_ACTOR_REQUESTED, (void*)rq));
    }
}

void VehicleInfoTPanel::DrawParkingBrakeButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->getTruckType() == NOT_DRIVEABLE || actorx->GetActor()->getTruckType() == BOAT)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->getParkingBrake(), m_parking_brake_icon, "Parking Brake", EV_TRUCK_PARKING_BRAKE))
    {
        actorx->GetActor()->parkingbrakeToggle();
    }
}

void VehicleInfoTPanel::DrawTractionControlButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->tc_nodash)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->tc_mode, m_traction_control_icon, "Traction Control", EV_TRUCK_TRACTION_CONTROL))
    {
        actorx->GetActor()->tractioncontrolToggle();
    }
}

void VehicleInfoTPanel::DrawAbsButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->alb_nodash)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->alb_mode, m_abs_icon, "ABS", EV_TRUCK_ANTILOCK_BRAKE))
    {
        actorx->GetActor()->antilockbrakeToggle();
    }
}

void VehicleInfoTPanel::DrawActorPhysicsButton(RoR::GfxActor* actorx)
{
    if (DrawSingleButtonRow(actorx->GetActor()->ar_physics_paused, m_actor_physics_icon, "Pause Actor Physics", EV_TRUCK_TOGGLE_PHYSICS))
    {
        actorx->GetActor()->ar_physics_paused = !actorx->GetActor()->ar_physics_paused;
    }
}

void VehicleInfoTPanel::DrawAxleDiffButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->getAxleDiffMode() == 0)
    {
        return;
    }

    if (DrawSingleButtonRow(App::GetInputEngine()->getEventBoolValue(EV_TRUCK_TOGGLE_INTER_AXLE_DIFF), m_a_icon, "Axle Differential", EV_TRUCK_TOGGLE_INTER_AXLE_DIFF))
    {
        actorx->GetActor()->toggleAxleDiffMode();
        actorx->GetActor()->displayAxleDiffMode();
    }
}

void VehicleInfoTPanel::DrawWheelDiffButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->getWheelDiffMode() == 0)
    {
        return;
    }
    
    if (DrawSingleButtonRow(App::GetInputEngine()->getEventBoolValue(EV_TRUCK_TOGGLE_INTER_WHEEL_DIFF), m_w_icon, "Wheel Differential", EV_TRUCK_TOGGLE_INTER_WHEEL_DIFF))
    {
        actorx->GetActor()->toggleWheelDiffMode();
        actorx->GetActor()->displayWheelDiffMode();
    }
}

void VehicleInfoTPanel::DrawTransferCaseModeButton(RoR::GfxActor* actorx)
{
    if (!actorx->GetActor()->ar_engine || !actorx->GetActor()->getTransferCaseMode() ||
         actorx->GetActor()->getTransferCaseMode()->tr_ax_2 < 0 || !actorx->GetActor()->getTransferCaseMode()->tr_2wd)
    {
        return;
    }

    if (DrawSingleButtonRow(App::GetInputEngine()->getEventBoolValue(EV_TRUCK_TOGGLE_TCASE_4WD_MODE), m_m_icon, "Transfer Case 4WD", EV_TRUCK_TOGGLE_TCASE_4WD_MODE))
    {
        actorx->GetActor()->toggleTransferCaseMode();
        actorx->GetActor()->displayTransferCaseMode();
    }
}

void VehicleInfoTPanel::DrawTransferCaseGearRatioButton(RoR::GfxActor* actorx)
{
    if (!actorx->GetActor()->ar_engine || !actorx->GetActor()->getTransferCaseMode() ||
         actorx->GetActor()->getTransferCaseMode()->tr_gear_ratios.size() < 2)
    {
        return;
    }

    if (DrawSingleButtonRow(App::GetInputEngine()->getEventBoolValue(EV_TRUCK_TOGGLE_TCASE_GEAR_RATIO), m_g_icon, "Transfer Case Gear Ratio", EV_TRUCK_TOGGLE_TCASE_GEAR_RATIO))
    {
        actorx->GetActor()->toggleTransferCaseGearRatio();
        actorx->GetActor()->displayTransferCaseMode();
    }
}

void VehicleInfoTPanel::DrawParticlesButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->ar_num_custom_particles == 0)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->getCustomParticleMode(), m_particle_icon, "Particles", EV_COMMON_TOGGLE_CUSTOM_PARTICLES))
    {
        actorx->GetActor()->toggleCustomParticles();
    }
}

void VehicleInfoTPanel::DrawBeaconButton(RoR::GfxActor* actorx)
{
    bool has_beacon = false;
    for (Prop& prop: actorx->getProps())
    {
        if (prop.pp_beacon_type != 0)
        {
            has_beacon = true;
        }
    }

    if (!has_beacon)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->getBeaconMode(), m_beacons_icon, "Beacons", EV_COMMON_TOGGLE_TRUCK_BEACONS))
    {
        actorx->GetActor()->beaconsToggle();
    }
}

void VehicleInfoTPanel::DrawShiftModeButton(RoR::GfxActor* actorx)
{
    if (!actorx->GetActor()->ar_engine)
    {
        return;
    }

    if (DrawSingleButtonRow(App::GetInputEngine()->getEventBoolValue(EV_TRUCK_SWITCH_SHIFT_MODES), m_shift_icon, "Shift Mode", EV_TRUCK_SWITCH_SHIFT_MODES))
    {
        actorx->GetActor()->ar_engine->ToggleAutoShiftMode();
        // force gui update
        actorx->GetActor()->RequestUpdateHudFeatures();

        // Inform player via chatbox
        const char* msg = nullptr;
        switch (actorx->GetActor()->ar_engine->GetAutoShiftMode())
        {
        case SimGearboxMode::AUTO: msg = "Automatic shift";
            break;
        case SimGearboxMode::SEMI_AUTO: msg = "Manual shift - Auto clutch";
            break;
        case SimGearboxMode::MANUAL: msg = "Fully Manual: sequential shift";
            break;
        case SimGearboxMode::MANUAL_STICK: msg = "Fully manual: stick shift";
            break;
        case SimGearboxMode::MANUAL_RANGES: msg = "Fully Manual: stick shift with ranges";
            break;
        }
        App::GetConsole()->putMessage(RoR::Console::CONSOLE_MSGTYPE_INFO, RoR::Console::CONSOLE_SYSTEM_NOTICE, _L(msg), "cog.png");
    }
}

void VehicleInfoTPanel::DrawEngineButton(RoR::GfxActor* actorx)
{
    if (!actorx->GetActor()->ar_engine)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->ar_engine->isRunning(), m_engine_icon, "Ignition", EV_TRUCK_TOGGLE_CONTACT))
    {
        if (actorx->GetActor()->ar_engine && actorx->GetActor()->ar_engine->isRunning())
        {
            actorx->GetActor()->ar_engine->toggleContact();
        }
        else if (actorx->GetActor()->ar_engine)
        {
            actorx->GetActor()->ar_engine->StartEngine();
        }
    }
}

void VehicleInfoTPanel::DrawCustomLightButton(RoR::GfxActor* actorx)
{
    int num_custom_flares = 0;

    for (int i = 0; i < MAX_CLIGHTS; i++)
    {
        if (actorx->GetActor()->countCustomLights(i) > 0)
        {
            ImGui::PushID(i);
            num_custom_flares++;

            if (i == 5 || i == 9) // Add new line every 4 buttons
            {
                ImGui::NewLine();
            }

            std::string label = "L" + std::to_string(i + 1);

            if (actorx->GetActor()->getCustomLightVisible(i))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
            }

            if (ImGui::Button(label.c_str(), ImVec2(32, 0)))
            {
                actorx->GetActor()->setCustomLightVisible(i, !actorx->GetActor()->getCustomLightVisible(i));
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::TextDisabled("%s %d (%s)", "Custom Light", i + 1, App::GetInputEngine()->getEventCommandTrimmed(EV_TRUCK_LIGHTTOGGLE01 + i).c_str());
                ImGui::EndTooltip();
            }
            ImGui::SameLine();

            ImGui::PopStyleColor();
            ImGui::PopID();
        }
    }
    if (num_custom_flares > 0)
    {
        ImGui::NewLine();
    }
}



void VehicleInfoTPanel::DrawCameraButton()
{
    if (DrawSingleButtonRow(false, m_camera_icon, "Switch Camera", EV_CAMERA_CHANGE))
    {
        if (App::GetCameraManager()->EvaluateSwitchBehavior())
        {
            App::GetCameraManager()->switchToNextBehavior();
        }
    }
}

void VehicleInfoTPanel::DrawLockButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->ar_hooks.empty())
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->isLocked(), m_lock_icon, "Lock", EV_COMMON_LOCK))
    {
        //actorx->GetActor()->hookToggle(-1, HOOK_TOGGLE, -1);
        ActorLinkingRequest* hook_rq = new ActorLinkingRequest();
        hook_rq->alr_type = ActorLinkingRequestType::HOOK_TOGGLE;
        hook_rq->alr_actor_instance_id = actorx->GetActor()->ar_instance_id;
        App::GetGameContext()->PushMessage(Message(MSG_SIM_ACTOR_LINKING_REQUESTED, hook_rq));

        //actorx->GetActor()->toggleSlideNodeLock();
        ActorLinkingRequest* slidenode_rq = new ActorLinkingRequest();
        slidenode_rq->alr_type = ActorLinkingRequestType::SLIDENODE_TOGGLE;
        slidenode_rq->alr_actor_instance_id = actorx->GetActor()->ar_instance_id;
        App::GetGameContext()->PushMessage(Message(MSG_SIM_ACTOR_LINKING_REQUESTED, slidenode_rq));
    }
}

void VehicleInfoTPanel::DrawSecureButton(RoR::GfxActor* actorx)
{
    if (actorx->GetActor()->ar_ties.empty())
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->isTied(), m_secure_icon, "Secure", EV_COMMON_SECURE_LOAD))
    {
        //actorx->GetActor()->tieToggle(-1, TIE_TOGGLE, -1);
        ActorLinkingRequest* tie_rq = new ActorLinkingRequest();
        tie_rq->alr_type = ActorLinkingRequestType::TIE_TOGGLE;
        tie_rq->alr_actor_instance_id = actorx->GetActor()->ar_instance_id;
        App::GetGameContext()->PushMessage(Message(MSG_SIM_ACTOR_LINKING_REQUESTED, tie_rq));
    }
}

void VehicleInfoTPanel::DrawCruiseControlButton(RoR::GfxActor* actorx)
{
    if (!actorx->GetActor()->ar_engine)
    {
        return;
    }

    if (DrawSingleButtonRow(actorx->GetActor()->cc_mode, m_cruise_control_icon, "Cruise Control", EV_TRUCK_CRUISE_CONTROL))
    {
        actorx->GetActor()->cruisecontrolToggle();
    }
}

void VehicleInfoTPanel::CacheIcons()
{
    // Icons used https://materialdesignicons.com/
    // Licence https://github.com/Templarian/MaterialDesign/blob/master/LICENSE

    m_headlight_icon = FetchIcon("car-light-high.png");
    m_left_blinker_icon = FetchIcon("arrow-left-bold.png");
    m_right_blinker_icon = FetchIcon("arrow-right-bold.png");
    m_warning_light_icon = FetchIcon("hazard-lights.png");
    m_horn_icon = FetchIcon("bugle.png");
    m_mirror_icon = FetchIcon("mirror-rectangle.png");
    m_repair_icon = FetchIcon("car-wrench.png");
    m_parking_brake_icon = FetchIcon("car-brake-alert.png");
    m_traction_control_icon = FetchIcon("car-traction-control.png");
    m_abs_icon = FetchIcon("car-brake-abs.png");
    m_physics_icon = FetchIcon("pause.png");
    m_actor_physics_icon = FetchIcon("pause-circle-outline.png");
    m_a_icon = FetchIcon("alpha-a-circle.png");
    m_w_icon = FetchIcon("alpha-w-circle.png");
    m_m_icon = FetchIcon("alpha-m-circle.png");
    m_g_icon = FetchIcon("alpha-g-circle.png");
    m_particle_icon = FetchIcon("water.png");
    m_shift_icon = FetchIcon("car-shift-pattern.png");
    m_engine_icon = FetchIcon("engine.png");
    m_beacons_icon = FetchIcon("alarm-light-outline.png");
    m_camera_icon = FetchIcon("camera-switch-outline.png");
    m_lock_icon = FetchIcon("alpha-l-circle.png");
    m_secure_icon = FetchIcon("lock-outline.png");
    m_cruise_control_icon = FetchIcon("car-cruise-control.png");

    m_icons_cached = true;
}