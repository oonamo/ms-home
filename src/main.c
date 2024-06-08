#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LOCAL_PATH "\\AppData\\Local\\ms_home\\conf.lua"

void error(lua_State *L, const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}

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
    char *conf_path = strcat(home, LOCAL_PATH);
    return conf_path;
}

static int l_run(lua_State *L)
{
    const char *command = lua_tostring(L, 1);
    printf("command: %s\n", command);
    if (!lua_istable(L, 2))
    {
        return luaL_error(L, "Expected table as second paramater");
    }
    const int argc = lua_rawlen(L, 2);

    char *argv[argc];
    for (int i = 1; i <= argc; i++)
    {
        lua_pushinteger(L, i);
        lua_gettable(L, 2);
        const char *value = lua_tostring(L, -1);
        printf("Val %s at idx %d\n", value, i);
        argv[i - 1] =
            (char *)value; // WARN: argument cannot be altered, do not mutate
        lua_pop(L, 1);     // remove value
    }

    for (int i = 0; i < argc; i++)
        printf("arg %d is %s\n", i, argv[i]);
    // TODO: pass arguements to system
    return 0;
}

void create_default_conf(const char *path)
{
    printf("creating default file at %s\n", path);
}

static void init_lua_state(lua_State *L)
{
    lua_newtable(L);                // mmh
    luaL_dofile(L, "lua/home.lua"); // mmh lua functions
    lua_pushcfunction(L, &l_run);
    lua_setfield(L, -2, "run");
    lua_setglobal(L, "home");
    lua_settop(L, 0); // empty the stack
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
        // TODO: Create default file
        printf("file does not exist.. creating default\n");
        create_default_conf(path);
        return -1;
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
        printf("there was error reading user file");

    lua_close(L);
    printf("closing...\n");
    return 0;
}
