#include "commands.h"
#include "optarg.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Arguments *parse_flags(int argc, char *argv[])
{
    int opt;
    Arguments *args = malloc(sizeof(Arguments));
    args->path = NULL;
    args->action = ACTION_DEFAULT;
    /* args->args[0] = NULL; */
    args->argc = 1;
    args->map = malloc((argc - 1) * sizeof(ArgMap));
    while ((opt = getopt(argc, argv, "e:r:t:h:")) != -1)
    {
        switch (opt)
        {
        case 'r':
            args->action = ACTION_EXECTUTE_RUNNER;
            args->map[args->argc].arg = optarg;
            args->map[args->argc].action = ACTION_EXECTUTE_RUNNER;
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
            args->map[args->argc].action = ACTION_EVALUATE;
        case 'h':
            args->home = optarg;
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
