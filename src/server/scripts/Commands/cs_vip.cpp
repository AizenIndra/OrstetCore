#include "AccountMgr.h"
#include "Chat.h"
#include "DataBaseEnv.h"
#include "GameTime.h"
#include "Group.h"
#include "InstanceSaveMgr.h"
#include "Language.h"
#include "Log.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "WorldSession.h"
#include "World.h"
#include "SharedDefines.h"

using namespace Acore::ChatCommands;

class vip_commandscript : public CommandScript
{
public:
    vip_commandscript() : CommandScript("vip_commandscript") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable vipCommandTable =
        {
            { "buff",             HandleVipBuffCommand,            SEC_PLAYER,     Console::No  },
            { "debuff",           HandleVipDebuffCommand,          SEC_PLAYER,     Console::No  },
            { "bank",             HandleVipBankCommand,            SEC_PLAYER,     Console::No  },
            { "mail",             HandlePremiumMailCommand,        SEC_PLAYER,     Console::No  },
            { "repair",           HandleVipRepairCommand,          SEC_PLAYER,     Console::No  },
            { "resettalents",     HandleVipResetTalentsCommand,    SEC_PLAYER,     Console::No  },
            { "taxi",             HandleVipTaxiCommand,            SEC_PLAYER,     Console::No  },
            { "home",             HandleVipHomeCommand,            SEC_PLAYER,     Console::No  },
            { "capital",          HandleVipCapitalCommand,         SEC_PLAYER,     Console::No  },
            { "changerace",       HandleChangeRaceCommand,         SEC_PLAYER,     Console::No  },
            { "customize",        HandleCustomizeCommand,          SEC_PLAYER,     Console::No  },
            { "app",              HandleVipAppearCommand,          SEC_PLAYER,     Console::No  },
            { "summon",           HandleVipSummonCommand,          SEC_PLAYER,     Console::No  },
            { "textcolor",        HandleVipTextColorCommand,       SEC_PLAYER,     Console::No  },
            { "free1day",        HandleVipFree1DayCommand,       SEC_PLAYER,     Console::No  },
            { "buy",              HandleBuyVipCommand,             SEC_PLAYER,     Console::No  },
            { "set",              HandleSetVipCommand,             SEC_GAMEMASTER, Console::Yes },
            { "del",              HandleDelVipCommand,             SEC_GAMEMASTER, Console::Yes },
            { "",                 HandleVipCommand,                SEC_PLAYER,     Console::No  },
        };
        static ChatCommandTable commandTable =
        {
            { "vip", vipCommandTable },
        };
        return commandTable;
    }

    // Helpers to reduce copy-paste in VIP commands
    static void UpdateVipMounts(Player* player)
    {
        if (!player)
            return;

        static constexpr uint32 PremiumMountSpells[] = { 31700, 18991, 18992 };

        if (player->IsPremium())
        {
            for (uint32 spellId : PremiumMountSpells)
                if (!player->HasSpell(spellId))
                    player->learnSpell(spellId, false, false);
        }
        else
        {
            for (uint32 spellId : PremiumMountSpells)
                if (player->HasSpell(spellId))
                    player->removeSpell(spellId, SPEC_MASK_ALL, false);
        }
    }

    static bool CheckVipBasic(ChatHandler* handler, Player* player)
    {
        if (!player || !player->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }
        return true;
    }

    static bool CheckVipCommandEnabled(ChatHandler* handler, bool enabledFlag)
    {
        if (!enabledFlag)
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
        return true;
    }

    static bool CheckVipCommonState(ChatHandler* handler, Player* player)
    {
        if (player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->isDead() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        return true;
    }

    static bool HandleSetVipCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        Player* target = handler->getSelectedPlayerOrSelf();

        char* days = strtok((char*)args, " ");
        char* accID = strtok(nullptr, " ");

        bool accidExist = false;
        // .vip set days accountID , where accid not required (if target exist)        
        if (!days)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 accountID;
        if (accID)
        {
            accountID = atoul(accID);
            accidExist = true;
        }
        else
            accountID = target->GetSession()->GetAccountId();

        uint32 days_bonus = atoul(days);

        // Validation: Check days_bonus range (1 to 3650 days = ~10 years max)
        if (days_bonus == 0 || days_bonus > 365)
        {
            handler->SendSysMessage("Неверное значение дней. Допустимо от 1 до 365.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Validation: Check accountID is valid
        if (accountID == 0)
        {
            handler->SendSysMessage("Неверный ID аккаунта.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        time_t current_time = GameTime::GetGameTime().count();
        time_t days_seconds = 24 * 60 * 60;
        time_t unsetdate = current_time + days_seconds * days_bonus;

        // Validation: Check for time_t overflow
        if (unsetdate < current_time || unsetdate < days_seconds * days_bonus)
        {
            handler->SendSysMessage("Обнаружено переполнение даты. Используйте меньшее количество дней.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        bool vip = false;

        if (!accidExist)
            vip = target->IsPremium();
        else
            vip = AccountMgr::GetVipStatus(accountID);

        // Use SetOrUpdateVipStatus to avoid race condition
        AccountMgr::UpdateVipStatus(accountID, unsetdate);

        // Log the operation
        uint32 gmAccountId = handler->GetSession() ? handler->GetSession()->GetAccountId() : 0;
        std::string targetName = accidExist ? "Unknown" : target->GetName();
        LOG_INFO("vip", "VIP status for account {} {} by GM account {} for {} days (Target: {}, unsetdate: {})",
            accountID, vip ? "updated" : "set", gmAccountId, days_bonus, targetName, unsetdate);

        if (accidExist)
        {
            ObjectGuid::LowType guid;
            guid = AccountMgr::GetGuidOfOnlineCharacter(accountID);
            if (guid)
            {
                if (Player* p = ObjectAccessor::FindPlayerByLowGUID(guid))
                {
                    p->SetPremiumStatus(true);
                    p->SetPremiumUnsetdate(unsetdate);
                    p->GetSession()->SetPremium(true);
                    UpdateVipMounts(p);
                    handler->PSendSysMessage("VIP привилегии установлены для аккаунта {}. Персонаж [{}] (онлайн, GUID {}), на {} дн.", accountID, p->GetName().c_str(), p->GetGUID().GetCounter(), days_bonus);
                }
            }
            else
                handler->PSendSysMessage("VIP привилегии установлены для аккаунта {} на {} дн., ни один персонаж не в сети.", accountID, days_bonus);
        }
        else
        {
            handler->PSendSysMessage("VIP привилегии установлены для аккаунта {} (IP: {}) персонаж [{}] (GUID {}), на {} дн.",
                accountID,
                target->GetSession()->GetRemoteAddress().c_str(),
                target->GetName().c_str(),
                target->GetGUID().GetCounter(),
                days_bonus);

            target->SetPremiumStatus(true);
            target->SetPremiumUnsetdate(unsetdate);
            target->GetSession()->SetPremium(true);
            UpdateVipMounts(target);
        }

        return true;
    }

    static bool HandleBuyVipCommand(ChatHandler* handler, char const* args)
    {
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : nullptr;
        if (!player)
            return false;

        // Allow purchase even without VIP
        uint32 days = 0;
        bool lifetime = false;

        if (!args || !*args)
            return false;

        std::string opt(args);
        opt = Acore::String::Trim(opt);

        if (opt == "life" || opt == "lifetime" || opt == "forever" || opt == "perm" || opt == "permanent")
            lifetime = true;
        else
            days = atoul(opt.c_str());

        // Prices in bonuses (account_donate.bonuses)
        constexpr int32 PRICE_1_DAY = 10;
        constexpr int32 PRICE_7_DAYS = 350;
        constexpr int32 PRICE_31_DAYS = 600;
        constexpr int32 PRICE_LIFETIME = 999;

        int32 price = 0;
        time_t addSeconds = 0;

        if (lifetime)
        {
            price = PRICE_LIFETIME;
            addSeconds = 50 * YEAR; // practical "lifetime"
        }
        else
        {
            switch (days)
            {
                case 1:  price = PRICE_1_DAY;  addSeconds = 1 * DAY; break;
                case 7:  price = PRICE_7_DAYS; addSeconds = 7 * DAY; break;
                case 31: price = PRICE_31_DAYS; addSeconds = 31 * DAY; break;
                default: return false;
            }
        }

        WorldSession* sess = player->GetSession();
        // Ensure account_donate row exists (inserts 0-balance row if missing)
        sess->AddDonateBonusOrVote(0, 1, false);

        if (!sess->SetAccountCurrency(price, 1, false))
        {
            handler->PSendSysMessage("Недостаточно бонусов. Нужно: {}, у вас: {}.", price, sess->GetAccountBalance());
            handler->SetSentErrorMessage(true);
            return false;
        }

        time_t now = GameTime::GetGameTime().count();
        time_t currentEnd = AccountMgr::GetVipEndTime(sess->GetAccountId());
        time_t base = currentEnd > now ? currentEnd : now;
        time_t newEnd = base + addSeconds;

        AccountMgr::UpdateVipStatus(sess->GetAccountId(), newEnd);

        player->SetPremiumStatus(true);
        player->SetPremiumUnsetdate(newEnd);
        sess->SetPremium(true);
        UpdateVipMounts(player);

        return true;
    }

    static bool HandleVipFree1DayCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : nullptr;
        if (!player)
            return false;

        if (player->IsPremium())
        {
            handler->SendSysMessage("VIP уже активен.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 accountId = player->GetSession()->GetAccountId();

        QueryResult res = LoginDatabase.Query(
            "SELECT 1 FROM account_premium_free_day WHERE id = {}", accountId);
        if (res)
        {
            handler->SendSysMessage("Бесплатный VIP на 7 дней уже был получен.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        time_t now = GameTime::GetGameTime().count();
        time_t unsetdate = now + 7 * 24 * 60 * 60;

        AccountMgr::UpdateVipStatus(accountId, unsetdate);
        player->SetPremiumStatus(true);
        player->SetPremiumUnsetdate(unsetdate);
        player->GetSession()->SetPremium(true);

        UpdateVipMounts(player);

        // Mark as claimed (one-time per account).
        LoginDatabase.Execute(
            "INSERT INTO account_premium_free_day (id, claimed_at) VALUES ({}, {})",
            accountId, static_cast<uint32>(now));

        handler->PSendSysMessage("Бесплатный VIP на 7 дней выдан.");

        return true;
    }

    static bool HandleVipTextColorCommand(ChatHandler* handler, char const* args)
    {
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : nullptr;
        if (!player)
            return false;

        if (!player->IsPremium() && !player->IsGameMaster())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!args || !*args)
            return false;

        uint32 colorId = atoul(args);
        if (colorId > 10)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SET_ACCOUNT_PREMIUM_CHAT_TEXT_COLOR);
        stmt->SetData(0, uint8(colorId));
        stmt->SetData(1, handler->GetSession()->GetAccountId());
        LoginDatabase.Execute(stmt);

        return true;
    }

    static bool HandleVipBuffCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        // VIP buffs list (requested)
        uint32 const buffs[] = { 25898, 48470, 53307, 48074, 48162, 48170, 57623, 43002, 47440 };

        for (uint32 spellId : buffs)
        {
            if (SpellInfo const* info = sSpellMgr->GetSpellInfo(spellId))
                player->CastSpell(player, info, true);
        }

        return true;
    }

    static bool HandleDelVipCommand(ChatHandler* handler, char const* args)
    {
        Player* target = handler->getSelectedPlayerOrSelf();

        char* accID = strtok((char*)args, " ");
        // .vip set days accountID , where accid not required (if target exist)

        bool accidExist = false;
        uint32 accountID;
        if (accID)
        {
            accountID = atoul(accID);
            accidExist = true;
        }
        else
            accountID = target->GetSession()->GetAccountId();

        bool vip = false;

        if (!accidExist)
            vip = target->IsPremium();
        else
            vip = AccountMgr::GetVipStatus(accountID);

        if (!vip)
        {
            handler->SendSysMessage(LANG_PLAYER_TARGET_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Validation: Check accountID is valid
        if (accountID == 0)
        {
            handler->SendSysMessage("Неверный ID аккаунта.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        AccountMgr::RemoveVipStatus(accountID);

        // Log the operation
        uint32 gmAccountId = handler->GetSession() ? handler->GetSession()->GetAccountId() : 0;
        std::string targetName = accidExist ? "Unknown" : target->GetName();
        LOG_INFO("vip", "VIP status for account {} removed by GM account {} (Target: {})",
            accountID, gmAccountId, targetName);

        if (accidExist)
        {
            ObjectGuid::LowType guid;
            guid = AccountMgr::GetGuidOfOnlineCharacter(accountID);
            if (guid)
            {
                if (Player* p = ObjectAccessor::FindPlayerByLowGUID(guid))
                {
                    p->SetPremiumStatus(false);
                    p->SetPremiumUnsetdate(0);
                    if (p->GetSession())
                        p->GetSession()->SetPremium(false);
                    UpdateVipMounts(p);
                    handler->PSendSysMessage("VIP привилегии сняты для аккаунта {}. Персонаж [{}] (онлайн, GUID {}).", accountID, p->GetName().c_str(), p->GetGUID().GetCounter());
                }
            }
            else
                handler->PSendSysMessage("VIP привилегии сняты для аккаунта {}, ни один персонаж не в сети.", accountID);
        }
        else
        {
            handler->PSendSysMessage("VIP привилегии сняты для аккаунта {} (IP: {}) персонаж [{}] (GUID {}).",
                accountID,
                target->GetSession()->GetRemoteAddress().c_str(),
                target->GetName().c_str(),
                target->GetGUID().GetCounter());

            target->SetPremiumStatus(false);
            target->SetPremiumUnsetdate(0);
            if (target->GetSession())
                target->GetSession()->SetPremium(false);
            UpdateVipMounts(target);
        }

        return true;
    }

    static bool HandleVipCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!player->IsPremium() && !player->IsGameMaster())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsPremium())
        {
            time_t unsetdate = player->GetPremiumUnsetdate();
            time_t currentTime = GameTime::GetGameTime().count();
            if (unsetdate > currentTime)
            {
                time_t diff = unsetdate - currentTime;
                handler->PSendSysMessage(LANG_PLAYER_VIP_TIME_EXIST, (secsToTimeString(diff, true)).c_str());
            }
        }

        bool allDisabled = sWorld->getBoolConfig(CONFIG_VIP_DEBUFF) && sWorld->getBoolConfig(CONFIG_VIP_BANK) &&
            sWorld->getBoolConfig(CONFIG_VIP_MAIL) && sWorld->getBoolConfig(CONFIG_VIP_REPAIR) &&
            sWorld->getBoolConfig(CONFIG_VIP_RESET_TALENTS) && sWorld->getBoolConfig(CONFIG_VIP_TAXI) &&
            sWorld->getBoolConfig(CONFIG_VIP_HOME) && sWorld->getBoolConfig(CONFIG_VIP_CAPITAL) &&
            sWorld->getBoolConfig(CONFIG_VIP_CHANGE_RACE) && sWorld->getBoolConfig(CONFIG_VIP_CUSTOMIZE) &&
            sWorld->getBoolConfig(CONFIG_VIP_APPEAR) && sWorld->getBoolConfig(CONFIG_VIP_SUMMON);
        if (allDisabled)
        {
            handler->PSendSysMessage("VIP команды отключены.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage("Доступные вам VIP команды:");
        if (sWorld->getBoolConfig(CONFIG_VIP_DEBUFF))
            handler->PSendSysMessage(" .vip debuff - снять негативные эффекты");
        if (sWorld->getBoolConfig(CONFIG_VIP_BANK))
            handler->PSendSysMessage(" .vip bank - открыть банк");
        if (sWorld->getBoolConfig(CONFIG_VIP_MAIL))
            handler->PSendSysMessage(" .vip mail - открыть почту");
        if (sWorld->getBoolConfig(CONFIG_VIP_REPAIR))
            handler->PSendSysMessage(" .vip repair - починка снаряжения");
        if (sWorld->getBoolConfig(CONFIG_VIP_RESET_TALENTS))
            handler->PSendSysMessage(" .vip resettalents - сброс талантов");
        if (sWorld->getBoolConfig(CONFIG_VIP_TAXI))
            handler->PSendSysMessage(" .vip taxi - открыть все такси");
        if (sWorld->getBoolConfig(CONFIG_VIP_HOME))
            handler->PSendSysMessage(" .vip home - телепорт домой");
        if (sWorld->getBoolConfig(CONFIG_VIP_CAPITAL))
            handler->PSendSysMessage(" .vip capital - телепорт в столицу");
        if (sWorld->getBoolConfig(CONFIG_VIP_CHANGE_RACE))
            handler->PSendSysMessage(" .vip changerace - смена расы");
        if (sWorld->getBoolConfig(CONFIG_VIP_CUSTOMIZE))
            handler->PSendSysMessage(" .vip customize - изменение внешности");
        if (sWorld->getBoolConfig(CONFIG_VIP_APPEAR))
            handler->PSendSysMessage(" .vip app - телепорт к члену группы");
        if (sWorld->getBoolConfig(CONFIG_VIP_SUMMON))
            handler->PSendSysMessage(" .vip summon <name> - призвать игрока к себе");
        handler->PSendSysMessage(" .vip textcolor <0..10> - цвет текста в чате");
        handler->PSendSysMessage(" .vip buff - наложить VIP бафы");
        handler->PSendSysMessage(" .vip buy <1|7|31|lifetime> - купить VIP за bonuses");

        if (handler->GetSession() && handler->GetSession()->GetSecurity() >= SEC_GAMEMASTER)
        {
            handler->PSendSysMessage(" .vip set - выдать/продлить VIP");
            handler->PSendSysMessage(" .vip del - снять VIP");
        }

        return true;
    }

    static bool HandleVipDebuffCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_DEBUFF)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        player->RemoveAurasDueToSpell(15007); // Resurrection Sickness
        player->RemoveAurasDueToSpell(26013); // Deserter
        return true;
    }

    static bool HandleVipBankCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_BANK)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        handler->GetSession()->SendShowBank(player->GetGUID());
        return true;
    }

    static bool HandlePremiumMailCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_MAIL)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        handler->GetSession()->SendShowMailBox(player->GetGUID());
        return true;
    }

    static bool HandleVipRepairCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_REPAIR)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        player->DurabilityRepairAll(false, 0, false);
        handler->PSendSysMessage(LANG_YOUR_ITEMS_REPAIRED, handler->GetNameLink(player).c_str());
        return true;
    }

    static bool HandleVipResetTalentsCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_RESET_TALENTS)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        player->resetTalents(true);
        player->SendTalentsInfoData(false);
        handler->PSendSysMessage(LANG_RESET_TALENTS_ONLINE, handler->GetNameLink(player).c_str());
        return true;
    }

    static bool HandleVipTaxiCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_TAXI)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        player->SetTaxiCheater(true);
        handler->PSendSysMessage(LANG_YOU_GIVE_TAXIS, handler->GetNameLink(player).c_str());
        if (handler->needReportToTarget(player))
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_YOURS_TAXIS_ADDED, handler->GetNameLink().c_str());
        return true;
    }

    static bool HandleVipHomeCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_HOME)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        player->RemoveSpellCooldown(8690, true);
        player->CastSpell(player, 8690, false);
        return true;
    }

    static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_CHANGE_RACE)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
        handler->SendSysMessage(LANG_VIP_CHANGE_RACE);
        return true;
    }

    static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {

        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_CUSTOMIZE)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
        handler->SendSysMessage(LANG_VIP_CHANGE_CUSTOMIZE);
        return true;
    }

    static bool HandleVipCapitalCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_CAPITAL)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        if (player->GetTeamId() == TEAM_HORDE)
            player->CastSpell(player, 3567, true);
        else
            player->CastSpell(player, 3561, true);
        return true;
    }

    static bool HandleVipAppearCommand(ChatHandler* handler, char const* args)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_APPEAR)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsMounted())
        {
            handler->SendSysMessage(LANG_CHAR_NON_MOUNTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        Player* target;
        ObjectGuid targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;


        if (target == player || targetGuid == player->GetGUID())
        {
            handler->SendSysMessage(LANG_CANT_TELEPORT_SELF);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target)
        {
            // check online security
            if (handler->HasLowerSecurity(target, ObjectGuid::Empty))
                return false;

            std::string chrNameLink = handler->playerLink(targetName);

            Map* map = target->GetMap();
            if (target->IsInCombat())
            {
                handler->SendSysMessage(LANG_YOU_IN_COMBAT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->IsInFlight())
            {
                handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->GetMap()->IsBattlegroundOrArena() || target->GetAreaId() == 616)
            {
                handler->SendSysMessage(LANG_VIP_BG);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->HasStealthAura())
            {
                handler->SendSysMessage(LANG_VIP_STEALTH);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->isDead() || target->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
            {
                handler->SendSysMessage(LANG_VIP_DEAD);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetGroup())
            {
                // we are in group, we can go only if we are in the player group
                if (player->GetGroup() != target->GetGroup())
                {
                    handler->SendSysMessage(LANG_VIP_GROUP);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }

            // if the player or the player's group is bound to another instance
            // the player will not be bound to another one
            InstancePlayerBind* bind = sInstanceSaveMgr->PlayerGetBoundInstance(player->GetGUID(), target->GetMapId(), target->GetDifficulty(map->IsRaid()));
            if (!bind)
            {
                Group* group = player->GetGroup();
                // if no bind exists, create a solo bind
                // Check group leader's bind if in group
                InstancePlayerBind* gBind = nullptr;
                if (group)
                {
                    ObjectGuid leaderGuid = group->GetLeaderGUID();
                    gBind = sInstanceSaveMgr->PlayerGetBoundInstance(leaderGuid, target->GetMapId(), target->GetDifficulty(map->IsRaid()));
                }
                if (!gBind)
                    if (InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(target->GetInstanceId()))
                        sInstanceSaveMgr->PlayerBindToInstance(player->GetGUID(), save, !save->CanReset(), player);
            }

            if (map->IsRaid())
                player->SetRaidDifficulty(target->GetRaidDifficulty());
            else
                player->SetDungeonDifficulty(target->GetDungeonDifficulty());

            handler->PSendSysMessage(LANG_APPEARING_AT, chrNameLink.c_str());

            // stop flight if need
            if (player->IsInFlight())
            {
                player->GetMotionMaster()->MovementExpired();
                player->CleanupAfterTaxiFlight();
            }
            // save only in non-flight case
            else
                player->SaveRecallPosition();
            // to point to see at target with same orientation
            float x, y, z;
            target->GetContactPoint(player, x, y, z);
            player->TeleportTo(target->GetMapId(), x, y, z, player->GetAbsoluteAngle(target), TELE_TO_GM_MODE);
            player->SetPhaseMask(target->GetPhaseMask(), true);
        }
        else
        {
            // check offline security
            if (handler->HasLowerSecurity(NULL, targetGuid))
                return false;

            std::string nameLink = handler->playerLink(targetName);

            handler->SendSysMessage(LANG_PLAYER_NOT_EXIST_OR_OFFLINE);
            handler->SetSentErrorMessage(true);
            return false;
        }
        return true;
    }

    static bool HandleVipSummonCommand(ChatHandler* handler, char const* args)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!CheckVipBasic(handler, player))
            return false;

        if (!CheckVipCommandEnabled(handler, sWorld->getBoolConfig(CONFIG_VIP_SUMMON)))
            return false;

        if (player->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!CheckVipCommonState(handler, player))
            return false;

        Player* target;
        ObjectGuid targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;

        if (target == player || targetGuid == player->GetGUID())
        {
            handler->SendSysMessage(LANG_CANT_TELEPORT_SELF);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_EXIST_OR_OFFLINE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (handler->HasLowerSecurity(target, ObjectGuid::Empty))
            return false;

        if (target->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->IsInFlight())
        {
            handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->GetMap()->IsBattlegroundOrArena() || target->GetAreaId() == 616)
        {
            handler->SendSysMessage(LANG_VIP_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->HasStealthAura())
        {
            handler->SendSysMessage(LANG_VIP_STEALTH);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->isDead() || target->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH))
        {
            handler->SendSysMessage(LANG_VIP_DEAD);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Summon is allowed only for players from the same group.
        if (!player->GetGroup() || player->GetGroup() != target->GetGroup())
        {
            handler->SendSysMessage(LANG_VIP_GROUP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (target->IsInFlight())
        {
            target->GetMotionMaster()->MovementExpired();
            target->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            target->SaveRecallPosition();

        float x, y, z;
        player->GetClosePoint(x, y, z, target->GetObjectSize());
        target->TeleportTo(player->GetMapId(), x, y, z, target->GetOrientation(), TELE_TO_GM_MODE, player);
        target->SetPhaseMask(player->GetPhaseMask(), true);

        return true;
    }
};

void AddSC_vip_commandscript()
{
    new vip_commandscript();
}
