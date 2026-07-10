---
description: Execute the current increment of the active phase plan (Project Arise)
---

You are the engineering floor of Paperclip Guild (see COMPANY.md), implementing Project Arise. A fresh session has no memory — re-orient first:

0. Identify the current increment's domain and delegate to the matching role subagent in `.opencode/agent/` (vr-engineer, systems-engineer, level-designer, ui-designer, tech-artist). Cross-domain increments: lead with the primary role, hand off per AGENTS.md.
0b. For ANY in-editor work, load the `unreal-mcp` skill (`.agents/skills/unreal-mcp/SKILL.md`) and follow its discovery flow (`list_toolsets` → `describe_toolset` → `call_tool`) and safety rules (save before bulk changes, sequential calls only, check every result, mind PIE).

MCP call discipline (non-negotiable):
- `toolset_name` must be copied EXACTLY from `list_toolsets` output. Never invent or guess dotted names. If you haven't seen the name in this session's `list_toolsets` result, you may not use it.
- Asset paths use the `/Game/` virtual root: `/Game/Arise/Characters/BP_ArisePawn`. NEVER `Content/...` or a disk path.
- C++ parent classes: try the UHT name (`ArisePawnBase`) and the script path (`/Script/Arise.ArisePawnBase`); the `A` prefix is usually dropped in reflection-facing APIs.
- One failure → re-run `describe_toolset`, re-read the schema, adjust ONCE. Second failure of the same call → STOP and report the exact error text verbatim. Never repeat an unchanged failing call.

1. Read `AGENTS.md` — the Global Rules and AI Tool Lanes bind you.
2. Read the active phase plan (currently `PHASE1_PLAN.md`), including its Ground Rules.
3. Find the **STATUS** block at the top of the plan. Execute ONLY the increment marked "Current increment". Increments marked not-yet-authorized are forbidden, even if you finish early.
4. Before creating anything, inspect what already exists (files under `Source/`, assets under `/Game/Arise/`) — a previous session may have partially completed this increment. Continue, don't duplicate.
5. At the increment's ⏸ STOP point: stop, report using the plan's Reporting Format, and wait. Never run git. Never edit Config/, *.uproject, or the docs (AGENTS/DESIGN/ROADMAP/QA/PHASE plans).

$ARGUMENTS
