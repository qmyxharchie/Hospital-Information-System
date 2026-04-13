# GUI界面模块 - 学习版

> 本文档为学习版，代码部分以实现思路为主，关键处给出可参考的核心片段
> 参考源码路径：`hospital-management/src/ui.c`、`hospital-management/include/ui.h`、`hospital-management/src/main_gui.cpp`

---

## 1. 模块概述

### 1.1 功能描述

GUI界面模块负责整个医院信息管理系统的用户交互层，同时提供两套界面：

| 界面类型 | 源文件 | 说明 |
|---------|--------|------|
| 控制台版本 | `src/ui.c` | 命令行菜单，适合学习期间调试 |
| 图形界面版本 | `src/main_gui.cpp` | 基于 StellarX，适合实际使用 |

### 1.2 界面模块与其他模块的关系

```
┌─────────────────────────────────────────────────────┐
│              GUI 界面模块 (ui.c / main_gui.cpp)      │
│  - 负责显示菜单、接收用户输入并调用业务层函数        │
│  - 不做数据运算，只做"展示"与"交互"                 │
└─────────────────────────────────────────────────────┬─
                   │ 调用各模块函数（patient.c、doctor.c …）
                   │
┌─────────────────────────────────────────────────────┬─
│              业务逻辑模块 (各模块 .c 文件)            │
│  - 增删改查操作                                   │
│  - 链表操作                                       │
└─────────────────────────────────────────────────────┬─
                   │ 调用文件读写函数
                   │
┌─────────────────────────────────────────────────────┐
│              数据持久化 (file_io.c)                │
│  - 从 data/*.txt 读入链表                          │
│  - 从链表写入 data/*.txt                           │
└─────────────────────────────────────────────────────┘
```

### 1.3 文件清单

| 目标路径 | 参考来源 | 说明 |
|---------|---------|------|
| `include/ui.h` | `hospital-management/include/ui.h` | 头文件：声明、颜色宏、尺寸宏 |
| `src/ui.c` | `hospital-management/src/ui.c` | 控制台版本实现 |
| `src/main_gui.cpp` | `hospital-management/src/main_gui.cpp` | 图形界面版本实现 |

---

## 2. ui.h 头文件

### 2.1 全局链表指针声明

UI 模块需要访问各模块的链表头尾指针，故 `ui.h` 中声明为 `extern`：

```c
// ui.h
// extern 声明告诉编译器"这个变量在其他文件定义"，链接时再找

extern Patient*         g_patientHead;
extern Patient*         g_patientTail;
extern Doctor*          g_doctorHead;
extern Doctor*          g_doctorTail;
extern Registration*    g_regHead;
extern Registration*    g_regTail;
extern Medicine*        g_medHead;
extern Medicine*        g_medTail;
extern Purchase*        g_purHead;
extern Purchase*        g_purTail;
extern Hospitalization* g_hosHead;
extern Hospitalization* g_hosTail;
extern Bed*             g_bedHead;
extern Bed*             g_bedTail;
```

> **提示**：这些指针在各模块 `.c` 文件中定义（`Patient* g_patientHead = NULL;`），不在 `ui.c` 中定义——UI 模块只是"使用它们"而已。

### 2.2 颜色定义

```c
// ui.h
// RGB(R, G, B)：R/G/B 取值范围 0~255

#define COLOR_BG           RGB(240, 248, 255)  // 背景色（AliceBlue）
#define COLOR_TITLE        RGB(70, 130, 180)    // 标题色（SteelBlue）
#define COLOR_TEXT         RGB(50, 50, 50)      // 文本色（深灰色）
#define COLOR_BUTTON       RGB(100, 149, 237)  // 按钮色（CornflowerBlue）
#define COLOR_BUTTON_HOVER RGB(70, 130, 180)   // 按钮悬停色
#define COLOR_SUCCESS      RGB(34, 139, 34)    // 成功色（Green）
#define COLOR_WARNING      RGB(255, 140, 0)    // 警告色（DarkOrange）
#define COLOR_ERROR        RGB(220, 20, 60)    // 错误色（Crimson）
```

### 2.3 尺寸定义

