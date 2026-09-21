#pragma once

// Strongly typed script model shared by the DSL and execution engine. An
// action only stores fields which are meaningful for that action.

using Milliseconds = std::chrono::milliseconds;

template <class... Callables> struct Overloaded : Callables... {
    using Callables::operator()...;
};

template <class... Callables> Overloaded(Callables...) -> Overloaded<Callables...>;

struct PartySwitchPlacement {
    std::string character;
    std::string gizmo;
};

struct PartyForceUse {
    std::string character;
    std::string gizmo;
};

struct CheckpointAction {
    std::string level;
};
struct WaitLoadedAction {};
struct WaitAction {
    Milliseconds duration;
};
struct StartPodraceAction {};
struct DrivePodraceAction {
    std::string level;
    f32 speed;
};

struct RailForward {
    f32 distance;
};
struct RailRelativeRoute {
    std::vector<NUVEC> waypoints;
};
struct RailToGizmo {
    std::string name;
};
struct RailThroughGizmo {
    std::string name;
    f32 overshoot;
};
struct RailThroughPathConnection {
    std::string from;
    std::string to;
};
struct RailToCharacter {
    std::string name;
    f32 arrival_tolerance;
};
struct RailToArea {
    std::string name;
};
struct RailToPathNode {
    std::string name;
};
struct RailToLocator {
    std::string name;
};
struct RailThroughLocator {
    std::string name;
};

using RailDestination =
    std::variant<RailForward, RailRelativeRoute, RailToGizmo, RailThroughGizmo, RailThroughPathConnection,
                 RailToCharacter, RailToArea, RailToPathNode, RailToLocator, RailThroughLocator>;

struct RailAction {
    RailDestination destination;
    f32 speed;
    bool follow_height;
};

struct JumpRelative {
    NUVEC offset;
};
struct JumpToGizmo {
    std::string name;
};
struct JumpToLocator {
    std::string name;
};
using JumpDestination = std::variant<JumpRelative, JumpToGizmo, JumpToLocator>;

struct NativeJumpAction {
    JumpDestination destination;
};
struct TeleportAction {
    NUVEC position;
};
struct UseForceAction {
    std::string gizmo;
};
struct UsePanelAction {
    std::string gizmo;
};
struct UseBuildItAction {
    std::string gizmo;
};
struct UseZipUpAction {
    std::string gizmo;
};
struct BlasterHitAction {
    std::string gizmo;
};
struct ClearHostilesAction {
    std::optional<std::string> character;
    f32 radius;
    Milliseconds quiet_period;
};
struct DestroyAiObjectAction {
    std::string object;
    f32 maximum_range;
};
struct DamageCharacterAction {
    std::string character;
    i32 target_health;
    f32 maximum_range;
};
struct SwitchCharacterAction {
    std::string character;
};
struct GizmoOutputCondition {
    std::string gizmo;
    i32 output_index;
    i32 expected;
};
struct WaitGizmoOutputAction {
    GizmoOutputCondition condition;
};
struct HoldGizmoAction {
    std::string standing_gizmo;
    GizmoOutputCondition condition;
    f32 return_speed;
};
struct HoldPartySwitchesAction {
    std::vector<PartySwitchPlacement> placements;
    GizmoOutputCondition condition;
};
struct HoldPartyForcesAction {
    std::vector<PartyForceUse> uses;
    GizmoOutputCondition condition;
};
struct WaitAiMessageAction {
    std::string message;
    i32 expected;
};
struct WaitLevelAction {
    std::string level;
};
struct WaitMenuAction {
    i32 menu_id;
};
struct WaitCutsceneEndAction {
    std::string level;
};
struct LogGizmosAction {};
struct ManualControlAction {};

using ActionPayload =
    std::variant<CheckpointAction, WaitLoadedAction, WaitAction, StartPodraceAction, DrivePodraceAction, RailAction,
                 NativeJumpAction, TeleportAction, UseForceAction, UsePanelAction, UseBuildItAction, UseZipUpAction,
                 BlasterHitAction, ClearHostilesAction, DestroyAiObjectAction, DamageCharacterAction,
                 SwitchCharacterAction, WaitGizmoOutputAction, HoldGizmoAction, HoldPartySwitchesAction,
                 HoldPartyForcesAction, WaitAiMessageAction, WaitLevelAction, WaitMenuAction, WaitCutsceneEndAction,
                 LogGizmosAction, ManualControlAction>;

