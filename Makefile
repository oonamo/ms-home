INCLUDE_PATH := ./include
run_tests: build
	@ echo "<== Testing ms-home build"
	creal ./tests/tester.creal
build:
	@ echo "<== Building ms-home"
	gcc ./src/main.c -LC:\gcc-lua-install\lua\bin -llua54 -IC:\gcc-lua-install\lua\include -o ms_home
build_local:
	@ echo "<== Building from local lua dll"
	@ echo "include_path is $(INCLUDE_PATH)"
	gcc ./src/main.c -L"$(INCLUDE_PATH)/bin" -llua54 -I"$(INCLUDE_PATH)" -o ms_home
run: build
	@ echo "<== Running ms-home"
	./ms_home
test_local: build_local
	@ echo "<== Testing local build of ms-home"
	creal ./tests/tester.creal