```c
// ui.h
#define WINDOW_WIDTH   1000   // 窗口宽度
#define WINDOW_HEIGHT  700    // 窗口高度
#define BUTTON_WIDTH   200    // 按钮宽度
#define BUTTON_HEIGHT  50     // 按钮高度
#define INPUT_WIDTH    300    // 输入框宽度
#define INPUT_HEIGHT   40     // 输入框高度
```

### 2.4 函数声明

```c
// ui.h

/**
 * @brief 初始化界面（GUI版需要，控制台版空实现）
 */
void initUI(void);

/**
 * @brief 关闭界面，释放资源（GUI版需要）
 */
void closeUI(void);

/**
 * @brief 显示登录页面（控制台版）
 * @return 登录状态码
 *         0 = LOGIN_FAILED      （登录失败）
 *         1 = LOGIN_SUCCESS_USER  （普通用户登录成功）
 *         2 = LOGIN_SUCCESS_ADMIN （管理员登录成功）
 *         3 = LOGIN_EXIT         （退出系统）
 */
int showLoginPage(void);

/**
 * @brief 显示主菜单
 */
void showMainMenu(void);

/**
 * @brief 病人管理菜单
 */
void showPatientManagement(void);

/**
 * @brief 医生管理菜单
 */
void showDoctorManagement(void);

/**
 * @brief 挂号管理菜单
 */
void showRegistrationManagement(void);

/**
 * @brief 药品管理菜单
 */
void showMedicineManagement(void);

/**
 * @brief 住院管理菜单
 */
void showHospitalizationManagement(void);

/**
 * @brief 床位管理菜单
 */
void showBedManagement(void);

/**
 * @brief 统计报表菜单
 */
void showStatisticsMenu(void);

/**
 * @brief 查询菜单
 */
void showQueryMenu(void);

/**
 * @brief 消息提示框
 * @param title   标题
 * @param message 提示内容
 */
void showMessage(const char* title, const char* message);

/**
 * @brief 确认对话框
 * @return true = 确认(y), false = 取消(n)
 */
bool confirmDialog(const char* title, const char* message);
```

---

## 3. 控制台版 ui.c

### 3.1 登录页面 — showLoginPage()

**实现思路**：
1. **步骤1**：打印登录菜单界面（用 `┌ ┐ └ │ ║` 画框）
2. **步骤2**：`scanf("%d", &choice)` 接收用户选择
3. **步骤3**：`getchar()` 清除输入缓冲区残留的换行符
4. **步骤4**：`switch(choice)` 分支处理：
   - `case 1` ： 提示输入账号密码，调用 `login()` 函数
   - `case 2` ： 同上（管理员入口，最终也调用 `login()`）
   - `case 3` ： 调用 `registerUser()` 注册新用户
   - `case 0` ： 返回 `LOGIN_EXIT`
5. **步骤5**：返回对应的登录状态码

**参考核心片段**：
```c
int showLoginPage(void) {
    char username[50] = {0};
    char password[50] = {0};

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║       医院综合信息管理系统 - 登录界面           ║\n");
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║  1. 用户登录                                     ║\n");
    printf("║  2. 管理员登录                                   ║\n");
    printf("║  3. 用户注册                                     ║\n");
    printf("║  0. 退出系统                                     ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    printf("请选择: ");

    int choice;
    scanf("%d", &choice);
    getchar();   // ★ 重要！清除 scanf 后的换行符

    switch (choice) {
        case 1:
            printf("\n请输入用户名: "); scanf("%49s", username);
            printf("请输入密码: ");     scanf("%49s", password);
            return login(username, password);  // 调用 login 模块

        case 2:   // 管理员登录，同上
            printf("\n请输入管理员用户名: "); scanf("%49s", username);
            printf("请输入密码: ");            scanf("%49s", password);
            return login(username, password);

        case 3:   // 注册
            printf("\n请输入要注册的用户名: "); scanf("%49s", username);
            printf("请输入密码: ");             scanf("%49s", password);
            registerUser(username, password, 0);  // role=0 普通用户
            return LOGIN_FAILED;

        case 0:
            return LOGIN_EXIT;

        default:
            printf("[ERROR] 无效选择！\n");
            return LOGIN_FAILED;
    }
}
```

---

### 3.2 主菜单 — showMainMenu()

**实现思路**：
1. **步骤1**：打印主菜单框（8个功能选项 + 退出）
2. **步骤2**：`scanf("%d", &module)` 接收模块选择
3. **步骤3**：`switch(module)` 跳转到对应模块菜单函数

