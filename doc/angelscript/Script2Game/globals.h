/*
This source file is part of Rigs of Rods

For more information, see http://www.rigsofrods.org/

Rigs of Rods is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3, as
published by the Free Software Foundation.

Rigs of Rods is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Rigs of Rods.  If not, see <http://www.gnu.org/licenses/>.
*/
// created on 25th May 2011.

/** \addtogroup ScriptSideAPIs
 *  @{
 */    

/** \addtogroup Script2Game
 *  @{
 */   

namespace Script2Game {

/**
 * This is an alias for game.log(string message).
 * @see game.log
 */
void log(const string message);

/**
 * This is an alias for game.log(string message).
 * @see game.log
 */
void print(const string message);

/**
 * Binding of `RoR::scriptEvents`; All the events that can be used by the script.
 * @see Script2Game::GameScriptClass::registerForEvent()
 */
 enum scriptEvents
 {
    SE_EVENTBOX_ENTER                  //!< An actor or person entered an eventbox; Arguments of `eventCallbackEx()`: #1 type, #2 Actor Instance ID (use `game.getTruckByNum()`), #3 Actor node ID, #4 unused, #5 object instance name, #6 eventbox name #7 unused #8 unused.
    SE_EVENTBOX_EXIT                   //!< An actor or person entered an eventbox; Arguments of `eventCallbackEx()`: #1 type, #2 Actor Instance ID (use `game.getTruckByNum()`), #3 unused, #4 unused, #5 object instance name, #6 eventbox name #7 unused #8 unused.
     
 	SE_TRUCK_ENTER                     //!< triggered when switching from person mode to truck mode, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_TRUCK_EXIT                      //!< triggered when switching from truck mode to person mode, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)

 	SE_TRUCK_ENGINE_DIED               //!< triggered when the trucks engine dies (from underrev, water, etc), the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_TRUCK_ENGINE_FIRE               //!< triggered when the planes engines start to get on fire, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_TRUCK_TOUCHED_WATER             //!< triggered when any part of the truck touches water, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_TRUCK_LIGHT_TOGGLE              //!< triggered when the main light is toggled, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_TRUCK_TIE_TOGGLE                //!< triggered when the user toggles ties, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_TRUCK_PARKINGBRAKE_TOGGLE       //!< triggered when the user toggles the parking brake, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_TRUCK_BEACONS_TOGGLE            //!< triggered when the user toggles beacons, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_TRUCK_CPARTICLES_TOGGLE         //!< triggered when the user toggles custom particles, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)

 	SE_GENERIC_NEW_TRUCK               //!< triggered when the user spawns a new truck, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
 	SE_GENERIC_DELETED_TRUCK           //!< triggered when the user deletes a truck, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
    
    SE_TRUCK_RESET                     //!< triggered when the user resets the truck, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
    SE_TRUCK_TELEPORT                  //!< triggered when the user teleports the truck, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)
    SE_TRUCK_MOUSE_GRAB                //!< triggered when the user uses the mouse to interact with the actor, the argument refers to the Actor Instance ID (use `game.getTruckByNum()`)

    SE_ANGELSCRIPT_MANIPULATIONS       //!< triggered when the user tries to dynamically use the scripting capabilities (prevent cheating) args:  #1 `angelScriptManipulationType` - see enum doc comments for more args.
    SE_ANGELSCRIPT_MSGCALLBACK         //!< The diagnostic info directly from AngelScript engine (see `asSMessageInfo`), args: #1 ScriptUnitID, #2 asEMsgType, #3 row, #4 col, #5 sectionName, #6 message
    SE_ANGELSCRIPT_LINECALLBACK        //!< The diagnostic info directly from AngelScript engine (see `SetLineCallback()`), args: #1 ScriptUnitID, #2 LineNumber, #3 CallstackSize, #4 unused, #5 FunctionName, #6 FunctionObjectTypeName #7 ObjectName
    SE_ANGELSCRIPT_EXCEPTIONCALLBACK   //!< The diagnostic info directly from AngelScript engine (see `SetExceptionCallback()`), args: #1 ScriptUnitID, #2 unused, #3 row (`GetExceptionLineNumber()`), #4 unused, #5 funcName, #6 message (`GetExceptionString()`)
    SE_ANGELSCRIPT_THREAD_STATUS       //!< Sent by background threads (i.e. CURL) when there's something important (like finishing a download). args: #1 type, see `Script2Game::angelScriptThreadStatus`.    

 	SE_GENERIC_MESSAGEBOX_CLICK        //!< triggered when the user clicks on a message box button, the argument refers to the button pressed
    SE_GENERIC_EXCEPTION_CAUGHT        //!< Triggered when C++ exception (usually Ogre::Exception) is thrown; #1 ScriptUnitID, #5 originFuncName, #6 type, #7 message.
    SE_GENERIC_MODCACHE_ACTIVITY       //!< Triggered when status of modcache changes, args: #1 type, #2 entry number, for other args see `RoR::modCacheActivityType`  

    SE_GENERIC_TRUCK_LINKING_CHANGED   //!< Triggered when 2 actors become linked or unlinked via ties/hooks/ropes/slidenodes; args: #1 state (1=linked, 0=unlinked), #2 action `ActorLinkingRequestType` #3 master ActorInstanceID_t, #4 slave ActorInstanceID_t

 	SE_ALL_EVENTS                      = 0xffffffff,
    SE_NO_EVENTS                       = 0

 };
 
/// Argument #1 of script event `SE_ANGELSCRIPT_MANIPULATIONS`
enum angelScriptManipulationType
{
    ASMANIP_CONSOLE_SNIPPET_EXECUTED = 0, // 0 for Backwards compatibility.
    ASMANIP_SCRIPT_LOADED,                //!< Triggered after the script's `main()` completed; may trigger additional processing (for example, it delivers the *.mission file to mission system script). Args: #2 ScriptUnitId_t, #3 RoR::ScriptCategory, #4 unused, #5 filename.
    ASMANIP_SCRIPT_UNLOADING,              //!< Triggered before unloading the script to let it clean up (important for missions). Args: #2 ScriptUnitId_t, #3 RoR::ScriptCategory, #4 unused, #5 filename.
    ASMANIP_ACTORSIMATTR_SET              //!< Triggered when `setSimAttribute()` is called; additional args: #2 `RoR::ActorSimAtr`, #3 ---, #4 ---, #5 attr name, #6 value converted to string.
};

enum angelScriptThreadStatus
{
    ASTHREADSTATUS_NONE,
    ASTHREADSTATUS_CURLSTRING_PROGRESS, //!< Args of `RoR::SE_ANGELSCRIPT_THREAD_STATUS`: arg#1 type, arg#2 percentage, arg#3 unused, arg#4 unused, arg#5 progress message (formatted by RoR)
    ASTHREADSTATUS_CURLSTRING_SUCCESS,  //!< Args of `RoR::SE_ANGELSCRIPT_THREAD_STATUS`: arg#1 type, arg#2 HTTP code, arg#3 CURLcode, arg#4 unused, arg#5 payload
    ASTHREADSTATUS_CURLSTRING_FAILURE,  //!< Args of `RoR::SE_ANGELSCRIPT_THREAD_STATUS`: arg#1 type, arg#2 HTTP code, arg#3 CURLcode, arg#4 unused, arg#5 message from `curl_easy_strerror()`
};

/// Argument #1 of script event `RoR::SE_GENERIC_MODCACHE_ACTIVITY`
enum modCacheActivityType
{
    MODCACHEACTIVITY_NONE,

