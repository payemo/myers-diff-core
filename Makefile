CORE_DIR = ./core
TESTS_DIR = ./tests

all: build-static-lib run-tests

build-static-lib:
	"$(MAKE)" build-static-lib -C $(CORE_DIR)

run-tests:
	"$(MAKE)" run-test -C $(TESTS_DIR)