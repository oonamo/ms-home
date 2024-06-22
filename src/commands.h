#ifndef COMMAND_H
#define COMMAND_H
typedef enum {
  ACTION_DEFAULT,
  ACTION_EVALUATE,
  ACTION_SEND_ARGS,
  ACTION_EXECUTE_RUNNER,
  ACTION_EXECUTE_TAG,
} action_t;

typedef struct {
  char *arg;
  action_t action;
  int allocated;
} arg_map_t;

typedef struct {
  action_t action;
  int argc;
  char *home;
  char *path;
  arg_map_t *map;
} args_t;

void destroy_arguments(args_t *arg);
args_t *parse_flags(int argc, char *argv[]);

void arguments_set_arg(args_t *args, char *optarg);

#endif