struct AutoplayAction {
    ActionPayload payload;
    Milliseconds timeout{};
};

template <typename Action> const Action *get_action(const AutoplayAction &action) {
    return std::get_if<Action>(&action.payload);
}

struct AutoplayOptions {
    bool allow_manual_input = false;
    bool enable_invincibility = true;
};

struct AutoplayScript {
    std::string description;
    std::vector<AutoplayAction> actions;
    AutoplayOptions options{};
    Milliseconds timeout{240000};
};

struct RailWaypoint {
    NUVEC position{};
    AIPATHCNX *incoming_connection = nullptr;
    i32 traversal_direction = 0;
};

struct NativeTraversal {
    f32 minimum_height;
    bool airborne = false;
};

struct RailState {
    std::vector<RailWaypoint> waypoints;
    usize waypoint_index = 0;
    NUVEC progress_origin{};
    GameObject_s *character = nullptr;
    LEVELDATA_s *starting_level = nullptr;
    std::optional<u64> progress_observed_at;
    std::optional<NativeTraversal> native_traversal;
    std::optional<i32> final_facing;
    bool stop_on_transition = false;
    bool vertical_travel = false;
};

struct PodraceRuntime {
    bool lap_advanced = false;
    u32 recovery_count = 0;
    u64 progress_observed_at = 0;
    u64 track_updated_at = 0;
    LEVELDATA_s *starting_level = nullptr;
    NUVEC progress_origin{};
    std::optional<SOCKPOSITION> track_position;
};
struct RailRuntime {
    RailState rail;
};
struct ForceRuntime {
    bool started = false;
};
struct BuildRuntime {
    bool started = false;
};
struct InteractionRuntime {
    bool started = false;
};
struct CombatRuntime {
    u64 last_hit_at = 0;
    std::optional<u64> last_target_seen_at;
};
struct CharacterSwitchRuntime {
    bool requested = false;
};
struct PartySwitchRuntime {
    std::vector<NUVEC> positions;
};

using ActionRuntime = std::variant<std::monostate, PodraceRuntime, RailRuntime, ForceRuntime, BuildRuntime,
                                   InteractionRuntime, CombatRuntime, CharacterSwitchRuntime, PartySwitchRuntime>;

template <typename Action> struct RuntimeFor {
    using type = std::monostate;
};
template <> struct RuntimeFor<DrivePodraceAction> {
    using type = PodraceRuntime;
};
template <> struct RuntimeFor<RailAction> {
    using type = RailRuntime;
};
template <> struct RuntimeFor<NativeJumpAction> {
    using type = RailRuntime;
};
template <> struct RuntimeFor<UseForceAction> {
    using type = ForceRuntime;
};
template <> struct RuntimeFor<UseBuildItAction> {
    using type = BuildRuntime;
};
template <> struct RuntimeFor<UseZipUpAction> {
    using type = InteractionRuntime;
};
template <> struct RuntimeFor<BlasterHitAction> {
    using type = InteractionRuntime;
};
template <> struct RuntimeFor<ClearHostilesAction> {
    using type = CombatRuntime;
};
template <> struct RuntimeFor<DestroyAiObjectAction> {
    using type = CombatRuntime;
};
template <> struct RuntimeFor<DamageCharacterAction> {
    using type = CombatRuntime;
};
template <> struct RuntimeFor<SwitchCharacterAction> {
    using type = CharacterSwitchRuntime;
};
template <> struct RuntimeFor<HoldGizmoAction> {
    using type = RailRuntime;
};
template <> struct RuntimeFor<HoldPartySwitchesAction> {
    using type = PartySwitchRuntime;
};

inline ActionRuntime make_runtime_for(const ActionPayload &payload) {
    return std::visit(
        [](const auto &action) -> ActionRuntime {
            using Action = std::decay_t<decltype(action)>;
            return typename RuntimeFor<Action>::type{};
        },
        payload);
}

constexpr Milliseconds kStartupTimeout{90000};
constexpr u32 kInvulnerabilityCheatFlag = 0x80;
