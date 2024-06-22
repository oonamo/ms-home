INCLUDE_PATH := ./include

after_build:
	copy /Y include\bin\lua54.dll build\

run_tests: build after_build
	@ echo "<== Testing ms-home build"
	creal ./tests/tester.creal

build: after_build
	@ echo "<== Building ms-home"
	gcc ./src/main.c -LC:\gcc-lua-install\lua\bin -llua54 -IC:\gcc-lua-install\lua\include -o build/ms_home

build_local: after_build
	@ echo "<== Building from local lua dll"
	@ echo "include_path is $(INCLUDE_PATH)"
	gcc ./src/main.c -L"$(INCLUDE_PATH)/bin" -llua54 -I"$(INCLUDE_PATH)" -o build/ms_home
run: build after_build
	@ echo "<== Running ms-home"
	./build/ms_home

test_local: build_local after_build
	@ echo "<== Testing local build of ms-home"
	creal ./tests/tester.creal
