#pragma once

// Runs without starting the renderer, audio, or game loop.
int host_run_save(int argc, char **argv);

struct GAMESAVE_s;
// Validate the mobile envelope and checksum; never modify the input file.
bool host_read_game_fixture(const char *path, GAMESAVE_s &game);
