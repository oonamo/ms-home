import subprocess

result = subprocess.run(
    ["./ms_home", "e", "./tests/runner/runner.lua"], capture_output=True
)
result.check_returncode()

if result.stdout == b"done\r\n":
    print("runner passed")
    exit(0)
else:
    print("failed asssert")
    print(result.stdout)
    print('expected \'b"done\\r\\n"')
    exit(1)
