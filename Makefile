INCLUDE_PATH := ./include

.PHONY: build run_tests run test_local
run_tests: build
	@ echo "<== Testing ms-home build"
	creal ./tests/tester.creal

build:
	@ echo "<== Building ms-home"
	gcc ./src/main.c -LC:\gcc-lua-install\lua\bin -llua54 -IC:\gcc-lua-install\lua\include -o build/ms_home

build_local:
	@ echo "<== Building from local lua dll"
	@ echo "include_path is $(INCLUDE_PATH)"
	gcc ./src/main.c -L"$(INCLUDE_PATH)/bin" -llua54 -I"$(INCLUDE_PATH)" -o build/ms_home
run: build
	@ echo "<== Running ms-home"
	./build/ms_home

test_local: build_local
	@ echo "<== Testing local build of ms-home"
	creal ./tests/tester.creal
debug:
	@ echo "<== GCC Debug"
	@ gcc -L"$(INCLUDE_PATH)/bin" -llua54 -I"$(INCLUDE_PATH)" -g -fno-inline -fno-omit-frame-pointer ./src/main.c -o build/ms_home 