    MODCACHEACTIVITY_ENTRY_ADDED,      //!< Args of `RoR::SE_GENERIC_MODCACHE_NOTIFICATION`: #1 type, #2 entry number, --, --, #5 fname, #6 fext
    MODCACHEACTIVITY_ENTRY_DELETED,    //!< Flagged as `deleted`, remains in memory until shared pointers expire; Args of `RoR::SE_GENERIC_MODCACHE_NOTIFICATION`: #1 type, #2 entry number, --, --, #5 fname, #6 fext

    MODCACHEACTIVITY_BUNDLE_LOADED,    //!< Args of `RoR::SE_GENERIC_MODCACHE_NOTIFICATION`: #1 type, #2 entry number, --, --, #5 rg name
    MODCACHEACTIVITY_BUNDLE_RELOADED,  //!< Args of `RoR::SE_GENERIC_MODCACHE_NOTIFICATION`: #1 type, #2 entry number, --, --, #5 rg name
    MODCACHEACTIVITY_BUNDLE_UNLOADED   //!< Args of `RoR::SE_GENERIC_MODCACHE_NOTIFICATION`: #1 type, #2 entry number
};

/// Argument #1 of script event `RoR::SE_GENERIC_FREEFORCES_ACTIVITY`
enum freeForcesActivityType
{
    FREEFORCESACTIVITY_NONE,

    FREEFORCESACTIVITY_ADDED,
    FREEFORCESACTIVITY_MODIFIED,
    FREEFORCESACTIVITY_REMOVED,

