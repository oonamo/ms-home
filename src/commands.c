#include "commands.h"
#include "optarg.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const ActionMap ACTION_MAP[] = {{"", ACTION_DEFAULT},
                                {"e", ACTION_EVALUATE},
                                {"eval", ACTION_EVALUATE},
                                {"evaluate", ACTION_EVALUATE},
                                {"send_args", ACTION_SEND_ARGS}};

Arguments *parse_flags(int argc, char *argv[])
{
    int opt;
    Arguments *args = malloc(sizeof(Arguments) + sizeof(char *) * (argc - 1));
    args->path = NULL;
    args->action = ACTION_DEFAULT;
    args->args[0] = NULL;
    args->argc = 1;
    while ((opt = getopt(argc, argv, "e:r:")) != -1)
    {
        switch (opt)
        {
        case 'r':
            args->action = ACTION_EXECTUTE_RUNNER;
            args->args[0] = optarg;
            args->argc = 1;
            break;
        case 'e':
            if (args->action == ACTION_DEFAULT)
                args->action = ACTION_EVALUATE;
            args->path = optarg;
            break;
        case '?':
        {
            fprintf(stderr, "Usage: %s [-r runner] [-e evaluate]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        }
    }
    return args;
}

void destroy_arguments(Arguments *arg)
{
    for (size_t i = 0; i < arg->argc; i++)
        free(arg->args[i]);
    free(arg);
}
