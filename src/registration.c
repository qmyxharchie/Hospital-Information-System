#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"        // 患者模块头文件
#include "doctor.h"         // 医生模块头文件
#include "registration.h"	// 挂号模块头文件
#include "login.h"          // 登录模块头文件
#include "file_io.h"        // 文件输入输出模块
#include "utils.h"          // 工具函数模块


// 外部全局变量（假设在其他地方定义）
extern char g_currentUsername[50];  // 当前登录用户名

//-------------------------
//函数名：addRegistration
//功能：为患者办理挂号手续，将患者与医生关联
//参数：head - 指向挂号记录链表头部的指针的指针
//      tail - 指向挂号记录链表尾部的指针的指针
//      patientCardNo - 患者卡号
//      patientName - 患者姓名
//      doctorEmpNo - 医生工号
//      doctorName - 医生姓名
//      dept - 科室
//返回值：成功返回1，失败返回0
int addRegistration(Registration** head, Registration** tail,
    char* patientCardNo, char* patientName,
    char* doctorEmpNo, char* doctorName, char* dept,
    char* appointmentDate, char* appointmentTime, UserRole createdBy){
    
    // 查找指定医生是否存在
    Doctor* d = findDoctorByEmpNo(getDoctorHead(), doctorEmpNo);
    if (d == NULL) {
        printf("未找到对应医生\n");
        return 0;
    }
    
    // 检查医生是否还能接收更多患者
    if (!canAcceptPatient(d)) {
        printf("该医生号已满\n");
        return 0;
    }
    
    // 创建新的挂号记录节点
    Registration* newNode = (Registration*)malloc(sizeof(Registration));
    if (newNode == NULL) {
        printf("内存分配失败\n");
        return 0;
    }
    
    // 初始化节点指针
    newNode->next = newNode->pre = NULL;
    
    // 生成挂号编号
    char id[20];
    generateUniqueId("REG", id);
    
    // 填充挂号记录数据
    strcpy(newNode->data.regNo, id);                    // 挂号编号
    strcpy(newNode->data.patientCardNo, patientCardNo); // 患者卡号
    strcpy(newNode->data.patientName, patientName);     // 患者姓名
    strcpy(newNode->data.doctorEmpNo, doctorEmpNo);     // 医生工号
    strcpy(newNode->data.doctorName, doctorName);       // 医生姓名
    strcpy(newNode->data.dept, dept);                   // 科室
    
    // 获取当前日期时间
    int year, month, day, hour, minute, second;
    getCurrentTime(&year, &month, &day);
    sprintf(newNode->data.date, "%04d-%02d-%02d", year, month, day);
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    hour = timeinfo->tm_hour;
    minute = timeinfo->tm_min;
    second = timeinfo->tm_sec;
    sprintf(newNode->data.time, "%02d:%02d:%02d", hour, minute, second);
    // 设置状态为"待就诊"
    strcpy(newNode->data.status, "待就诊");
    
    // 将新节点添加到链表尾部
    if (*tail == NULL) {
        *head = newNode;
        *tail = newNode;
    }
    else {
        (*tail)->next = newNode;
        newNode->pre = *tail;
        *tail = newNode;
    }
    
    // 增加医生的当前患者数量
    incrementPatientCount(d);
    
    // 更新挂号记录文件
    rebuildRegistrationFile(*head);
    
    // 输出挂号成功信息
    printf("挂号成功！挂号编号：%s，患者：%s，医生%s，科室：%s，日期：%s %s\n",
        newNode->data.regNo, newNode->data.patientName, newNode->data.doctorName,
        newNode->data.dept, newNode->data.date, newNode->data.time);
  
    return 1;
}
//-------------------------


//-------------------------
//函数名：cancelRegistration
//功能：取消患者的挂号记录
//参数：head - 指向挂号记录链表头部的指针的指针
//      tail - 指向挂号记录链表尾部的指针的指针
//      r - 要取消的挂号记录节点指针
//返回值：成功返回1，失败返回0
int cancelRegistration(Registration** head, Registration** tail,
    Registration* r) {
    
    if (r == NULL) {
        printf("未找到对应记录\n");
        return 0;
    }
   
    // 检查挂号状态，只有"待就诊"状态的挂号才能取消
    if (strcmp(r->data.status, "待就诊") != 0) {
        printf("只有待就诊状态的挂号才能取消\n");
        return 0;
    }
    
    // 查找对应医生并减少其当前患者数量
    Doctor* d = findDoctorByEmpNo(getDoctorHead(), r->data.doctorEmpNo);
    if (d != NULL && d->data.currentPatients > 0) {
        d->data.currentPatients--;
    }
    
    // 修改挂号记录状态为"已取消"
    strcpy(r->data.status, "已取消");
    
    // 更新挂号记录文件
    rebuildRegistrationFile(*head);
    
    printf("取消成功\n");
    

    
    return 1;
}
//-------------------------


//-------------------------
//函数名：completeRegistration
//功能：完成挂号记录（标记为已就诊）
//参数：r - 挂号记录节点指针
//返回值：成功返回1，失败返回0
int completeRegistration(Registration* r) {
    if (r == NULL) {
        printf("挂号信息不能为空\n");
        return 0;
    }
    
    // 检查挂号状态是否为"待就诊"或"就诊中"
    if (strcmp(r->data.status, "待就诊") != 0 &&
        strcmp(r->data.status, "就诊中") != 0) {
        printf("挂号信息状态异常，无法完成就诊\n");
        return 0;
    }
    
    // 修改状态为"已完成"
    strcpy(r->data.status, "已完成");
    
    printf("患者%s就诊完成\n", r->data.patientName);
    
    return 1;
}
//-------------------------


