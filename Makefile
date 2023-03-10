# Master Makefile
# Quinton Graham
# makes the reversi projects files

# Compiler used
CXX = gcc

# Included Paths for any header files or such
INCLUDE = -Iinclude

# Library paths for external or compiled libraries
LIBRARY = -Llib/32bit -Llib/64bit 

# Any compiler flags we wish to use
COMPILE_FLAGS = 

# Linker flags for librarys
LINKER_FLAGS = -lSDL2 -lSDL2_ttf

# Output file name
OUTPUT_FILE_NAME = reversi

# Source directory base
SD = src

# Files for make file to hunt for
OBJS = reversi.c $(SD)/graphics/board_display.c $(SD)/graphics/text_display.c $(SD)/game_logic.c
#$(shell dir src /s /b) reversi_copy.c


all: 
	$(CXX) $(INCLUDE) $(OBJS) $(LIBRARY) $(COMPILE_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT_FILE_NAME)