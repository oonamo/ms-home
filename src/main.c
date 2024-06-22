#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "commands.c"

#ifdef _WIN32
# define CONFIG_VAR "USERPROFILE"
# define CONFIG_EXT "\\AppData\\Local\\ms_home\\conf.lua"
# include "platforms/win.c"
#else
// TODO: fallback to $HOME
// Also Handle diffrent platforms
// also check if this is actually the right var...
# define CONFIG_VAR         "XDG_CONFIG_HOME"
# define CONFIG_EXT         "/ms_home/conf.lua"
# define c_get_appearence() 1
#endif

void error(lua_State *L, args_t *a, const char *fmt, ...)
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
  for (i = 1; i <= top; i++) { /* repeat for each level */
    int t = lua_type(L, i);
    switch (t) {
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
  if (fp != NULL) {
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
  lua_pop(L, 1);  // remove string from stack
  return 0;
}

static int get_appearance(lua_State *L)
{
#ifndef _WIN32
  lua_pushstring(L, "dark");
  return 1;
#endif
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
  // TODO: get global instead of returning
  lua_newtable(L);                 // home
  luaL_dofile(L, "lua/home.lua");  // home lua functions
  lua_getglobal(L, "home");
  lua_pushcfunction(L, &l_system);
  lua_setfield(L, -2, "system");
  /*lua_setglobal(L, "home");*/
  /* lua_settop(L, 0);                // empty the stack */
  luaL_dofile(L, "lua/utils.lua");  // add utils table to stack
  lua_pushcfunction(L, &get_appearance);
  lua_setfield(L, -2, "get_appearance");
  lua_setglobal(L, "utils");
  lua_settop(L, 0);  // empty the stack
}

/// Executes Home[idx].execute_runner() if action is ACTION_EXECUTE_RUNNER,
/// otherwise executes Home[idx].execute_tag()
static void execute_tag_runner(lua_State *L, args_t *args, const char *home, const char *tag_runner,
                               int idx)
{
  lua_getglobal(L, "Homes");
  if (!lua_istable(L, -1))
    error(L, args,
          "Global 'Homes' was overwritten. Please remove any reference to "
          "'Home' in your config.\n");
  int home_c = lua_rawlen(L, -1);
  bool matched = false;
  for (int i = 1; i <= home_c; i++) {
    lua_rawgeti(L, -1, i);        // Homes[i]
    lua_getfield(L, -1, "name");  // Homes[i].name
    const char *home_name = lua_tostring(L, -1);
    lua_pop(L, 1);  // pop name, Homes[i] will be ontop
    if (strcmp(home_name, home) == 0) {
      if (args->map[i].action == ACTION_EXECUTE_RUNNER)
        lua_getfield(L, -1, "execute_runner");
      else
        lua_getfield(L, -1, "execute_tag");
      lua_pushvalue(L,
                    -2);  // push Homes[i] to the top of the stack
      lua_pushstring(L, tag_runner);
      int err = lua_pcall(L, 2, 0,
                          0);  // Homes[i].execute_runner(self, name)
      if (err)
        error(L, args, "There was an error executing your runner/tag:\n\t%s\n",
              lua_tostring(L, -1));
      lua_settop(L, 0);
      matched = true;
      break;
    }
    lua_pop(L, 1);
  }

  if (!matched)
    error(L, args, "Did not find home '%s'\n", home);
}

int main(int argc, char *argv[])
{
  args_t *args = parse_flags(argc, argv);
  char *path = get_conf_path();
  bool err;

  if (args->path != NULL) {
    path = strdup(args->path);
  }

  if (path == NULL) {
    fprintf(stderr, "could not find path\n");
    destroy_arguments(args);
    exit(EXIT_FAILURE);
  }

  if (!file_exists(path)) {
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

  // Run Config
  err = luaL_dofile(L, path);
  if (err)
    error(L, args, "there was an error loading %s:\n\t%s\n", path, lua_tostring(L, -1));
  if (args->map == NULL) {
    error(L, args, "how did we get here\n");
  }
  for (int i = 0; i < args->argc; i++) {
    switch (args->map[i].action) {
    case ACTION_EXECUTE_RUNNER:
    case ACTION_EXECUTE_TAG: {
      execute_tag_runner(L, args, args->home, args->map[i].arg, i);
      break;
    }
    case ACTION_SEND_ARGS:
    case ACTION_DEFAULT:
    case ACTION_EVALUATE:
      break;
    }
  }
  lua_close(L);
  destroy_arguments(args);
  return 0;
}
