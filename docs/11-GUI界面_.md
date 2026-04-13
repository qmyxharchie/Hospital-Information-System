 GUI界面模块 - 实战版

> 本文档为**实战版**，代码部分基于 `hospital-management` 参考项目中实际可运行的代码编写。
> 源码参考路径：`hospital-management/src/ui.c`、`hospital-management/include/ui.h`、`hospital-management/src/main_gui.cpp`

---

## 1. 模块概述

### 1.1 功能描述

GUI界面模块负责整个医院信息管理系统的图形界面交互，提供两套UI：

- **控制台版本**（`ui.c`）：命令行菜单驱动，适合学习和调试
- **图形界面版本**（`main_gui.cpp`）：基于 StellarX 框架，适合实际使用

### 1.2 文件对应关系

| 文件 | 说明 | 负责成员 |
|------|------|---------|
| `include/ui.h` | 界面函数声明、颜色/尺寸宏定义 | 参考 `hospital-management/include/ui.h` |
| `src/ui.c` | 控制台版本实现 | 参考 `hospital-management/src/ui.c` |
| `src/main_gui.cpp` | 图形界面版本实现 | 参考 `hospital-management/src/main_gui.cpp` |

### 1.3 开发依赖

- 控制台版本：C 标准库（`<stdio.h>`、`<stdlib.h>`、`<conio.h>`）
- 图形界面版本：StellarX 框架（`include/StellarX/`），框架地址：`https://github.com/Ysm-04/StellarX`

---

## 2. ui.h 头文件

在 `include/ui.h` 中定义界面相关的声明、颜色常量和尺寸常量。

### 2.1 颜色定义

```c
// ui.h
// 所有颜色使用 RGB() 宏，参数为 R/G/B 值（0~255）

#define COLOR_BG         RGB(240, 248, 255)   // 背景色：AliceBlue
#define COLOR_TITLE      RGB(70, 130, 180)     // 标题色：SteelBlue
#define COLOR_TEXT       RGB(50, 50, 50)       // 文本色：深灰
#define COLOR_BUTTON     RGB(100, 149, 237)    // 按钮色：CornflowerBlue
#define COLOR_BUTTON_HOVER RGB(70, 130, 180)   // 按钮悬停色
#define COLOR_SUCCESS    RGB(34, 139, 34)     // 成功色：Green
#define COLOR_WARNING    RGB(255, 140, 0)      // 警告色：DarkOrange
#define COLOR_ERROR      RGB(220, 20, 60)      // 错误色：Crimson
```

### 2.2 尺寸定义

```c
// ui.h
#define WINDOW_WIDTH   1000   // 窗口默认宽度
#define WINDOW_HEIGHT  700    // 窗口默认高度
#define BUTTON_WIDTH   200    // 按钮宽度
#define BUTTON_HEIGHT  50     // 按钮高度
#define INPUT_WIDTH    300    // 输入框宽度
#define INPUT_HEIGHT   40     // 输入框高度
```

### 2.3 函数声明

```c
// ui.h
// 全局链表头尾指针声明（由各模块 .c 提供）
extern Patient*    g_patientHead;
extern Patient*    g_patientTail;
extern Doctor*     g_doctorHead;
extern Doctor*     g_doctorTail;
extern Registration* g_regHead;
extern Registration* g_regTail;
extern Medicine*   g_medHead;
extern Medicine*   g_medTail;
extern Purchase*   g_purHead;
extern Purchase*   g_purTail;
extern Hospitalization* g_hosHead;
extern Hospitalization* g_hosTail;
extern Bed*        g_bedHead;
extern Bed*        g_bedTail;

/**
 * @brief 初始化图形界面
 */
void initUI(void);

/**
 * @brief 关闭图形界面（释放资源）
 */
void closeUI(void);

/**
 * @brief 显示登录页面（控制台版）
 * @return 登录状态码
 *         LOGIN_FAILED       = 0  （登录失败）
 *         LOGIN_SUCCESS_USER = 1  （普通用户登录成功）
 *         LOGIN_SUCCESS_ADMIN = 2 （管理员登录成功）
 *         LOGIN_EXIT         = 3  （退出系统）
 */
int showLoginPage(void);

/**
 * @brief 显示主菜单（控制台版）
 */
void showMainMenu(void);

/**
 * @brief 显示病人管理界面（控制台版）
 */
void showPatientManagement(void);

/**
 * @brief 显示医生管理界面（控制台版）
 */
void showDoctorManagement(void);

/**
 * @brief 显示挂号管理界面（控制台版）
 */
void showRegistrationManagement(void);

/**
 * @brief 显示药品管理界面（控制台版）
 */
void showMedicineManagement(void);

/**
 * @brief 显示住院管理界面（控制台版）
 */
void showHospitalizationManagement(void);

/**
 * @brief 显示床位管理界面（控制台版）
 */
void showBedManagement(void);

/**
 * @brief 显示统计报表界面（控制台版）
 */
void showStatisticsMenu(void);

/**
 * @brief 显示查询功能界面（控制台版）
 */
void showQueryMenu(void);

/**
 * @brief 输入框组件（控制台版，GUI 中用 TextBox 替代）
 * @param x, y        坐标位置（GUI版使用）
 * @param title       提示文字
 * @param input       接收用户输入（输出参数）
 * @param maxLen      最大输入长度
 * @param isPassword  是否密码模式（显示 * 代替字符）
 * @return true=确认, false=取消
 */
bool inputBox(int x, int y, const char* title, char* input, int maxLen, bool isPassword);

/**
 * @brief 消息提示框（控制台版，GUI 中用 MessageBox 替代）
 * @param title   标题
 * @param message 提示内容
 */
void showMessage(const char* title, const char* message);

/**
 * @brief 确认对话框（控制台版，GUI 中用 MessageBox::YesNo 替代）
 * @param title   标题
 * @param message 确认提示
 * @return true=确认(y), false=取消(n)
 */
bool confirmDialog(const char* title, const char* message);
```

---

## 3. 控制台版本 ui.c

参考 `hospital-management/src/ui.c`，核心思路是"用 `printf` 打印菜单，用 `scanf` 接收输入，调用对应模块函数完成操作，操作后同步写回文件"。

### 3.1 登录页面

```c
// ui.c - showLoginPage()
int showLoginPage(void) {
    char username[50] = {0};
    char password[50] = {0};

    printf("\n");
    printf("╔═══════════════════════════════════════════════════╗\n");
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
    getchar();  // 清除输入缓冲区

    switch (choice) {
        case 1:  // 用户登录
            printf("\n请输入用户名: ");
            scanf("%49s", username);
            printf("请输入密码: ");
            scanf("%49s", password);
            return login(username, password);  // 调用 login 模块

        case 2:  // 管理员登录
            printf("\n请输入管理员用户名: ");
            scanf("%49s", username);
            printf("请输入密码: ");
            scanf("%49s", password);
            return login(username, password);    // 调用 login 模块

        case 3:  // 注册
            printf("\n请输入要注册的用户名: ");
            scanf("%49s", username);
            printf("请输入密码: ");
            scanf("%49s", password);
            registerUser(username, password, 0); // role=0 普通用户
            return LOGIN_FAILED;

        case 0:
            return LOGIN_EXIT;

        default:
            printf("[ERROR] 无效选择！\n");
            return LOGIN_FAILED;
    }
}
```

