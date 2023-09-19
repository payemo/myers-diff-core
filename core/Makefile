CXX := g++	
CXXFLAGS := -std=c++14 -Wall

LIB_DIR := ../lib
OBJ_DIR := ../objs

OBJECTS = Diff.o Patch.o MyersDiffMatch.o
LIB = myersdiff

all: $(OBJECTS) build-static-lib $(clean-obj-dir)

$(OBJECTS): %.o: %.cpp | mkdirobj
	$(CXX) -c $(CXXFLAGS) $< -o $(OBJ_DIR)/$@

# create object directories rule
.PHONY: mkdirobj mkdirlib build-static-lib

mkdirobj:
	@mkdir $(OBJ_DIR)
	@echo "$(OBJ_DIR) created."

mkdirlib:
	@mkdir $(LIB_DIR)
	@echo $(LIB_DIR) created.

# rule for clearing object files
.PHONY: cleanall cleandirs

cleanall: cleandirs

cleandirs: $(clean-obj-dir) $(clean-lib-dir)

define clean-obj-dir
	@rm -r $(OBJ_DIR)
	@echo [$(OBJ_DIR)] folder has been removed.
endef

define clean-lib-dir
	@rm -r $(OBJ_DIR)
	@echo [$(OBJ_DIR)] folder has been removed.
endef

build-static-lib: $(OBJECTS) | mkdirlib
	@ar rcs $(addprefix $(LIB_DIR)/lib,$(LIB)).a $(wildcard $(OBJ_DIR)/*.o)
	$(clean-obj-dir)