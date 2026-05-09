#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"        // 病人模块头文件
#include "registration.h"   // 挂号模块头文件
#include "file_io.h"        // 文件输入输出模块
#include "utils.h"          // 工具函数模块
#include "ui.h" 

// Getter 函数实现 - 用于其他模块访问本模块的全局变量
Patient* getPatientHead(void) {
    return g_patientHead;  // 直接使用 ui.h 中 extern 声明的变量
}
Patient* getPatientTail(void) {
    return g_patientTail;
}
//--------------------

//--------------------
// 以下为添加病人函数
// 功能：向病人链表中添加一个新的病人节点
// 参数：head - 指向链表头指针的指针，tail - 指向链表尾指针的指针
//      name - 姓名，age - 年龄， gender - 性别，idCard - 身份证号，phone - 电话号码
void addPatient(Patient** head, Patient** tail,
    char name[], int age, char gender[], char idCard[], char phone[]) {
    // 1. 为新节点分配内存空间
    Patient* newNode = (Patient*)malloc(sizeof(Patient));
    
    // 2. 初始化新节点的指针域，防止野指针
    newNode->next = newNode->pre = NULL;                // 申请新节点第一件事就是指针初始化！非常重要
    
    // 3. 生成唯一的门诊卡号
    char id[20];
    generateUniqueId("PT", id);                         // 生成以"PT"开头的唯一ID
    
    // 4. 保存病人具体信息到新节点的数据域
    strcpy(newNode->data.cardNo, id);                   // 门诊卡号
    strcpy(newNode->data.name, name);                   // 姓名
    newNode->data.age = age;                            // 年龄
    strcpy(newNode->data.gender, gender);               // 性别
    strcpy(newNode->data.idCard, idCard);               // 身份证号
    strcpy(newNode->data.phone, phone);                 // 电话号码
    newNode->data.isActive = 0;                         // 设置初始住院状态为0（未住院）
    
    // 5. 将新节点插入到链表尾部
    if (*tail == NULL) {
        // 如果链表为空（尾指针为NULL），则新节点既是头节点也是尾节点
        *head = newNode;                                // 设置头指针指向新节点
        *tail = newNode;                                // 设置尾指针指向新节点
    }
    else {
        // 如果链表不为空，则将新节点连接到尾节点之后
        (*tail)->next = newNode;                        // 当前尾节点的next指向新节点
        newNode->pre = *tail;                           // 新节点的pre指向前一个尾节点
        *tail = newNode;                                // 更新尾指针指向新节点
    }
    
    // 6. 将更新后的链表数据保存到文件
    rebuildPatientFile(*head);
    
    // 7. 输出添加成功的提示信息
    printf("[OK] 添加成功,卡号为%s。\n", id);
}
//--------------------

//--------------------
// 以下为删除病人函数
// 功能：从病人链表中删除指定的病人节点
// 参数：head - 指向链表头指针的指针，tail - 指向链表尾指针的指针
//      p - 要删除的病人数据（根据cardNo匹配）
void delPatient(Patient** head, Patient** tail, PatientData p) {
    // 1. 检查链表是否为空
    if (*head == NULL) {
        printf("[ERROR] 暂无病人数据。\n");
        return;
    }
    
    // 2. 遍历链表寻找要删除的节点
    Patient* cur = *head;                               // 当前节点指针
    Patient* pre = NULL;                                // 前一个节点指针
    while (cur != NULL) {
        if (strcmp(cur->data.cardNo, p.cardNo) == 0) {  // 当卡号匹配的时候
            break;                                      // 跳出循环
        }
        pre = cur;                                      // 记录前一个节点
        cur = cur->next;                                // 移动到下一个节点
    }
    
    // 3. 检查是否找到要删除的节点
    if (cur == NULL) {
        printf("[ERROR] 未找到该病人，无法删除。\n");
        return;
    }
    
    // 4. 根据要删除节点的位置执行不同的删除操作
    if (cur == *head && cur == *tail) {
        *head = NULL;                                   // 1.链表只有一个节点，删除后链表变空
        *tail = NULL;
    }
    else if (cur == *head) {
        *head = cur->next;                              // 2.删除头节点，头指针指向下一个节点
        (*head)->pre = NULL;                            // 新头节点的前驱设为NULL
    }
    else if (cur == *tail) {
        *tail = cur->pre;                               // 3.删除尾节点，尾指针指向前一个节点
        (*tail)->next = NULL;                           // 新尾节点的后继设为NULL
    }
    else {
        pre->next = cur->next;                          // 4.删除中间节点，前节点的next指向后节点
        cur->next->pre = pre;                           // 后节点的pre指向前节点
    }
    
    // 5. 释放被删除节点的内存
    free(cur);
    
    // 6. 输出删除成功的提示信息
    printf("[OK] 删除成功。\n");
    
    // 7. 将更新后的链表数据保存到文件
    rebuildPatientFile(*head);
}
//--------------------

