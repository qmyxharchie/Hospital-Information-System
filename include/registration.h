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

typedef struct RegistrationData {
    char regNo[20];              // 挂号单号（如REG001）
    char patientCardNo[20];      // 病人卡号
    char patientName[50];        // 病人姓名
    char doctorEmpNo[20];        // 医生工号
    char doctorName[50];         // 医生姓名
    char dept[50];               // 科室
    char date[20];               // 挂号日期
    char status[20];             // 状态（待就诊/已完成/已取消）
} RegistrationData;

typedef struct Registration {
    RegistrationData data;
    struct Registration* next;
    struct Registration* pre;
} Registration;

int addRegistration(Registration** head, Registration** tail,
    char* patientCardNo, char* patientName,
    char* doctorEmpNo, char* doctorName, char* dept);                   //挂号
int cancelRegistration(Registration** head, Registration** tail,
    Registration* r);                                                   //取消挂号
int completeRegistration(Registration* r);                              //完成就诊                               
//查询函数
Registration* findRegistrationByNo(Registration* head, char* regNo);    //按挂号单号查询
Registration* findRegistrationsByPatient(Registration* head, 
    char* patientCardNo);                                               //按病人卡号查询
Registration* findRegistrationsByDoctor(Registration* head, 
    char* doctorEmpNo);                                                 //按医生查询
Registration* findRegistrationsByDateRange(Registration* head, int y1, 
    int m1, int d1, int y2, int m2, int d2);                            //按时间范围查询
void listAllRegistrations(Registration* head);                          //列表显示

#endif // !_REGISTRATION_H_