//-------------------------
//函数名：findRegistrationByNo
//功能：根据挂号编号查询
//参数：head - 挂号记录链表头指针
//      regNo - 挂号编号
//返回值：返回找到的挂号记录节点指针，未找到返回NULL
Registration* findRegistrationByNo(Registration* head, char* regNo) {
    Registration* r = head;
    while (r != NULL) {
        if (strcmp(r->data.regNo, regNo) == 0) {          // 比较挂号编号是否匹配
            return r;                                     // 找到则返回该节点指针
        }
        r = r->next;                                      
    }
    return NULL;
}
//-------------------------


//-------------------------
//函数名：findRegistrationsByPatient
//功能：根据患者卡号查询
//参数：head - 挂号记录链表头指针
//      patientCardNo - 患者卡号
//返回值：返回找到的挂号记录节点指针，未找到返回NULL
Registration* findRegistrationsByPatient(Registration* head,
    char* patientCardNo) {
    Registration* r = head;
    while (r != NULL) {
        if (strcmp(r->data.patientCardNo, patientCardNo) == 0) { // 比较患者卡号是否匹配
            return r;                                            // 找到则返回该节点指针
        }
        r = r->next;
    }
    return NULL;
}
//-------------------------


//-------------------------
//函数名：findRegistrationsByDoctor
//功能：根据医生查询
//参数：head - 挂号记录链表头指针
//      doctorEmpNo - 医生工号
//返回值：返回包含符合条件挂号记录的新链表头指针
Registration* findRegistrationsByDoctor(Registration* head,
    char* doctorEmpNo) {
    Registration* r = head;
    Registration* resultHead = NULL;
    Registration* resultTail = NULL;
    
    int count = 0;
    while (r != NULL) {
        if (strcmp(r->data.doctorEmpNo, doctorEmpNo) == 0){
            // 创建新节点并复制数据
            Registration* newNode = (Registration*)malloc(sizeof(Registration));
            if (newNode != NULL) {
                newNode->data = r->data;
                newNode->next = NULL;
                newNode->pre = NULL;
                if (resultHead == NULL){
                    resultHead = newNode;
                    resultTail = newNode;
                }
                else{
                    resultTail->next = newNode;
                    newNode->pre = resultTail;
                    resultTail = newNode;
                }
                count++;
            }
        }
        r = r->next;
    }
    
    return resultHead;
}
//-------------------------


//-------------------------
//函数名：findRegistrationsByDateRange
//功能：按日期范围查询
//参数：head - 挂号记录链表头指针
//      y1 - 起始年份
//      m1 - 起始月份
//      d1 - 起始日期
//      y2 - 结束年份
//      m2 - 结束月份
//      d2 - 结束日期
//返回值：返回包含符合条件挂号记录的新链表头指针
Registration* findRegistrationsByDateRange(Registration* head, int y1,
    int m1, int d1, int y2, int m2, int d2) {
    Registration* r = head;
    Registration* resultHead = NULL;
    Registration* resultTail = NULL;
    
    int count = 0;
    while (r != NULL) {
        int regYear, regMonth, regDay;
        sscanf(r->data.date, "%d-%d-%d", &regYear, &regMonth, &regDay);
        // 使用compareDate函数比较日期
        // 注意：>= 起始日期 且 <= 结束日期
        if (compareDate(regYear, regMonth, regDay, y1, m1, d1) >= 0 &&
            compareDate(regYear, regMonth, regDay, y2, m2, d2) <= 0) {
            Registration* newNode = (Registration*)malloc(sizeof(Registration));
            if (newNode != NULL) {
                newNode->data = r->data;
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
                count++;
            }
        }
        r = r->next;
    }
    
    return resultHead;
}
//-------------------------


//-------------------------
//函数名：listAllRegistrations
//功能：显示所有挂号列表
//参数：head - 挂号记录链表头指针
//返回值：无
void listAllRegistrations(Registration* head) {
    // 1. 检查链表是否为空
    if (head == NULL) {
        printf("暂无挂号信息\n");
        return;
    }

    // 2. 统计记录数量
    int count = 0;
    Registration* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // 3. 输出表头信息
    printf("=== 挂号列表 ===\n");
    printf("%-20s %-20s %-20s %-20s %-20s %-20s\n",
        "挂号编号", "患者姓名", " 医生姓名", "科室", "挂号日期", "状态");

    // 4. 遍历链表输出每条挂号信息
    current = head;                              // 从头节点开始
    while (current != NULL) {
        // 格式化输出当前挂号的各项信息
        printf("%-20s %-20s %-20s %-20s %-20s %-20s\n",
            current->data.regNo,                         // 挂号编号
            current->data.patientName,                   // 患者姓名
            current->data.doctorName,                    // 医生姓名
            current->data.dept,                          // 科室
            current->data.date,                          // 挂号日期
            current->data.status);                       // 就诊状态
        current = current->next;                         // 移动到下一个节点
    }
    
    printf("总计: %d 条记录\n", count);
}
//-------------------------