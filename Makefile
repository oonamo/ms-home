run_tests: build
	python ./tests/runner/runner.py
	python ./tests/commands/command.py
build:
	gcc ./src/main.c -LC:\gcc-lua-install\lua\bin -llua54 -IC:\gcc-lua-install\lua\include -o ms_home
run: build
	./ms_home