### 3.2 主菜单

```c
// ui.c - showMainMenu()
void showMainMenu(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║       医院综合信息管理系统 - 主菜单              ║\n");
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║  1. 病人信息管理                                   ║\n");
    printf("║  2. 医生信息管理                                   ║\n");
    printf("║  3. 挂号管理                                       ║\n");
    printf("║  4. 药品管理                                       ║\n");
    printf("║  5. 住院管理                                       ║\n");
    printf("║  6. 床位管理                                       ║\n");
    printf("║  7. 统计报表                                       ║\n");
    printf("║  8. 查询功能                                       ║\n");
    printf("║  0. 退出系统                                       ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
}
```

### 3.3 病人管理菜单（带完整增删改查流程）

```c
// ui.c - showPatientManagement()
// 参考 hospital-management/src/ui.c 中的实现

void showPatientManagement(void) {
    int choice;
    char name[50], gender[10], idCard[20], phone[15], cardNo[20];
    Patient* p;

    while (1) {
        printf("\n-------- 病人信息管理 --------\n");
        printf("1. 添加病人\n");
        printf("2. 删除病人\n");
        printf("3. 修改病人信息\n");
        printf("4. 查找病人\n");
        printf("5. 显示所有病人\n");
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
                rebuildPatientFile(g_patientHead);  // 同步写回文件
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

            case 3:  // 修改
                printf("请输入病人卡号: ");
                scanf("%19s", cardNo);
                p = findPatientByCardNo(g_patientHead, cardNo);
                if (p) {
                    printf("请输入新电话: ");
                    scanf("%14s", phone);
                    modifyPatient(p, phone);
                    rebuildPatientFile(g_patientHead);
                    printf("[OK] 病人信息已修改！\n");
                } else {
                    printf("[ERROR] 未找到该病人！\n");
                }
                break;

            case 4:  // 查找
                printf("请输入病人卡号: ");
                scanf("%19s", cardNo);
                p = findPatientByCardNo(g_patientHead, cardNo);
                if (p) {
                    printf("\n卡号: %s\n", p->cardNo);
                    printf("姓名: %s\n", p->name);
                    printf("性别: %s\n", p->gender);
                    printf("身份证: %s\n", p->idCard);
                    printf("电话: %s\n", p->phone);
                    printf("状态: %s\n", p->isActive ? "住院中" : "未住院");
                } else {
                    printf("[ERROR] 未找到该病人！\n");
                }
                break;

            case 5:  // 列表
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

### 3.4 医生管理菜单

```c
// ui.c - showDoctorManagement()
void showDoctorManagement(void) {
    int choice;
    char name[50], dept[50], schedule[100], empNo[20];
    int maxPatients;
    Doctor* d;

    while (1) {
        printf("\n-------- 医生信息管理 --------\n");
        printf("1. 添加医生\n");
        printf("2. 删除医生\n");
        printf("3. 修改医生信息\n");
        printf("4. 查找医生\n");
        printf("5. 显示所有医生\n");
        printf("0. 返回上级菜单\n");
        printf("请选择: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\n请输入医生信息:\n");
                printf("姓名: ");      scanf("%49s", name);
                printf("科室: ");      scanf("%49s", dept);
                printf("出诊时间: ");  scanf("%99s", schedule);
                printf("每日最大接诊数: "); scanf("%d", &maxPatients);
                addDoctor(&g_doctorHead, &g_doctorTail,
                          name, dept, schedule, maxPatients);
                rebuildDoctorFile(g_doctorHead);
                printf("[OK] 医生添加成功！\n");
                break;

            case 2:
                printf("请输入要删除的医生工号: ");
                scanf("%19s", empNo);
                d = findDoctorByEmpNo(g_doctorHead, empNo);
                if (d) {
                    delDoctor(&g_doctorHead, &g_doctorTail, d);
                    rebuildDoctorFile(g_doctorHead);
                    printf("[OK] 医生已删除！\n");
                } else {
                    printf("[ERROR] 未找到该医生！\n");
                }
                break;

            case 3:
                printf("请输入医生工号: ");
                scanf("%19s", empNo);
                d = findDoctorByEmpNo(g_doctorHead, empNo);
                if (d) {
                    printf("出诊时间: ");      scanf("%99s", schedule);
                    printf("最大接诊数: ");    scanf("%d", &maxPatients);
                    modifyDoctor(d, schedule, maxPatients);
                    rebuildDoctorFile(g_doctorHead);
                    printf("[OK] 医生信息已修改！\n");
                } else {
                    printf("[ERROR] 未找到该医生！\n");
                }
                break;

            case 4:
                printf("请输入医生工号: ");
                scanf("%19s", empNo);
                d = findDoctorByEmpNo(g_doctorHead, empNo);
                if (d) {
                    printf("\n工号: %s\n", d->empNo);
                    printf("姓名: %s\n", d->name);
                    printf("科室: %s\n", d->dept);
                    printf("出诊: %s\n", d->schedule);
                    printf("已挂号/上限: %d/%d\n",
                           d->currentPatients, d->maxPatients);
                } else {
                    printf("[ERROR] 未找到该医生！\n");
                }
                break;

            case 5:
                listAllDoctors(g_doctorHead);
                break;

            case 0:
                return;

            default:
                printf("[ERROR] 无效选择！\n");
        }
    }
}
```

### 3.5 挂号管理菜单

```c
// ui.c - showRegistrationManagement()
void showRegistrationManagement(void) {
    int choice;
    char patientCardNo[20], patientName[50], doctorEmpNo[20];
    char doctorName[50], dept[50], regNo[20];
    Registration* r;

    while (1) {
        printf("\n-------- 挂号管理 --------\n");
        printf("1. 挂号\n");
        printf("2. 取消挂号\n");
        printf("3. 完成就诊\n");
        printf("4. 查看挂号记录\n");
        printf("0. 返回上级菜单\n");
        printf("请选择: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\n请输入挂号信息:\n");
                printf("病人卡号: ");   scanf("%19s", patientCardNo);
                printf("病人姓名: ");   scanf("%49s", patientName);
                printf("医生工号: ");   scanf("%19s", doctorEmpNo);
                printf("医生姓名: ");   scanf("%49s", doctorName);
                printf("科室: ");      scanf("%49s", dept);
                addRegistration(&g_regHead, &g_regTail,
                    patientCardNo, patientName,
                    doctorEmpNo, doctorName, dept);
                rebuildRegistrationFile(g_regHead);
                rebuildDoctorFile(g_doctorHead);
                printf("[OK] 挂号成功！\n");
                break;

            case 2:
                printf("请输入要取消的挂号单号: ");
                scanf("%19s", regNo);
                r = findRegistrationByNo(g_regHead, regNo);
                if (r) {
                    cancelRegistration(&g_regHead, &g_regTail, r);
                    rebuildRegistrationFile(g_regHead);
                    printf("[OK] 挂号已取消！\n");
                } else {
                    printf("[ERROR] 未找到该挂号记录！\n");
                }
                break;

            case 3:
                printf("请输入要完成的挂号单号: ");
                scanf("%19s", regNo);
                r = findRegistrationByNo(g_regHead, regNo);
                if (r) {
                    completeRegistration(r);
                    rebuildRegistrationFile(g_regHead);
                    printf("[OK] 就诊已完成！\n");
                } else {
                    printf("[ERROR] 未找到该挂号记录！\n");
                }
                break;

            case 4:
                listAllRegistrations(g_regHead);
                break;

            case 0:
                return;

            default:
                printf("[ERROR] 无效选择！\n");
        }
    }
}
```

### 3.6 药品管理菜单

```c
// ui.c - showMedicineManagement()
void showMedicineManagement(void) {
    int choice;
    char genericName[50], brandName[50], spec[50], medNo[20];
    double price;
    int stock, minStock, quantity;
    Medicine* m;

    while (1) {
        printf("\n-------- 药品管理 --------\n");
        printf("1. 添加药品\n");
        printf("2. 删除药品\n");
        printf("3. 修改药品信息\n");
        printf("4. 补充库存\n");
        printf("5. 库存预警\n");
        printf("6. 购药\n");
        printf("7. 显示所有药品\n");
        printf("0. 返回上级菜单\n");
        printf("请选择: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\n请输入药品信息:\n");
                printf("通用名: ");    scanf("%49s", genericName);
                printf("商品名: ");    scanf("%49s", brandName);
                printf("规格: ");     scanf("%49s", spec);
                printf("单价: ");     scanf("%lf", &price);
                printf("库存: ");     scanf("%d", &stock);
                printf("最低库存预警值: "); scanf("%d", &minStock);
                addMedicine(&g_medHead, &g_medTail,
                    genericName, brandName, spec, price, stock, minStock);
                rebuildMedicineFile(g_medHead);
                printf("[OK] 药品添加成功！\n");
                break;

            case 2:
                printf("请输入要删除的药品编号: ");
                scanf("%19s", medNo);
                m = findMedicineByNo(g_medHead, medNo);
                if (m) {
                    delMedicine(&g_medHead, &g_medTail, m);
                    rebuildMedicineFile(g_medHead);
                    printf("[OK] 药品已删除！\n");
                } else {
                    printf("[ERROR] 未找到该药品！\n");
                }
                break;

            case 3:
                printf("请输入药品编号: ");
                scanf("%19s", medNo);
                m = findMedicineByNo(g_medHead, medNo);
                if (m) {
                    printf("单价: ");     scanf("%lf", &price);
                    printf("库存: ");     scanf("%d", &stock);
                    printf("最低库存: "); scanf("%d", &minStock);
                    modifyMedicine(m, price, stock, minStock);
                    rebuildMedicineFile(g_medHead);
                    printf("[OK] 药品信息已修改！\n");
                } else {
                    printf("[ERROR] 未找到该药品！\n");
                }
                break;

            case 4:
                printf("请输入药品编号: ");
                scanf("%19s", medNo);
                m = findMedicineByNo(g_medHead, medNo);
                if (m) {
                    printf("补充数量: "); scanf("%d", &quantity);
                    replenishStock(m, quantity);
                    rebuildMedicineFile(g_medHead);
                    printf("[OK] 库存已补充！\n");
                } else {
                    printf("[ERROR] 未找到该药品！\n");
                }
                break;

            case 5:
                checkLowStock(g_medHead);
                break;

            case 6:
                {
                    char patientCardNo[20];
                    printf("病人卡号: "); scanf("%19s", patientCardNo);
                    printf("药品编号: "); scanf("%19s", medNo);
                    printf("数量: ");     scanf("%d", &quantity);
                    addPurchase(&g_purHead, &g_purTail,
                                patientCardNo, medNo, quantity);
                    rebuildPurchaseFile(g_purHead);
                    rebuildMedicineFile(g_medHead);
                    printf("[OK] 购药成功！\n");
                }
                break;

            case 7:
                listAllMedicines(g_medHead);
                break;

            case 0:
                return;

            default:
                printf("[ERROR] 无效选择！\n");
        }
    }
}
```

### 3.7 住院管理菜单

```c
// ui.c - showHospitalizationManagement()
void showHospitalizationManagement(void) {
    int choice;
    char patientCardNo[20], patientName[50], bedNo[20];
    char recordNo[20], ward[30];
    double prepay, totalCost;
    Hospitalization* h;

    while (1) {
        printf("\n-------- 住院管理 --------\n");
        printf("1. 入院登记\n");
        printf("2. 出院结算\n");
        printf("3. 查看住院记录\n");
        printf("0. 返回上级菜单\n");
        printf("请选择: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:  // 入院
                printf("\n请输入入院信息:\n");
                printf("病人卡号: ");  scanf("%19s", patientCardNo);
                printf("病人姓名: ");  scanf("%49s", patientName);
                printf("首选病区: ");  scanf("%29s", ward);
                printf("预交金额: ");  scanf("%lf", &prepay);

                // 自动分配空闲床位
                Bed* bed = findAvailableBeds(g_bedHead);
                if (bed != NULL) {
                    strcpy(bedNo, bed->bedNo);
                    allocateBed(g_bedHead, patientCardNo, patientName, ward);
                    addHospitalization(&g_hosHead, &g_hosTail,
                                       patientCardNo, patientName, bedNo, prepay);
                    rebuildHospitalizationFile(g_hosHead);
                    rebuildBedFile(g_bedHead);

                    // 更新病人住院状态
                    Patient* p = findPatientByCardNo(g_patientHead, patientCardNo);
                    if (p) {
                        p->isActive = 1;
                        rebuildPatientFile(g_patientHead);
                    }
                    printf("[OK] 入院成功！分配床位: %s\n", bedNo);
                } else {
                    printf("[ERROR] 暂无空闲床位！\n");
                }
                break;

            case 2:  // 出院
                printf("请输入住院单号: ");
                scanf("%19s", recordNo);
                h = findHospitalizationByNo(g_hosHead, recordNo);
                if (h) {
                    printf("总费用: ");
                    scanf("%lf", &totalCost);
                    dischargePatient(h, totalCost);
                    rebuildHospitalizationFile(g_hosHead);

                    // 释放床位
                    Bed* bed = findBedByNo(g_bedHead, h->bedNo);
                    if (bed) {
                        freeBed(bed);
                        rebuildBedFile(g_bedHead);
                    }

                    // 更新病人状态
                    Patient* p = findPatientByCardNo(g_patientHead, h->patientCardNo);
                    if (p) {
                        p->isActive = 0;
                        rebuildPatientFile(g_patientHead);
                    }
                    printf("[OK] 出院结算完成！\n");
                } else {
                    printf("[ERROR] 未找到该住院记录！\n");
                }
                break;

            case 3:
                listAllHospitalizations(g_hosHead);
                break;

            case 0:
                return;

            default:
                printf("[ERROR] 无效选择！\n");
        }
    }
}
```

### 3.8 床位管理菜单

```c
// ui.c - showBedManagement()
void showBedManagement(void) {
    int choice;
    char ward[30], bedNo[20];
    int total, occupied;
    Bed* b;

    while (1) {
        printf("\n-------- 床位管理 --------\n");
        printf("1. 添加床位\n");
        printf("2. 删除床位\n");
        printf("3. 查看病区床位\n");
        printf("4. 显示所有床位\n");
        printf("0. 返回上级菜单\n");
        printf("请选择: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\n请输入床位信息:\n");
                printf("病区: ");  scanf("%29s", ward);
                printf("床位号: "); scanf("%19s", bedNo);
                addBed(&g_bedHead, &g_bedTail, ward, bedNo);
                rebuildBedFile(g_bedHead);
                printf("[OK] 床位添加成功！\n");
                break;

            case 2:
                printf("请输入要删除的床位号: ");
                scanf("%19s", bedNo);
                b = findBedByNo(g_bedHead, bedNo);
                if (b) {
                    delBed(&g_bedHead, &g_bedTail, b);
                    rebuildBedFile(g_bedHead);
                    printf("[OK] 床位已删除！\n");
                } else {
                    printf("[ERROR] 未找到该床位！\n");
                }
                break;

            case 3:
                printf("请输入病区: ");
                scanf("%29s", ward);
                getWardStats(g_bedHead, ward, &total, &occupied);
                printf("\n病区: %s\n", ward);
                printf("总床位: %d\n", total);
                printf("已占用: %d\n", occupied);
                printf("空闲: %d\n", total - occupied);
                break;

            case 4:
                listAllBeds(g_bedHead);
                break;

            case 0:
                return;

            default:
                printf("[ERROR] 无效选择！\n");
        }
    }
}
```

### 3.9 通用对话框函数

```c
// ui.c - 通用对话框

