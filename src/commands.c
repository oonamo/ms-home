#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "optarg.c"

void arguments_set_arg(args_t *args, char *optarg)
{
  if (args->map[args->argc].allocated) {
    // reallocate
    args->map[args->argc].arg = realloc(args->map[args->argc].arg, strlen(optarg) + 1);
  } else {
    args->map[args->argc].arg = malloc(strlen(optarg) + 1);
  }
  strcpy(args->map[args->argc].arg, optarg);
  args->map[args->argc].allocated = 1;
}

args_t *parse_flags(int argc, char *argv[])
{
  int opt;
  args_t *args = malloc(sizeof(args_t));
  if (args == NULL) {
    fprintf(stderr, "failed to allocate memory for Arguments struct, exiting\n");
    exit(EXIT_FAILURE);
  }
  args->path = NULL;
  args->action = ACTION_DEFAULT;
  args->argc = 1;
  args->map = malloc((argc - 1) * sizeof(arg_map_t));
  args->map[0].allocated = 0;

  // strlen("home") + 1 = 5
  args->home = malloc((size_t)5);
  strcpy(args->home, "home");
  while ((opt = getopt(argc, argv, "e:r:t:h:")) != -1) {
    args->map[args->argc].allocated = 0;
    switch (opt) {
    case 'r':
      args->action = ACTION_EXECUTE_RUNNER;
      arguments_set_arg(args, optarg);
      args->map[args->argc].action = ACTION_EXECUTE_RUNNER;
      break;
    case 'e':
      if (args->action == ACTION_DEFAULT)
        args->action = ACTION_EVALUATE;
      if (args->path == NULL) {
        args->path = malloc(strlen(optarg) + 1);
        strcpy(args->path, optarg);
      }
      arguments_set_arg(args, optarg);
      args->map[args->argc].action = ACTION_EVALUATE;
      break;
    case 't':
      /*args->map[args->argc].arg = optarg;*/
      arguments_set_arg(args, optarg);
      args->map[args->argc].action = ACTION_EXECUTE_TAG;
      break;
    case 'h': {
      args->home = realloc(args->home, strlen(optarg) + 1);
      strcpy(args->home, optarg);
      break;
    }
    case '?': {
      fprintf(stderr,
              "Usage: %s [-r runner_name] [-e file] [-h home_name] [-t "
              "tag_name]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
    }
    args->argc++;
  }
  return args;
}

void destroy_arguments(args_t *args)
{
  for (int i = 0; i < args->argc; i++) {
    if (args->map[i].allocated) {
      free(args->map[i].arg);
    }
  }
  if (args->home != NULL) {
    free(args->home);
  }
  if (args->path != NULL) {
    free(args->path);
  }
  if (args->map != NULL) {
    free(args->map);
  }
  free(args);
}