**参考片段**：
```c
void showMainMenu(void) {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║       医院综合信息管理系统 - 主菜单              ║\n");
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║  1. 病人信息管理   5. 住院管理                    ║\n");
    printf("║  2. 医生信息管理   6. 床位管理                    ║\n");
    printf("║  3. 挂号管理       7. 统计报表                    ║\n");
    printf("║  4. 药品管理       8. 查询功能                    ║\n");
    printf("║  0. 退出系统                                       ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
}
```

---

### 3.3 病人管理菜单 — showPatientManagement()

**实现思路**：
1. **步骤1**：`while(1)` 循环显示菜单，直到用户选 `0` 返回
2. **步骤2**：`scanf("%d", &choice)` + `getchar()` 接收选择
3. **步骤3**：`switch(choice)` 分支：
   - `case 1`（添加）： 提示输入各字段 → 调用 `addPatient()` → 调用 `rebuildPatientFile()` 写文件
   - `case 2`（删除）： 输入卡号 → `findPatientByCardNo()` 查找 → `delPatient()` 删除 → 写文件
   - `case 3`（修改）： 查找后修改指定字段 → 写文件
   - `case 4`（查找）： 按卡号查找并打印
   - `case 5`（列表）： 调用 `listAllPatients()` 遍历打印
   - `case 0` ： `return` 退出当前函数
4. **步骤4**：每个增删改操作后都需 `rebuildPatientFile()` 同步写回磁盘

**参考核心片段**：
```c
void showPatientManagement(void) {
    int choice;
    char name[50], gender[10], idCard[20], phone[15], cardNo[20];
    Patient* p;

    while (1) {
        printf("\n-------- 病人信息管理 --------\n");
        printf("1. 添加病人   4. 查找病人\n");
        printf("2. 删除病人   5. 显示所有病人\n");
        printf("3. 修改病人信息\n");
        printf("0. 返回上级菜单\n");
        printf("请选择: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:  // 添加
                printf("\n请输入病人信息:\n");
                printf("姓名: ");     scanf("%49s", name);
                printf("性别: ");     scanf("%9s", gender);
                printf("身份证号: "); scanf("%19s", idCard);
                printf("联系电话: "); scanf("%14s", phone);
                addPatient(&g_patientHead, &g_patientTail,
                           name, gender, idCard, phone);
                rebuildPatientFile(g_patientHead);  // ★ 增删改后必须写文件
                printf("[OK] 病人添加成功！\n");
                break;

            case 2:  // 删除
                printf("请输入要删除的病人卡号: ");
                scanf("%19s", cardNo);
                p = findPatientByCardNo(g_patientHead, cardNo);
                if (p) {
                    delPatient(&g_patientHead, &g_patientTail, p);
                    rebuildPatientFile(g_patientHead);
                    printf("[OK] 病人已删除！\n");
                } else {
                    printf("[ERROR] 未找到该病人！\n");
                }
                break;

            case 5:
                listAllPatients(g_patientHead);
                break;

            case 0:
                return;

            default:
                printf("[ERROR] 无效选择！\n");
        }
    }
}
```

---

### 3.4 医生 / 挂号 / 药品 / 住院 / 床位管理

> **提示**：其他五个模块的菜单实现逻辑与"病人管理菜单"完全相同，区别仅在于：
> - 调用的函数名不同（如 `addDoctor`、`addRegistration` …）
> - 输入的字段不同
> - 写回的文件不同（如 `rebuildDoctorFile()`）
>
> 参考 `hospital-management/src/ui.c` 中的完整实现，照此模板套用即可实现。