void showMessage(const char* title, const char* message) {
    printf("\n[%s] %s\n", title, message);
}

bool confirmDialog(const char* title, const char* message) {
    printf("\n[%s] %s (y/n): ", title, message);
    char c = getchar();
    getchar();  // 清除回车
    return (c == 'y' || c == 'Y');
}

// 输入框实现
bool inputBox(int x, int y, const char* title, char* input, int maxLen, bool isPassword) {
    printf("%s: ", title);
    if (isPassword) {
        int i = 0;
        char c;
        while ((c = getch()) != '\r' && i < maxLen - 1) {
            if (c == '\b' && i > 0) {
                printf("\b \b");
                i--;
            } else if (c != '\b') {
                printf("*");
                input[i++] = c;
            }
        }
        input[i] = '\0';
        printf("\n");
    } else {
        scanf("%s", input);
    }
    return true;
}

void initUI(void)   { /* 控制台版本无需额外初始化 */ }
void closeUI(void)  { /* 控制台版本无需额外清理 */ }
```

---

## 4. 图形界面版本 main_gui.cpp

参考 `hospital-management/src/main_gui.cpp`，使用 **StellarX** 框架实现。

### 4.1 入口函数

> **注意**：GUI 版本入口为 `WinMain()`，不是 `main()`

```cpp
// main_gui.cpp
#include <iostream>
#include <memory>
#include <functional>
#include "StellarX.h"    // StellarX 主头文件
#include "patient.h"
#include "doctor.h"
// ... 其他模块头文件

