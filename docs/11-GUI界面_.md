# GUI图形界面 - 学习版

> 本文档为学习版，代码部分已替换为组件名和实现思路提示。
> ⚠️ 注意：`ui.h` 尚未由团队完成，以下内容为框架性指导，具体接口以实际编写时为准。

---

## 1. GUI 框架选择

### 1.1 本项目方案：StellarX

本项目使用 **StellarX** 框架，基于 EasyX 的现代化 C++ GUI 框架，轻量级、组件化设计。

- GitHub: https://github.com/Ysm-04/StellarX
- 官网: https://stellarx-gui.top
- 框架文件位置：`include/StellarX/`

**提示**：在 `main_gui.cpp` 中包含：
```cpp
#include "StellarX/StellarX.h"  // 或具体组件头文件
using namespace StellarX;         // 所有组件在 StellarX 命名空间下
```

### 1.2 团队 ui.h 待完成

`include/ui.h` 由 **成员3（panxl）** 负责，目前尚未提交。完成后的 `ui.h` 应包含各模块界面的初始化函数声明，如：

```c
// ui.h（待完成，由 panxl 负责）
void initPatientUI(Window* parent);
void initDoctorUI(Window* parent);
void initMedicineUI(Window* parent);
void initHospitalizationUI(Window* parent);
void initRegistrationUI(Window* parent);
void showLoginWindow();
void showMainWindow();
```

---

## 2. 核心组件

> **统一格式**：`[返回类型] 函数名(...)` — 所属头文件

### 2.1 Window（窗口）

```c
// StellarX 框架
Window(int width, int height, Window* parent, int bgColor, const char* title);
void draw();
void runEventLoop();
```

**组件作用**：GUI 的主容器，所有其他组件都放在窗口里。

**实现思路**：
1. **第1步**：构造 `Window(w, h, nullptr, COLOR, "标题")` 创建主窗口
2. **第2步**：调用 `draw()` 绘制窗口背景
3. **第3步**：调用 `runEventLoop()` 启动事件循环（程序会在这里阻塞，直到用户关闭窗口）

### 2.2 Button（按钮）

```c
// StellarX 框架
Button(int x, int y, int w, int h, const char* text);
void setOnClickListener(std::function<void()> callback);
void addControl(Control* c);
```

**组件作用**：用户点击触发操作，对应命令行中的"请选择："交互。

**实现思路**：
1. **第1步**：构造 `Button(x, y, w, h, "按钮文字")`
2. **第2步**：调用 `setOnClickListener([](){ /* 写点击后要做什么 */ })` 绑定点击事件
3. **第3步**：调用 `parentWindow.addControl(&btn)` 添加到窗口

### 2.3 TextBox（文本框）

```c
// StellarX 框架
TextBox(int x, int y, int w, int h);
std::string getText();
void setText(const char* text);
void clear();
void addControl(Control* c);
```

**组件作用**：用户输入文字，对应命令行中的 scanf。

**实现思路**：
1. **第1步**：构造 `TextBox(x, y, w, h)`
2. **第2步**：用 `getText()` 获取用户输入（返回 `std::string`）
3. **第3步**：用 `setText()` 设置默认/预填充文字
4. **第4步**：用 `clear()` 清空内容

### 2.4 Label（标签）

```c
// StellarX 框架
Label(int x, int y, const char* text);
void setText(const char* text);
void setTextStyle(int nHeight, const char* lpszFace);
void addControl(Control* c);
```

**组件作用**：显示文字，对应 printf 的输出。

**实现思路**：
1. **第1步**：构造 `Label(x, y, "显示文字")`
2. **第2步**：用 `setTextStyle(nHeight, "字体名")` 设置字号和字体
3. **第3步**：用 `setText()` 动态更新显示内容

### 2.5 Table（表格）

```c
// StellarX 框架
Table(int x, int y, int w, int h);
void setHeaderText(int col, const char* text);
void setCellText(int row, int col, const char* text);
void setRowCount(int count);
void clear();
void addControl(Control* c);
```

**组件作用**：以表格形式显示数据列表，对应 listAllXxx() 遍历打印。

**实现思路**：
1. **第1步**：构造 `Table(x, y, w, h)`
2. **第2步**：调用 `setHeaderText(0, "列1")`、`setHeaderText(1, "列2")` 设置表头
3. **第3步**：调用 `setRowCount(n)` 设置总行数
4. **第4步**：遍历链表，用 `setCellText(row, col, value)` 填充每一格
5. **第5步**：调用 `addControl(&table)` 添加到窗口