    FREEFORCESACTIVITY_DEFORMED, //!< Only with `HALFBEAM_*` types; arg #5 (string containing float) the actual stress, arg #6 (string containing float) maximum stress.
    FREEFORCESACTIVITY_BROKEN, //!< Only with `HALFBEAM_*` types; arg #5 (string containing float) the applied force, arg #6 (string containing float) breaking threshold force.
};

enum inputEvents
{
    EV_AIRPLANE_AIRBRAKES_FULL=0,
    EV_AIRPLANE_AIRBRAKES_LESS,
    EV_AIRPLANE_AIRBRAKES_MORE,
    EV_AIRPLANE_AIRBRAKES_NONE,
    EV_AIRPLANE_BRAKE,           //!< normal brake for an aircraft.
    EV_AIRPLANE_ELEVATOR_DOWN,   //!< pull the elevator down in an aircraft.
    EV_AIRPLANE_ELEVATOR_UP,     //!< pull the elevator up in an aircraft.
    EV_AIRPLANE_FLAPS_FULL,      //!< full flaps in an aircraft.
    EV_AIRPLANE_FLAPS_LESS,      //!< one step less flaps.
    EV_AIRPLANE_FLAPS_MORE,      //!< one step more flaps.
    EV_AIRPLANE_FLAPS_NONE,      //!< no flaps.
    EV_AIRPLANE_PARKING_BRAKE,   //!< airplane parking brake.
    EV_AIRPLANE_REVERSE,         //!< reverse the turboprops
    EV_AIRPLANE_RUDDER_LEFT,     //!< rudder left
    EV_AIRPLANE_RUDDER_RIGHT,    //!< rudder right
    EV_AIRPLANE_STEER_LEFT,      //!< steer left
    EV_AIRPLANE_STEER_RIGHT,     //!< steer right
    EV_AIRPLANE_THROTTLE,        
    EV_AIRPLANE_THROTTLE_AXIS,   //!< throttle axis. Only use this if you have fitting hardware :) (i.e. a Slider)
    EV_AIRPLANE_THROTTLE_DOWN,   //!< decreases the airplane thrust
    EV_AIRPLANE_THROTTLE_FULL,   //!< full thrust
    EV_AIRPLANE_THROTTLE_NO,     //!< no thrust
    EV_AIRPLANE_THROTTLE_UP,     //!< increase the airplane thrust
    EV_AIRPLANE_TOGGLE_ENGINES,  //!< switch all engines on / off
    EV_BOAT_CENTER_RUDDER,       //!< center the rudder
    EV_BOAT_REVERSE,             //!< no thrust
    EV_BOAT_STEER_LEFT,          //!< steer left a step
    EV_BOAT_STEER_LEFT_AXIS,     //!< steer left (analog value!)
    EV_BOAT_STEER_RIGHT,         //!< steer right a step
    EV_BOAT_STEER_RIGHT_AXIS,    //!< steer right (analog value!)
    EV_BOAT_THROTTLE_AXIS,       //!< throttle axis. Only use this if you have fitting hardware :) (i.e. a Slider)
    EV_BOAT_THROTTLE_DOWN,       //!< decrease throttle
    EV_BOAT_THROTTLE_UP,         //!< increase throttle
    EV_SKY_DECREASE_TIME,        //!< decrease day-time
    EV_SKY_DECREASE_TIME_FAST,   //!< decrease day-time a lot faster
    EV_SKY_INCREASE_TIME,        //!< increase day-time
    EV_SKY_INCREASE_TIME_FAST,   //!< increase day-time a lot faster
    EV_CAMERA_CHANGE,            //!< change camera mode
    EV_CAMERA_DOWN,
    EV_CAMERA_FREE_MODE,
    EV_CAMERA_FREE_MODE_FIX,
    EV_CAMERA_LOOKBACK,          //!< look back (toggles between normal and lookback)
    EV_CAMERA_RESET,             //!< reset the camera position
    EV_CAMERA_ROTATE_DOWN,       //!< rotate camera down
    EV_CAMERA_ROTATE_LEFT,       //!< rotate camera left
    EV_CAMERA_ROTATE_RIGHT,      //!< rotate camera right
    EV_CAMERA_ROTATE_UP,         //!< rotate camera up
    EV_CAMERA_UP,
    EV_CAMERA_ZOOM_IN,           //!< zoom camera in
    EV_CAMERA_ZOOM_IN_FAST,      //!< zoom camera in faster
    EV_CAMERA_ZOOM_OUT,          //!< zoom camera out
    EV_CAMERA_ZOOM_OUT_FAST,     //!< zoom camera out faster
    EV_CHARACTER_BACKWARDS,      //!< step backwards with the character
    EV_CHARACTER_FORWARD,        //!< step forward with the character
    EV_CHARACTER_JUMP,           //!< let the character jump
    EV_CHARACTER_LEFT,           //!< rotate character left
    EV_CHARACTER_RIGHT,          //!< rotate character right
    EV_CHARACTER_ROT_DOWN,
    EV_CHARACTER_ROT_UP,
    EV_CHARACTER_RUN,            //!< let the character run
    EV_CHARACTER_SIDESTEP_LEFT,  //!< sidestep to the left
    EV_CHARACTER_SIDESTEP_RIGHT, //!< sidestep to the right
    EV_COMMANDS_01,              //!< Command 1
    EV_COMMANDS_02,              //!< Command 2
    EV_COMMANDS_03,              //!< Command 3
    EV_COMMANDS_04,              //!< Command 4
    EV_COMMANDS_05,              //!< Command 5
    EV_COMMANDS_06,              //!< Command 6
    EV_COMMANDS_07,              //!< Command 7
    EV_COMMANDS_08,              //!< Command 8
    EV_COMMANDS_09,              //!< Command 9
    EV_COMMANDS_10,              //!< Command 10
    EV_COMMANDS_11,              //!< Command 11
    EV_COMMANDS_12,              //!< Command 12
    EV_COMMANDS_13,              //!< Command 13
    EV_COMMANDS_14,              //!< Command 14
    EV_COMMANDS_15,              //!< Command 15
    EV_COMMANDS_16,              //!< Command 16
    EV_COMMANDS_17,              //!< Command 17
    EV_COMMANDS_18,              //!< Command 18
    EV_COMMANDS_19,              //!< Command 19
    EV_COMMANDS_20,              //!< Command 20
    EV_COMMANDS_21,              //!< Command 21
    EV_COMMANDS_22,              //!< Command 22
    EV_COMMANDS_23,              //!< Command 23
    EV_COMMANDS_24,              //!< Command 24
    EV_COMMANDS_25,              //!< Command 25
    EV_COMMANDS_26,              //!< Command 26
    EV_COMMANDS_27,              //!< Command 27
    EV_COMMANDS_28,              //!< Command 28
    EV_COMMANDS_29,              //!< Command 29
    EV_COMMANDS_30,              //!< Command 30
    EV_COMMANDS_31,              //!< Command 31
    EV_COMMANDS_32,              //!< Command 32
    EV_COMMANDS_33,              //!< Command 33
    EV_COMMANDS_34,              //!< Command 34
    EV_COMMANDS_35,              //!< Command 35
    EV_COMMANDS_36,              //!< Command 36
    EV_COMMANDS_37,              //!< Command 37
    EV_COMMANDS_38,              //!< Command 38
    EV_COMMANDS_39,              //!< Command 39
    EV_COMMANDS_40,              //!< Command 40
    EV_COMMANDS_41,              //!< Command 41
    EV_COMMANDS_42,              //!< Command 42
    EV_COMMANDS_43,              //!< Command 43
    EV_COMMANDS_44,              //!< Command 44
    EV_COMMANDS_45,              //!< Command 45
    EV_COMMANDS_46,              //!< Command 46
    EV_COMMANDS_47,              //!< Command 47
    EV_COMMANDS_48,              //!< Command 48
    EV_COMMANDS_49,              //!< Command 49
    EV_COMMANDS_50,              //!< Command 50
    EV_COMMANDS_51,              //!< Command 51
    EV_COMMANDS_52,              //!< Command 52
    EV_COMMANDS_53,              //!< Command 53
    EV_COMMANDS_54,              //!< Command 54
    EV_COMMANDS_55,              //!< Command 55
    EV_COMMANDS_56,              //!< Command 56
    EV_COMMANDS_57,              //!< Command 57
    EV_COMMANDS_58,              //!< Command 58
    EV_COMMANDS_59,              //!< Command 59
    EV_COMMANDS_60,              //!< Command 50
    EV_COMMANDS_61,              //!< Command 61
    EV_COMMANDS_62,              //!< Command 62
    EV_COMMANDS_63,              //!< Command 63
    EV_COMMANDS_64,              //!< Command 64
    EV_COMMANDS_65,              //!< Command 65
    EV_COMMANDS_66,              //!< Command 66
    EV_COMMANDS_67,              //!< Command 67
    EV_COMMANDS_68,              //!< Command 68
    EV_COMMANDS_69,              //!< Command 69
    EV_COMMANDS_70,              //!< Command 70
    EV_COMMANDS_71,              //!< Command 71
    EV_COMMANDS_72,              //!< Command 72
    EV_COMMANDS_73,              //!< Command 73
    EV_COMMANDS_74,              //!< Command 74
    EV_COMMANDS_75,              //!< Command 75
    EV_COMMANDS_76,              //!< Command 76
    EV_COMMANDS_77,              //!< Command 77
    EV_COMMANDS_78,              //!< Command 78
    EV_COMMANDS_79,              //!< Command 79
    EV_COMMANDS_80,              //!< Command 80
    EV_COMMANDS_81,              //!< Command 81
    EV_COMMANDS_82,              //!< Command 82
    EV_COMMANDS_83,              //!< Command 83
    EV_COMMANDS_84,              //!< Command 84
    EV_COMMON_ACCELERATE_SIMULATION, //!< accelerate the simulation speed
    EV_COMMON_DECELERATE_SIMULATION, //!< decelerate the simulation speed
    EV_COMMON_RESET_SIMULATION_PACE, //!< reset the simulation speed
    EV_COMMON_AUTOLOCK,              //!< unlock autolock hook node
    EV_COMMON_CONSOLE_TOGGLE,        //!< show / hide the console
    EV_COMMON_ENTER_CHATMODE,        //!< enter the chat mode
    EV_COMMON_ENTER_OR_EXIT_TRUCK,   //!< enter or exit a truck
    EV_COMMON_ENTER_NEXT_TRUCK,      //!< enter next truck
    EV_COMMON_ENTER_PREVIOUS_TRUCK,  //!< enter previous truck
    EV_COMMON_REMOVE_CURRENT_TRUCK,  //!< remove current truck
    EV_COMMON_RESPAWN_LAST_TRUCK,    //!< respawn last truck
    EV_COMMON_FOV_LESS,           //!<decreases the current FOV value
    EV_COMMON_FOV_MORE,           //!<increases the current FOV value
    EV_COMMON_FOV_RESET,          //!<reset the FOV value
    EV_COMMON_FULLSCREEN_TOGGLE,
    EV_COMMON_HIDE_GUI,           //!< hide all GUI elements
    EV_COMMON_TOGGLE_DASHBOARD,   //!< display or hide the dashboard overlay
    EV_COMMON_LOCK,               //!< connect hook node to a node in close proximity
    EV_COMMON_NETCHATDISPLAY,
    EV_COMMON_NETCHATMODE,
    EV_COMMON_OUTPUT_POSITION,    //!< write current position to log (you can open the logfile and reuse the position)
    EV_COMMON_GET_NEW_VEHICLE,    //!< get new vehicle
    EV_COMMON_PRESSURE_LESS,      //!< decrease tire pressure (note: only very few trucks support this)
    EV_COMMON_PRESSURE_MORE,      //!< increase tire pressure (note: only very few trucks support this)
    EV_COMMON_QUICKLOAD,          //!< quickload scene from the disk
    EV_COMMON_QUICKSAVE,          //!< quicksave scene to the disk
    EV_COMMON_QUIT_GAME,          //!< exit the game
    EV_COMMON_REPAIR_TRUCK,       //!< repair truck to original condition
    EV_COMMON_REPLAY_BACKWARD,
    EV_COMMON_REPLAY_FAST_BACKWARD,
    EV_COMMON_REPLAY_FAST_FORWARD,
    EV_COMMON_REPLAY_FORWARD,
    EV_COMMON_RESCUE_TRUCK,       //!< teleport to rescue truck
    EV_COMMON_RESET_TRUCK,        //!< reset truck to original starting position
    EV_COMMON_TOGGLE_RESET_MODE,  //!< toggle truck reset truck mode (soft vs. hard)
    EV_COMMON_ROPELOCK,           //!< connect hook node to a node in close proximity
    EV_COMMON_SAVE_TERRAIN,       //!< save terrain mesh to file
    EV_COMMON_SCREENSHOT,         //!< take a screenshot
    EV_COMMON_SCREENSHOT_BIG,     //!< take a BIG screenshot
    EV_COMMON_SECURE_LOAD,        //!< tie a load to the truck
    EV_COMMON_SEND_CHAT,          //!< send the chat text
    EV_COMMON_TOGGLE_DEBUG_VIEW,  //!< toggle debug view mode
    EV_COMMON_CYCLE_DEBUG_VIEWS,  //!< cycle debug view mode
    EV_COMMON_TOGGLE_TERRAIN_EDITOR,   //!< toggle terrain editor
    EV_COMMON_TOGGLE_CUSTOM_PARTICLES, //!< toggle particle cannon
    EV_COMMON_TOGGLE_MAT_DEBUG,   //!< debug purpose - dont use (currently not used)
    EV_COMMON_TOGGLE_RENDER_MODE, //!< toggle render mode (solid, wireframe and points)
    EV_COMMON_TOGGLE_REPLAY_MODE, //!< toggle replay mode
    EV_COMMON_TOGGLE_PHYSICS,     //!< toggle physics on/off
    EV_COMMON_TOGGLE_STATS,       //!< toggle Ogre statistics (FPS etc.)
    EV_COMMON_TOGGLE_TRUCK_BEACONS, //!< toggle truck beacons
    EV_COMMON_TOGGLE_TRUCK_LIGHTS,//!< toggle truck front lights
    EV_COMMON_TRUCK_INFO,         //!< toggle truck HUD
    EV_COMMON_TRUCK_DESCRIPTION,  //!< toggle truck description
    EV_COMMON_TRUCK_REMOVE,
    EV_GRASS_LESS,                //!< EXPERIMENTAL: remove some grass
    EV_GRASS_MORE,                //!< EXPERIMENTAL: add some grass
    EV_GRASS_MOST,                //!< EXPERIMENTAL: set maximum amount of grass
    EV_GRASS_NONE,                //!< EXPERIMENTAL: remove grass completely
    EV_GRASS_SAVE,                //!< EXPERIMENTAL: save changes to the grass density image
    EV_MENU_DOWN,                 //!< select next element in current category
    EV_MENU_LEFT,                 //!< select previous category
    EV_MENU_RIGHT,                //!< select next category
    EV_MENU_SELECT,               //!< select focussed item and close menu
    EV_MENU_UP,                   //!< select previous element in current category
    EV_SURVEY_MAP_TOGGLE_ICONS,   //!< toggle map icons
    EV_SURVEY_MAP_CYCLE,          //!< cycle overview-map mode
    EV_SURVEY_MAP_TOGGLE,         //!< toggle overview-map mode
    EV_SURVEY_MAP_ZOOM_IN,        //!< increase survey map scale
    EV_SURVEY_MAP_ZOOM_OUT,       //!< decrease survey map scale

