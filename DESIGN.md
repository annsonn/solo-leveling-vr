# DESIGN.md — Project Arise (Source of Truth)

> Solo Leveling–inspired VR fitness action RPG. Personal, non-commercial fan project.
> This document is maintained by the Game Director agent. Propose changes here BEFORE implementing them.

---

## 1. Concept

The player is **Sung Jin-Woo**, immediately after surviving the double dungeon and awakening to **the System**. The System issues daily training quests, tracks stats and levels, and opens gates to dungeons. The twist: the player's **real body is the controller**. Squats, punches, marching, lunges, and twists — detected from the HMD and motion controllers — power combat and progression. The game is a workout that disguises interval training as dungeon crawling, with difficulty that adapts to the player's actual fitness.

**Pillars**
1. **Earn it with your body** — progression comes from real reps, honestly counted.
2. **The System is watching** — cold, diegetic UI; the fantasy of being managed by an inscrutable power.
3. **Arise at your own pace** — DDA meets the player where they are; manual override always available.
4. **Never hurt the player** — 90 FPS lock, comfort-first VR, fatigue-aware design.

---

## 2. Technical Foundation

| Area | Decision |
|---|---|
| Engine | Unreal Engine 5.8 |
| Platform | PCVR, SteamVR via **OpenXR** plugin |
| Rendering | **Forward renderer + MSAA**. No Lumen/Nanite/RT. Baked-first lighting. |
| Framerate | **90 FPS hard floor** (safety requirement for exercise in VR) |
| Language split | C++ core systems; Blueprint subclasses for content/tuning |
| Input | Enhanced Input (IA_/IMC_ assets) |
| Ability/stat framework | **Gameplay Ability System (GAS)** |
| Enemy AI | **StateTree** (Behavior Trees only if a case demands it) |
| Source control | Git + Git LFS (`*.uasset`, `*.umap` in LFS) |
| AI tooling | Claude (Cowork/Code): files, git, docs. opencode + Unreal MCP plugin: in-editor automation. Lanes defined in AGENTS.md |

**Module layout:** single game module `Arise` initially; split later only if compile times demand.
**Content root:** `Content/Arise/{Core, Characters, Enemies, Dungeons, UI, VFX, Audio, Input, Data}`.

---

## 3. Core Systems

### 3.1 Stats & Leveling (GAS)
- **Attributes** (`UAriseAttributeSet`): Strength, Agility, Vitality, Intelligence, Perception. Derived: MaxHP (Vitality), MaxMP (Intelligence), stamina/fatigue meta-attribute.
- Level-up grants **stat points** (default 3/level, +bonus from quests) allocated via the status window.
- All modifications through **Gameplay Effects**; UI listens to attribute-change delegates.
- XP curve in `DT_XPCurve`; sources: quest completion, enemy kills, dungeon clears, rep quality bonuses.
- Stat gameplay meaning: Strength → damage per quality punch; Agility → dodge window & move speed; Vitality → HP & fatigue resistance; Intelligence → MP & skill potency; Perception → enemy telegraph clarity/slow-mo dodge cue duration.

### 3.2 Daily Quest System
- Anime-faithful **"Preparation for Becoming Strong"**: punches, squats, standing twists, march/jog in place. Base 100/100/100/10min, **scaled by DDA** to the player's fitness (a new player might start at 20/20/20/3min).
- VR-safe substitutions (headset on, standing): push-ups → power punches; sit-ups → standing twist/crunch reaches; running → march in place.
- Rewards: XP, stat points, streak bonuses. **Penalty redesign:** missing a day triggers a narrative "penalty zone" flavor event with a *reduced* catch-up quest — never a punishment workout. Resting is never punished mechanically.
- Quest definitions in `DT_DailyQuests`; state machine: Issued → InProgress → Completed/Expired.

### 3.3 Exercise Detection (`UExerciseDetectionComponent`)
- Inputs: HMD pose + both controller poses (position, velocity), sampled per-frame in one component.
- **Squat:** HMD vertical drop ≥ calibrated threshold (default ~30% of standing height) + hold + return; hysteresis to prevent double counts.
- **Punch:** controller forward velocity spike ≥ threshold along facing vector + extension distance; per-hand.
- **March/jog:** alternating HMD bob + controller swing cadence.
- **Lunge / twist:** HMD drop with forward offset / HMD-controller yaw rotation pattern.
- Output: gameplay events `Exercise.<Type>.RepCompleted` with `Quality` (0–1: depth, speed, form proxies) payload.
- **Calibration:** first-run height calibration; thresholds in `DA_ExerciseThresholds`, per-profile adjustable.
- Known limits (no trackers): can't verify floor exercises or leg detail — mapping avoids these.

