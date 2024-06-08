#include "commands.h"
#include <string.h>

const ActionMap ACTION_MAP[] = {
    {"", ACTION_DEFAULT},
    {"e", ACTION_EVALUATE},
    {"eval", ACTION_EVALUATE},
    {"evaluate", ACTION_EVALUATE},
};

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
