import subprocess

print("running runner tests...")

# test system call to command
result = subprocess.run(
    ["./ms_home", "-e", "./tests/runner/runner.lua"], capture_output=True
)
result.check_returncode()

if result.stdout == b"done\r\n":
    exit(0)
else:
    print("failed asssert")
    print(result.stdout)
    print('expected \'b"done\\r\\n"')
    exit(1)

result = subprocess.run(
    ["./ms_home", "-r", "Create Something", "-e", "./tests/runner/runner.lua"]
)

print("success")
