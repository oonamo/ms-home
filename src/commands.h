#ifndef COMMAND_H
#define COMMAND_H
typedef enum {
  ACTION_DEFAULT,
  ACTION_EVALUATE,
  ACTION_SEND_ARGS,
  ACTION_EXECUTE_RUNNER,
  ACTION_EXECUTE_TAG,
} Action;

typedef struct {
  char *arg;
  Action action;
} ArgMap;

typedef struct {
  Action action;
  int argc;
  char *home;
  char *path;
  ArgMap *map;
} Arguments;

void destroy_arguments(Arguments *arg);
Arguments *parse_flags(int argc, char *argv[]);
#endif
