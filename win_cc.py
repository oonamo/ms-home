#!/usr/local/bin/python
# Windows compile_commands.json
# semi-manual generator
import os

dir_path = os.path.dirname(os.path.realpath(__file__))
file_path = dir_path + "\\src\\main.c"
command = "gcc {file_path} -LC:\\gcc-lua-install\\lua\\bin -llua54 -IC:\\gcc-lua-install\\lua\\include -o mmh".format(
    file_path=file_path
)

command = command.replace("\\", "\\\\")
dir_path = dir_path.replace("\\", "\\\\")
file_path = file_path.replace("\\", "\\\\")

compile_commands_path = dir_path + "\\compile_commands.json"

with open(compile_commands_path, "w") as f:
    f.write("[\t\n")
    f.write("\t{\n")
    f.write('\t\t"directory": "' + dir_path + '",\n')
    f.write('\t\t"command": "' + command + '",\n')
    f.write('\t\t"file": "' + file_path + '"\n')
    f.write("\t}\n")
    f.write("]")

print("wrote compile_commands.json to " + compile_commands_path)
