-- Предметы для квестов
DELETE FROM item_template WHERE entry BETWEEN 44834 AND 44860;
INSERT INTO item_template (entry, class, subclass, SoundOverrideSubclass, name, displayid, Quality, Flags, FlagsExtra, BuyCount, BuyPrice, SellPrice, InventoryType, AllowableClass, AllowableRace, ItemLevel, RequiredLevel, RequiredSkill, RequiredSkillRank, requiredspell, requiredhonorrank, RequiredCityRank, RequiredReputationFaction, RequiredReputationRank, maxcount, stackable, ContainerSlots, StatsCount, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, stat_type4, stat_value4, stat_type5, stat_value5, stat_type6, stat_value6, stat_type7, stat_value7, stat_type8, stat_value8, stat_type9, stat_value9, stat_type10, stat_value10, ScalingStatDistribution, ScalingStatValue, dmg_min1, dmg_max1, dmg_type1, dmg_min2, dmg_max2, dmg_type2, armor, holy_res, fire_res, nature_res, frost_res, shadow_res, arcane_res, delay, ammo_type, RangedModRange, spellid_1, spelltrigger_1, spellcharges_1, spellppmRate_1, spellcooldown_1, spellcategory_1, spellcategorycooldown_1, spellid_2, spelltrigger_2, spellcharges_2, spellppmRate_2, spellcooldown_2, spellcategory_2, spellcategorycooldown_2, spellid_3, spelltrigger_3, spellcharges_3, spellppmRate_3, spellcooldown_3, spellcategory_3, spellcategorycooldown_3, spellid_4, spelltrigger_4, spellcharges_4, spellppmRate_4, spellcooldown_4, spellcategory_4, spellcategorycooldown_4, spellid_5, spelltrigger_5, spellcharges_5, spellppmRate_5, spellcooldown_5, spellcategory_5, spellcategorycooldown_5, bonding, description, PageText, LanguageID, PageMaterial, startquest, lockid, Material, sheath, RandomProperty, RandomSuffix, block, itemset, MaxDurability, area, Map, BagFamily, TotemCategory, socketColor_1, socketContent_1, socketColor_2, socketContent_2, socketColor_3, socketContent_3, socketBonus, GemProperties, RequiredDisenchantSkill, ArmorDamageModifier, duration, ItemLimitCategory, HolidayId, ScriptName, DisenchantID, FoodType, minMoneyLoot, maxMoneyLoot, flagsCustom) VALUES

-- Древние реликвии (44834)
(44834, 12, 0, -1, 'Древняя реликвия', 65123, 3, 0, 0, 1, 0, 0, 0, -1, -1, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 1, 'Древний артефакт, излучающий таинственную энергию', 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0),

-- Мощное зелье исцеления (44835)
(44835, 0, 1, -1, 'Мощное зелье исцеления', 65124, 3, 0, 0, 1, 0, 0, 0, -1, -1, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 43185, 0, -1, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 1, 'Восстанавливает 4500-5500 здоровья', 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0),

-- Военные планы (44859)
(44859, 12, 0, -1, 'Военные планы врага', 65125, 3, 0, 0, 1, 0, 0, 0, -1, -1, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 1, 'Секретные планы врага', 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0),

-- Священные реликвии троллей (44839)
(44839, 12, 0, -1, 'Священная реликвия троллей', 65126, 3, 0, 0, 1, 0, 0, 0, -1, -1, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 1, 'Древняя реликвия троллей, наполненная мощной магией', 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0),

-- Вечный лед (44855)
(44855, 7, 0, -1, 'Вечный лед', 65127, 3, 0, 0, 1, 0, 0, 0, -1, -1, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 1, 'Кусок магического льда, который никогда не тает', 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0),

-- Древесина духов (44856)
(44856, 7, 0, -1, 'Древесина духов', 65128, 3, 0, 0, 1, 0, 0, 0, -1, -1, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 1, 'Древесина, пропитанная духовной энергией', 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0),

-- Секретные документы (44860)
(44860, 12, 0, -1, 'Секретные документы', 65129, 3, 0, 0, 1, 0, 0, 0, -1, -1, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 1, 'Важные документы, содержащие секретную информацию', 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0);

-- NPC для квестов
DELETE FROM creature_template WHERE entry BETWEEN 31333 AND 31364;
INSERT INTO creature_template (entry, difficulty_entry_1, difficulty_entry_2, difficulty_entry_3, KillCredit1, KillCredit2, modelid1, modelid2, modelid3, modelid4, name, subname, IconName, gossip_menu_id, minlevel, maxlevel, exp, faction, npcflag, speed_walk, speed_run, scale, rank, dmgschool, BaseAttackTime, RangeAttackTime, BaseVariance, RangeVariance, unit_class, unit_flags, unit_flags2, dynamicflags, family, trainer_type, trainer_spell, trainer_class, trainer_race, type, type_flags, lootid, pickpocketloot, skinloot, resistance1, resistance2, resistance3, resistance4, resistance5, resistance6, spell1, spell2, spell3, spell4, spell5, spell6, spell7, spell8, PetSpellDataId, VehicleId, mingold, maxgold, AIName, MovementType, InhabitType, HoverHeight, HealthModifier, ManaModifier, ArmorModifier, DamageModifier, ExperienceModifier, RacialLeader, movementId, RegenHealth, mechanic_immune_mask, flags_extra, ScriptName) VALUES