### 3.4 Combat
- **Punch = melee strike** (later: dagger-style rapid strikes). Damage = base × Strength × rep quality.
- **Squat = dodge** (ducks under telegraphed horizontal attacks) and **charge** (full squat charges a power attack / skill meter).
- **March = advance/reposition** (in-place locomotion drives forward movement when needed) and sustains stamina regen.
- Enemies telegraph clearly (Perception scales telegraph readability). Combat waves follow **interval-training cadence**: 30–60 s burst, 15–30 s lull.
- Enemy AI: StateTree states (Spawn → Approach → Telegraph → Attack → Recover → Stagger/Die); parameters (HP, speed, telegraph time) from `DT_Enemies`, scaled by DDA.

### 3.5 Dynamic Difficulty Adjustment (DDA)
- **Fitness model inputs (motion-first, v1):** rep rate vs. session baseline, rep quality trend, inter-rep rest gaps, squat depth decay, session duration, rolling multi-session history (stored in save).
- **Fatigue signals:** quality decay >25% from session baseline, rest gaps lengthening, cadence drop → triggers rest prompt + downscale.
- **Outputs:** enemy HP/attack speed multipliers, wave size, required rep goals, quest volumes, lull duration.
- **Policy:** scale down fast (immediate on fatigue), scale up slow (only across sessions with consistent headroom).
- **Manual override:** difficulty tiers **E / D / C / B / A / S rank**; selectable anytime from pause menu; DDA then adjusts *within* the chosen tier's bounds only.
- **v2 (Phase 9):** optional Bluetooth HRM integration; HR zones refine the fitness model + calorie estimation.

### 3.6 Session Structure & Safety
- Every play session: **Warm-up** (System issues light mobility quest, 2–3 min) → **Main** (quests/dungeon) → **Cool-down** (stretch-flavored wind-down quest).
- Rest prompts: diegetic System messages (`[Hunter, your vitals require recovery. Rest.]`) + audio cue; combat pauses safely (enemies back off) — resting never causes death.
- Pause menu reachable with one button at all times; includes difficulty override, comfort settings, session summary.

### 3.7 UI — The System
- All UI **diegetic, world-space** UMG widgets: floating translucent blue windows, ~1–1.5 m from player, large high-contrast type.
- Screens: Status (stats/level/rank + point allocation), Daily Quest tracker, Inventory (later), Notifications (level-up, quest arrival, title earned), rep counter (glanceable, near play focus).
- Interaction: laser pointer from either controller. No head-locked UI.

### 3.8 Progression Content (later phases)
- **Skills** (GAS abilities): unlocked at level milestones; activation = exercise combos (e.g., 3 rapid squats → Sprint buff; punch flurry → Vital Strike).
- **Shadow-style summons** (Phase 8): defeat elite enemy → extraction minigame (hold deep squat while "commanding") → summon fights alongside player; summon roster in save.
- **Dungeons/Gates:** hub room → gate select → instanced dungeon (streamed level), waves + boss, rank-gated (E-rank gates first).

### 3.9 Save System
- `USaveGame`-based profile: level, XP, stats, allocated points, quest streaks, DDA fitness history, calibration data, settings, summon roster.
- Autosave on quest completion, level-up, dungeon exit.

---

## 4. Narrative Frame
- Start: waking in hospital after the double dungeon; first System contact; tutorial = first daily quest in Jin-Woo's room (hub).
- The System's voice: terse, bracketed, imperative. All strings in string tables (`ST_System`, `ST_Quests`).
- Beats land with phases: first quest (P4), first combat instance (P5), first gate (P6), first skill & job-change tease (P8).

## 5. IP Position
Personal non-commercial fan project. No distribution, no storefront, no monetization, no public marketing using Solo Leveling names/marks. Any future distribution requires full reskin: renamed character, original System branding, original story analog. Assets/audio must be original or licensed — no rips.

## 6. Open Questions (Director maintains)
- Locomotion beyond march-in-place for dungeon traversal: node-teleport between arena rooms (current lean) vs. smooth stick locomotion option?
- Left-hand/accessibility mode for punch mappings.
- HRM device targets for Phase 9 (BLE heart rate profile — vendor-agnostic preferred).