//--------------------
// 以下为修改病人信息函数
// 功能：修改指定门诊卡号的病人信息
// 参数：head - 链表头指针，cardNo - 要修改的病人卡号，newData - 新的病人数据
// 返回值：1-修改成功，0-修改失败
int modifyPatient(Patient* head, char* cardNo, PatientData newData){
    Patient* target = NULL;
    
    // 1. 根据卡号查找要修改的病人节点
    target = findPatientByCardNo(head, cardNo);
    
    // 2. 检查是否找到要修改的病人
    if (target == NULL) {
        printf("[ERROR] 未找到门诊卡号为%s的病人。\n", cardNo);
        return 0;                                       // 返回0表示修改失败
    }
    
    // 3. 显示当前病人信息
    printf("当前患者信息为：\n");
    printf("门诊卡号：%s\n", target->data.cardNo);
    printf("姓名：%s\n", target->data.name);
    printf("年龄：%d\n", target->data.age);
    printf("性别：%s\n", target->data.gender);
    printf("身份证号：%s\n", target->data.idCard);
    printf("联系电话：%s\n", target->data.phone);
    printf("是否住院：%s\n", target->data.isActive ? "住院" : "非住院");
    
    // 4. 更新病人信息
    safeStringCopy(target->data.name, newData.name, 50);        // 安全复制姓名
    target->data.age = newData.age;                             // 复制年龄
    safeStringCopy(target->data.gender, newData.gender, 10);    // 安全复制性别
    safeStringCopy(target->data.idCard, newData.idCard, 20);    // 安全复制身份证号
    safeStringCopy(target->data.phone, newData.phone, 15);      // 安全复制电话号码
    target->data.isActive = newData.isActive;                   // 更新住院状态
    
    // 5. 将更新后的链表数据保存到文件
    rebuildPatientFile(head);
    
    // 6. 输出修改成功的提示信息
    printf("[OK] 患者信息修改成功！\n");
    
    return 1;                                             // 返回1表示修改成功
}
//--------------------

//--------------------
// 以下为查看挂号记录函数
// 功能：患者查看个人挂号记录
// 参数：regHead — 挂号链表头，patientCardNo — 患者卡号
//--------------------
void patientViewOwnRegistrations(Registration* regHead, 
    char* patientCardNo){
    printf("\n========== 我的挂号记录 ==========\n");
    printf("%-14s %-10s %-10s %-12s %-10s %-10s\n",
        "挂号编号", "医生", "科室", "预约时间", "状态", "方式");
    printf("──────────────────────────────────────────────────────\n");

    Registration* cur = regHead;
    int count = 0;
    while (cur != NULL) {
        if (strcmp(cur->data.patientCardNo, patientCardNo) == 0) {
            char* statusStr;
            switch (cur->data.status) {
            case PENDING:     statusStr = "待就诊"; break;
            case IN_PROGRESS: statusStr = "就诊中"; break;
            case COMPLETED:   statusStr = "已完成"; break;
            case CANCELLED:   statusStr = "已取消"; break;
            default:          statusStr = "未知";
            }
            char* methodStr = (cur->data.createdBy == PATIENT) ? "预约" : "现场";

            printf("%-14s %-10s %-10s %-12s %-10s %-10s\n",
                cur->data.regNo, cur->data.doctorName, cur->data.dept,
                cur->data.appointmentTime, statusStr, methodStr);
            count++;
        }
        cur = cur->next;
    }

    if (count == 0) {
        printf("暂无挂号记录\n");
    }
    else {
        printf("共 %d 条记录\n", count);
    }
}
//--------------------

