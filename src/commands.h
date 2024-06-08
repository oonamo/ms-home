#ifndef COMMAND_H
#define COMMAND_H
#include <stdlib.h>
typedef enum
{
    ACTION_DEFAULT,
    ACTION_EVALUATE,
    ACTION_SEND_ARGS,
    ACTION_EXECTUTE_RUNNER,
} Action;

typedef struct
{
    Action action;
    char *path;
    int argc;
    char *args[];
} Arguments;

typedef struct
{
    const char *action_str;
    Action action_enum;
} ActionMap;

extern const ActionMap ACTION_MAP[];
void destroy_arguments(Arguments *arg);
Arguments *parse_flags(int argc, char *argv[]);
#endif
