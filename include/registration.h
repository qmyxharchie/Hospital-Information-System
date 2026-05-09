//-----------------
// 挂号记录管理模块
//-----------------

/*挂号模块负责管理病人的挂号记录：
- 病人挂号（自动检查医生名额）
- 取消挂号
- 完成就诊
- 时间范围查询
*/

/*业务规则

1. 病人必须存在
2. 医生必须有剩余名额
3. 同一医生同时只能有一个待就诊挂号
*/

#ifndef _REGISTRATION_H_
#define _REGISTRATION_H_

#include "login.h"

typedef enum {
    PENDING = 0,     // 待就诊
    IN_PROGRESS = 1, // 就诊中
    COMPLETED = 2,   // 已完成
    CANCELLED = 3    // 已取消
} RegStatus;

typedef struct RegistrationData {
    char regNo[20];              // 挂号单号
    char patientCardNo[20];      // 病人卡号
    char patientName[50];        // 病人姓名
    char doctorEmpNo[20];        // 医生工号
    char doctorName[50];         // 医生姓名
    char dept[50];               // 科室
    char date[20];               // 挂号日期（格式：YYYY-MM-DD）
    char time[20];               // 挂号时间（格式：HH:MM:SS）
    char appointmentDate[20];    // 预约日期（格式：YYYY-MM-DD，预约挂号=患者指定日期；现场挂号=当天日期）
    char appointmentTime[20];    // 预约时间（格式：HH:MM，预约挂号=患者指定时段；现场挂号=当前时间）
    RegStatus status;            // 挂号状态（PENDING/IN_PROGRESS/COMPLETED/CANCELLED）
    UserRole createdBy;          // 挂号方式（PATIENT-预约挂号, NURSE-现场挂号）
    float consultationFee;       // 诊疗费用
    char remarks[100];           // 备注信息
} RegistrationData;

typedef struct Registration {
    RegistrationData data;
    struct Registration* next;
    struct Registration* pre;
} Registration;

//挂号操作
int addRegistration(Registration** head, Registration** tail,
    char* patientCardNo, char* patientName,
    char* doctorEmpNo, char* doctorName, char* dept,
    char* appointmentDate, char* appointmentTime, UserRole createdBy);  //添加挂号记录
int cancelRegistration(Registration** head, Registration** tail,
    Registration* r);                                                   //取消挂号
int completeRegistration(Registration* r);                              //完成就诊                               

Registration* buildWaitingQueue(Registration* head,
    char* doctorEmpNo, char* today);                                    //生成候诊队列
void listWaitingQueue(Registration* queueHead, char* doctorName);       //显示指定医生的候诊队列
int callNextPatient(Registration** head, Registration** tail,
    char* doctorEmpNo);                                                 //医生叫号


//查询函数
Registration* findRegistrationByNo(Registration* head, char* regNo);    //按挂号单号查询
Registration* findRegistrationsByPatient(Registration* head,char* patientCardNo);                                               //按病人卡号查询
Registration* findRegistrationsByDoctor(Registration* head,char* doctorEmpNo);                                                 //按医生查询
Registration* findRegistrationsByDateRange(Registration* head, int y1,int m1, int d1, int y2, int m2, int d2);                            //按时间范围查询
Registration* findRegistrationsByStatus(Registration* head,RegStatus status);                                                  //按状态查询

void listAllRegistrations(Registration* head);                          //列表显示

#endif // !_REGISTRATION_H_