//--------------------
// 以下为取消自己挂号函数
// 功能：患者取消自己的挂号（带权限校验：只能取消自己的、PENDING 状态的）
// 参数：regHead/regTail — 挂号链表，patientCardNo — 患者卡号，regNo — 挂号编号
//---------------------
int patientCancelRegistration(Registration** regHead, Registration** regTail,
    char* patientCardNo, char* regNo)
{
    // 1. 按挂号编号查找
    Registration* r = findRegistrationByNo(*regHead, regNo);
    if (r == NULL) {
        printf("[ERROR] 未找到挂号编号 %s\n", regNo);
        return 0;
    }

    // 2. 权限校验：只能取消自己的挂号
    if (strcmp(r->data.patientCardNo, patientCardNo) != 0) {
        printf("[ERROR] 无权取消他人的挂号记录\n");
        return 0;
    }

    // 3. 调用通用取消函数
    return cancelRegistration(regHead, regTail, r);
}
//--------------------

//--------------------
//以下为查找病人信息函数

//按卡号精确查找
// 功能：根据门诊卡号精确查找病人
// 参数：head - 链表头指针，cardNo - 要查找的门诊卡号
// 返回值：找到的病人节点指针，未找到返回NULL
Patient* findPatientByCardNo(Patient* head, char* cardNo) {
    Patient* p = head;                                    // 从头节点开始查找
    while (p != NULL) {
        if (strcmp(p->data.cardNo, cardNo) == 0) {        // 比较门诊卡号是否匹配
            return p;                                     // 找到则返回该节点指针
        }
        p = p->next;                                      // 移动到下一个节点
    }
    return NULL;                                          // 未找到返回NULL
}

//按姓名模糊
Patient* findPatientsByName(Patient* head, char* name) {
    Patient* p = head;                                    // 从头节点开始查找
    Patient* resultHead = NULL;
    Patient* resultTail = NULL;
    while (p != NULL) {
        if (strstr(p->data.name, name) != NULL) {
            Patient* newNode = (Patient*)malloc(sizeof(Patient));
            newNode->data = p->data;
            newNode->next = NULL;
            newNode->pre = NULL;
            if (resultHead == NULL) {
                resultHead = newNode;
                resultTail = newNode;
            }
            else {
                resultTail->next = newNode;
                newNode->pre = resultTail;
                resultTail = newNode;
            }
        }           
        p = p->next;                                      // 移动到下一个节点
    }
    return resultHead;
}
//--------------------

//--------------------
//以下为列表显示函数
// 功能：遍历并显示所有病人信息
// 参数：head - 链表头指针
void listAllPatients(Patient* head) {
    // 1. 检查链表是否为空
    if (head == NULL) {
        printf("暂无病人信息\n");
        return;
    }
    
    // 2. 输出表头信息
    printf("=== 病人列表 ===\n");
    printf("%-20s %-50s %-10s %-10s %-20s %-15s %-10s\n",
        "卡号", "姓名", "年龄", "性别", "身份证", "电话", "住院状态");

    // 3. 遍历链表并输出每个病人的信息
    Patient* current = head;                              // 从头节点开始
    while (current != NULL) {
        // 格式化输出当前病人的各项信息
        printf("%-20s %-50s %-10d %-10s %-20s %-15s %-10s\n",
            current->data.cardNo,                         // 门诊卡号
            current->data.name,                           // 姓名
            current->data.age,                            // 年龄
            current->data.gender,                         // 性别
            current->data.idCard,                         // 身份证号
            current->data.phone,                          // 电话号码
            current->data.isActive ? "住院" : "非住院");  // 住院状态（0=非住院，1=住院）
        current = current->next;                          // 移动到下一个节点
    }
}