**通用模式**（以医生为例）：
```c
void showDoctorManagement(void) {
    int choice;
    char name[50], dept[50], schedule[100], empNo[20];
    int maxPatients;
    Doctor* d;

    while (1) {
        printf("\n-------- 医生信息管理 --------\n");
        printf("1. 添加医生   4. 查找医生\n");
        printf("2. 删除医生   5. 显示所有医生\n");
        printf("3. 修改医生信息\n");
        printf("0. 返回上级菜单\n");
        printf("请选择: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:  // 添加
                printf("\n请输入医生信息:\n");
                printf("姓名: ");      scanf("%49s", name);
                printf("科室: ");      scanf("%49s", dept);
                printf("出诊时间: ");  scanf("%99s", schedule);
                printf("每日最大接诊数: "); scanf("%d", &maxPatients);
                addDoctor(&g_doctorHead, &g_doctorTail,
                          name, dept, schedule, maxPatients);
                rebuildDoctorFile(g_doctorHead);  // ★ 写回文件
                printf("[OK] 医生添加成功！\n");
                break;
            // ... case 2~5 与病人模块相同逻辑
            case 0:
                return;
        }
    }
}
```

---

### 3.5 主循环入口 — main()

**实现思路**：
1. **步骤1**：调用 `initUI()`（控制台版空实现）
2. **步骤2**：`while(1)` 外层循环，调用 `showLoginPage()` 等待登录
3. **步骤3**：登录成功（返回 1 或 2）后，进入内层 `while(1)` 主菜单循环
4. **步骤4**：根据用户选择的模块编号，调用对应 `showXxxManagement()` 函数
5. **步骤5**：选 `0` 则调用 `closeUI()` 并退出系统

**参考片段**：
```c
int main(void) {
    initUI();

    while (1) {
        int status = showLoginPage();

        if (status == LOGIN_EXIT) {
            printf("感谢使用，再见！\n");
            break;
        }
        if (status == LOGIN_FAILED) {
            printf("登录失败，请重试。\n");
            continue;
        }

        // 登录成功后进入主循环
        while (1) {
            showMainMenu();
            printf("请选择功能模块: ");
            int module;
            scanf("%d", &module);
            getchar();

            switch (module) {
                case 1: showPatientManagement();         break;
                case 2: showDoctorManagement();          break;
                case 3: showRegistrationManagement();    break;
                case 4: showMedicineManagement();         break;
                case 5: showHospitalizationManagement(); break;
                case 6: showBedManagement();             break;
                case 7: showStatisticsMenu();           break;
                case 8: showQueryMenu();                 break;
                case 0:
                    printf("正在退出系统...\n");
                    closeUI();
                    return 0;
                default:
                    printf("[ERROR] 无效选择！\n");
            }
        }
    }

    closeUI();
    return 0;
}
```

---

## 4. 图形界面版本 main_gui.cpp

### 4.1 框架介绍 — StellarX

本项目图形界面基于 **StellarX** 框架开发（GitHub: `https://github.com/Ysm-04/StellarX`）。

**核心特点**：
- 基于 EasyX 的 C++ GUI 框架，组件化设计
- 使用 `std::unique_ptr` 管理控件生命周期
- 事件绑定使用 Lambda 表达式（`setOnClickListener([&](){ ... })`）
- 入口函数为 `WinMain()`，不使用 `main()`

**常用组件速查**：

| 组件 | 作用 | 对应控制台操作 |
|------|------|--------------|
| `Window` | 主窗口容器 | 无 |
| `Label` | 文字标签 | `printf` 输出文字 |
| `Button` | 按钮 | 菜单选项选择 |
| `TextBox` | 文本输入框 | `scanf` 输入 |
| `Table` | 表格（显示列表） | `listAllXxx()` 遍历打印 |
| `Canvas` | 页面容器 | 每个功能模块专用 Canvas |
| `TabControl` | 左侧导航栏 | 主菜单各项切换 |
| `MessageBox` | 弹出消息框 | `printf` 提示信息 |

### 4.2 窗口和入口 — WinMain()

**实现思路**：
1. **步骤1**：调用 `initSystem()` 从文件加载所有数据到链表
2. **步骤2**：创建 `Window` 主窗口
3. **步骤3**：创建登录页 Canvas（控件 + 背景图）
4. **步骤4**：创建主页面 TabControl（含各模块页面）
5. **步骤5**：绑定登录按钮事件（调用 `login()` 函数）
6. **步骤6**：调用 `mainWindow.draw()` 绘制，`runEventLoop()` 启动循环
7. **步骤7**：窗口关闭后调用 `exitSystem()` 保存并释放

