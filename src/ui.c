#include"ui.h"


//--------------------
//登录界面
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

//--------------------
// 以下为医生管理菜单显示函数
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
            printf("\n请输入医生信息:");
            printf("\n姓名: ");      scanf("%49s", name);
            printf("\n科室: ");      scanf("%49s", dept);
            printf("\n出诊时间: ");  scanf("%99s", schedule);
            printf("\n每日最大接诊数: "); scanf("%d", &maxPatients);
            addDoctor(&g_doctorHead, &g_doctorTail,
                name, dept, schedule, maxPatients);
            rebuildDoctorFile(g_doctorHead);  // ★ 写回文件
            printf("[OK] 医生添加成功！\n");
            break;
           
        case 2://删除
            printf("请输入要删除的医生工号：\n");
            scanf("%19s", empNo);
            d = findDoctorByEmpNo(g_doctorHead, empNo);
            delDoctor(&g_doctorHead, &g_doctorTail, d);
            rebuildDoctorFile(g_doctorHead);
            break;
        case 3://修改医生信息
            DoctorData newData;
            printf("\n请输入医生信息:");
            printf("\n姓名: ");      scanf("%49s", newData.name);
            printf("\n科室: ");      scanf("%49s", newData.dept);
            printf("\n出诊时间: ");  scanf("%99s", newData.schedule);
            printf("\n每日最大接诊数: "); scanf("%d", &newData.maxPatients);
            modifyDoctor(&g_doctorHead, &empNo, newData);
            break;

        case 4://查找
            printf("请选择查找方式：\n");
            printf("1、按照工号查找\n");
            printf("2、按照姓名查找\n");
            printf("3、按照科室查找\n");
            int choice2;
            scanf("%d", &choice2);
            switch (choice2)
            {
            case 1://按工号
                printf("请输入要查找医生的工号:\n"); scanf("%19s", empNo);
                Doctor* d = findDoctorsByDept(g_doctorHead, empNo);
                printf("\n");
                printf("%-20s %-50s %-50s %-100s %-10s %-10s\n",
                    "工号", "姓名", "科室", "出诊时间", "每日最大接诊数", "今日已接诊数");
                printf("%-20s %-50s %-50s %-100s %-10d %-10d\n",
                    d->data.empNo,                         // 工号
                    d->data.name,                          // 姓名
                    d->data.dept,                          // 科室
                    d->data.schedule,                      // 出诊时间
                    d->data.maxPatients,                   // 每日最大接诊数
                    d->data.currentPatients);              // 今日已接诊数
                break;
            case 2://按姓名
                printf("请输入要查找医生的姓名:\n"); scanf("%49s", name);
                Doctor* d = findDoctorsByDept(g_doctorHead, name);
                printf("\n");
                printf("%-20s %-50s %-50s %-100s %-10s %-10s\n",
                    "工号", "姓名", "科室", "出诊时间", "每日最大接诊数", "今日已接诊数");
                printf("%-20s %-50s %-50s %-100s %-10d %-10d\n",
                    d->data.empNo,                         // 工号
                    d->data.name,                          // 姓名
                    d->data.dept,                          // 科室
                    d->data.schedule,                      // 出诊时间
                    d->data.maxPatients,                   // 每日最大接诊数
                    d->data.currentPatients);              // 今日已接诊数
                break;
            case 3://按科室
                printf("请输入要查找医生的科室:\n"); scanf("%49s", dept);
                Doctor* d = findDoctorsByDept(g_doctorHead, dept);
                printf("\n");
                printf("%-20s %-50s %-50s %-100s %-10s %-10s\n",
                    "工号", "姓名", "科室", "出诊时间", "每日最大接诊数", "今日已接诊数");
                printf("%-20s %-50s %-50s %-100s %-10d %-10d\n",
                    d->data.empNo,                         // 工号
                    d->data.name,                          // 姓名
                    d->data.dept,                          // 科室
                    d->data.schedule,                      // 出诊时间
                    d->data.maxPatients,                   // 每日最大接诊数
                    d->data.currentPatients);              // 今日已接诊数
                break;
            default:
                printf("[ERROR] 无效选择！");
            }
        case 5:
            listAllDoctors(g_doctorHead);
            break;
        case 0:
            return;
        default:
            printf("[ERROR] 无效选择！");
            break;
        }
    }
}
//--------------------

