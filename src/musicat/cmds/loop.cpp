#include "musicat/cmds.h"

namespace musicat {
    namespace command {
        namespace loop {
            dpp::slashcommand get_register_obj(const dpp::snowflake& sha_id) {
                return dpp::slashcommand(
                    "loop",
                    "Configure [repeat] mode",
                    sha_id
                ).add_option(
                    dpp::command_option(
                        dpp::co_integer,
                        "mode",
                        "Set [to this] mode",
                        true
                    ).add_choice(
                        dpp::command_option_choice("One", player::loop_mode_t::l_song)
                    ).add_choice(
                        dpp::command_option_choice("Queue", player::loop_mode_t::l_queue)
                    ).add_choice(
                        dpp::command_option_choice("One/Queue", player::loop_mode_t::l_song_queue)
                    ).add_choice(
                        dpp::command_option_choice("Off", player::loop_mode_t::l_none)
                    )
                );
            }

            void slash_run(const dpp::interaction_create_t& event, player::player_manager_ptr player_manager)
            {
                if (!player_manager->voice_ready(event.command.guild_id))
                {
                    event.reply("Please wait while I'm getting ready to stream");
                    return;
                }
                const dpp::snowflake sha_id = player_manager->sha_id;
                static const char* loop_message[] = { "Turned off repeat mode", "Set to repeat a song", "Set to repeat queue", "Set to repeat a song and not to remove skipped song" };

                std::pair<dpp::channel*, std::map<dpp::snowflake, dpp::voicestate>> uvc;
                std::pair<dpp::channel*, std::map<dpp::snowflake, dpp::voicestate>> cvc;
                try
                {
                    uvc = get_voice_from_gid(event.command.guild_id, event.command.usr.id);
                }
                catch (const char* e)
                {
                    return event.reply("You're not in a voice channel");
                }
                try
                {
                    cvc = get_voice_from_gid(event.command.guild_id, sha_id);
                }
                catch (const char* e)
                {
                    return event.reply("I'm not playing anything right now");
                }
                if (uvc.first->id != cvc.first->id) return event.reply("You're not in my voice channel");
                int64_t g_l = 0;
                get_inter_param(event, "mode", &g_l);

                int8_t a_l = (int8_t)g_l;

                if (a_l < 0 || a_l > 3)
                {
                    event.reply("Invalid mode");
                    return;
                }

                auto player = player_manager->create_player(event.command.guild_id);
                player->from = event.from;
		if (player->saved_config_loaded != true) player_manager->load_guild_player_config(event.command.guild_id);

                if (player->loop_mode == a_l)
                {
                    event.reply("Already set to that mode");
                    return;
                }

                player->set_loop_mode(a_l);
                event.reply(loop_message[a_l]);
                try
                {
                    player_manager->update_info_embed(event.command.guild_id);
                }
                catch (...)
                {
                    // Meh
                }
            }
        }
    }
}
