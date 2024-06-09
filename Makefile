# python -m tests.runner.runner
run_tests: build
	python -m tests.runner.runner
	python -m tests.commands.commands
build:
	gcc ./src/main.c -LC:\gcc-lua-install\lua\bin -llua54 -IC:\gcc-lua-install\lua\include -o ms_home
run: build
	./ms_home
