# 医院信息系统 - UI/GUI/MAIN模块开发教程

## 1. UI模块开发指南

### 1.1 UI模块概述
UI模块是医院信息管理系统的用户交互层，负责提供友好的用户界面。本项目包含两个版本的UI：
- 控制台版本（适用于学习和调试）
- 图形界面版本（适用于实际部署）

### 1.2 UI模块的文件结构
```
Hospital-Information-System/
├── include/
│   └── ui.h              # UI函数声明和常量定义
└── src/
    └── ui.c              # UI功能实现
```

### 1.3 UI模块设计原则
1. **模块化设计**：UI功能与其他业务逻辑分离
2. **可扩展性**：易于添加新功能和修改现有功能
3. **用户体验**：提供清晰、直观的操作界面
4. **一致性**：保持界面风格和交互方式的一致性

### 1.4 UI模块实现步骤

#### 步骤1：定义UI接口（ui.h）
首先，我们需要定义UI模块的接口，包括函数声明、颜色常量和尺寸常量。

```c
// 颜色定义
#define COLOR_BG         RGB(240, 248, 255)   // 背景色
#define COLOR_TITLE      RGB(70, 130, 180)     // 标题色
#define COLOR_TEXT       RGB(50, 50, 50)       // 文本色

// 尺寸定义
#define WINDOW_WIDTH   1000   // 窗口默认宽度
#define WINDOW_HEIGHT  700    // 窗口默认高度

// 函数声明
void initUI(void);
void showMainMenu(void);
bool inputBox(int x, int y, const char* title, char* input, int maxLen, bool isPassword);
```

#### 步骤2：实现UI功能（ui.c）
接下来，我们实现具体的UI功能，包括菜单显示、输入处理等。

```c
void showMainMenu(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║       医院综合信息管理系统 - 主菜单              ║\n");
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║  1. 病人信息管理                                   ║\n");
    printf("║  2. 医生信息管理                                   ║\n");
    printf("║  3. 挂号管理                                       ║\n");
    printf("║  4. 统计报表                                       ║\n");
    printf("║  0. 退出系统                                       ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    printf("请选择: ");
}
```

## 2. GUI模块开发指南

### 2.1 GUI模块概述
GUI模块使用StellarX框架实现图形用户界面，提供更直观的用户体验。

### 2.2 GUI模块的文件结构
```
Hospital-Information-System/
└── src/
    └── main_gui.cpp      # GUI主入口
```

### 2.3 GUI模块开发步骤

#### 步骤1：理解StellarX框架
StellarX是一个轻量级的C++ GUI框架，提供了窗口、控件、事件处理等基本功能。

#### 步骤2：创建主窗口
```cpp
// main_gui.cpp
int main() {
    // 初始化StellarX框架
    // 创建主窗口
    // 设置窗口属性
    // 显示登录界面
    // 进入消息循环
    return 0;
}
```

#### 步骤3：实现界面布局
- 设计主界面布局
- 创建菜单栏和工具栏
- 实现各个功能模块的界面

## 3. Main模块开发指南

### 3.1 Main模块概述
Main模块是程序的入口点，负责初始化系统、处理用户输入、协调各个模块的工作。

### 3.2 Main模块的文件结构
```
Hospital-Information-System/
└── src/
    └── main.c            # 程序主入口
```

### 3.3 Main模块开发步骤

#### 步骤1：系统初始化
```c
int main() {
    // 初始化日志系统
    initLogging();
    
    // 加载所有数据
    buildUserChain(&g_userHead, &g_userTail);
    buildPatientChain(&g_patientHead, &g_patientTail);
    // ... 加载其他数据
    
    // 显示主菜单
    showMainMenu();
    
    // 清理资源
    cleanupLogging();
    return 0;
}
```

#### 步骤2：实现主循环
主循环负责处理用户的输入并调用相应的功能模块。

## 4. 项目集成指南

### 4.1 编译配置
确保Makefile正确包含所有UI相关文件：

```makefile
SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.cpp)
```

### 4.2 模块间通信
UI模块通过全局变量与业务逻辑模块通信：
```c
// 全局链表头尾指针
extern Patient*    g_patientHead;
extern Doctor*     g_doctorHead;
extern Registration* g_regHead;
```

## 5. 常见问题和解决方案

### 5.1 编译错误
- 确保头文件路径正确
- 检查函数声明和实现的一致性

### 5.2 内存管理
- 确保链表节点正确释放
- 避免内存泄漏

### 5.3 用户体验
- 提供清晰的错误提示
- 实现输入验证功能

## 6. 进阶开发建议

### 6.1 代码重构
- 将UI逻辑与业务逻辑进一步分离
- 使用回调函数处理事件

### 6.2 功能扩展
- 添加数据验证功能
- 实现数据备份和恢复

### 6.3 性能优化
- 优化数据加载速度
- 减少内存占用

这份教程文档详细介绍了UI/GUI/MAIN模块的开发方法，可以帮助开发者理解和实现这些部分。