    EV_TRUCK_ACCELERATE,             //!< accelerate the truck
    EV_TRUCK_ACCELERATE_MODIFIER_25, //!< accelerate with 25 percent pedal input
    EV_TRUCK_ACCELERATE_MODIFIER_50, //!< accelerate with 50 percent pedal input
    EV_TRUCK_ANTILOCK_BRAKE,      //!< toggle antilockbrake system
    EV_TRUCK_AUTOSHIFT_DOWN,      //!< shift automatic transmission one gear down
    EV_TRUCK_AUTOSHIFT_UP,        //!< shift automatic transmission one gear up
    EV_TRUCK_BLINK_LEFT,          //!< toggle left direction indicator (blinker)
    EV_TRUCK_BLINK_RIGHT,         //!< toggle right direction indicator (blinker)
    EV_TRUCK_BLINK_WARN,          //!< toggle all direction indicators
    EV_TRUCK_BRAKE,               //!< brake
    EV_TRUCK_BRAKE_MODIFIER_25,   //!< brake with 25 percent pedal input
    EV_TRUCK_BRAKE_MODIFIER_50,   //!< brake with 50 percent pedal input
    EV_TRUCK_CRUISE_CONTROL,      //!< toggle cruise control
    EV_TRUCK_CRUISE_CONTROL_ACCL, //!< increase target speed / rpm
    EV_TRUCK_CRUISE_CONTROL_DECL, //!< decrease target speed / rpm
    EV_TRUCK_CRUISE_CONTROL_READJUST, //!< match target speed / rpm with current truck speed / rpm
    EV_TRUCK_HORN,                //!< truck horn
    EV_TRUCK_LEFT_MIRROR_LEFT,
    EV_TRUCK_LEFT_MIRROR_RIGHT,
    EV_TRUCK_LIGHTTOGGLE01,       //!< toggle custom light 1
    EV_TRUCK_LIGHTTOGGLE02,       //!< toggle custom light 2
    EV_TRUCK_LIGHTTOGGLE03,       //!< toggle custom light 3
    EV_TRUCK_LIGHTTOGGLE04,       //!< toggle custom light 4
    EV_TRUCK_LIGHTTOGGLE05,       //!< toggle custom light 5
    EV_TRUCK_LIGHTTOGGLE06,       //!< toggle custom light 6
    EV_TRUCK_LIGHTTOGGLE07,       //!< toggle custom light 7
    EV_TRUCK_LIGHTTOGGLE08,       //!< toggle custom light 8
    EV_TRUCK_LIGHTTOGGLE09,       //!< toggle custom light 9
    EV_TRUCK_LIGHTTOGGLE10,       //!< toggle custom light 10
    EV_TRUCK_MANUAL_CLUTCH,       //!< manual clutch (for manual transmission)
    EV_TRUCK_PARKING_BRAKE,       //!< toggle parking brake
    EV_TRUCK_TRAILER_PARKING_BRAKE, //!< toggle trailer parking brake
    EV_TRUCK_RIGHT_MIRROR_LEFT,
    EV_TRUCK_RIGHT_MIRROR_RIGHT,
    EV_TRUCK_SHIFT_DOWN,          //!< shift one gear down in manual transmission mode
    EV_TRUCK_SHIFT_GEAR01,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR02,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR03,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR04,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR05,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR06,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR07,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR08,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR09,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR10,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR11,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR12,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR13,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR14,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR15,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR16,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR17,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR18,        //!< shift directly into this gear
    EV_TRUCK_SHIFT_GEAR_REVERSE,  //!< shift directly into this gear
    EV_TRUCK_SHIFT_HIGHRANGE,     //!< select high range (13-18) for H-shaft
    EV_TRUCK_SHIFT_LOWRANGE,      //!< select low range (1-6) for H-shaft
    EV_TRUCK_SHIFT_MIDRANGE,      //!< select middle range (7-12) for H-shaft
    EV_TRUCK_SHIFT_NEUTRAL,       //!< shift to neutral gear in manual transmission mode
    EV_TRUCK_SHIFT_UP,            //!< shift one gear up in manual transmission mode
    EV_TRUCK_STARTER,             //!< hold to start the engine
    EV_TRUCK_STEER_LEFT,          //!< steer left
    EV_TRUCK_STEER_RIGHT,         //!< steer right
    EV_TRUCK_SWITCH_SHIFT_MODES,  //!< toggle between transmission modes
    EV_TRUCK_TOGGLE_CONTACT,      //!< toggle ignition
    EV_TRUCK_TOGGLE_FORWARDCOMMANDS,  //!< toggle forwardcommands
    EV_TRUCK_TOGGLE_IMPORTCOMMANDS,   //!< toggle importcommands
    EV_TRUCK_TOGGLE_INTER_AXLE_DIFF,  //!< toggle the inter axle differential mode
    EV_TRUCK_TOGGLE_INTER_WHEEL_DIFF, //!< toggle the inter wheel differential mode
    EV_TRUCK_TOGGLE_PHYSICS,      //!< toggle physics simulation
    EV_TRUCK_TOGGLE_TCASE_4WD_MODE,   //!< toggle the transfer case 4wd mode
    EV_TRUCK_TOGGLE_TCASE_GEAR_RATIO, //!< toggle the transfer case gear ratio
    EV_TRUCK_TOGGLE_VIDEOCAMERA,  //!< toggle videocamera update
    EV_TRUCK_TRACTION_CONTROL,    //!< toggle antilockbrake system