// 声明全局链表指针（与 ui.c 中相同）
Patient*         g_patientHead = nullptr;
Patient*         g_patientTail = nullptr;
Doctor*          g_doctorHead  = nullptr;
Doctor*          g_doctorTail  = nullptr;
// ... 其他模块

// 系统初始化和退出函数（声明）
void initSystem();
void exitSystem();
void refreshPatientTable(Table* table);
void refreshDoctorTable(Table* table);
void refreshMedicineTable(Table* table);
void refreshBedTable(Table* table);

// 主入口
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    initSystem();  // 1. 从文件加载数据到链表

    // 2. 创建主窗口（1300 x 800，白色背景）
    Window mainWindow(1300, 800, nullptr,
                      RGB(255, 255, 255), "医院综合信息管理系统");

    // === 登录页面（Canvas 容器，初始可见）===
    auto log_Canvas = std::make_unique<Canvas>(0, 0, 1300, 800);
    Canvas* log_Canvas_ptr = log_Canvas.get();
    log_Canvas_ptr->setCanvasfillMode(StellarX::FillMode::Null);
    log_Canvas_ptr->setShape(StellarX::ControlShape::B_RECTANGLE);
    log_Canvas_ptr->setLayoutMode(StellarX::LayoutMode::AnchorToEdges);

    // 登录标签
    std::unique_ptr<Label> logIn_label[3];
    logIn_label[0] = std::make_unique<Label>(90, 150, "欢迎使用医院信息管理系统");
    logIn_label[1] = std::make_unique<Label>(400, 300, "账号");
    logIn_label[2] = std::make_unique<Label>(400, 400, "密码");
    for (auto& log : logIn_label) {
        log->setTextdisap(true);
        log->setLayoutMode(StellarX::LayoutMode::AnchorToEdges);
        log->textStyle.lpszFace = "微软雅黑";
    }
    logIn_label[0]->textStyle.nHeight = 100;
    logIn_label[1]->textStyle.nHeight = 50;
    logIn_label[2]->textStyle.nHeight = 50;

    // 登录输入框
    std::unique_ptr<TextBox> logIn_textBox[2];
    TextBox* logIn_textBox_ptr[2];
    logIn_textBox[0] = std::make_unique<TextBox>(500, 295, 450, 50);
    logIn_textBox[1] = std::make_unique<TextBox>(500, 395, 450, 50);
    logIn_textBox_ptr[0] = logIn_textBox[0].get();
    logIn_textBox_ptr[1] = logIn_textBox[1].get();
    for (auto& tb : logIn_textBox) {
        tb->setLayoutMode(StellarX::LayoutMode::AnchorToEdges);
        tb->setMaxCharLen(20);
    }

    // 登录按钮
    std::unique_ptr<Button> logIn_Button[2];
    Button* logIn_Button_ptr[2];
    logIn_Button[0] = std::make_unique<Button>(350, 500, 300, 50, "用户登录");
    logIn_Button[1] = std::make_unique<Button>(750, 500, 300, 50, "管理员登录");
    logIn_Button_ptr[0] = logIn_Button[0].get();
    logIn_Button_ptr[1] = logIn_Button[1].get();
    for (auto& b : logIn_Button)
        b->setLayoutMode(StellarX::LayoutMode::AnchorToEdges);

    // 控件加入登录 Canvas
    for (auto& b  : logIn_Button)   log_Canvas_ptr->addControl(std::move(b));
    for (auto& tb : logIn_textBox)  log_Canvas_ptr->addControl(std::move(tb));
    for (auto& la : logIn_label)     log_Canvas_ptr->addControl(std::move(la));

    // === 主页面：TabControl 导航 ===
    auto tabControl = std::make_unique<TabControl>(10, 10, 1280, 780);
    TabControl* tabControl_ptr = tabControl.get();
    tabControl_ptr->setIsVisible(false);  // 登录成功前隐藏
    tabControl_ptr->setCanvasfillMode(StellarX::FillMode::Null);
    tabControl_ptr->setShape(StellarX::ControlShape::ROUND_RECTANGLE);
    tabControl_ptr->setTabPlacement(StellarX::TabPlacement::Left);   // 左侧标签
    tabControl_ptr->setTabBarHeight(100);

    // --- 各模块页面加入 tabControl ---
    // 详见 4.2 ~ 4.8 节

    // === 登录按钮事件 ===
    // 用户登录
    logIn_Button_ptr[0]->setOnClickListener([&]() {
        std::string username = logIn_textBox_ptr[0]->getText();
        std::string password = logIn_textBox_ptr[1]->getText();

        if (username.empty() || password.empty()) {
            if (username.empty()) logIn_textBox_ptr[0]->setTextBoxBk(RGB(255,0,0));
            if (password.empty()) logIn_textBox_ptr[1]->setTextBoxBk(RGB(255,0,0));
            StellarX::MessageBox::showModal(mainWindow, "账号或密码不能为空！", "提示");
            return;
        }

        int result = login((char*)username.c_str(), (char*)password.c_str());
        if (result == LOGIN_SUCCESS_USER || result == LOGIN_SUCCESS_ADMIN) {
            log_Canvas_ptr->setIsVisible(false);
            tabControl_ptr->setIsVisible(true);
            mainWindow.draw("image\\bk1.jpg");
            refreshPatientTable(patientTable_ptr);
            refreshBedTable(bedTable_ptr);
        } else {
            StellarX::MessageBox::showModal(mainWindow, "登录失败！", "提示");
        }
    });

    // 管理员登录
    logIn_Button_ptr[1]->setOnClickListener([&]() {
        std::string username = logIn_textBox_ptr[0]->getText();
        std::string password = logIn_textBox_ptr[1]->getText();

        if (username.empty() || password.empty()) {
            if (username.empty()) logIn_textBox_ptr[0]->setTextBoxBk(RGB(255,0,0));
            if (password.empty()) logIn_textBox_ptr[1]->setTextBoxBk(RGB(255,0,0));
            StellarX::MessageBox::showModal(mainWindow, "账号或密码不能为空！", "提示");
            return;
        }

        int result = login((char*)username.c_str(), (char*)password.c_str());
        if (result == LOGIN_SUCCESS_ADMIN) {
            log_Canvas_ptr->setIsVisible(false);
            tabControl_ptr->setIsVisible(true);
            mainWindow.draw("image\\bk1.jpg");
            refreshPatientTable(patientTable_ptr);
            refreshBedTable(bedTable_ptr);
        } else {
            StellarX::MessageBox::showModal(mainWindow, "管理员登录失败！", "提示");
        }
    });

    // === 添加控件并启动 ===
    mainWindow.addControl(std::move(log_Canvas));
    mainWindow.addControl(std::move(tabControl));
    mainWindow.draw("image\\lo_bk2.jpg");  // 登录页背景图

    int result = mainWindow.runEventLoop();  // 阻塞直到用户关闭窗口

    exitSystem();  // 退出前保存所有数据
    return result;
}
```

### 4.2 病人管理页面

```cpp
// 病人管理 Canvas
auto patientPage = std::make_pair(
    std::make_unique<Button>(0, 0, 100, 100, "病人"),
    std::make_unique<Canvas>(0, 0, 1100, 780)
);
patientPage.second->setCanvasfillMode(StellarX::FillMode::Null);

