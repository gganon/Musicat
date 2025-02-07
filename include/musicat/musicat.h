#ifndef MUSICAT_H
#define MUSICAT_H

#include <dpp/dpp.h>
#include <string>
#include <vector>
#include <stdexcept>
#include "musicat/player.h"

namespace musicat
{
    // Main
    int run(int argc, const char* argv[]);

    template <typename T>
    typename std::vector<T>::iterator vector_find(std::vector<T>* _vec, T _find);

    struct settings
    {
        std::string defaultPrefix;

        std::map<dpp::snowflake, std::string> prefix;
        std::vector<dpp::snowflake> joining_list;

        std::string get_prefix(const dpp::snowflake guildId) const;

        auto set_prefix(const dpp::snowflake guildId, const std::string newPrefix);

        void set_joining_vc(dpp::snowflake vc_id);

        void remove_joining_vc(dpp::snowflake vc_id);
    };

    /**
     * @brief Destroy and reset connecting state of the guild, must be invoked when failed to join or left a vc
     *
     * @param client The client
     * @param guild_id Guild Id of the vc client connecting in
     * @param delete_voiceconn Whether to delete found voiceconn, can cause segfault if the underlying structure doesn't exist
     */
    void reset_voice_channel(dpp::discord_client* client, dpp::snowflake guild_id, bool delete_voiceconn = false);

    /**
     * @brief Get the voice object and connected voice members a vc of a guild
     *
     * @param guild Guild the member in
     * @param user_id Target member
     * @return std::pair<dpp::channel*, std::map<dpp::snowflake, dpp::voicestate>>
     * @throw const char* User isn't in vc
     */
    std::pair<dpp::channel*, std::map<dpp::snowflake, dpp::voicestate>> get_voice(dpp::guild* guild, dpp::snowflake user_id);

    /**
     * @brief Get the voice object and connected voice members a vc of a guild
     *
     * @param guild_id Guild Id the member in
     * @param user_id Target member
     * @return std::pair<dpp::channel*, std::map<dpp::snowflake, dpp::voicestate>>
     * @throw const char* Unknown guild or user isn't in vc
     */
    std::pair<dpp::channel*, std::map<dpp::snowflake, dpp::voicestate>> get_voice_from_gid(dpp::snowflake guild_id, dpp::snowflake user_id);

    /**
     * @brief Execute shell cmd and return anything it printed to console
     *
     * @param cmd Command
     * @return std::string
     * @throw const char* Exec failed (can't call popen or unknown command)
     */
    std::string exec(std::string cmd);

    bool has_listener(std::map<dpp::snowflake, dpp::voicestate>* vstate_map);

    bool has_listener_fetch(dpp::cluster* client, std::map<dpp::snowflake, dpp::voicestate>* vstate_map);

    template<typename T, typename E> void get_inter_param(const E& event, std::string param_name, T* param)
    {
        auto p = event.get_parameter(param_name);
        if (p.index()) *param = std::get<T>(p);
    }

    class exception : std::exception {
    private:
        const char* message;
        int c;

    public:
        exception(const char* _message, int _code = 0);

        virtual const char* what() const noexcept;

        virtual int code() const noexcept;
    };

    int cli(dpp::cluster& client, dpp::snowflake sha_id, int argc, const char* argv[], bool* running_state = nullptr);

    bool has_permissions(dpp::guild* guild, dpp::user* user, dpp::channel* channel, std::vector<uint64_t> permissions = {});
    bool has_permissions_from_ids(dpp::snowflake guild_id, dpp::snowflake user_id, dpp::snowflake channel_id, std::vector<uint64_t> permissions = {});

    /**
     * @brief Format ms duration to HH:MM:SS
     *
     * @param dur
     * @return std::string
     */
    std::string format_duration(uint64_t dur);

    /**
     * @brief Get random index of len to use as base to shuffle an array
     *
     * @param len
     * @return std::vector<size_t>
     */
    std::vector<size_t> shuffle_indexes(size_t len);

    /**
     * @brief Attempt to join voice channel
     *
     * @param from
     * @param player_manager
     * @param guild_id
     * @param user_id
     * @param sha_id
     *
     * @return int 0 if request to connect sent
     */
    int join_voice(dpp::discord_client* from,
	    player::player_manager_ptr player_manager,
	    const dpp::snowflake& guild_id,
	    const dpp::snowflake& user_id,
	    const dpp::snowflake& sha_id);

}

#endif
