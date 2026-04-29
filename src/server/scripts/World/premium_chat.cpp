/*
 * VIP/Premium chat coloring
 */

#include "Chat.h"
#include "DatabaseEnv.h"
#include "GameTime.h"
#include "Language.h"
#include "LoginDatabase.h"
#include "Player.h"
#include "PlayerScript.h"
#include "WorldSession.h"

namespace
{
    // Same ids as Premium.lua COLOR_LIST (0..10)
    char const* GetVipColorCode(uint8 id)
    {
        switch (id)
        {
            case 0:  return "";          // off
            case 1:  return "|cFFFFD700"; // gold
            case 2:  return "|cFFFF4444"; // red
            case 3:  return "|cFF00FF00"; // green
            case 4:  return "|cFF00BFFF"; // light blue
            case 5:  return "|cFFB048F8"; // purple
            case 6:  return "|cFFFF8000"; // orange
            case 7:  return "|cFFFF69B4"; // pink
            case 8:  return "|cFF00FFFF"; // turquoise
            case 9:  return "|cFFFFFFFF"; // white
            case 10: return "|cFFE94560"; // scarlet
            default: return "|cFFFFD700";
        }
    }

    uint8 LoadVipTextColor(uint32 accountId)
    {
        LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_ACCOUNT_PREMIUM_CHAT_TEXT_COLOR);
        stmt->SetData(0, accountId);
        PreparedQueryResult res = LoginDatabase.Query(stmt);
        return res ? (*res)[0].Get<uint8>() : 9; // white
    }

    bool ShouldColorize(uint32 type, uint32 language)
    {
        // Never touch addon channel payloads
        if (language == LANG_ADDON)
            return false;

        switch (type)
        {
            case CHAT_MSG_SAY:
            case CHAT_MSG_YELL:
            case CHAT_MSG_PARTY:
            case CHAT_MSG_PARTY_LEADER:
            case CHAT_MSG_RAID:
            case CHAT_MSG_RAID_LEADER:
            case CHAT_MSG_RAID_WARNING:
            case CHAT_MSG_GUILD:
            case CHAT_MSG_OFFICER:
            case CHAT_MSG_WHISPER:
            case CHAT_MSG_CHANNEL:
                return true;
            default:
                return false;
        }
    }

}

class PremiumChat : public PlayerScript
{
public:
    PremiumChat() : PlayerScript("PremiumChat",
        {
            PLAYERHOOK_CAN_PLAYER_USE_CHAT,
            PLAYERHOOK_CAN_PLAYER_USE_PRIVATE_CHAT,
            PLAYERHOOK_CAN_PLAYER_USE_GROUP_CHAT,
            PLAYERHOOK_CAN_PLAYER_USE_GUILD_CHAT,
            PLAYERHOOK_CAN_PLAYER_USE_CHANNEL_CHAT,
        })
    {
    }

    bool OnPlayerCanUseChat(Player* player, uint32 type, uint32 language, std::string& msg) override
    {
        return Colorize(player, type, language, msg);
    }

    bool OnPlayerCanUseChat(Player* player, uint32 type, uint32 language, std::string& msg, Player* /*receiver*/) override
    {
        return Colorize(player, type, language, msg);
    }

    bool OnPlayerCanUseChat(Player* player, uint32 type, uint32 language, std::string& msg, Group* /*group*/) override
    {
        return Colorize(player, type, language, msg);
    }

    bool OnPlayerCanUseChat(Player* player, uint32 type, uint32 language, std::string& msg, Guild* /*guild*/) override
    {
        return Colorize(player, type, language, msg);
    }

    bool OnPlayerCanUseChat(Player* player, uint32 type, uint32 language, std::string& msg, Channel* /*channel*/) override
    {
        return Colorize(player, type, language, msg);
    }

private:
    static bool Colorize(Player* player, uint32 type, uint32 language, std::string& msg)
    {
        if (!player || msg.empty() || !ShouldColorize(type, language))
            return true;

        WorldSession* sess = player->GetSession();
        if (!sess || !sess->IsPremium())
            return true;

        uint8 textColorId = LoadVipTextColor(sess->GetAccountId());

        if (textColorId == 0)
            return true;

        char const* textColorCode = GetVipColorCode(textColorId);
        if (!textColorCode || !*textColorCode)
            return true;

        std::string const original = msg;
        std::string const msgColor = (textColorCode && *textColorCode) ? textColorCode : "|cFFFFFFFF";
        // Important: this hook can safely color message body only.
        // Player name/prefix ([Name] says:) is composed by chat packets separately.
        msg = msgColor + original + "|r";

        return true;
    }
};

void AddSC_premium_chat()
{
    new PremiumChat();
}

