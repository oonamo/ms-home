import subprocess
from .. import utils

utils.prGreen("testing commands...")


class Test:
    def __init__(self, test, code):
        self.test = test
        self.code = code

    def run_test(self):
        result = subprocess.run(self.test)
        return result.returncode == self.code
        # return subprocess.run(self.test).returncode == self.code


tests = [
    Test(["./ms_home"], 0),
    Test(["./ms_home", "-f", "./tests/commands/dne.lua"], 1),
    Test(["./ms_home", "-e", "./tests/commands/should_pass.lua"], 0),
]

for test in tests:
    # print(test.test)
    result = test.run_test()
    # print(result)
    if not result:
        utils.prRed("test failed")
        utils.prRed("command: " + str(test.test))
        print("expected: " + str(test.code))
        print("got: " + str(result))
        exit(1)


utils.prGreen("passed commands.py")