//--------------------
//以下为药品管理菜单显示函数
void showMedicineManagement(void) {
    int choice;
    char medNo[20], name[50], patientCardNo[20], date[20];
    int quantity;
    double totalCost;
    Medicine* m;

    while (1) {
        printf("\n-------- 药品管理 --------\n");
        printf("1. 按编号查询   4. 购药登记\n");
        printf("2. 按名称查询   5. 显示所有药品\n");
        printf("3. 补充库存     6. 库存预警检查\n");
        printf("0. 返回上级菜单\n");
        printf("请选择: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:  // 按编号查询
            printf("请输入药品编号: "); scanf("%19s", medNo);
            m = findMedicineByNo(g_medHead, medNo);
            if (m) {
                printf("%-10s %-20s %-20s %-15s %-10s %-10s %-10s\n",
                    "编号", "通用名", "商品名", "规格", "单价", "库存", "最低库存");
                printf("%-10s %-20s %-20s %-15s %-10.2f %-10d %-10d\n",
                    m->data.medNo, m->data.genericName, m->data.brandName,
                    m->data.spec, m->data.price, m->data.stock, m->data.minStock);
            }
            else {
                printf("[ERROR] 未找到该药品！\n");
            }
            break;

        case 2:  // 按名称查询
            printf("请输入药品名称关键字: "); scanf("%49s", name);
            m = findMedicineByName(g_medHead, name);
            if (m) {
                printf("%-10s %-20s %-20s %-15s %-10s %-10s %-10s\n",
                    "编号", "通用名", "商品名", "规格", "单价", "库存", "最低库存");
                printf("%-10s %-20s %-20s %-15s %-10.2f %-10d %-10d\n",
                    m->data.medNo, m->data.genericName, m->data.brandName,
                    m->data.spec, m->data.price, m->data.stock, m->data.minStock);
            }
            else {
                printf("[ERROR] 未找到该药品！\n");
            }
            break;

        case 3:  // 补充库存
            printf("请输入药品编号: "); scanf("%19s", medNo);
            printf("请输入补充数量: "); scanf("%d", &quantity);
            m = findMedicineByNo(g_medHead, medNo);
            if (m) {
                replenishStock(g_medHead, medNo, quantity);
                printf("[OK] 库存补充成功！当前库存: %d\n", m->data.stock + quantity);
            }
            else {
                printf("[ERROR] 未找到该药品！\n");
            }
            break;

        case 4:  // 购药登记
            printf("请输入病人卡号: ");     scanf("%19s", patientCardNo);
            printf("请输入药品编号: ");     scanf("%19s", medNo);
            printf("请输入购买数量: ");     scanf("%d", &quantity);
            printf("请输入总费用: ");       scanf("%lf", &totalCost);
            printf("请输入购药日期(YYYY-MM-DD): "); scanf("%19s", date);
            m = findMedicineByNo(g_medHead, medNo);
            if (m && m->data.stock >= quantity) {
                addPurchaseRecord(&g_purHead, &g_purTail,
                    patientCardNo, medNo, quantity, totalCost, date);
                printf("[OK] 购药登记成功！\n");
            }
            else {
                printf("[ERROR] 药品不存在或库存不足！\n");
            }
            break;

        case 5:
            listAllMedicines(g_medHead);
            break;

        case 6:  // 库存预警
            checkLowStock(g_medHead);
            break;

        case 0:
            return;

        default:
            printf("[ERROR] 无效选择！\n");
        }
    }
}
//--------------------

