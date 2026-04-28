# 医院信息管理系统

## 项目概述

这是一个基于C语言开发的医院信息管理系统，采用链表数据结构实现，支持患者管理、医生管理、挂号管理等功能。

## 项目结构

```
Hospital-Information-System/
├── src/                    # 源代码目录
│   ├── main.c             # 主程序入口
│   ├── ui.c               # UI界面实现 (新增)
│   ├── main_gui.cpp       # GUI主入口 (新增)
│   ├── patient.c          # 患者管理模块
│   ├── doctor.c           # 医生管理模块
│   ├── registration.c     # 挂号管理模块
│   ├── login.c            # 登录模块
│   ├── utils.c            # 工具函数模块
│   ├── file_io.c          # 文件I/O模块
│   └── StellarX/          # GUI框架源码
├── include/               # 头文件目录
│   ├── ui.h               # UI界面头文件 (新增)
│   ├── patient.h          # 患者管理头文件
│   ├── doctor.h           # 医生管理头文件
│   ├── registration.h     # 挂号管理头文件
│   ├── login.h            # 登录模块头文件
│   ├── utils.h            # 工具函数头文件
│   ├── file_io.h          # 文件I/O头文件
│   └── StellarX/          # GUI框架头文件
├── data/                  # 数据文件目录
├── docs/                  # 文档目录
├── bin/                   # 可执行文件目录
├── obj/                   # 目标文件目录
├── Makefile               # 构建脚本
└── README.md              # 项目说明
```

## 最近更新

### 2026-04-28 更新内容

#### 1. UI/GUI 部分
- [已删除] `include/ui.h`, `src/ui.c`, `src/main_gui.cpp` - UI模块相关文件已被删除，需参考教程 `docs/18-1-UI模块开发教程.md` 重新创建
- [新增] `docs/18-7-主程序入口开发教程.md` - 指导如何实现main.c程序入口文件
- [新增] `docs/18-8-GUI主程序入口开发教程.md` - 指导如何实现main_gui.cpp GUI主程序入口文件
- [新增] `docs/18-9-项目完成综合指南.md` - 项目完成的综合指导文档，整合所有教程

#### 2. 修复的语法错误
- 修复了 `patient.h` 中 `int age` 缺少分号的问题
- 修复了 `patient.c` 中 `listAllPatients` 函数的printf参数问题
- 修复了 `doctor.c` 中的多处语法错误
- 修复了 `utils.h` 和 `utils.c` 中的语法错误
- 修复了 `file_io.c` 中的语法错误

#### 3. 项目完整性提升
- 创建了 `Analysis_Report.md` - 项目分析报告
- 创建了 `Project_Incomplete_Parts.md` - 项目不完整部分说明
- 创建了 `docs/Detailed_UI_Module_Tutorial.md` - UI模块详细开发教程
- 创建了 `docs/Syntax_Error_Fix_Tutorial.md` - 语法错误修复教程
- 创建了 `docs/Project_Completion_Tutorial.md` - 项目完善教程
- 创建了 `docs/Project_Structure_Tutorial.md` - 项目结构组织教程

## 编译和运行

### 编译项目
```bash
make all
```

### 运行程序
```bash
make run
```

### 清理编译文件
```bash
make clean
```

## 系统功能

### 已实现功能
- 用户登录/注册
- 患者信息管理（添加、查询、列表）
- 医生信息管理（添加、查询、列表）
- 挂号管理（挂号、查询、列表）
- 数据文件持久化

### 待实现功能
- 完整的GUI界面实现
- 药品管理模块
- 住院管理模块
- 床位管理模块
- 完善的错误处理机制
- 数据验证和安全性改进

## 技术特点

- 使用双向链表数据结构
- 模块化设计
- 支持数据文件持久化
- 提供控制台和GUI两种界面选项

## 开发说明

本项目使用StellarX GUI框架，如需编译GUI版本，需要确保系统已安装相应的Windows开发库。