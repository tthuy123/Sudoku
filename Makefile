# Hướng dẫn:
# - `make` hoặc `make all`: Biên dịch và chạy
# - `make compile`: Chỉ biên dịch
# - `make run`: Chạy chương trình
# - `make clean`: Xóa file object

# Use bash shell instead of PowerShell
SHELL := /bin/bash

TARGET = Sudoku

# Path to SDL2
SDL2_path = .

CC = g++
CFLAGS = -std=c++17 -Wall -pedantic -MMD -MP 

# Định nghĩa thư mục
OUTDIR = .
SRC_DIR = ./src
OBJ_DIR = ./obj
LIB_DIR = ./dep/lib

# Tìm tất cả file .cpp trong src/
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC = $(call rwildcard, $(SRC_DIR), *.cpp)

# Định nghĩa file .o tương ứng trong obj/
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:$(SRC_DIR)/%.cpp=%.o))

# Định nghĩa thư mục include
INC_DIRS = -Idep/include -I$(SDL2_path)/dep/include

# Định nghĩa thư viện
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf
LIB_DIRS = -Ldep/lib

# Mục tiêu chính: Biên dịch & chạy
.PHONY: all 
all: compile run

# Chỉ biên dịch
.PHONY: compile
compile: $(TARGET)

# Biên dịch chương trình
$(TARGET): $(OBJS)
	$(CC) -o $(OUTDIR)/$@ $(OBJS) $(LIB_DIRS) $(LIBS)

# Biên dịch từng file .cpp thành .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(subst /,\,$(@D))  # Create directory if it doesn't exist
	$(CC) -o $@ $(CFLAGS) -c $< $(INC_DIRS)

# Xóa file biên dịch
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*  # Remove object files

# Chạy chương trình
.PHONY: run
run:
	$(OUTDIR)/$(TARGET)
