//--------------
// 病人模块
//--------------

//功能
//病人模块负责管理病人的基本信息：
//-添加新病人（自动生成门诊卡号）
//- 删除病人
//- 修改病人信息
//- 查询病人
//- 列表显示所有病人

#ifndef _PATIENT_H_
#define _PATIENT_H_

#include "registration.h"

typedef struct PatientData {
    char cardNo[20];          // 门诊卡号（唯一标识）
    char name[50];            // 姓名
    int age;                   // 年龄
    char gender[10];          // 性别
    char idCard[20];          // 身份证号
    char phone[15];           // 联系电话
    int isActive;             // 是否住院（0=否，1=是）
} PatientData;

typedef struct Patient {
    PatientData data;
    struct Patient* next;     // 下一个节点
    struct Patient* pre;      // 上一个节点
} Patient;

Patient* getPatientHead(void);                                          // Getter 函数实现
Patient* getPatientTail(void);
void addPatient(Patient** head, Patient** tail,
    char name[], int age, char gender[], char idCard[], char phone[]);  //添加病人
void delPatient(Patient** head, Patient** tail, PatientData p);         //删除病人
int modifyPatient(Patient* head, char* cardNo, PatientData newData);    //修改病人信息

//挂号
void patientViewOwnRegistrations(Registration* regHead, 
    char* patientCardNo);                                               //查看挂号记录
int patientCancelRegistration(Registration** regHead, Registration** regTail,
    char* patientCardNo, char* regNo);                                  //取消自己挂号

//查找
Patient* findPatientByCardNo(Patient* head, char* cardNo);              //按卡号精确查找
Patient* findPatientsByName(Patient* head, char* name);                 //按姓名模糊/精确查找
void listAllPatients(Patient* head);                                    //列表显示

#endif                       
