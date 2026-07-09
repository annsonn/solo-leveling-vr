# QA Checklist — Phase 1: VR Basics & Greybox Hub

> Owner: qa. Derived from ROADMAP.md Phase 1 success criteria.
> Phase 1 is NOT complete until every item passes in-headset. Log results in the Session Log.

## Prerequisites
- [ ] Phase 0 headset test passed (template at 90 FPS via SteamVR, hands tracked)
- [ ] Project compiles from clean clone (`git clone` → generate files → build → editor opens)

## 1. BP_ArisePawn (C++ base)
- [ ] Pawn possesses on level start in `L_XRTemplate`-replacement hub level
- [ ] HMD camera tracks head 1:1, no lag or jitter at rest
- [ ] Both motion controller hands visible and tracking
- [ ] Hands survive controller sleep/wake (set controllers down 30 s, pick up — tracking resumes)

## 2. Enhanced Input
- [ ] `IMC_Default` active on possession (verify via console `showdebug enhancedinput`)
- [ ] `IA_Pause` opens pause menu from BOTH controllers
- [ ] Pause menu closes and returns cleanly to play
- [ ] `IA_UIPoint`/`IA_UIClick`: laser pointer selects a test button from both hands
- [ ] No leftover template bindings fire (grab/shoot/teleport do nothing or are removed)

## 3. Greybox Hub Room
- [ ] Room reads as small standing space (Jin-Woo's room scale)
- [ ] All interactables reachable from a fixed standing spot (no forced walking)
- [ ] No geometry within arm-swing radius of spawn point (punch clearance check: full-extension punches in all directions touch nothing)
- [ ] Floor level matches real floor (crouch and touch — no offset)

## 4. Height Calibration
- [ ] First-run flow triggers on fresh save profile
- [ ] "Stand straight, confirm" stores standing height
- [ ] Stored value persists across editor restart (check SaveGame)
- [ ] Re-calibration reachable from settings menu
- [ ] Sanity bounds: reject obviously bad captures (< 1.0 m or > 2.5 m) with a retry prompt

## 5. Comfort Baseline
- [ ] Snap turn works (both directions, no smooth rotation unless enabled)
- [ ] Vignette asset present; OFF by default in standing play; toggle works
- [ ] No head-locked UI anywhere
- [ ] No forced camera movement in any hub interaction

## 6. Performance
- [ ] Forward renderer + MSAA confirmed in project settings (screenshot logged)
- [ ] `stat unit` baseline captured in hub, standing still AND during 20 fast punches
- [ ] 90 FPS held throughout (no transient drops during arm swings)
- [ ] Baseline numbers logged in PERFORMANCE.md (frame/game/draw/GPU ms)

## Sign-off
| Item | Pass/Fail | Date | Notes |
|---|---|---|---|
| | | | |