**入口函数签名**：
```cpp
// main_gui.cpp
// 注意：是 WinMain，不是 main！
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    initSystem();  // ★ 从各模块 .c 文件加载数据到链表

    // 创建 1300 x 800 的白色主窗口
    Window mainWindow(1300, 800, nullptr,
                      RGB(255, 255, 255), "医院综合信息管理系统");

    // ... 创建登录 Canvas + 主页面 TabControl ...

    mainWindow.addControl(std::move(log_Canvas));
    mainWindow.addControl(std::move(tabControl));

    mainWindow.draw("image\\lo_bk2.jpg");         // 登录页背景图
    int result = mainWindow.runEventLoop();         // ★ 阻塞在这里

    exitSystem();  // ★ 窗口关闭后自动执行：保存文件 + 释放内存
    return result;
}
```

---

### 4.3 登录页面 — Label + TextBox + Button

**实现思路**：
1. **步骤1**：创建登录 Canvas 容器，设置透明背景
2. **步骤2**：创建 `Label`（标题"欢迎使用" + 账号/密码标签）
3. **步骤3**：创建两个 `TextBox`（账号输入框、密码输入框），设置最大字符数
4. **步骤4**：创建两个 `Button`："用户登录"和"管理员登录"
5. **步骤5**：所有控件加入 Canvas，Canvas 加入 Window

**参考核心片段**：
```cpp
// 创建登录 Canvas 容器
auto log_Canvas = std::make_unique<Canvas>(0, 0, 1300, 800);
Canvas* log_Canvas_ptr = log_Canvas.get();
log_Canvas_ptr->setCanvasfillMode(StellarX::FillMode::Null);

// 标题 Label
auto title_label = std::make_unique<Label>(90, 150, "欢迎使用医院信息管理系统");
title_label->setLayoutMode(StellarX::LayoutMode::AnchorToEdges);
title_label->textStyle.lpszFace = "微软雅黑";
title_label->textStyle.nHeight = 100;   // 字号

// 账号/密码标签
auto acc_label = std::make_unique<Label>(400, 300, "账号");
auto pwd_label = std::make_unique<Label>(400, 400, "密码");
acc_label->textStyle.nHeight = 50;
pwd_label->textStyle.nHeight = 50;

// 账号/密码输入框
auto username_box = std::make_unique<TextBox>(500, 295, 450, 50);
auto password_box = std::make_unique<TextBox>(500, 395, 450, 50);
username_box->setMaxCharLen(20);
password_box->setMaxCharLen(20);

// 登录按钮
auto userLogin_btn  = std::make_unique<Button>(350, 500, 300, 50, "用户登录");
auto adminLogin_btn = std::make_unique<Button>(750, 500, 300, 50, "管理员登录");

// 加入 Canvas
log_Canvas_ptr->addControl(std::move(userLogin_btn));
log_Canvas_ptr->addControl(std::move(adminLogin_btn));
log_Canvas_ptr->addControl(std::move(username_box));
log_Canvas_ptr->addControl(std::move(password_box));
log_Canvas_ptr->addControl(std::move(title_label));
log_Canvas_ptr->addControl(std::move(acc_label));
log_Canvas_ptr->addControl(std::move(pwd_label));
```

---

### 4.4 主页面 — TabControl 导航栏

**实现思路**：
1. **步骤1**：创建 `TabControl`，设置标签在左侧（`TabPlacement::Left`）
2. **步骤2**：为每个模块创建一对：`Button`（标签）+ `Canvas`（页面内容）
3. **步骤3**：每个 Canvas 上放 `Table`（数据表格）+ 若干 `Button`（操作按钮）
4. **步骤4**：调用 `tabControl.add(pair)` 把每个模块页面加进去
5. **步骤5**：初始 `setIsVisible(false)` 隐藏，登录成功后改为 `true`

