#include "commands.c"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define CONFIG_VAR "USERPROFILE"
#define CONFIG_EXT "\\AppData\\Local\\ms_home\\conf.lua"
#include "platforms/win.c"
#else
// TODO: fallback to $HOME
// Also Handle diffrent platforms
// also check if this is actually the right var...
#define CONFIG_VAR "XDG_CONFIG_HOME"
#define CONFIG_EXT "/ms_home/conf.lua"
#endif

// from lua docs
void error(lua_State *L, Arguments *a, const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    destroy_arguments(a);
    exit(EXIT_FAILURE);
}

static void stackDump(lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++)
    { /* repeat for each level */
        int t = lua_type(L, i);
        switch (t)
        {

        case LUA_TSTRING: /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;

        case LUA_TBOOLEAN: /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;

        case LUA_TNUMBER: /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;

        default: /* other values */
            printf("%s", lua_typename(L, t));
            break;
        }
        printf("  "); /* put a separator */
    }
    printf("\n"); /* end the listing */
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
    char *home = getenv(CONFIG_VAR);
    if (home == NULL)
        return NULL;
    char *conf_path = strcat(home, CONFIG_EXT);
    return conf_path;
}

// TODO: Return output as string
static int l_system(lua_State *L)
{
    const char *command = lua_tostring(L, 1);
    system(command);
    lua_pop(L, 1); // remove string from stack
    return 0;
}

static int get_appearance(lua_State *L)
{
    bool is_dark = c_get_appearence();
    if (is_dark)
        lua_pushstring(L, "dark");
    else
        lua_pushstring(L, "light");
    return 1;
}

void create_default_conf(const char *path)
{
    printf("creating default file at %s\n", path);
}

static void init_lua_state(lua_State *L)
{
    lua_newtable(L);                // mmh
    luaL_dofile(L, "lua/home.lua"); // mmh lua functions
    lua_pushcfunction(L, &l_system);
    lua_setfield(L, -2, "system");
    lua_setglobal(L, "home");
    /* lua_settop(L, 0);                // empty the stack */
    luaL_dofile(L, "lua/utils.lua"); // add utils table to stack
    lua_pushcfunction(L, &get_appearance);
    lua_setfield(L, -2, "get_appearance");
    lua_setglobal(L, "utils");
    lua_settop(L, 0); // empty the stack
}

int main(int argc, char *argv[])
{
    Arguments *args = parse_flags(argc, argv);
    const char *path = get_conf_path();

    if (args->path != NULL)
    {
        path = args->path;
    }

    if (path == NULL)
    {
        fprintf(stderr, "could not find path\n");
        destroy_arguments(args);
        exit(EXIT_FAILURE);
    }

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
    {
        printf("there was error\n");
        lua_close(L);
        destroy_arguments(args);
        exit(EXIT_FAILURE);
    }

    // Run Config
    ok ^= luaL_dofile(L, path);
    if (args->action == ACTION_EXECTUTE_RUNNER)
    {
        if (*args->args == NULL)
            error(L, args, "no runner name, bad program\n");
        const char *runner_name = args->args[0];

        // home table
        lua_getglobal(L, "home");
        if (!lua_istable(L, -1))
            error(L, args, "'home' is not a table");
        lua_getfield(L, -1, "execute_runner");
        lua_pushvalue(L, -2);           // home table (aka self)
        lua_pushstring(L, runner_name); // name
        lua_pcall(L, 2, 0, 0);          // home.execute_runner(self, name)
        lua_close(L);
        destroy_arguments(args);
        exit(EXIT_SUCCESS);
    }

    /* switch (args->action) */
    /* { */
    /* case ACTION_DEFAULT: */
    /*     break; */
    /* case ACTION_EVALUATE: */
    /*     break; */
    /* case ACTION_EXECTUTE_RUNNER: */
    /*     break; */
    /* case ACTION_SEND_ARGS: */
    /*     break; */
    /* } */

    if (!ok)
    {
        printf("there was error reading user file %s\n", path);
        lua_close(L);
        destroy_arguments(args);
        exit(EXIT_FAILURE);
    }

    lua_close(L);
    destroy_arguments(args);
    return 0;
}