    // Savegames
    EV_COMMON_QUICKSAVE_01,
    EV_COMMON_QUICKSAVE_02,
    EV_COMMON_QUICKSAVE_03,
    EV_COMMON_QUICKSAVE_04,
    EV_COMMON_QUICKSAVE_05,
    EV_COMMON_QUICKSAVE_06,
    EV_COMMON_QUICKSAVE_07,
    EV_COMMON_QUICKSAVE_08,
    EV_COMMON_QUICKSAVE_09,
    EV_COMMON_QUICKSAVE_10,

    EV_COMMON_QUICKLOAD_01,
    EV_COMMON_QUICKLOAD_02,
    EV_COMMON_QUICKLOAD_03,
    EV_COMMON_QUICKLOAD_04,
    EV_COMMON_QUICKLOAD_05,
    EV_COMMON_QUICKLOAD_06,
    EV_COMMON_QUICKLOAD_07,
    EV_COMMON_QUICKLOAD_08,
    EV_COMMON_QUICKLOAD_09,
    EV_COMMON_QUICKLOAD_10,

    EV_TRUCKEDIT_RELOAD,
}

enum keyCodes
{
    // PLEASE maintain the same order as in 'InputEngine.h' and 'InputEngineAngelscript.cpp'

    // Numpad
    KC_NUMPAD1,
    KC_NUMPAD2,
    KC_NUMPAD3,
    KC_NUMPAD4,
    KC_NUMPAD5,
    KC_NUMPAD6,
    KC_NUMPAD7,
    KC_NUMPAD8,
    KC_NUMPAD9,
    KC_NUMPAD0,

    // Number keys (not the numpad)
    KC_1,
    KC_2,
    KC_3,
    KC_4,
    KC_5,
    KC_6,
    KC_7,
    KC_8,
    KC_9,
    KC_0,

    // Function keys
    KC_F1 ,
    KC_F2 ,
    KC_F3 ,
    KC_F4 ,
    KC_F5 ,
    KC_F6 ,
    KC_F7 ,
    KC_F8 ,
    KC_F9 ,
    KC_F10,
    KC_F11,
    KC_F12,

    // Edit keys
    KC_INSERT,
    KC_DELETE,
    KC_BACKSPACE,
    KC_CAPSLOCK,
    KC_NUMLOCK,
    KC_SCROLLLOCK,
    KC_TAB,

    // Navigation keys
    KC_ESCAPE,
    KC_RETURN,
    KC_LEFT  ,
    KC_RIGHT ,
    KC_HOME  ,
    KC_UP    ,
    KC_PGUP  ,
    KC_END   ,
    KC_DOWN  ,
    KC_PGDOWN,
    KC_PAUSE ,

    // Modifiers
    KC_LCTRL ,
    KC_RCTRL ,
    KC_LSHIFT,
    KC_RSHIFT,
    KC_LALT  ,
    KC_RALT  ,
    KC_LWIN  ,
    KC_RWIN  ,