// 病人表格
auto patientTable = std::make_unique<Table>(50, 50, 1000, 400);
patientTable->setHeaderText(0, "卡号");
patientTable->setHeaderText(1, "姓名");
patientTable->setHeaderText(2, "性别");
patientTable->setHeaderText(3, "电话");
patientTable->setHeaderText(4, "状态");
patientPage.second->addControl(std::move(patientTable));
Table* patientTable_ptr = dynamic_cast<Table*>(patientPage.second->getControl(0));
refreshPatientTable(patientTable_ptr);

// 操作按钮
auto patAddBtn = std::make_unique<Button>(50, 470, 120, 40, "添加病人");
patAddBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "添加病人：请使用控制台操作", "提示");
});
patientPage.second->addControl(std::move(patAddBtn));

auto patDelBtn = std::make_unique<Button>(190, 470, 120, 40, "删除病人");
patDelBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "删除病人：请使用控制台操作", "提示");
});
patientPage.second->addControl(std::move(patDelBtn));

auto patRefreshBtn = std::make_unique<Button>(330, 470, 120, 40, "刷新");
patRefreshBtn->setOnClickListener([&mainWindow, &patientTable_ptr]() {
    refreshPatientTable(patientTable_ptr);
    StellarX::MessageBox::showModal(mainWindow, "已刷新", "提示");
});
patientPage.second->addControl(std::move(patRefreshBtn));

auto patSearchBtn = std::make_unique<Button>(470, 470, 120, 40, "搜索");
patSearchBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "搜索功能：输入关键字查找", "提示");
});
patientPage.second->addControl(std::move(patSearchBtn));

tabControl_ptr->add(std::move(patientPage));
```

### 4.3 医生管理页面

```cpp
auto doctorPage = std::make_pair(
    std::make_unique<Button>(0, 100, 100, 100, "医生"),
    std::make_unique<Canvas>(0, 0, 1100, 780)
);
doctorPage.second->setCanvasfillMode(StellarX::FillMode::Null);

