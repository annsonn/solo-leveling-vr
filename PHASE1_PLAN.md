# PHASE1_PLAN.md — Execution Plan for opencode

## STATUS (Director updates this — do not edit)
- **Current increment: 2** (BP_ArisePawn, BP_AriseGameMode, L_Hub)
- Increment 1 (AArisePawnBase C++): ✅ approved & committed
- Increments 3–5: ⛔ not yet authorized

> **Audience:** opencode (with Unreal MCP access).
> **Authority:** This plan is written by the Game Director (Claude). Read `AGENTS.md` (all Global Rules apply), `DESIGN.md` §2–3, and `ROADMAP.md` Phase 1 before starting.
> **Workflow:** Execute ONE increment at a time. STOP at every ⏸ STOP point, report results, and wait for the developer to test in-headset and relay Director review. Do not proceed past a failed acceptance check.

## Ground Rules
- You may edit/create files in `Source/`, `Content/Arise/` (via editor/MCP), and nothing else. Never touch `Content/XRFramework/`, `Config/`, `Saved/`, `Intermediate/`, `.uproject`, or the docs (AGENTS/DESIGN/ROADMAP/QA). Config changes in this plan are listed for the Director to apply — flag when ready, don't edit them yourself.
- **No git commands, ever.** The Director commits after review.
- Naming per AGENTS.md conventions (`BP_`, `IA_`, `IMC_`, `L_`, `WBP_`…). All C++ in the `Arise` module.
- 90 FPS budget: no per-frame logic beyond what this plan specifies. No Lumen/Nanite/RT features.
- If anything is ambiguous, STOP and ask — do not improvise design decisions.

---

## Increment 1 — `AArisePawnBase` (C++)

Create `Source/Arise/Public/Player/ArisePawnBase.h` + `Source/Arise/Private/Player/ArisePawnBase.cpp`.

**Spec**
- Subclass `APawn` (NOT ACharacter — standing-in-place play, no capsule movement).
- Components: `USceneComponent* VROrigin` (root); `UCameraComponent* Camera` (child of VROrigin); `UMotionControllerComponent* MotionControllerLeft` / `MotionControllerRight` (children of VROrigin, motion sources `Left`/`Right`); placeholder hand visuals: two small `UStaticMeshComponent`s (child of each controller), mesh left unset in C++ (BP assigns).
- `BeginPlay`: set tracking origin to Floor/Stage (`UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage)` — use the UE 5.8 API name; check `LocalFloor` if `Stage` unavailable).
- All components `UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Arise|VR")`.
- Expose `UFUNCTION(BlueprintCallable, Category="Arise|VR") FVector GetHMDLocationLocal() const;` returning camera position relative to VROrigin (exercise detection will consume this in Phase 3).
- No input bindings in this increment. No Tick (set `PrimaryActorTick.bCanEverTick = false`).
- Log `LogArise` line on BeginPlay: `ArisePawnBase ready`.

**Acceptance:** module compiles clean; class visible in editor class picker.
**⏸ STOP** — report compile result; developer builds + Director reviews before Increment 2.

## Increment 2 — `BP_ArisePawn`, `BP_AriseGameMode`, `L_Hub`

Editor work via MCP:
1. `Content/Arise/Characters/BP_ArisePawn` — subclass of `AArisePawnBase`. Assign simple placeholder hand meshes (engine basic shapes, e.g. small cube 8 cm, scaled). Do NOT use XRFramework hand assets.
2. `Content/Arise/Core/BP_AriseGameMode` — subclass of `AGameModeBase`, Default Pawn = `BP_ArisePawn`.
3. `Content/Arise/Dungeons/L_Hub` — new empty level: floor plane (~4×4 m), one directional light + sky atmosphere (cheap defaults, no Lumen), Player Start at floor center.
4. World Settings of `L_Hub`: GameMode Override = `BP_AriseGameMode`.
5. Save all. Do not change project default maps (Config is Director's lane — flag when ready).

**Acceptance:** PIE in `L_Hub` possesses `BP_ArisePawn`; in VR preview developer sees floor and two placeholder hands tracking.
**⏸ STOP** — developer headset test: hands tracked, floor at real floor height.

## Increment 3 — Input Assets (ours, not the template's)

1. In `Content/Arise/Input/` create Input Actions: `IA_Pause` (bool), `IA_UIPoint` (bool), `IA_UIClick` (bool), `IA_SnapTurn` (Axis1D float).
2. Create `IMC_Arise_Default` mapping: Pause → both controllers' menu button; UIClick → both triggers; SnapTurn → right thumbstick X (PSVR2 Sense controllers map via standard OpenXR bindings — use generic OpenXR controller keys, not vendor-specific).
3. In `BP_ArisePawn` (or C++ if Director approves later): add `IMC_Arise_Default` (priority 1) on possession via Enhanced Input local player subsystem.
4. `IA_Pause` handler: print-string placeholder "PAUSE" (real menu is Phase 2). `IA_SnapTurn`: rotate VROrigin yaw ±45° on threshold press (deadzone 0.7, must return to <0.3 before re-trigger — no smooth rotation).
5. Flag for Director: template IMCs are auto-injected via `Config/DefaultInput.ini` `DefaultMappingContexts` — Director will strip them for our maps at review time.

**Acceptance:** in `L_Hub`, snap turn works both directions with no smooth rotation; PAUSE prints from both controllers; no template weapon/grab/teleport bindings fire.
**⏸ STOP** — developer headset test per QA/PHASE1_CHECKLIST.md §2 & §5.

## Increment 4 — Height Calibration

C++ (`Source/Arise/Public/Player/AriseCalibrationComponent.h` + cpp):
- `UActorComponent` on the pawn. `UFUNCTION(BlueprintCallable) void CaptureStandingHeight();` reads HMD local Z, validates 1.0–2.5 m (reject with retry-able error state + `LogArise` warning), stores to SaveGame.
- `UAriseSaveGame : USaveGame` (`Source/Arise/Public/Core/`) with `float StandingHeightCm` and version int. Save slot name `AriseProfile`, user index 0. BlueprintCallable load/save helpers.
- Delegate `FOnCalibrationChanged` broadcast on successful capture.

Editor: minimal `Content/Arise/UI/WBP_Calibration` — world-space text "Stand straight, then pull either trigger", shown on first run (no saved height), calls `CaptureStandingHeight` on `IA_UIClick`, hides on success.

**Acceptance:** first VR run in `L_Hub` shows prompt; trigger captures height; restart editor → prompt does NOT reappear; saved value survives (developer checks `Saved/SaveGames/AriseProfile.sav` exists).
**⏸ STOP** — developer headset test per checklist §4.

## Increment 5 — Comfort Baseline + Perf Snapshot

1. Vignette: simple radial-dark `M_ComfortVignette` material + widget or post-process approach in `Content/Arise/VFX/`, wired but **default OFF** (toggle bool on pawn, BlueprintCallable). No heavy material work — Tech Artist pass comes later.
2. Developer runs `stat fps` + `stat unit` in `L_Hub` standing and during 20 fast punches; you record the numbers the developer reports into nothing — the Director writes `PERFORMANCE.md`. Just remind the developer to capture them.

**Acceptance:** QA/PHASE1_CHECKLIST.md §5 + §6 pass.
**⏸ STOP** — Phase 1 QA gate: full checklist run, Director closes phase.

---

## Reporting Format (every increment)
Reply to the developer with: files created/changed (paths), what compiled/saved, acceptance-check results, anything ambiguous you stopped on. The developer relays to the Director for review + commit.