    // Special characters
    KC_MINUS     ,
    KC_EQUALS    ,
    KC_LBRACKET  ,
    KC_RBRACKET  ,
    KC_SEMICOLON ,
    KC_APOSTROPHE,
    KC_GRAVE     ,
    KC_BACKSLASH ,
    KC_COMMA     ,
    KC_PERIOD    ,
    KC_SLASH     ,
    KC_MULTIPLY  ,
    KC_SPACE     ,
    KC_SUBTRACT  ,
    KC_ADD       ,
}

/**
 * Binding of RoR::ActorType
 */
enum truckTypes {
    TT_NOT_DRIVEABLE,
    TT_TRUCK,
    TT_AIRPLANE,
    TT_BOAT,
    TT_MACHINE,
    TT_AI,
}

/**
 * Binding of RoR::ActorState
 */
enum TruckState {
	TS_SIMULATED,      //!< locally simulated and active
	TS_SLEEPING,       //!< locally simulated but sleeping
	TS_NETWORKED,      //!< controlled by network data
};

/**
 * Binding of RoR::FlareType
 */
enum FlareType {
    FLARE_TYPE_NONE,
    FLARE_TYPE_HEADLIGHT,
    FLARE_TYPE_HIGH_BEAM,
    FLARE_TYPE_FOG_LIGHT,
    FLARE_TYPE_TAIL_LIGHT,
    FLARE_TYPE_BRAKE_LIGHT,
    FLARE_TYPE_REVERSE_LIGHT,
    FLARE_TYPE_SIDELIGHT,
    FLARE_TYPE_BLINKER_LEFT,
    FLARE_TYPE_BLINKER_RIGHT,
    FLARE_TYPE_USER,
    FLARE_TYPE_DASHBOARD,
};

/**
 * Binding of RoR::BlinkType
 */
enum BlinkType {
    BLINK_NONE,
    BLINK_LEFT,
    BLINK_RIGHT,
    BLINK_WARN,
}

/**
* Binding of RoR::ActorModifyRequest::Type; use with `MSG_SIM_MODIFY_ACTOR_REQUESTED`
*/
enum ActorModifyRequestType
{
    ACTOR_MODIFY_REQUEST_INVALID,
    ACTOR_MODIFY_REQUEST_RELOAD,               //!< Full reload from filesystem, requested by user
    ACTOR_MODIFY_REQUEST_RESET_ON_INIT_POS,
    ACTOR_MODIFY_REQUEST_RESET_ON_SPOT,
    ACTOR_MODIFY_REQUEST_SOFT_RESET,
    ACTOR_MODIFY_REQUEST_RESTORE_SAVED,        //!< Internal, DO NOT USE
    ACTOR_MODIFY_REQUEST_WAKE_UP
};

/**
* Binding of `RoR::ScriptCategory` ~ for `game.pushMessage(MSG_APP_LOAD_SCRIPT_REQUESTED ...)`
*/
enum ScriptCategory
{
    SCRIPT_CATEGORY_INVALID,
    SCRIPT_CATEGORY_ACTOR,  //!< Defined in truck file under 'scripts', contains global variable `BeamClass@ thisActor`.
    SCRIPT_CATEGORY_TERRAIN, //!< Defined in terrn2 file under '[Scripts]', receives terrain eventbox notifications.
    SCRIPT_CATEGORY_CUSTOM, //!< Loaded by user via either: A) ingame console 'loadscript'; B) RoR.cfg 'diag_custom_scripts'; C) commandline '-runscript'.
};

/**
* Binding of RoR::MsgType; Global gameplay message loop.
*/
enum MsgType
{
    MSG_INVALID,
    // Application
    MSG_APP_SHUTDOWN_REQUESTED,                //!< Immediate application shutdown. No params.
    MSG_APP_SCREENSHOT_REQUESTED,              //!< Capture screenshot. No params.
    MSG_APP_DISPLAY_FULLSCREEN_REQUESTED,      //!< Switch to fullscreen. No params.
    MSG_APP_DISPLAY_WINDOWED_REQUESTED,        //!< Switch to windowed display. No params.
    MSG_APP_MODCACHE_LOAD_REQUESTED,           //!< Internal for game startup, DO NOT PUSH MANUALLY.
    MSG_APP_MODCACHE_UPDATE_REQUESTED,         //!< Rescan installed mods and update cache. No params.
    MSG_APP_MODCACHE_PURGE_REQUESTED,          //!< Request cleanup and full rebuild of mod cache.
    MSG_APP_LOAD_SCRIPT_REQUESTED,             //!< Request loading a script from resource(file) or memory; Params 'filename' (string)/'buffer'(string - has precedence over filename), 'category' (ScriptCategory), 'associated_actor' (int - only for SCRIPT_CATEGORY_ACTOR)
    MSG_APP_UNLOAD_SCRIPT_REQUESTED,           //!< Request unloading a script; Param 'id' (int - the ID of the script unit, see 'Script Monitor' tab in console UI.)   
    MSG_APP_REINIT_INPUT_REQUESTED,            //!< Request restarting the entire input subsystem (mouse, keyboard, controllers) including reloading input mappings. Use with caution.
    // Networking
    MSG_NET_CONNECT_REQUESTED,                 //!< Request connection to multiplayer server specified by cvars 'mp_server_host, mp_server_port, mp_server_password'. No params.
    MSG_NET_CONNECT_STARTED,                   //!< Networking notification, DO NOT PUSH MANUALLY.
    MSG_NET_CONNECT_PROGRESS,                  //!< Networking notification, DO NOT PUSH MANUALLY.
    MSG_NET_CONNECT_SUCCESS,                   //!< Networking notification, DO NOT PUSH MANUALLY.
    MSG_NET_CONNECT_FAILURE,                   //!< Networking notification, DO NOT PUSH MANUALLY.
    MSG_NET_SERVER_KICK,                       //!< Networking notification, DO NOT PUSH MANUALLY.
    MSG_NET_DISCONNECT_REQUESTED,              //!< Request disconnect from multiplayer. No params.
    MSG_NET_USER_DISCONNECT,                   //!< Networking notification, DO NOT PUSH MANUALLY.
    MSG_NET_RECV_ERROR,                        //!< Networking notification, DO NOT PUSH MANUALLY.
    MSG_NET_REFRESH_SERVERLIST_SUCCESS,        //!< Background task notification, DO NOT PUSH MANUALLY.
    MSG_NET_REFRESH_SERVERLIST_FAILURE,        //!< Background task notification, DO NOT PUSH MANUALLY.
    MSG_NET_REFRESH_REPOLIST_SUCCESS,          //!< Background task notification, DO NOT PUSH MANUALLY.
    MSG_NET_OPEN_RESOURCE_SUCCESS,             //!< Background task notification, DO NOT PUSH MANUALLY.
    MSG_NET_REFRESH_REPOLIST_FAILURE,          //!< Background task notification, DO NOT PUSH MANUALLY.
    MSG_NET_FETCH_AI_PRESETS_SUCCESS,          //!< Background task notification, DO NOT PUSH MANUALLY.
    MSG_NET_FETCH_AI_PRESETS_FAILURE,          //!< Background task notification, DO NOT PUSH MANUALLY.
    // Simulation
    MSG_SIM_PAUSE_REQUESTED,                   //!< Pause game. No params.
    MSG_SIM_UNPAUSE_REQUESTED,                 //!< Unpause game. No params.
    MSG_SIM_LOAD_TERRN_REQUESTED,              //!< Request loading terrain. Param 'filename' (string)
    MSG_SIM_LOAD_SAVEGAME_REQUESTED,           //!< Request loading saved game. Param 'filename' (string)
    MSG_SIM_UNLOAD_TERRN_REQUESTED,            //!< Request returning to main menu. No params.
    MSG_SIM_SPAWN_ACTOR_REQUESTED,             //!< Request spawning an actor. Params: 'filename' (string), 'position' (vector3), 'rotation' (quaternion), 'instance_id' (int, optional), 'config' (string, optional), 'skin' (string, optional), 'enter' (bool, optional, default true), , 'free_position' (bool, default false)
    MSG_SIM_MODIFY_ACTOR_REQUESTED,            //!< Request change of actor. Params: 'type' (enum ActorModifyRequestType)
    MSG_SIM_DELETE_ACTOR_REQUESTED,            //!< Request actor removal. Params: 'instance_id' (int)
    MSG_SIM_SEAT_PLAYER_REQUESTED,             //!< Put player character in a vehicle. Params: 'instance_id' (int), use -1 to get out of vehicle.
    MSG_SIM_TELEPORT_PLAYER_REQUESTED,         //!< Teleport player character anywhere on terrain. Param 'position' (vector3)
    MSG_SIM_HIDE_NET_ACTOR_REQUESTED,          //!< Request hiding of networked actor; used internally by top menubar. Params: 'instance_id' (int)
    MSG_SIM_UNHIDE_NET_ACTOR_REQUESTED,        //!< Request revealing of hidden networked actor; used internally by top menubar. Params: 'instance_id' (int)
    MSG_SIM_SCRIPT_EVENT_TRIGGERED,            //!< Internal notification about triggering a script event, DO NOT PUSH MANUALLY.
    MSG_SIM_SCRIPT_CALLBACK_QUEUED,            //!< Internal notification about triggering a script event, DO NOT PUSH MANUALLY.
    // GUI
    MSG_GUI_OPEN_MENU_REQUESTED,
    MSG_GUI_CLOSE_MENU_REQUESTED,
    MSG_GUI_OPEN_SELECTOR_REQUESTED,           //!< Use `game.showChooser()` instead.
    MSG_GUI_CLOSE_SELECTOR_REQUESTED,          //!< No params.
    MSG_GUI_MP_CLIENTS_REFRESH,                //!< No params.
    MSG_GUI_SHOW_MESSAGE_BOX_REQUESTED,        //!< Use `game.showMessageBox()` instead.
    MSG_GUI_DOWNLOAD_PROGRESS,                 //!< Background task notification, DO NOT PUSH MANUALLY.
    MSG_GUI_DOWNLOAD_FINISHED,                 //!< Background task notification, DO NOT PUSH MANUALLY.
    // Editing
    MSG_EDI_MODIFY_GROUNDMODEL_REQUESTED,      //!< Used by Friction UI, DO NOT PUSH MANUALLY.
    MSG_EDI_ENTER_TERRN_EDITOR_REQUESTED,      //!< No params.
    MSG_EDI_LEAVE_TERRN_EDITOR_REQUESTED,      //!< No params.
    MSG_EDI_LOAD_BUNDLE_REQUESTED,             //!< Load a resource bundle (= ZIP or directory) for a given cache entry. Params: 'cache_entry' (CacheEntryClass@)
    MSG_EDI_RELOAD_BUNDLE_REQUESTED,           //!< This deletes all actors using that bundle (= ZIP or directory)! Params: 'cache_entry' (CacheEntryClass@)
    MSG_EDI_UNLOAD_BUNDLE_REQUESTED,           //!< This deletes all actors using that bundle (= ZIP or directory)! Params: 'cache_entry' (CacheEntryClass@)
    MSG_EDI_CREATE_PROJECT_REQUESTED,          //!< Creates a subdir under 'projects/', pre-populates it and adds to modcache. Params: 'name' (string), 'ext' (string, optional), 'source_entry' (CacheEntryClass@)
    MSG_EDI_ADD_FREEBEAMGFX_REQUESTED,         //!< Adds visuals for a freebeam (pair of HALFBEAM freeforces); Params: 'id' (int, use `game.getFreeBeamGfxNextId()`), 'freeforce_primary' (int), 'freeforce_secondary' (), 'mesh_name' (string), 'material_name' (string) ; For internals see `RoR::FreeBeamGfxRequest`
    MSG_EDI_MODIFY_FREEBEAMGFX_REQUESTED,      //!< Updates visuals for a freebeam (pair of HALFBEAM freeforces); Params: 'id' (int, use `game.getFreeBeamGfxNextId()`), 'freeforce_primary' (int), 'freeforce_secondary' (), 'mesh_name' (string), 'material_name' (string) ; For internals see `RoR::FreeBeamGfxRequest`
    MSG_EDI_DELETE_FREEBEAMGFX_REQUESTED,      //!< Removes visuals of a freebeam (pair of HALFBEAM freeforces).
};

/// Binding of `RoR::ScriptRetCode`; Common return codes for script manipulation funcs (add/get/delete | funcs/variables)
enum ScriptRetCode
{
    