// 医生表格
auto doctorTable = std::make_unique<Table>(50, 50, 1000, 400);
doctorTable->setHeaderText(0, "工号");
doctorTable->setHeaderText(1, "姓名");
doctorTable->setHeaderText(2, "科室");
doctorTable->setHeaderText(3, "出诊时间");
doctorTable->setHeaderText(4, "已挂号/上限");
doctorPage.second->addControl(std::move(doctorTable));
Table* doctorTable_ptr = dynamic_cast<Table*>(doctorPage.second->getControl(0));
refreshDoctorTable(doctorTable_ptr);

// 按钮
auto docAddBtn = std::make_unique<Button>(50, 470, 120, 40, "添加医生");
docAddBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "添加医生：请使用控制台操作", "提示");
});
doctorPage.second->addControl(std::move(docAddBtn));

auto docDelBtn = std::make_unique<Button>(190, 470, 120, 40, "删除医生");
docDelBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "删除医生：请使用控制台操作", "提示");
});
doctorPage.second->addControl(std::move(docDelBtn));

auto docRefreshBtn = std::make_unique<Button>(330, 470, 120, 40, "刷新");
docRefreshBtn->setOnClickListener([&mainWindow, &doctorTable_ptr]() {
    refreshDoctorTable(doctorTable_ptr);
    StellarX::MessageBox::showModal(mainWindow, "已刷新", "提示");
});
doctorPage.second->addControl(std::move(docRefreshBtn));

auto docDeptBtn = std::make_unique<Button>(470, 470, 150, 40, "科室统计");
docDeptBtn->setOnClickListener([&mainWindow, &g_doctorHead]() {
    char msg[500] = "科室统计：\n\n";
    char buf[100];
    Doctor* d = g_doctorHead;
    int count = 0;
    while (d != nullptr) {
        count++;
        sprintf(buf, "%s - %s\n", d->name, d->dept);
        strcat(msg, buf);
        d = d->next;
    }
    if (count == 0) strcpy(msg, "暂无医生数据");
    StellarX::MessageBox::showModal(mainWindow, msg, "科室统计");
});
doctorPage.second->addControl(std::move(docDeptBtn));

tabControl_ptr->add(std::move(doctorPage));
```

### 4.4 挂号管理页面

```cpp
auto regPage = std::make_pair(
    std::make_unique<Button>(0, 200, 100, 100, "挂号"),
    std::make_unique<Canvas>(0, 0, 1100, 780)
);
regPage.second->setCanvasfillMode(StellarX::FillMode::Null);

auto regTable = std::make_unique<Table>(50, 50, 1000, 400);
regTable->setHeaderText(0, "单号");
regTable->setHeaderText(1, "病人");
regTable->setHeaderText(2, "医生");
regTable->setHeaderText(3, "科室");
regTable->setHeaderText(4, "日期");
regTable->setHeaderText(5, "状态");
regPage.second->addControl(std::move(regTable));
Table* regTable_ptr = dynamic_cast<Table*>(regPage.second->getControl(0));

auto regAddBtn = std::make_unique<Button>(50, 470, 150, 40, "挂号");
regAddBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "挂号功能：请到控制台操作", "提示");
});
regPage.second->addControl(std::move(regAddBtn));

auto regDelBtn = std::make_unique<Button>(220, 470, 150, 40, "删除记录");
regPage.second->addControl(std::move(regDelBtn));

tabControl_ptr->add(std::move(regPage));
```

### 4.5 药品管理页面

```cpp
auto medPage = std::make_pair(
    std::make_unique<Button>(0, 300, 100, 100, "药品"),
    std::make_unique<Canvas>(0, 0, 1100, 780)
);
medPage.second->setCanvasfillMode(StellarX::FillMode::Null);

auto medTable = std::make_unique<Table>(50, 50, 1000, 400);
medTable->setHeaderText(0, "编号");
medTable->setHeaderText(1, "通用名");
medTable->setHeaderText(2, "规格");
medTable->setHeaderText(3, "单价");
medTable->setHeaderText(4, "库存");
medTable->setHeaderText(5, "状态");
medPage.second->addControl(std::move(medTable));
Table* medTable_ptr = dynamic_cast<Table*>(medPage.second->getControl(0));
refreshMedicineTable(medTable_ptr);

auto medAddBtn = std::make_unique<Button>(50, 470, 150, 40, "添加药品");
medAddBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "添加药品：请到控制台操作", "提示");
});
medPage.second->addControl(std::move(medAddBtn));

auto medCheckBtn = std::make_unique<Button>(220, 470, 150, 40, "库存预警");
medCheckBtn->setOnClickListener([&mainWindow, &g_medHead]() {
    char msg[500] = {0};
    char buf[100];
    strcpy(msg, "库存预警药品：\n");
    Medicine* m = g_medHead;
    int count = 0;
    while (m != nullptr) {
        if (m->stock <= m->minStock) {
            sprintf(buf, "%s: %d/%d\n", m->genericName, m->stock, m->minStock);
            strcat(msg, buf);
            count++;
        }
        m = m->next;
    }
    if (count == 0) strcpy(msg, "所有药品库存正常");
    StellarX::MessageBox::showModal(mainWindow, msg, "库存预警");
});
medPage.second->addControl(std::move(medCheckBtn));

auto medRefreshBtn = std::make_unique<Button>(390, 470, 150, 40, "刷新");
medRefreshBtn->setOnClickListener([&mainWindow, &medTable_ptr, &g_medHead]() {
    refreshMedicineTable(medTable_ptr);
    StellarX::MessageBox::showModal(mainWindow, "已刷新", "提示");
});
medPage.second->addControl(std::move(medRefreshBtn));

tabControl_ptr->add(std::move(medPage));
```

### 4.6 住院管理页面

```cpp
auto hosPage = std::make_pair(
    std::make_unique<Button>(0, 400, 100, 100, "住院"),
    std::make_unique<Canvas>(0, 0, 1100, 780)
);
hosPage.second->setCanvasfillMode(StellarX::FillMode::Null);

auto hosTable = std::make_unique<Table>(50, 50, 1000, 400);
hosTable->setHeaderText(0, "单号");
hosTable->setHeaderText(1, "病人");
hosTable->setHeaderText(2, "床位");
hosTable->setHeaderText(3, "入院日期");
hosTable->setHeaderText(4, "预交金额");
hosTable->setHeaderText(5, "状态");
hosPage.second->addControl(std::move(hosTable));
Table* hosTable_ptr = dynamic_cast<Table*>(hosPage.second->getControl(0));

auto hosAddBtn = std::make_unique<Button>(50, 470, 150, 40, "入院登记");
hosAddBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "入院登记：请到控制台操作", "提示");
});
hosPage.second->addControl(std::move(hosAddBtn));

auto hosDischargeBtn = std::make_unique<Button>(220, 470, 150, 40, "出院结算");
hosPage.second->addControl(std::move(hosDischargeBtn));

tabControl_ptr->add(std::move(hosPage));
```

### 4.7 床位管理页面

```cpp
auto bedPage = std::make_pair(
    std::make_unique<Button>(0, 500, 100, 100, "床位"),
    std::make_unique<Canvas>(0, 0, 1100, 780)
);
bedPage.second->setCanvasfillMode(StellarX::FillMode::Null);