**参考核心片段**：
```cpp
// 创建 TabControl（左侧标签导航）
auto tabControl = std::make_unique<TabControl>(10, 10, 1280, 780);
TabControl* tabControl_ptr = tabControl.get();
tabControl_ptr->setIsVisible(false);  // 登录前隐藏主页
tabControl_ptr->setTabPlacement(StellarX::TabPlacement::Left);  // ★ 左侧标签
tabControl_ptr->setTabBarHeight(100);  // 每标签高度

// 病人管理页面：Button标签 + Canvas页面内容
auto patientPage = std::make_pair(
    std::make_unique<Button>(0, 0, 100, 100, "病人"),    // 标签
    std::make_unique<Canvas>(0, 0, 1100, 780)             // 内容页
);
patientPage.second->setCanvasfillMode(StellarX::FillMode::Null);

// 病人表格（Table 组件）
auto patientTable = std::make_unique<Table>(50, 50, 1000, 400);
patientTable->setHeaderText(0, "卡号");
patientTable->setHeaderText(1, "姓名");
patientTable->setHeaderText(2, "性别");
patientTable->setHeaderText(3, "电话");
patientTable->setHeaderText(4, "状态");
patientPage.second->addControl(std::move(patientTable));

// 添加病人按钮
auto patAddBtn = std::make_unique<Button>(50, 470, 120, 40, "添加病人");
patAddBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow,
        "添加病人：请使用控制台操作", "提示");
});
patientPage.second->addControl(std::move(patAddBtn));

// 加入 TabControl（参数：标签Button + 页面Canvas）
tabControl_ptr->add(std::move(patientPage));

// 其他模块页面同理（医生、挂号、药品、住院、床位、统计）
```

> **提示**：医生页面在 TabControl 中的标签位置参数 `Button(0, 100, 100, 100, "医生")`，`y=100` 与病人标签 `y=0` 错开，保证各标签不重叠，后续模块依次向下偏移。

---

### 4.5 登录事件绑定

**实现思路**：
1. **步骤1**：调用 `textBox->getText()` 获取用户输入（返回 `std::string`）
2. **步骤2**：空值检查——若账号或密码为空，调用 `setTextBoxBk(RGB(255,0,0))` 变红提示
3. **步骤3**：调用 `login((char*)username.c_str(), (char*)password.c_str())`
4. **步骤4**：登录成功 — 隐藏登录 Canvas (`setIsVisible(false)`)、显示主页 (`setIsVisible(true)`)、切换背景图
5. **步骤5**：失败 — `MessageBox::showModal()` 弹出提示

**参考片段**：
```cpp
// 用户登录按钮事件
userLogin_btn_ptr->setOnClickListener([&]() {
    std::string username = username_box_ptr->getText();
    std::string password = password_box_ptr->getText();

    // 空检查
    if (username.empty() || password.empty()) {
        if (username.empty()) username_box_ptr->setTextBoxBk(RGB(255, 0, 0));
        if (password.empty()) password_box_ptr->setTextBoxBk(RGB(255, 0, 0));
        StellarX::MessageBox::showModal(mainWindow, "账号或密码不能为空！", "提示");
        return;
    }

    // 调用登录函数（来自 login.c）
    int result = login((char*)username.c_str(), (char*)password.c_str());

    if (result == LOGIN_SUCCESS_USER || result == LOGIN_SUCCESS_ADMIN) {
        // 切换到主页面
        log_Canvas_ptr->setIsVisible(false);
        tabControl_ptr->setIsVisible(true);
        mainWindow.draw("image\\bk1.jpg");         // 主页面背景图
        refreshPatientTable(patientTable_ptr);     // 刷新数据
        refreshBedTable(bedTable_ptr);
    } else {
        StellarX::MessageBox::showModal(mainWindow, "登录失败", "提示");
    }
});
```

---

### 4.6 表格刷新函数

**实现思路**：
1. **步骤1**：调用 `table->clear()` 清空旧数据
2. **步骤2**：遍历对应链表（`g_patientHead`、`g_doctorHead` …）
3. **步骤3**：每遍历一个节点，调用 `setCellText(row, col, value)` 填充对应格子
4. **步骤4**：数字字段用 `sprintf()` 转字符串再填入

**参考（病人表格）**：
```cpp
void refreshPatientTable(Table* table) {
    if (table == nullptr) return;
    table->clear();  // ★ 先清空旧数据
    int row = 0;

    Patient* p = g_patientHead;
    while (p != nullptr && row < 100) {
        table->setCellText(row, 0, p->cardNo);
        table->setCellText(row, 1, p->name);
        table->setCellText(row, 2, p->gender);
        table->setCellText(row, 3, p->phone);
        table->setCellText(row, 4, p->isActive ? "住院中" : "正常");
        p = p->next;
        row++;
    }
}
```

---

### 4.7 统计功能

