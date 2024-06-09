import subprocess
from .. import utils


class Test:
    def __init__(self, cmd, comp):
        self.cmd = cmd
        self.comp = comp

    def test(self):
        result = subprocess.run(self.cmd, capture_output=True)
        result.check_returncode()
        if result.stdout != self.comp:
            utils.prRed("fail:")
            utils.prRed("expected:")
            utils.prRed(self.comp)
            print("found:")
            print(result.stdout)
            return False
        return True


# test system call to command
tests = [
    Test(["./ms_home", "-e", "./tests/runner/runner.lua"], b"done\r\n"),
    Test(
        ["./ms_home", "-r", "Create Something", "-e", "./tests/runner/runner.lua"],
        b"done\r\nsomething\r\n",
    ),
    Test(
        ["./ms_home", "-r", "functional", "-e", "./tests/runner/runner.lua"],
        b"done\r\nfunctional\r\n",
    ),
]

result = True
for test in tests:
    result = test.test()

if result is False:
    utils.prRed("failed runner.py")
    exit(1)
else:
    utils.prGreen("passed runner.py")
