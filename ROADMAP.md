# ROADMAP.md — Project Arise

> Working agreement: one phase at a time, small tasks, every phase ends with an in-headset test.
> Agents: update checkboxes and the Session Log at the end of every work session.
> New ideas go to BACKLOG — not into the active phase.

**Active phase:** Phase 0

---

## Phase 0 — Environment & Pipeline Setup
**Goal:** Dev environment works end to end: engine, VR, source control, AI tooling.
**Agents:** director, systems-engineer, vr-engineer, qa

- [x] Create UE 5.8 VR template project (done by developer)
- [ ] Verify OpenXR plugin enabled; SteamVR set as OpenXR runtime; template runs in headset
- [x] Create UE-specific `.gitignore`; init Git + Git LFS (`*.uasset`, `*.umap` tracked; history migrated to LFS)
- [x] Initial commit of clean project
- [ ] Restructure content: create `Content/Arise/` folder tree per DESIGN.md
- [ ] Add C++ game module `Arise` (empty skeleton compiles)
- [ ] Install & connect Unreal MCP plugin; verify Claude can query the editor
- [x] Add AGENTS.md / DESIGN.md / ROADMAP.md to repo root (this port)
- [ ] QA: write Phase 1 checklist

**Headset test:** VR template level runs at 90 FPS via SteamVR; hands tracked; project compiles from clean clone.

---

## Phase 1 — VR Basics & Greybox Hub
**Goal:** Our own pawn in our own space.
**Agents:** vr-engineer, level-designer, tech-artist, qa

