CXX = g++
CXXFLAGS = -std=c++14 -Wall

core_src_dir = ../core
core_inc_dir := -I$(core_src_dir)

test_src_dir = ./src
test_include_dir = -I./include

tests_srcs = $(wildcard $(test_src_dir)/*cpp)
tests_objs = $(patsubst $(test_src_dir)/%.cpp, $(build_dir)/%.o, $(tests_srcs))

core_lib = ../lib/libmyersdiff.a

build_dir = ./build
exe_name = myersdiff-tests

all: $(build_dir) cleanobjs

$(build_dir): $(tests_objs) $(core_lib)
	$(CXX) $(CXXFLAGS) $(core_inc_dir) $(test_include_dir) diff_patch_test.cpp $^ -o $@/$(exe_name)

$(tests_objs): $(build_dir)/%.o: $(test_src_dir)/%.cpp | mkobjdir
	$(CXX) $(CXXFLAGS) $(core_inc_dir) $(test_include_dir) -c $< -o $@

.PHONY: mkobjdir cleanobjs test

mkobjdir:
	@mkdir $(build_dir)
	@echo $(build_dir) was created.

run-test: $(build_dir) cleanobjs
	@echo Running tests...
	@$(build_dir)/$(exe_name).exe
	@echo Delete [$(exe_name).exe] file.
	@rm -r $(build_dir)
	@echo Delete [../lib] directory.
	@rm -r ../lib

cleanobjs:
	@rm -f $(build_dir)/*.o