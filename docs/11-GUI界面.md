# GUI图形界面 - 教程文档

> 本文档详细介绍StellarX图形界面的使用

---

## 1. StellarX框架介绍

### 1.1 什么是StellarX？

StellarX是一个基于EasyX的现代化C++ GUI框架：
- 轻量级，无需复杂依赖
- 组件化设计，易于使用
- 支持按钮、文本框、表格等多种控件

### 1.2 项目地址

- GitHub: https://github.com/Ysm-04/StellarX
- 官网: https://stellarx-gui.top

---

## 2. 核心组件

### 2.1 Window（窗口）

```cpp
// 创建窗口
Window mainWindow(1300, 800, nullptr, RGB(255,255,255), "标题");
mainWindow.draw();
mainWindow.runEventLoop();
```

### 2.2 Button（按钮）

```cpp
// 创建按钮
auto btn = std::make_unique<Button>(x, y, width, height, "按钮文字");

// 添加点击事件
btn->setOnClickListener([]() {
    // 点击后执行的代码
});

// 添加到窗口
mainWindow.addControl(std::move(btn));
```

### 2.3 TextBox（文本框）

```cpp
// 创建文本框
auto textBox = std::make_unique<TextBox>(x, y, width, height);

// 获取输入内容
std::string content = textBox->getText();

// 设置文本
textBox->setText("默认文字");
```

### 2.4 Label（标签）

```cpp
// 创建标签
auto label = std::make_unique<Label>(x, y, "标签文字");

// 设置字体大小
label->textStyle.nHeight = 50;

// 设置字体
label->textStyle.lpszFace = "微软雅黑";
```

### 2.5 Table（表格）

```cpp
// 创建表格
auto table = std::make_unique<Table>(x, y, width, height);

// 设置表头
table->setHeaderText(0, "列1");
table->setHeaderText(1, "列2");

// 设置单元格内容
table->setCellText(row, col, "内容");

// 清空表格
table->clear();
```

### 2.6 TabControl（选项卡）

```cpp
// 创建TabControl
auto tabs = std::make_unique<TabControl>(x, y, width, height);
tabs->setTabPlacement(StellarX::TabPlacement::Left); // 左侧标签

// 添加页面
auto page = std::make_pair(
    std::make_unique<Button>(0, 0, 100, 100, "标签名"),
    std::make_unique<Canvas>(0, 0, width, height)
);
tabs->add(std::move(page));
```

---

## 3. 主程序结构

```cpp
#include "StellarX.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // 1. 创建主窗口
    Window win(1300, 800, nullptr, RGB(255,255,255), "标题");
    
    // 2. 创建登录页面组件
    // ...
    
    // 3. 创建主页面TabControl
    // ...
    
    // 4. 绑定事件
    loginBtn->setOnClickListener([&]() {
        // 登录逻辑
    });
    
    // 5. 添加组件到窗口
    win.addControl(std::move(loginCanvas));
    win.addControl(std::move(tabControl));
    
    // 6. 绘制并运行
    win.draw("image/background.jpg");
    return win.runEventLoop();
}
```

---

## 4. 布局示例

### 4.1 左侧导航栏布局

```
┌────┬──────────────────────────────────────┐
│ 病人 │                                      │
├────┤  内容区域                              │
│ 医生 │  ┌────────────────────────────────┐ │
├────┤  │                                │ │
│ 挂号 │  │      表格显示区域               │ │
├────┤  │                                │ │
│ 药品 │  └────────────────────────────────┘ │
├────┤                                      │
│ 住院 │  [添加] [删除] [刷新] [搜索]         │
└────┴──────────────────────────────────────┘
```

---

## 5. 下一步学习

继续学习：
- [12-文件IO.md](./12-文件IO.md)
- [13-工具函数.md](./13-工具函数.md)