- [ ] `BP_ArisePawn` (from C++ base): HMD camera, motion controller hands
- [ ] Enhanced Input setup: IMC_Default, IA_Pause, IA_UIPoint/Click
- [ ] Greybox hub room (Jin-Woo's room scale: small, standing play)
- [ ] Height calibration flow (first-run: stand straight, confirm)
- [ ] Comfort baseline: vignette asset ready (off by default in standing play), snap turn
- [ ] Forward renderer + MSAA confirmed in project settings; baseline `stat unit` capture logged in PERFORMANCE.md

**Headset test:** Stand in hub room, see hands, pause menu opens/closes, calibration stores height. 90 FPS confirmed.

---

## Phase 2 — The System UI
**Goal:** Diegetic System windows exist and feel right.
**Agents:** ui-designer, narrative, systems-engineer, audio, qa

- [ ] World-space widget framework (widget component pattern, laser-pointer interaction)
- [ ] `WBP_SystemWindow` master style (translucent blue, large type)
- [ ] Status window: level, rank, 5 stats (static placeholder data)
- [ ] Notification window: `[You have acquired the qualifications to be a Player. Will you accept?]` intro sequence
- [ ] String tables ST_System created; narrative pass on intro text
- [ ] System notification sound v1 (original sting)

**Headset test:** Intro notification appears on level start; status window opens via controller; text readable at 1 m without leaning; interaction works from both hands.

---

## Phase 3 — Exercise Detection v1
**Goal:** Squats and punches counted reliably.
**Agents:** vr-engineer, fitness-designer, qa

- [ ] `UExerciseDetectionComponent` skeleton (single sampling point, event outputs)
- [ ] Squat detection (depth threshold from calibration, hysteresis) + rep quality
- [ ] Punch detection (velocity + extension, per hand) + rep quality
- [ ] `DA_ExerciseThresholds` with tunable values
- [ ] Debug HUD widget: live rep counts + quality readout
- [ ] Fitness-designer spec sheet: thresholds, quality formula v1
- [ ] QA detection protocol run: 10 slow / 10 fast / 10 partial reps per exercise; log accuracy %

**Headset test:** 20 honest squats count 20 (±1); half-squats score low quality; punches don't trigger from arm waves. Accuracy logged.

---

## Phase 4 — Daily Quest Loop (First Real Gameplay)
**Goal:** The anime's training quest, playable start to finish.
**Agents:** systems-engineer, fitness-designer, narrative, ui-designer, audio, qa

- [ ] GAS setup: AbilitySystemComponent on pawn, `UAriseAttributeSet` (5 stats + HP/MP)
- [ ] XP + level system (`DT_XPCurve`), level-up grants stat points
- [ ] `DT_DailyQuests` + quest state machine (Issued → InProgress → Completed)
- [ ] Quest tracker window bound to live rep events
- [ ] "Preparation for Becoming Strong" quest v1 (starter volumes: 20/20/20/3min)
- [ ] Level-up sequence: notification + sound + stat point allocation UI
- [ ] Rep-completion audio cues (per exercise, satisfying)
- [ ] Save/load: profile persists level, XP, stats, quest streak
- [ ] Warm-up quest before main quest (session structure v1)

**Headset test:** Complete the full daily quest as a real workout; earn XP; level up; allocate a stat point; quit and reload — progress persists.

---

## Phase 5 — First Combat
**Goal:** One enemy, fought with your body.
**Agents:** systems-engineer, fitness-designer, level-designer, tech-artist, audio, qa

- [ ] `BP_Enemy_Base` (C++ base) + first enemy (goblin-tier), `DT_Enemies`
- [ ] StateTree AI: Approach → Telegraph → Attack → Recover; clear telegraph animation/audio
- [ ] Punch → damage pipeline (GAS gameplay effect; damage = base × Strength × quality)
- [ ] Squat → dodge (duck under telegraphed horizontal swipe; i-frame window)
- [ ] Player HP + hit feedback (comfort-safe: no screen shake; flash + audio)
- [ ] Combat greybox arena (separate test level)
- [ ] Impact VFX/SFX v1 within particle budget

**Headset test:** Fight and kill one enemy using real punches; dodge its telegraphed attack with a real squat; die if you stand still. 90 FPS holds in combat.

---

## Phase 6 — First Dungeon (Vertical Slice)
**Goal:** An E-rank gate: enter, clear waves, beat a boss, get rewards.
**Agents:** level-designer, systems-engineer, fitness-designer, narrative, tech-artist, audio, qa

- [ ] Gate/portal in hub → load instanced dungeon level (streaming)
- [ ] Dungeon greybox: 3 arena rooms, node-based traversal between rooms
- [ ] Wave spawner with interval-training cadence (burst/lull from DESIGN.md)
- [ ] Boss v1: bigger HP, two telegraphed attack patterns (one duck, one punch-interrupt)
- [ ] Clear rewards: XP, bonus stat point; return-to-hub flow
- [ ] Dungeon ambience + adaptive music v1 (calm ↔ combat layers)
- [ ] First-gate narrative beat

**Headset test:** Full dungeon run start to finish = a genuine ~15-minute workout; boss kill; rewards granted; return to hub; save intact.

---

## Phase 7 — Dynamic Difficulty v1
**Goal:** The game meets the player where they are.
**Agents:** fitness-designer, systems-engineer, qa

- [ ] Session intensity tracker (rep rate, quality trend, rest gaps) feeding a fitness score
- [ ] Multi-session history in save profile
- [ ] DDA outputs wired: enemy HP/speed multipliers, wave sizes, quest rep volumes
- [ ] Fatigue detection → diegetic rest prompt → combat safe-pause; auto downscale
- [ ] Manual rank tiers (E–S) in pause menu; DDA constrained within tier bounds
- [ ] Scale-up policy: cross-session only, conservative
- [ ] QA safety audit: simulate fatigue, verify downscale + rest prompt from every combat state

**Headset test:** Play deliberately exhausted vs. fresh — quest volumes and enemy pressure visibly differ; rest prompt triggers on slowdown; manual S-rank override works instantly.

---

## Phase 8 — Progression Depth
**Goal:** Stats matter, skills exist, shadows rise.
**Agents:** systems-engineer, fitness-designer, ui-designer, tech-artist, narrative, qa

- [ ] Stat effects fully wired (Agility dodge window, Perception telegraph clarity, etc.)
- [ ] First 2 skills as GAS abilities with exercise-combo activation
- [ ] Skill UI + unlock notifications
- [ ] Shadow extraction v1: elite enemy → extraction hold (deep squat) → summon ally
- [ ] Summon follows + attacks (simple StateTree); roster in save
- [ ] Shadow VFX (Niagara smoke) within budget
- [ ] Second enemy type + second dungeon variant

**Headset test:** Unlock and trigger a skill via exercise combo; extract a shadow and watch it fight beside you in the next run.

---

## Phase 9 — Heart Rate & Calories (Optional Hardware)
**Goal:** Real vitals refine the fitness model.
**Agents:** vr-engineer, fitness-designer, ui-designer, qa

- [ ] BLE heart-rate monitor integration (standard HR profile, vendor-agnostic)
- [ ] HR zones feed DDA fitness model (v2)
- [ ] Calorie estimation (motion + HR blended); session summary screen
- [ ] Graceful no-HRM fallback (v1 model remains default)

**Headset test:** With HRM strapped: live HR visible in session summary; pushing into high HR zone triggers earlier rest prompts than motion-only mode.

---

## Phase 10+ — Content, Polish, Longevity
**Goal:** More game, better feel.
**Agents:** all

- [ ] Additional dungeons, enemy types, boss patterns
- [ ] Art pass on hub + first dungeons (within budget)
- [ ] Weekly quests, titles, streak meta
- [ ] Instant dungeon keys (repeatable workout modes: "cardio gate", "strength gate")
- [ ] Cool-down/stretch content pass
- [ ] Accessibility: seated-mode audit, left-hand mode
- [ ] Full performance + comfort regression pass

---

## BACKLOG (ideas parked by the Director)
- Job-change quest arc
- Multiplayer raid gates (almost certainly out of scope — parked)
- Mixed reality passthrough mode for home workouts
- Companion phone app for quest streaks

---

## Session Log
| Date | Phase | Done | Test result | Next |
|---|---|---|---|---|
| 2026-07-04 | 0 | Git LFS set up + history migrated (189 assets); baseline commit; renderer config cleaned (RT/Lumen/Substrate/VSM/distance fields OFF, forward+MSAA kept); .uproject trimmed to Windows-only, eye tracker + PICO disabled | Pending: open editor (expect shader recompile), run template in headset via SteamVR, confirm 90 FPS + hands | Content/Arise tree; Arise C++ module; connect Unreal MCP connector; QA Phase 1 checklist |
