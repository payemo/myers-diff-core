CXX ?= g++

# path
BUILD_PATH = ./build
TEST_SRC_PATH = ./tests

TEST_SRCS := $(wildcard $(TEST_SRC_PATH)/*cpp)

# exe
EXE_NAME = diff-patch

# flags
CXXFLAGS = -std=c++17 -Wall
INCLUDES = -I./core

# objects
TEST_OBJS := $(TEST_SRCS:%.cpp=%.o)

$(TEST_OBJS): $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_SRCS) -o $(BUILD_PATH)/$(EXE_NAME)

.PHONY: makedir
makedir:
	$(info "Creating directory: $(BUILD_PATH)")
	mkdir -p $(BUILD_PATH)

# debug executable
debug: CXXFLAGS += -DDEBUG -g
debug: BUILD_PATH := $(addsuffix /debug, $(BUILD_PATH))
debug: EXE_NAME := $(addsuffix -d, $(EXE_NAME))
debug: exec

# release executable	
release: CXXFLAGS += -O2
release: BUILD_PATH := $(addsuffix /release, $(BUILD_PATH))
release: exec

exec: makedir $(TEST_OBJS)