**实现思路**：
1. **步骤1**：遍历对应链表，累加统计量（人数、费用、数量）
2. **步骤2**：用 `sprintf(msg, ...)` 拼接统计结果字符串
3. **步骤3**：`MessageBox::showModal(window, msg, "标题")` 弹出结果

**参考（医生业务量统计）**：
```cpp
auto statDocBtn = std::make_unique<Button>(50, 50, 200, 50, "医生业务量统计");
statDocBtn->setOnClickListener([&mainWindow, &g_doctorHead]() {
    char msg[1000] = "医生业务量统计：\n\n";
    char buf[100];
    Doctor* d = g_doctorHead;
    while (d != nullptr) {
        sprintf(buf, "%s(%s): %d/%d人\n",
                d->name, d->dept, d->currentPatients, d->maxPatients);
        strcat(msg, buf);
        d = d->next;
    }
    StellarX::MessageBox::showModal(mainWindow, msg, "医生业务量");
});
```

---

### 4.8 系统初始化和退出

```cpp
// main_gui.cpp 中 初始化：从文件加载所有数据
void initSystem() {
    ensureDirectoriesExist();  // 确保 data/ 目录存在

    // 从 data/*.txt 读入链表
    buildPatientChain(&g_patientHead, &g_patientTail);
    buildDoctorChain(&g_doctorHead, &g_doctorTail);
    buildRegistrationChain(&g_regHead, &g_regTail);
    buildMedicineChain(&g_medHead, &g_medTail);
    buildPurchaseChain(&g_purHead, &g_purTail);
    buildHospitalizationChain(&g_hosHead, &g_hosTail);
    buildBedChain(&g_bedHead, &g_bedTail);
}

// main_gui.cpp 中 退出：保存 + 释放内存
void exitSystem() {
    // 保存链表数据到文件
    rebuildPatientFile(g_patientHead);
    rebuildDoctorFile(g_doctorHead);
    rebuildRegistrationFile(g_regHead);
    rebuildMedicineFile(g_medHead);
    rebuildPurchaseFile(g_purHead);
    rebuildHospitalizationFile(g_hosHead);
    rebuildBedFile(g_bedHead);

    // 释放链表内存
    freePatientChain(g_patientHead);
    freeDoctorChain(g_doctorHead);
    freeRegistrationChain(g_regHead);
    freeMedicineChain(g_medHead);
    freePurchaseChain(g_purHead);
    freeHospitalizationChain(g_hosHead);
    freeBedChain(g_bedHead);
}
```

---

## 5. 开发注意事项

### 5.1 控制台版 vs 图形版选择

| 场景 | 推荐版本 |
|------|---------|
| 学习数据结构（链表增删改查） | 控制台版 `ui.c` |
| 快速验证功能逻辑 | 控制台版 |
| 实际项目展示 | 图形版 `main_gui.cpp` |

### 5.2 数据一致性原理

> **核心原则：增删改后必须同步写回文件**

```
内存链表（g_xxxHead）
      ↑
      ↑  rebuildXxxFile(g_xxxHead)   ↑ 操作后必须调用
      ↑
磁盘文件（data/xxx.txt）
```

每次 `addXxx()` / `delXxx()` / `modifyXxx()` 之后，必须调用 `rebuildXxxFile()` 将修改写回磁盘，否则程序重启后数据丢失。

### 5.3 输入安全

- `scanf("%s", buf)` 应改为 `scanf("%49s", buf)`，防止缓冲区溢出
- `scanf()` 后加 `getchar()` 清除换行符，避免影响下一次输入
- 密码输入用 `getch()` 实时读取字符不回显

### 5.4 GUI 调试建议

1. 先确保控制台版功能正确，再迁移到 GUI
2. GUI 中复杂操作（增删改）暂时跳转到控制台完成，GUI 仅做展示
3. `WinMain` 中可以加 `printf` 语句，通过 Visual Studio 的输出窗口查看调试信息

---

## 6. 下一步学习

1. [12-文件IO_.md](./12-文件IO_.md) — 数据如何持久化存储
2. [05-病人模块_.md](./05-病人模块_.md) — 病人模块数据操作
3. [10-登录模块_.md](./10-登录模块_.md) — 登录与权限控制

---

**文档版本**：v1.0（学习版 · 实现思路引导 · 参考 `hospital-management` 源码）