    SCRIPTRETCODE_SUCCESS = AngelScript::asSUCCESS, //!< Generic success - 0 by common convention.

    // AngelScript technical codes
    SCRIPTRETCODE_AS_ERROR = AngelScript::asERROR,
    SCRIPTRETCODE_AS_CONTEXT_ACTIVE = AngelScript::asCONTEXT_ACTIVE,
    SCRIPTRETCODE_AS_CONTEXT_NOT_FINISHED = AngelScript::asCONTEXT_NOT_FINISHED,
    SCRIPTRETCODE_AS_CONTEXT_NOT_PREPARED = AngelScript::asCONTEXT_NOT_PREPARED,
    SCRIPTRETCODE_AS_INVALID_ARG = AngelScript::asINVALID_ARG,
    SCRIPTRETCODE_AS_NO_FUNCTION = AngelScript::asNO_FUNCTION,
    SCRIPTRETCODE_AS_NOT_SUPPORTED = AngelScript::asNOT_SUPPORTED,
    SCRIPTRETCODE_AS_INVALID_NAME = AngelScript::asINVALID_NAME,
    SCRIPTRETCODE_AS_NAME_TAKEN = AngelScript::asNAME_TAKEN,
    SCRIPTRETCODE_AS_INVALID_DECLARATION = AngelScript::asINVALID_DECLARATION,
    SCRIPTRETCODE_AS_INVALID_OBJECT = AngelScript::asINVALID_OBJECT,
    SCRIPTRETCODE_AS_INVALID_TYPE = AngelScript::asINVALID_TYPE,
    SCRIPTRETCODE_AS_ALREADY_REGISTERED = AngelScript::asALREADY_REGISTERED,
    SCRIPTRETCODE_AS_MULTIPLE_FUNCTIONS = AngelScript::asMULTIPLE_FUNCTIONS,
    SCRIPTRETCODE_AS_NO_MODULE = AngelScript::asNO_MODULE,
    SCRIPTRETCODE_AS_NO_GLOBAL_VAR = AngelScript::asNO_GLOBAL_VAR,
    SCRIPTRETCODE_AS_INVALID_CONFIGURATION = AngelScript::asINVALID_CONFIGURATION,
    SCRIPTRETCODE_AS_INVALID_INTERFACE = AngelScript::asINVALID_INTERFACE,
    SCRIPTRETCODE_AS_CANT_BIND_ALL_FUNCTIONS = AngelScript::asCANT_BIND_ALL_FUNCTIONS,
    SCRIPTRETCODE_AS_LOWER_ARRAY_DIMENSION_NOT_REGISTERED = AngelScript::asLOWER_ARRAY_DIMENSION_NOT_REGISTERED,
    SCRIPTRETCODE_AS_WRONG_CONFIG_GROUP = AngelScript::asWRONG_CONFIG_GROUP,
    SCRIPTRETCODE_AS_CONFIG_GROUP_IS_IN_USE = AngelScript::asCONFIG_GROUP_IS_IN_USE,
    SCRIPTRETCODE_AS_ILLEGAL_BEHAVIOUR_FOR_TYPE = AngelScript::asILLEGAL_BEHAVIOUR_FOR_TYPE,
    SCRIPTRETCODE_AS_WRONG_CALLING_CONV = AngelScript::asWRONG_CALLING_CONV,
    SCRIPTRETCODE_AS_BUILD_IN_PROGRESS = AngelScript::asBUILD_IN_PROGRESS,
    SCRIPTRETCODE_AS_INIT_GLOBAL_VARS_FAILED = AngelScript::asINIT_GLOBAL_VARS_FAILED,
    SCRIPTRETCODE_AS_OUT_OF_MEMORY = AngelScript::asOUT_OF_MEMORY,
    SCRIPTRETCODE_AS_MODULE_IS_IN_USE = AngelScript::asMODULE_IS_IN_USE,