-- Глубинное чудовище
(31333, 0, 0, 0, 0, 0, 27823, 0, 0, 0, 'Глубинное чудовище', '', '', 0, 80, 80, 2, 14, 0, 1, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 1, 0, 31333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 1500, 'SmartAI', 1, 3, 1, 15, 1, 1, 3, 1, 0, 0, 1, 0, 0, ''),

-- Ледяной вирмлинг
(31334, 0, 0, 0, 0, 0, 27824, 0, 0, 0, 'Ледяной вирмлинг', '', '', 0, 80, 80, 2, 14, 0, 1, 1.14286, 0.5, 1, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 2, 0, 31334, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 800, 'SmartAI', 1, 3, 1, 8, 1, 1, 2, 1, 0, 0, 1, 0, 0, ''),

-- Ледяной великан
(31338, 0, 0, 0, 0, 0, 27825, 0, 0, 0, 'Ледяной великан', '', '', 0, 80, 80, 2, 14, 0, 1, 1.14286, 2, 1, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 4, 0, 31338, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 2500, 'SmartAI', 1, 3, 1, 25, 1, 1, 4, 1, 0, 0, 1, 0, 0, ''),

-- Буревестник
(31339, 0, 0, 0, 0, 0, 27826, 0, 0, 0, 'Буревестник', '', '', 0, 80, 80, 2, 14, 0, 1, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 1, 0, 31339, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 800, 1800, 'SmartAI', 1, 3, 1, 12, 1, 1, 2.5, 1, 0, 0, 1, 0, 0, ''),

-- Кристальный защитник
(31340, 0, 0, 0, 0, 0, 27827, 0, 0, 0, 'Кристальный защитник', '', '', 0, 80, 80, 2, 14, 0, 1, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 4, 0, 31340, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 900, 2000, 'SmartAI', 1, 3, 1, 15, 1, 1, 3, 1, 0, 0, 1, 0, 0, ''),

-- Драконий прислужник
(31341, 0, 0, 0, 0, 0, 27828, 0, 0, 0, 'Драконий прислужник', '', '', 0, 80, 80, 2, 14, 0, 1, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 2, 0, 31341, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 1500, 'SmartAI', 1, 3, 1, 10, 1, 1, 2, 1, 0, 0, 1, 0, 0, ''),

-- Командир культистов
(31363, 0, 0, 0, 0, 0, 27829, 0, 0, 0, 'Командир культистов', '', '', 0, 80, 80, 2, 14, 0, 1, 1.14286, 1, 2, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 7, 0, 31363, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 3500, 'SmartAI', 1, 3, 1, 30, 1, 1, 5, 1, 0, 0, 1, 0, 0, ''),

-- Ледяной лорд
(31364, 0, 0, 0, 0, 0, 27830, 0, 0, 0, 'Ледяной лорд', '', '', 0, 80, 80, 2, 14, 0, 1, 1.14286, 1.5, 3, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 4, 0, 31364, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000, 5000, 'SmartAI', 1, 3, 1, 40, 1, 1, 6, 1, 0, 0, 1, 0, 0, '');

-- Добавляем модели для NPC
DELETE FROM creature_template_model WHERE CreatureID BETWEEN 31333 AND 31364;
INSERT INTO creature_template_model (CreatureID, Idx, CreatureDisplayID, DisplayScale, Probability) VALUES
(31333, 0, 27823, 1, 1), -- Глубинное чудовище
(31334, 0, 27824, 0.5, 1), -- Ледяной вирмлинг
(31338, 0, 27825, 2, 1), -- Ледяной великан
(31339, 0, 27826, 1, 1), -- Буревестник
(31340, 0, 27827, 1, 1), -- Кристальный защитник
(31341, 0, 27828, 1, 1), -- Драконий прислужник
(31363, 0, 27829, 1, 1), -- Командир культистов
(31364, 0, 27830, 1.5, 1); -- Ледяной лорд

-- Добавляем лут для мобов
DELETE FROM creature_loot_template WHERE Entry BETWEEN 31333 AND 31364;
INSERT INTO creature_loot_template (Entry, Item, Reference, Chance, QuestRequired, LootMode, GroupId, MinCount, MaxCount) VALUES
(31333, 44834, 0, 35, 1, 1, 0, 1, 1), -- Древняя реликвия с Глубинного чудовища
(31334, 44859, 0, 25, 1, 1, 0, 1, 1), -- Военные планы с Ледяного вирмлинга
(31338, 44855, 0, 40, 1, 1, 0, 1, 2), -- Вечный лед с Ледяного великана
(31339, 44855, 0, 30, 1, 1, 0, 1, 1), -- Вечный лед с Буревестника
(31340, 44839, 0, 35, 1, 1, 0, 1, 1), -- Священная реликвия с Кристального защитника
(31341, 44856, 0, 35, 1, 1, 0, 1, 1), -- Древесина духов с Драконьего прислужника
(31363, 44860, 0, 45, 1, 1, 0, 1, 1), -- Секретные документы с Командира культистов
(31364, 44860, 0, 100, 1, 1, 0, 1, 2); -- Гарантированные секретные документы с Ледяного лорда 