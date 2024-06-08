import subprocess

print("running runner tests...")

# test system call to command
result = subprocess.run(
    ["./ms_home", "-e", "./tests/runner/runner.lua"], capture_output=True
)
result.check_returncode()

if result.stdout != b"done\r\n":
    print("failed asssert")
    print(result.stdout)
    print('expected \'b"done\\r\\n"')
    exit(1)
print("called system from evalue success")

result = subprocess.run(
    ["./ms_home", "-r", "Create Something", "-e", "./tests/runner/runner.lua"],
    capture_output=True,
)

# runner.lua executes runner that calls "echo done" and then cli calls "echo something"
if result.stdout != b"done\r\nsomething\r\n":
    print("failed asssert")
    print(result.stdout)
    print('expected \'b"something\\r\\n"')
    exit(1)