auto bedTable = std::make_unique<Table>(50, 50, 1000, 400);
bedTable->setHeaderText(0, "病区");
bedTable->setHeaderText(1, "床位号");
bedTable->setHeaderText(2, "病人");
bedTable->setHeaderText(3, "状态");
bedPage.second->addControl(std::move(bedTable));
Table* bedTable_ptr = dynamic_cast<Table*>(bedPage.second->getControl(0));
refreshBedTable(bedTable_ptr);

auto bedAddBtn = std::make_unique<Button>(50, 470, 150, 40, "添加床位");
bedAddBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "添加床位：请使用控制台操作", "提示");
});
bedPage.second->addControl(std::move(bedAddBtn));

auto bedDelBtn = std::make_unique<Button>(220, 470, 150, 40, "删除床位");
bedDelBtn->setOnClickListener([&mainWindow]() {
    StellarX::MessageBox::showModal(mainWindow, "删除床位：请使用控制台操作", "提示");
});
bedPage.second->addControl(std::move(bedDelBtn));

auto bedRefreshBtn = std::make_unique<Button>(390, 470, 150, 40, "刷新");
bedRefreshBtn->setOnClickListener([&bedTable_ptr]() {
    refreshBedTable(bedTable_ptr);
});
bedPage.second->addControl(std::move(bedRefreshBtn));

auto bedStatsBtn = std::make_unique<Button>(560, 470, 150, 40, "床位统计");
bedStatsBtn->setOnClickListener([&mainWindow, &g_bedHead]() {
    int total = 0, occupied = 0;
    Bed* b = g_bedHead;
    while (b != nullptr) {
        total++;
        if (strcmp(b->status, "占用") == 0) occupied++;
        b = b->next;
    }
    char msg[200];
    sprintf(msg, "床位统计：\n总床位：%d\n占用：%d\n空闲：%d",
            total, occupied, total - occupied);
    StellarX::MessageBox::showModal(mainWindow, msg, "床位统计");
});
bedPage.second->addControl(std::move(bedStatsBtn));

tabControl_ptr->add(std::move(bedPage));
```

### 4.8 统计报表页面

```cpp
auto statPage = std::make_pair(
    std::make_unique<Button>(0, 600, 100, 100, "统计"),
    std::make_unique<Canvas>(0, 0, 1100, 780)
);
statPage.second->setCanvasfillMode(StellarX::FillMode::Null);

// 医生业务量统计
auto statDocBtn = std::make_unique<Button>(50, 50, 200, 50, "医生业务量统计");
statDocBtn->setOnClickListener([&mainWindow, &g_doctorHead]() {
    char msg[1000] = {0}, buf[100];
    strcpy(msg, "医生业务量统计：\n\n");
    Doctor* d = g_doctorHead;
    int count = 0;
    while (d != nullptr) {
        count++;
        sprintf(buf, "%s(%s): %d/%d人\n",
                d->name, d->dept, d->currentPatients, d->maxPatients);
        strcat(msg, buf);
        d = d->next;
    }
    if (count == 0) strcpy(msg, "暂无医生数据");
    StellarX::MessageBox::showModal(mainWindow, msg, "医生业务量");
});
statPage.second->addControl(std::move(statDocBtn));

// 住院统计
auto statHosBtn = std::make_unique<Button>(270, 50, 200, 50, "住院统计");
statHosBtn->setOnClickListener([&mainWindow, &g_hosHead]() {
    int total = 0, current = 0;
    double totalIncome = 0;
    Hospitalization* h = g_hosHead;
    while (h != nullptr) {
        total++;
        if (h->status == 0) current++;
        h = h->next;
    }
    char msg[300];
    sprintf(msg, "住院统计：\n总住院人次：%d\n当前住院：%d\n总收入：%.2f元",
            total, current, totalIncome);
    StellarX::MessageBox::showModal(mainWindow, msg, "住院统计");
});
statPage.second->addControl(std::move(statHosBtn));

// 药品统计
auto statMedBtn = std::make_unique<Button>(490, 50, 200, 50, "药品库存统计");
statMedBtn->setOnClickListener([&mainWindow, &g_medHead]() {
    int total = 0, low = 0;
    Medicine* m = g_medHead;
    while (m != nullptr) {
        total++;
        if (m->stock <= m->minStock) low++;
        m = m->next;
    }
    char msg[200];
    sprintf(msg, "药品统计：\n药品种类：%d\n库存预警：%d", total, low);
    StellarX::MessageBox::showModal(mainWindow, msg, "药品统计");
});
statPage.second->addControl(std::move(statMedBtn));

// 收入统计
auto statIncomeBtn = std::make_unique<Button>(50, 120, 200, 50, "收入统计");
statIncomeBtn->setOnClickListener([&mainWindow, &g_hosHead, &g_purHead]() {
    double hosIncome = 0, medIncome = 0;
    Hospitalization* h = g_hosHead;
    while (h != nullptr) { hosIncome += h->totalCost; h = h->next; }
    Purchase* p = g_purHead;
    while (p != nullptr) { medIncome += p->totalCost; p = p->next; }
    char msg[300];
    sprintf(msg, "收入统计：\n住院收入：%.2f元\n药品收入：%.2f元\n总收入：%.2f元",
            hosIncome, medIncome, hosIncome + medIncome);
    StellarX::MessageBox::showModal(mainWindow, msg, "收入统计");
});
statPage.second->addControl(std::move(statIncomeBtn));

// 挂号统计
auto statRegBtn = std::make_unique<Button>(490, 120, 200, 50, "挂号统计");
statRegBtn->setOnClickListener([&mainWindow, &g_regHead]() {
    int total = 0, pending = 0, completed = 0, cancelled = 0;
    Registration* r = g_regHead;
    while (r != nullptr) {
        total++;
        if (r->status == 0) pending++;
        else if (r->status == 1) completed++;
        else if (r->status == 2) cancelled++;
        r = r->next;
    }
    char msg[300];
    sprintf(msg, "挂号统计：\n总挂号数：%d\n待就诊：%d\n已完成：%d\n已取消：%d",
            total, pending, completed, cancelled);
    StellarX::MessageBox::showModal(mainWindow, msg, "挂号统计");
});
statPage.second->addControl(std::move(statRegBtn));

