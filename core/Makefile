CXX := g++	
CXXFLAGS := -std=c++14 -Wall

LIB_DIR := ../lib
OBJ_DIR := ../objs

OBJECTS = Diff.o Patch.o MyersDiffMatch.o
LIB = myersdiff

all: $(OBJECTS) $(LIB) cleanobjdir

$(OBJECTS): %.o: %.cpp | mkdirobj
	$(CXX) -c $(CXXFLAGS) $< -o $(OBJ_DIR)/$@

$(LIB): | mkdirlib
	ar rcs $(addprefix $(LIB_DIR)/lib,$(LIB)).a $(wildcard $(OBJ_DIR)/*.o)

# create object directories rule
.PHONY: mkdirobj mkdirlib

mkdirobj:
	mkdir $(OBJ_DIR)

mkdirlib:
	mkdir $(LIB_DIR)

# rule for clearing object files
.PHONY: cleanall cleandirs cleanobjdir cleanlibdir

cleanall: cleandirs

cleandirs: cleanobjdir cleanlibdir

cleanobjdir:
	rm -r $(OBJ_DIR)
	$(info [$(OBJ_DIR)] folder has been removed.)

cleanlibdir:
	rm -r $(LIB_DIR)
	$(info [$(LIB_DIR)] folder has been removed.)