    // RoR ScriptEngine return codes
    SCRIPTRETCODE_UNSPECIFIED_ERROR = -1001,
    SCRIPTRETCODE_ENGINE_NOT_CREATED = -1002,
    SCRIPTRETCODE_CONTEXT_NOT_CREATED = -1003,
    SCRIPTRETCODE_SCRIPTUNIT_NOT_EXISTS = -1004,
    SCRIPTRETCODE_SCRIPTUNIT_NO_MODULE = -1005,
    SCRIPTRETCODE_FUNCTION_NOT_EXISTS = -1006,
};

///  Parameter to `Actor::setSimAttribute()` and `Actor::getSimAttribute()`; allows advanced users to tweak physics internals via script.
///  Each value represents a variable, either directly in `Actor` or a subsystem, i.e. `EngineSim`.
///  PAY ATTENTION to the 'safe value' limits below - those may not be checked when setting attribute values!
enum ActorSimAttr
{
    ACTORSIMATTR_NONE,

    // TractionControl
    ACTORSIMATTR_TC_RATIO, //!< Regulating force, safe values: <1 - 20>
    ACTORSIMATTR_TC_PULSE_TIME, //!< Pulse duration in seconds, safe values <0.005 - 1>
    ACTORSIMATTR_TC_WHEELSLIP_CONSTANT //!< Minimum wheel slip threshold, safe value = 0.25
    
    // Engine
    ACTORSIMATTR_ENGINE_SHIFTDOWN_RPM, //!< Automatic transmission - Param #1 of 'engine'
    ACTORSIMATTR_ENGINE_SHIFTUP_RPM, //!< Automatic transmission - Param #2 of 'engine'
    ACTORSIMATTR_ENGINE_TORQUE, //!< Engine torque in newton-meters (N/m) - Param #3 of 'engine'
    ACTORSIMATTR_ENGINE_DIFF_RATIO, //!< Differential ratio (aka global gear ratio) - Param #4 of 'engine'
    ACTORSIMATTR_ENGINE_GEAR_RATIOS_ARRAY, //!< Gearbox - Format: "<reverse_gear> <neutral_gear> <forward_gear 1> [<forward gear 2>]..."; Param #5 and onwards of 'engine'.

    // Engoption
    ACTORSIMATTR_ENGOPTION_ENGINE_INERTIA, //!< - Param #1 of 'engoption'
    ACTORSIMATTR_ENGOPTION_ENGINE_TYPE, //!< - Param #2 of 'engoption'
    ACTORSIMATTR_ENGOPTION_CLUTCH_FORCE, //!< - Param #3 of 'engoption'
    ACTORSIMATTR_ENGOPTION_SHIFT_TIME, //!< - Param #4 of 'engoption'
    ACTORSIMATTR_ENGOPTION_CLUTCH_TIME, //!< - Param #5 of 'engoption'
    ACTORSIMATTR_ENGOPTION_POST_SHIFT_TIME, //!< Time (in seconds) until full torque is transferred - Param #6 of 'engoption'
    ACTORSIMATTR_ENGOPTION_STALL_RPM, //!< RPM where engine stalls - Param #7 of 'engoption'
    ACTORSIMATTR_ENGOPTION_IDLE_RPM, //!< Target idle RPM - Param #8 of 'engoption'
    ACTORSIMATTR_ENGOPTION_MAX_IDLE_MIXTURE, //!< Max throttle to maintain idle RPM - Param #9 of 'engoption'
    ACTORSIMATTR_ENGOPTION_MIN_IDLE_MIXTURE, //!< Min throttle to maintain idle RPM - Param #10 of 'engoption'
    ACTORSIMATTR_ENGOPTION_BRAKING_TORQUE, //!< How much engine brakes on zero throttle - Param #11 of 'engoption'

    // Engturbo2 (actually 'engturbo' with Param #1 [type] set to "2" - the recommended variant)
    ACTORSIMATTR_ENGTURBO2_INERTIA_FACTOR, //!< Time to spool up - Param #2 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_NUM_TURBOS, //!< Number of turbos - Param #3 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_MAX_RPM, //!< MaxPSI * 10000 ~ calculated from Param #4 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_ENGINE_RPM_OP, //!< Engine RPM threshold for turbo to operate - Param #5 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_BOV_ENABLED, //!< Blow-off valve - Param #6 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_BOV_MIN_PSI, //!< Blow-off valve PSI threshold - Param #7 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_WASTEGATE_ENABLED, //!<  - Param #8 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_WASTEGATE_MAX_PSI, //!<  - Param #9 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_WASTEGATE_THRESHOLD_N, //!< 1 - WgThreshold ~ calculated from  Param #10 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_WASTEGATE_THRESHOLD_P, //!< 1 + WgThreshold ~ calculated from  Param #10 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_ANTILAG_ENABLED, //!<  - Param #11 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_ANTILAG_CHANCE, //!<  - Param #12 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_ANTILAG_MIN_RPM, //!<  - Param #13 of 'engturbo2'
    ACTORSIMATTR_ENGTURBO2_ANTILAG_POWER, //!<  - Param #14 of 'engturbo2'    
};

} // namespace Script2Game

/// @}    //addtogroup Script2Game
/// @}    //addtogroup ScriptSideAPIs