// 全院汇总
auto statAllBtn = std::make_unique<Button>(50, 190, 200, 50, "全院汇总");
statAllBtn->setOnClickListener([&mainWindow,
    &g_patientHead, &g_doctorHead, &g_medHead,
    &g_bedHead, &g_hosHead, &g_regHead, &g_purHead]() {
    char msg[800];
    int patientCount = 0, doctorCount = 0, medCount = 0,
        bedTotal = 0, bedOcc = 0, hosCount = 0, regCount = 0;
    Patient* p = g_patientHead; while (p) { patientCount++; p = p->next; }
    Doctor* d = g_doctorHead; while (d) { doctorCount++; d = d->next; }
    Medicine* m = g_medHead; while (m) { medCount++; m = m->next; }
    Bed* b = g_bedHead; while (b) {
        bedTotal++;
        if (strcmp(b->status, "占用") == 0) bedOcc++;
        b = b->next;
    }
    Hospitalization* h = g_hosHead; while (h) { hosCount++; h = h->next; }
    Registration* r = g_regHead; while (r) { regCount++; r = r->next; }

    sprintf(msg,
        "全院数据汇总：\n"
        "病人总数：%d\n医生总数：%d\n药品种类：%d\n"
        "床位总数/占用：%d/%d\n住院记录：%d\n挂号记录：%d",
        patientCount, doctorCount, medCount,
        bedTotal, bedOcc, hosCount, regCount);
    StellarX::MessageBox::showModal(mainWindow, msg, "全院汇总");
});
statPage.second->addControl(std::move(statAllBtn));

tabControl_ptr->add(std::move(statPage));
```

### 4.9 辅助函数

```cpp
// main_gui.cpp - 系统初始化
void initSystem() {
    ensureDirectoriesExist();
    buildPatientChain(&g_patientHead, &g_patientTail);
    buildDoctorChain(&g_doctorHead, &g_doctorTail);
    buildRegistrationChain(&g_regHead, &g_regTail);
    buildMedicineChain(&g_medHead, &g_medTail);
    buildPurchaseChain(&g_purHead, &g_purTail);
    buildHospitalizationChain(&g_hosHead, &g_hosTail);
    buildBedChain(&g_bedHead, &g_bedTail);
}

// main_gui.cpp - 退出保存
void exitSystem() {
    rebuildPatientFile(g_patientHead);
    rebuildDoctorFile(g_doctorHead);
    rebuildRegistrationFile(g_regHead);
    rebuildMedicineFile(g_medHead);
    rebuildPurchaseFile(g_purHead);
    rebuildHospitalizationFile(g_hosHead);
    rebuildBedFile(g_bedHead);

    freePatientChain(g_patientHead);
    freeDoctorChain(g_doctorHead);
    freeRegistrationChain(g_regHead);
    freeMedicineChain(g_medHead);
    freePurchaseChain(g_purHead);
    freeHospitalizationChain(g_hosHead);
    freeBedChain(g_bedHead);
}

// 刷新病人表格
void refreshPatientTable(Table* table) {
    if (table == nullptr) return;
    table->clear();
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

// 刷新医生表格
void refreshDoctorTable(Table* table) {
    if (table == nullptr) return;
    table->clear();
    int row = 0;
    char buf[50];
    Doctor* d = g_doctorHead;
    while (d != nullptr && row < 100) {
        table->setCellText(row, 0, d->empNo);
        table->setCellText(row, 1, d->name);
        table->setCellText(row, 2, d->dept);
        table->setCellText(row, 3, d->schedule);
        sprintf(buf, "%d/%d", d->currentPatients, d->maxPatients);
        table->setCellText(row, 4, buf);
        d = d->next;
        row++;
    }
}

// 刷新药品表格
void refreshMedicineTable(Table* table) {
    if (table == nullptr) return;
    table->clear();
    int row = 0;
    char buf[50];
    Medicine* m = g_medHead;
    while (m != nullptr && row < 100) {
        table->setCellText(row, 0, m->medNo);
        table->setCellText(row, 1, m->genericName);
        table->setCellText(row, 2, m->spec);
        sprintf(buf, "%.2f", m->price);
        table->setCellText(row, 3, buf);
        sprintf(buf, "%d", m->stock);
        table->setCellText(row, 4, buf);
        table->setCellText(row, 5, m->stock <= m->minStock ? "库存预警" : "正常");
        m = m->next;
        row++;
    }
}

// 刷新床位表格
void refreshBedTable(Table* table) {
    if (table == nullptr) return;
    table->clear();
    int row = 0;
    Bed* b = g_bedHead;
    while (b != nullptr && row < 100) {
        table->setCellText(row, 0, b->ward);
        table->setCellText(row, 1, b->bedNo);
        table->setCellText(row, 2,
            strcmp(b->patientName, "无") == 0 ? "-" : b->patientName);
        table->setCellText(row, 3, b->status);
        b = b->next;
        row++;
    }
}
```

---

## 5. 控制台主循环（ui.c / main.c 入口）

```c
// main.c 或 ui.c - 主循环入口
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
                case 1: showPatientManagement();     break;
                case 2: showDoctorManagement();      break;
                case 3: showRegistrationManagement(); break;
                case 4: showMedicineManagement();     break;
                case 5: showHospitalizationManagement(); break;
                case 6: showBedManagement();          break;
                case 7: showStatisticsMenu();         break;
                case 8: showQueryMenu();              break;
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

## 6. 开发注意事项

### 6.1 GUI vs 控制台的选择

| 场景 | 推荐版本 | 原因 |
|------|---------|------|
| 学习数据结构与算法 | 控制台版（ui.c） | 代码简洁，专注逻辑 |
| 快速原型验证 | 控制台版 | 修改调试方便 |
| 实际项目交付 | 图形界面版（main_gui.cpp） | 交互体验更好 |

### 6.2 内存管理

- 每次**增删改操作后**必须调用 `rebuildXxxFile()` 同步写回磁盘
- 程序退出前必须调用 `freeXxxChain()` 释放所有链表节点内存
- GUI 版本在 `exitSystem()` 中统一处理

### 6.3 输入安全

- `scanf("%s", ...)` 换成 `scanf("%49s", ...)` 防止缓冲区溢出
- `getchar()` 放在 `scanf` 后清除换行符残留
- 密码输入使用 `getch()` 实时读取，不回显

### 6.4 GUI 调试建议

- 先确保控制台版本功能正确，再迁移到 GUI
- 复杂操作（增删改）建议暂时跳转到控制台完成，GUI 中仅做展示
- `WinMain` 中加 `printf` 调试语句，通过控制台窗口查看输出

---

## 7. 参考文件汇总

| 参考路径 | 内容说明 |
|---------|---------|
| `hospital-management/include/ui.h` | 头文件：颜色宏、尺寸宏、函数声明 |
| `hospital-management/src/ui.c` | 控制台版本完整实现 |
| `hospital-management/src/main_gui.cpp` | StellarX 图形界面完整实现 |
| `hospital-management/include/` | 各模块数据结构声明 |
| `hospital-management/src/` | 各模块函数实现 |
| `docs/05-病人模块_.md` | 病人模块参考（数据结构+函数） |
| `docs/10-登录模块_.md` | 登录模块参考（登录状态枚举） |