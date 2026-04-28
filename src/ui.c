#include"ui.h"


//--------------------
int showLoginPage(void) {
    char username[50] = { 0 };
    char password[50] = { 0 };

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
    getchar();   // 清除 scanf 后的换行符

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
//--------------------

//--------------------
// 以下为根据不同角色显示菜单的函数
void showMainMenuByRole(int userRole, char* username) {
    printf("\n╔═════════════════════════════════════════╗\n");
    printf("║ 医院综合信息管理系统 - 主菜单           ║\n");
    printf("║ 当前用户: %-15s 角色: ", username);

    // 根据角色显示角色名称
    switch (userRole) {
    case 0: printf("患者"); break;
    case 1: printf("护士"); break;
    case 2: printf("医生"); break;
    case 3: printf("管理员"); break;
    default: printf("未知"); break;
    }
    printf("    ║\n");
    printf("╠═════════════════════════════════════════╣\n");

    // 根据角色显示相应的菜单选项
    if (userRole >= 0) { // 患者及以上
        printf("║ 1. 个人信息管理                         ║\n");
        printf("║ 2. 挂号预约                             ║\n");
    }

    if (userRole >= 1) { // 护士及以上
        printf("║ 3. 患者信息管理                         ║\n");
        printf("║ 4. 床位管理                             ║\n");
    }

    if (userRole >= 2) { // 医生及以上
        printf("║ 5. 医生信息管理                         ║\n");
        printf("║ 6. 药品管理                             ║\n");
        printf("║ 7. 住院管理                             ║\n");
    }

    if (userRole >= 3) { // 管理员
        printf("║ 8. 统计报表                             ║\n");
        printf("║ 9. 系统管理                             ║\n");
    }

    printf("║ 0. 退出系统                             ║\n");
    printf("╚═════════════════════════════════════════╝\n");
    // 处理用户选择
    int module;
    printf("请选择功能模块: ");
    scanf("%d", &module);
    getchar();

    // 根据角色和选择执行相应功能
    handleModuleChoiceByRole(module, userRole);
}
//--------------------

//--------------------
// 以下为病人管理菜单显示函数
void showPatientManagement(void) {
    int choice;
    char name[50], gender[10], idCard[20], phone[15], cardNo[20];
    int age;
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
            printf("\n请输入病人信息:");
            printf("\n姓名: ");     scanf("%49s", name);
            printf("\n年龄: ");     scanf("%d", &age);
            printf("\n性别: ");     scanf("%9s", gender);
            printf("\n身份证号: "); scanf("%19s", idCard);
            printf("\n联系电话: "); scanf("%14s", phone);
            addPatient(&g_patientHead, &g_patientTail,
                name, age, gender, idCard, phone);
            rebuildPatientFile(g_patientHead);  // 增删改后必须写文件
            break;

        case 2:  // 删除
            printf("请输入要删除的病人卡号: ");
            scanf("%19s", cardNo);
            p = findPatientByCardNo(g_patientHead, cardNo);
            delPatient(&g_patientHead, &g_patientTail, p);
            rebuildPatientFile(g_patientHead);
            break;

        case 3://修改
            PatientData newData;
            printf("\n请输入病人信息:");
            printf("\n姓名: ");     scanf("%49s", newData.name);
            printf("\n年龄: ");     scanf("%d", newData.age);
            printf("\n性别: ");     scanf("%9s", newData.gender);
            printf("\n身份证号: "); scanf("%19s", newData.idCard);
            printf("\n联系电话: "); scanf("%14s", newData.phone);
            modifyPatient(g_patientHead, &cardNo,newData);
            break;

        case 4://查找病人
            printf("请选择查找方式：\n");
            printf("1、按照卡号查找\n");
            printf("2、按照姓名查找\n");
            int choice1;
            scanf("%d", &choice1);
            switch (choice1)
            {
            case 1:
                printf("请输入卡号："); scanf("%19s", cardNo);
                Patient* pt = findPatientByCardNo(g_patientHead, cardNo);
                printf("\n");
                printf("%-20s %-50s %-10s %-10s %-20s %-15s %-10s\n",
                    "卡号", "姓名", "年龄", "性别", "身份证", "电话", "住院状态");
                printf("%-20s %-50s %-10s %-20s %-15s %-10s\n",
                    pt->data.cardNo,                         // 门诊卡号
                    pt->data.name,                           // 姓名
                    pt->data.age,                            // 年龄
                    pt->data.gender,                         // 性别
                    pt->data.idCard,                         // 身份证号
                    pt->data.phone,                          // 电话号码
                    pt->data.isActive ? "住院" : "非住院");  // 住院状态（0=非住院，1=住院）
                break;
            case 2:
                printf("请输入姓名："); scanf("%49s", name);
                Patient* pt = findPatientsByName(g_patientHead, name);
                printf("\n");
                printf("%-20s %-50s %-10s %-10s %-20s %-15s %-10s\n",
                    "卡号", "姓名", "年龄", "性别", "身份证", "电话", "住院状态");
                printf("%-20s %-50s %-10s %-20s %-15s %-10s\n",
                    pt->data.cardNo,                         // 门诊卡号
                    pt->data.name,                           // 姓名
                    pt->data.age,                            // 年龄
                    pt->data.gender,                         // 性别
                    pt->data.idCard,                         // 身份证号
                    pt->data.phone,                          // 电话号码
                    pt->data.isActive ? "住院" : "非住院");  // 住院状态（0=非住院，1=住院）
                break;
            default:
                printf("[ERROR] 无效选择！\n");
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
//--------------------