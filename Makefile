# 医疗信息管理系统 - Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
DATADIR = data

# 源文件
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/hospital_system.exe

# 创建必要的目录
DIRECTORIES = $(OBJDIR) $(BINDIR) $(DATADIR)

# 默认目标
all: directories $(TARGET)

# 创建目录
directories: $(DIRECTORIES)
$(DIRECTORIES):
	mkdir -p $@

# 编译目标
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ -lm

# 编译源文件
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# 清理
clean:
	rm -rf $(OBJDIR)/*
	rm -f $(TARGET)

# 重新构建
rebuild: clean all

# 运行
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run directories