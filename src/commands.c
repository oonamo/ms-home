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

Arguments parse_flags(int argc, char *argv[])
{
    int opt;
    Arguments args = {
        .action = ACTION_DEFAULT,
        .path = NULL,
    };
    while ((opt = getopt(argc, argv, "e:r:")) != -1)
    {
        switch (opt)
        {
        case 'r':
            args.action = ACTION_EXECTUTE_RUNNER;
            args.args[0] = optarg;
        case 'e':
            args.action = ACTION_EVALUATE;
            args.path = optarg;
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

Arguments parse_args(int argc, char *argv[])
{
    Arguments args = {
        .action = ACTION_DEFAULT,
        .path = NULL,
    };

    if (argc < 2)
        return args;
    if (argc == 2)
    {
        args.path = argv[1];
        return args;
    }
    for (int i = 0; i < sizeof(ACTION_MAP) / sizeof(ACTION_MAP[0]); i++)
    {
        if (strcmp(argv[1], ACTION_MAP[i].action_str) == 0)
        {
            args.action = ACTION_MAP[i].action_enum;
            break;
        }
    }
    args.path = argv[2];
    return args;
}
