cmake_minimum_required(VERSION 4.3)
project(Fantatennis C)

set(CMAKE_C_STANDARD 11)

add_executable(Fantatennis src/main.c
        include/models/tennis_player.h
        src/models/user.c
        include/models/user.h
        src/cli/cli_login.c
        include/cli/cli_login.h
        src/services/auth_service.c
        include/services/auth_service.h
        include/models/tennis_player.h
        src/app_context.c
        include/app_context.h
        src/cli/cli_startuperr.c
        include/cli/cli_startuperr.h
        src/cli/cli_utils.c
        include/cli/cli_utils.h
        src/cli/cli_mainmenu.c
        include/cli/cli_mainmenu.h
        include/models/tournament.h
        include/models/match.h
        include/models/team.h
        src/entity_table.c
        include/entity_table.h
        src/models/team.c
        src/models/match.c
        src/services/seeding_service.c
        include/services/seeding_service.h
        src/cli/submodules/cli_tennisplayers.c
        include/cli/submodules/cli_tennisplayers.h
        include/cli/submodules/cli_tennisplayers.h
        src/services/players_service.c
        include/services/players_service.h
        src/utils/string_utils.c
        include/utils/string_utils.h
        include/common_types.h
        src/services/tournaments_service.c
        include/services/tournaments_service.h
        src/cli/submodules/cli_tournaments.c
        include/cli/submodules/cli_tournaments.h
        src/cli/submodules/cli_leaderboards.c
        include/cli/submodules/cli_leaderboards.h
        src/cli/submodules/cli_matches.c
        include/cli/submodules/cli_matches.h
        src/services/matches_service.c
        include/services/matches_service.h
        src/services/teams_service.c
        include/services/teams_service.h
        src/services/scoring_service.c
        include/services/scoring_service.h
        src/cli/submodules/cli_play.c
        include/cli/submodules/cli_play.h
        src/services/system_service.c
        include/services/system_service.h
)

enable_testing()
add_subdirectory(tests)
if(MSVC)
    target_compile_options(Fantatennis PRIVATE /W4 /permissive-)
else()
    target_compile_options(Fantatennis PRIVATE -Wall -Wextra -pedantic)
endif()