#ifndef COMMAND_H
#define COMMAND_H
typedef enum
{
    ACTION_DEFAULT,
    ACTION_EVALUATE,
} Action;

typedef struct
{
    Action action;
    char *path;
} Arguments;

typedef struct ActionMap
{
    const char *action_str;
    Action action_enum;
} ActionMap;

extern const ActionMap ACTION_MAP[];

Arguments parse_args(int argc, char *argv[]);
#endif
