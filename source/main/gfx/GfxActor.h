/*
    This source file is part of Rigs of Rods
    Copyright 2005-2012 Pierre-Michel Ricordel
    Copyright 2007-2012 Thomas Fischer
    Copyright 2016-2020 Petr Ohlidal

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

/// @file
/// @author Petr Ohlidal
/// @date   2017/04
/// @brief  Manager for all visuals belonging to a single actor.

#pragma once

#include "AutoPilot.h"
#include "Differentials.h"
#include "ForwardDeclarations.h"
#include "GfxData.h"
#include "RigDef_Prerequisites.h"
#include "SimBuffers.h"
#include "SurveyMapEntity.h"
#include "ThreadPool.h" // class Task

#include <OgreAxisAlignedBox.h>
#include <OgreColourValue.h>
#include <OgreMaterial.h>
#include <OgreQuaternion.h>
#include <OgreTexture.h>
#include <OgreVector3.h>
#include <string>
#include <vector>

namespace RoR {

/// @addtogroup Gfx
/// @{

class GfxActor
{
    friend class ActorSpawner; // The factory

public:

    GfxActor(ActorPtr actor, ActorSpawner* spawner, std::string ogre_resource_group,
        RoR::Renderdash* renderdash);

    ~GfxActor();

    // Adding elements

    void                 RegisterCabMaterial(Ogre::MaterialPtr mat, Ogre::MaterialPtr mat_trans);
    void                 RegisterCabMesh(Ogre::Entity* ent, Ogre::SceneNode* snode, FlexObj* flexobj);
    void                 SortFlexbodies();

    // Retrieving elements

    std::vector<FlexBody*>& GetFlexbodies() { return m_flexbodies; };
    std::vector<Prop>&      getProps() { return m_props; }
    std::vector<CParticle>& getCParticles() { return m_cparticles; }
    std::vector<Exhaust>&   getExhausts() { return m_exhausts; }
    std::vector<VideoCamera>& getVideoCameras() { return m_videocameras; }

    // Visual changes

    void                 SetMaterialFlareOn(int flare_index, bool state_on);
    void                 SetCabLightsActive(bool state_on);
    void                 SetVideoCamState(VideoCamState state);
    void                 ScaleActor(Ogre::Vector3 relpos, float ratio);
    void                 ToggleDebugView();
    void                 CycleDebugViews();
    void                 ResetFlexbodies();
    void                 SetRenderdashActive(bool active);
    void                 SetBeaconsEnabled(bool beacon_light_is_active);
    void                 SetDebugView(DebugViewType dv);
    void                 SetNodeHot(NodeNum_t nodenum, bool value);
    void                 RemoveBeam(int beam_index);

    // Visibility

    void                 SetRodsVisible(bool visible);
    void                 SetFlexbodyVisible (bool visible);
    void                 SetWheelsVisible(bool value);
    void                 SetAllMeshesVisible(bool value);
    void                 SetWingsVisible(bool visible);
    void                 SetCastShadows(bool value);
    void                 SetFlexbodiesVisible(bool visible);
    void                 SetPropsVisible(bool visible);
    void                 SetAeroEnginesVisible(bool visible);

    // Visual updates

    void                 UpdateVideoCameras(float dt);
    void                 UpdateParticles(float dt);
    void                 UpdateRods();
    void                 UpdateWheelVisuals();
    void                 UpdateFlexbodies();
    void                 UpdateDebugView();
    void                 UpdateCabMesh();
    void                 UpdateWingMeshes();
    void                 UpdateBeaconFlare(Prop & prop, float dt, bool is_player_actor);
    void                 UpdateProps(float dt, bool is_player_actor);
    void                 UpdatePropAnimations(float dt);
    void                 UpdateAirbrakes();
    void                 UpdateCParticles();
    void                 UpdateExhausts();
    void                 UpdateAeroEngines();
    void                 UpdateNetLabels(float dt);
    void                 UpdateFlares(float dt, bool is_player);
    void                 UpdateRenderdashRTT ();

    // SimBuffers

    void                 UpdateSimDataBuffer(); //!< Copies sim. data from `Actor` to `GfxActor` for later update
    ActorSB&             GetSimDataBuffer() { return m_simbuf; }
    NodeSB*              GetSimNodeBuffer() { return m_simbuf.simbuf_nodes.data(); }

    // Internal updates

    void                 FinishWheelUpdates();
    void                 FinishFlexbodyTasks();

    // Helpers

    bool                 IsActorLive() const; //!< Should the visuals be updated for this actor?
    bool                 IsActorInitialized() const  { return m_initialized; } //!< Temporary TODO: Remove once the spawn routine is fixed
    void                 InitializeActor() { m_initialized = true; } //!< Temporary TODO: Remove once the spawn routine is fixed
    void                 CalculateDriverPos(Ogre::Vector3& out_pos, Ogre::Quaternion& out_rot);
    int                  GetActorId() const;
    int                  GetActorState() const;
    VideoCamState        GetVideoCamState() const { return m_vidcam_state; }
    DebugViewType        GetDebugView() const { return m_debug_view; }
    Ogre::String         GetResourceGroup() { return m_custom_resource_group; }
    ActorPtr             GetActor(); // Watch out for multithreading with this!
    Ogre::MaterialPtr&   GetCabTransMaterial() { return m_cab_mat_visual_trans; }
    Ogre::TexturePtr     GetHelpTex() { return m_help_tex; }
    Ogre::MaterialPtr    GetHelpMat() { return m_help_mat; }
    bool                 HasDriverSeatProp() const { return m_driverseat_prop_index != -1; }
    void                 CalcPropAnimation(PropAnim& anim, float& cstate, int& div, float dt);
    SurveyMapEntity&     getSurveyMapEntity() { return m_surveymap_entity; }
    WheelSide            getWheelSide(WheelID_t wheel_id) { return (wheel_id >= 0 && (size_t)wheel_id < m_wheels.size()) ? m_wheels[wheel_id].wx_side : WheelSide::INVALID; }
    std::string          getWheelRimMeshName(WheelID_t wheel_id) { return (wheel_id >= 0 && (size_t)wheel_id < m_wheels.size()) ? m_wheels[wheel_id].wx_rim_mesh_name : ""; }
    const ActorPtr&      getOwningActor() { return m_actor; }
    int                  countBeaconProps() const;
private:

    float UpdateSmoothShift(PropAnim& anim, float dt, float new_target_cstate); // Helper for `CalcPropAnimation()`

    // Static info
    ActorPtr                    m_actor;
    std::string                 m_custom_resource_group;
    int                         m_driverseat_prop_index = -1;
    Ogre::SceneNode*            m_gfx_beams_parent_scenenode = nullptr;

    // Game state
    bool                        m_initialized = false;
    VideoCamState               m_vidcam_state = VideoCamState::VCSTATE_ENABLED_ONLINE;
    DebugViewType               m_debug_view = DebugViewType::DEBUGVIEW_NONE;
    DebugViewType               m_last_debug_view = DebugViewType::DEBUGVIEW_SKELETON; // intentional
    bool                        m_beaconlight_active = true; // 'true' will trigger SetBeaconsEnabled(false) on the first buffer update
    float                       m_prop_anim_crankfactor_prev = 0.f;
    float                       m_prop_anim_shift_timer = 0.f;
    int                         m_prop_anim_prev_gear = 0;

    // Threaded tasks
    std::vector<std::shared_ptr<Task>> m_flexwheel_tasks;
    std::vector<std::shared_ptr<Task>> m_flexbody_tasks;

    // Elements
    std::vector<NodeGfx>        m_gfx_nodes;
    std::vector<BeamGfx>        m_gfx_beams;
    std::vector<AirbrakeGfx>    m_gfx_airbrakes;
    std::vector<Prop>           m_props;
    std::vector<FlexBody*>      m_flexbodies;
    std::vector<WheelGfx>       m_wheels;
    std::vector<VideoCamera>    m_videocameras;
    std::vector<FlareMaterial>  m_flare_materials;
    RoR::Renderdash*            m_renderdash = nullptr;
    std::vector<CParticle>      m_cparticles;
    std::vector<Exhaust>        m_exhausts;
    
    // Particles
    DustPool*                   m_particles_drip = nullptr;
    DustPool*                   m_particles_dust = nullptr; // dust, vapour and tyre smoke
    DustPool*                   m_particles_splash = nullptr;
    DustPool*                   m_particles_ripple = nullptr;
    DustPool*                   m_particles_sparks = nullptr;
    DustPool*                   m_particles_clump = nullptr;

    // Cab mesh ('submesh' in truck fileformat)
    FlexObj*                    m_cab_mesh = nullptr;
    Ogre::SceneNode*            m_cab_scene_node = nullptr;
    Ogre::Entity*               m_cab_entity = nullptr;
    Ogre::MaterialPtr           m_cab_mat_visual; //!< Updated in-place from templates
    Ogre::MaterialPtr           m_cab_mat_visual_trans;
    Ogre::MaterialPtr           m_cab_mat_template_plain;
    Ogre::MaterialPtr           m_cab_mat_template_emissive;

    // GUI
    Ogre::MaterialPtr           m_help_mat;
    Ogre::TexturePtr            m_help_tex;
    SurveyMapEntity             m_surveymap_entity;

    ActorSB                     m_simbuf;
};

/// @} // addtogroup Gfx

} // Namespace RoR
