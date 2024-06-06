#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool file_exists(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    bool exists = false;
    if (fp != NULL)
    {
        exists = true;
        fclose(fp);
    }
    return exists;
}

char *get_conf_path(void)
{
    char *home = getenv("USERPROFILE");
    if (home == NULL)
        return NULL;
    char *app_path = "\\AppData\\Local\\manage_my_home\\conf.lua";

    char *conf_path = strcat(home, app_path);
    return conf_path;
}

static int addTwo(lua_State *L)
{
    int lhs = lua_tonumber(L, 1);
    int rhs = lua_tonumber(L, 2);

    lua_pushnumber(L, lhs + rhs);

    return 1;
}

void create_default_conf(const char *path)
{
    printf("creating default file at %s\n", path);
}

static void init_lua_state(lua_State *L)
{
    lua_newtable(L);               // mmh
    luaL_dofile(L, "lua/mmh.lua"); // mmh lua functions
    lua_pushcfunction(L, &addTwo);
    lua_setfield(L, -2, "addTwo");
    lua_setglobal(L, "mmh");
    lua_settop(L, 0); // set stack at the bottom
}

int main(void)
{

    const char *path = get_conf_path();
    if (path)
        printf("conf path: %s\n", path);
    else
        printf("could not find path\n");
    bool ok = file_exists(path);

    if (!ok)
    {
        printf("file does not exist.. creating default\n");
    }

    // Initialize Lua Interperter
    lua_State *L = luaL_newstate();

    // Load lua libraries
    luaL_openlibs(L);
    init_lua_state(L);

    if (!ok)
        printf("there was error");

    // Run Config
    ok &= luaL_dofile(L, path);

    if (ok)
        printf("there was error");

    lua_close(L);
    return 0;
}
