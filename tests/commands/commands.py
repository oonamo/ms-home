import subprocess
from .. import utils

utils.prGreen("testing commands...")


class Test:
    def __init__(self, test, code):
        self.test = test
        self.code = code

    def run_test(self):
        result = subprocess.run(self.test)
        if result.returncode != self.code:
            utils.prRed("test failed")
            utils.prRed("command: " + str(test.test))
            print("expected: " + str(test.code))
            print("got: " + str(result.returncode))
            exit(1)
            return False
        return True
        # return subprocess.run(self.test).returncode == self.code


tests = [
    Test(["./ms_home"], 0),
    Test(["./ms_home", "-f", "./tests/commands/bad_flag.lua.lua"], 1),
    Test(["./ms_home", "-e", "./tests/commands/should_pass.lua"], 0),
    Test(
        ["./ms_home", "-e", "./tests/commands/should_pass.lua", "-t", "test_runner"], 0
    ),
    Test(
        [
            "./ms_home",
            "-e",
            "./tests/commands/fake_runner.lua",
            "-t",
            "not_real_runner",
        ],
        1,
    ),
    Test(
        [
            "./ms_home",
            "-e",
            "./tests/commands/duplicate_home.lua",
        ],
        1,
    ),
]

for test in tests:
    result = test.run_test()


utils.prGreen("passed commands.py")
