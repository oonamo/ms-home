#include "commands.h"
#include "optarg.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Arguments *parse_flags(int argc, char *argv[])
{
    int opt;
    Arguments *args = malloc(sizeof(Arguments));
    if (args == NULL)
    {
        fprintf(stderr,
                "failed to allocate memory for Arguments struct, exiting\n");
        exit(EXIT_FAILURE);
    }
    args->path = NULL;
    args->action = ACTION_DEFAULT;
    args->argc = 1;
    args->map = malloc((argc - 1) * sizeof(ArgMap));
    args->home = "home";
    while ((opt = getopt(argc, argv, "e:r:t:h:")) != -1)
    {
        switch (opt)
        {
        case 'r':
            args->action = ACTION_EXECUTE_RUNNER;
            args->map[args->argc].arg = optarg;
            args->map[args->argc].action = ACTION_EXECUTE_RUNNER;
            break;
        case 'e':
            if (args->action == ACTION_DEFAULT)
                args->action = ACTION_EVALUATE;
            args->path = optarg;
            args->map[args->argc].arg = optarg;
            args->map[args->argc].action = ACTION_EVALUATE;
            break;
        case 't':
            args->map[args->argc].arg = optarg;
            args->map[args->argc].action = ACTION_EXECUTE_TAG;
            break;
        case 'h':
            args->home = optarg;
            break;
        case '?':
        {
            fprintf(stderr, "Usage: %s [-r runner] [-e evaluate]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        }
        args->argc++;
    }
    return args;
}

void destroy_arguments(Arguments *arg)
{
    free(arg->map);
    free(arg);
}