### 2.6 TabControl（选项卡）

```c
// StellarX 框架
TabControl(int x, int y, int w, int h);
void setTabPlacement(int placement);  // Left/Top/Bottom/Right
void add(int index, const char* tabName, Canvas* page);
Canvas* getCurrentPage();
void addControl(Control* c);
```

**组件作用**：左侧导航栏，点击不同标签切换不同模块页面，对应命令行中的多级菜单。

**实现思路**：
1. **第1步**：构造 `TabControl(x, y, w, h)`
2. **第2步**：调用 `setTabPlacement(Left)` 设置标签在左侧
3. **第3步**：为每个模块创建 `Canvas` 页面，往上面放表格、文本框、按钮
4. **第4步**：调用 `tab.add(0, "病人", &patientCanvas)` 添加各模块页面
5. **第5步**：调用 `addControl(&tab)` 添加到窗口

---

## 3. 主程序结构

**入口函数**：`int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)`（不是 `main()`）

### 3.1 main_gui.cpp 结构

```c
// main_gui.cpp
#include "StellarX/StellarX.h"
#include "ui.h"        // 尚未完成，由 panxl 负责
using namespace StellarX;

// 声明所有模块链表头尾指针（来自各模块 .c 文件的全局变量）
extern Patient* g_patientHead;
extern Patient* g_patientTail;
extern Doctor* g_doctorHead;
extern Doctor* g_doctorTail;
// ... 其他模块同理

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int show) {
    // 1. 加载所有模块数据（从文件到链表）
    buildPatientChain(g_patientHead, g_patientTail);
    buildDoctorChain(g_doctorHead, g_doctorTail);
    // ... 其他模块

    // 2. 创建主窗口
    Window win(1280, 720, nullptr, WHITE, "医院信息管理系统");

    // 3. 创建登录界面（或直接调用 ui.h 的函数）
    showLoginWindow(&win);

    // 4. 登录成功后创建主界面
    if (g_isLoggedIn) {
        showMainWindow(&win);
    }

    // 5. 绘制并启动事件循环
    win.draw();
    win.runEventLoop();

    // 6. 退出前保存所有数据
    rebuildPatientFile(*g_patientHead);
    rebuildDoctorFile(*g_doctorHead);
    // ... 其他模块

    return 0;
}
```

---

## 4. 登录界面布局

```
┌──────────────────────────────────────────┐
│                                          │
│         医院信息管理系统                    │
│                                          │
│     用户名:  [_______________]           │
│     密码:    [_______________]           │
│                                          │
│         [    登录    ]  [  注册  ]        │
│                                          │
│         提示: 请输入账号密码               │
└──────────────────────────────────────────┘
```

**实现思路**：
1. **第1步**：创建 `Label` 显示标题
2. **第2步**：创建 `TextBox` 接收用户名和密码
3. **第3步**：创建 `Button` 绑定点击事件，调用 `login()` 函数验证
4. **第4步**：登录成功后，销毁登录界面组件，切换到主界面

---

## 5. 主界面布局

```
┌──────────────────────────────────────────────────────────┐
│  医院信息管理系统                           [退出登录]    │
├────────┬─────────────────────────────────────────────────┤
│  病人   │  ┌──────────────────────────────────────────┐  │
│  医生   │  │  Table（表格显示数据列表）                 │  │
│  挂号   │  └──────────────────────────────────────────┘  │
│  药品   │  ┌──────────────────────────────────────────┐  │
│  住院   │  │  TextBox（输入框）+ Button（操作按钮）    │  │
│  统计   │  └──────────────────────────────────────────┘  │
│         │  [添加]  [删除]  [修改]  [刷新]  [搜索]        │
└────────┴─────────────────────────────────────────────────┘
```

**实现思路**：
1. **第1步**：创建 `TabControl` 左侧导航，添加各模块标签
2. **第2步**：为每个模块创建独立 `Canvas`，上面放置 `Table` 和操作按钮
3. **第3步**：各按钮的 `setOnClickListener` 中调用对应模块的 `.c` 文件函数
4. **第4步**：操作后刷新 `Table` 内容（调用 `table.clear()` 后重新填充）

---

## 6. 下一步学习

1. [12-文件IO_.md](./12-文件IO_.md) - 文件读写
2. [05-病人模块_.md](./05-病人模块_.md) - 病人模块（动手实践）

---

**文档版本**：v2.0（标注返回类型和头文件 · GUI 框架说明）
