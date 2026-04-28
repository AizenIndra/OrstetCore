-- NPC Transmogrifier SQL template
-- Fill in the placeholders before applying it to your world database.

-- ------------------------------------------------------------
-- Case 1: NPC already exists
-- Set the transmogrifier flag on an existing creature_template row.
-- Replace <ENTRY> with the creature entry id.
-- Replace <TRANSMOGRIFIER_FLAG_VALUE> with the numeric value of CREATURE_FLAG_EXTRA_TRANSMOGRIFIER.
-- ------------------------------------------------------------

UPDATE creature_template
SET flags_extra = flags_extra | 0x90000000
WHERE entry = 190010;

-- ------------------------------------------------------------
-- Case 2: NPC does not exist yet
-- Insert a new creature_template row, then spawn it in creature.
-- Fill in all placeholders with your actual data.
-- ------------------------------------------------------------

-- INSERT INTO creature_template (
--     entry, modelid1, name, subname, minlevel, maxlevel, faction, npcflag,
--     unit_flags, flags_extra, ScriptName, AIName
-- ) VALUES (
--     <ENTRY>, <MODEL_ID>, 'Transmogrifier', '', <MIN_LEVEL>, <MAX_LEVEL>, <FACTION>,
--     0, 0, <TRANSMOGRIFIER_FLAG_VALUE>, 'npc_transmogrifier', ''
-- );

-- INSERT INTO creature (
--     guid, id, map, zoneId, areaId, spawnMask, phaseMask, modelid, position_x,
--     position_y, position_z, orientation, spawntimesecs, wander_distance, currentwaypoint,
--     curhealth, curmana, MovementType, ScriptName, VerifiedBuild
-- ) VALUES (
--     <GUID>, <ENTRY>, <MAP>, <ZONE_ID>, <AREA_ID>, 1, 1, 0, <X>,
--     <Y>, <Z>, <O>, 300, 0, 0,
--     1, 0, 0, '', 0
-- );

-- Notes:
-- 1. The NPC must have CREATURE_FLAG_EXTRA_TRANSMOGRIFIER in flags_extra.
-- 2. The script name used by the server-side gossip handler is npc_transmogrifier.
-- 3. If you already have a custom NPC, only the UPDATE statement is needed.