//--------------------
// 以下为住院管理菜单函数
void showHospitalizationManagement(void) {
    int choice;
    char recordNo[20], patientCardNo[20], patientName[50];
    double prepay, totalCost;
    Hospitalization* h;

    while (1) {
        printf("\n-------- 住院管理 --------\n");
        printf("1. 入院登记        5. 显示当前在院病人\n");
        printf("2. 出院结算        6. 显示所有住院记录\n");
        printf("3. 按卡号查询      7. 追加预交金\n");
        printf("4. 按住院单号查询  8. 修改住院信息（转床/追加押金）")
        printf("0. 返回上级菜单\n");
        printf("请选择: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:  // 入院登记
            printf("\n请输入入院信息:\n");
            printf("病人卡号: "); scanf("%19s", patientCardNo);
            printf("病人姓名: "); scanf("%49s", patientName);
            printf("预交金额: "); scanf("%lf", &prepay);
            addHospitalization(&g_hosHead, &g_hosTail, "",
                patientCardNo, patientName, prepay);
            printf("[OK] 入院登记成功！住院单号与床位已自动分配。\n");
            break;

        case 2:  // 出院结算
            printf("请输入住院单号: "); scanf("%19s", recordNo);
            h = findHospitalizationByNo(g_hosHead, recordNo);
            if (h) {
                printf("当前状态: %s，预交金额: %.2f\n",
                    h->data.status, h->data.prepay);
                printf("请输入总费用: "); scanf("%lf", &totalCost);
                dischargePatient(h, totalCost);
                rebuildHospitalizationFile(g_hosHead);
                rebuildBedFile(g_bedHead);
            }
            else {
                printf("[ERROR] 未找到该住院记录！\n");
            }
            break;

        case 3:  // 按卡号查询
            printf("请输入病人卡号: "); scanf("%19s", patientCardNo);
            h = findHospitalizationByCardNo(g_hosHead, patientCardNo);
            if (h) {
                printf("%-12s %-12s %-10s %-10s %-10s %-10s %-10s\n",
                    "住院单号", "姓名", "床位号", "预交金", "总费用", "入院日期", "状态");
                printf("%-12s %-12s %-10s %-10.2f %-10.2f %-10s %-10s\n",
                    h->data.recordNo, h->data.patientName, h->data.bedNo,
                    h->data.prepay, h->data.totalCost,
                    h->data.admissionDate, h->data.status);
            }
            else {
                printf("[ERROR] 未找到该病人的住院记录！\n");
            }
            break;

        case 4:  // 按住院单号查询
            printf("请输入住院单号: "); scanf("%19s", recordNo);
            h = findHospitalizationByNo(g_hosHead, recordNo);
            if (h) {
                printf("%-12s %-12s %-10s %-10s %-10s %-10s %-10s\n",
                    "住院单号", "姓名", "床位号", "预交金", "总费用", "入院日期", "状态");
                printf("%-12s %-12s %-10s %-10.2f %-10.2f %-10s %-10s\n",
                    h->data.recordNo, h->data.patientName, h->data.bedNo,
                    h->data.prepay, h->data.totalCost,
                    h->data.admissionDate, h->data.status);
            }
            else {
                printf("[ERROR] 未找到该住院记录！\n");
            }
            break;

        case 5:  // 当前在院病人
            h = findAllCurrentHospitalizations(g_hosHead);
            if (h) {
                listAllHospitalizations(h);
                freeHospitalizationResultChain(h);
            }
            else {
                printf("当前无在院病人。\n");
            }
            break;

        case 6:
            listAllHospitalizations(g_hosHead);
            break;

        case 7: {  // 追加预交金
            printf("请输入住院单号: \n");  scanf("%19s", recordNo);
            printf("请输入追加金额:\n ");  scanf("%lf", &prepay);
            Hospitalization* h = findHospitalizationByNo(g_hosHead, recordNo);
            if (h) {
                addPrepay(h, prepay);
            }
            else {
                printf("[ERROR] 未找到该住院记录！\n");
            }
            break;
        }

        case 8: {  // 修改住院信息（转床 + 追加预交金）
            printf("请输入住院单号:\n ");     scanf("%19s", recordNo);
            printf("请输入新床位号(不转床输0): \n");  scanf("%19s", bedNo);
            printf("请输入追加预交金(不追加输0): \n"); scanf("%lf", &prepay);
            if (strcmp(bedNo, "0") == 0) {
                strcpy(bedNo, "");        // 空字符串表示不转床
            }
            modifyHospitalization(g_hosHead, recordNo, bedNo, prepay);
            break;
        }

        case 0:
            return;

        default:
            printf("[ERROR] 无效选择！\n");
        }
    }
}
//--------------------

//--------------------
// 以下是床位管理菜单函数

//--------------------