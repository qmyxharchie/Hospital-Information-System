//-----------------
// 医生信息管理模块
//-----------------

/*医生模块负责管理医生的基本信息，包括：
- 添加新医生（自动生成工号）
- 删除医生
- 修改出诊信息
- 查询医生
- 医生接诊数量限制*/

#ifndef _DOCTOR_H_
#define _DOCTOR_H_

#include "registration.h"

typedef struct DoctorData {
    char empNo[20];              // 工号
    char name[50];               // 姓名
    char dept[50];               // 科室
    int maxPatients;             // 每日最大接诊数
    int currentPatients;         // 今日已接诊数
    char ownerUsername[50];      // 绑定的用户账号（"无"=未绑定）
} DoctorData;
typedef struct Doctor {
    DoctorData data;
    struct Doctor* next;
    struct Doctor* pre;
} Doctor;

Doctor* getDoctorHead(void);                                            // Getter 函数实现
Doctor* getDoctorTail(void);

void addDoctor(Doctor** head, Doctor** tail,
    char name[], char dept[], int maxPatients,
    const char* ownerUsername);                                         //添加医生
void delDoctor(Doctor** head, Doctor** tail, DoctorData d);             //删除医生
int modifyDoctor(Doctor* head, char* empNo, DoctorData newData);        //修改医生信息

//查询医生
Doctor* findDoctorByEmpNo(Doctor* head, char* empNo);                   //按工号精确查找
Doctor* findDoctorsByName(Doctor* head, char* name);                    //按姓名查找
Doctor* findDoctorsByDept(Doctor* head, char* dept);                    //按科室查找（返回所有匹配）
Doctor* findDoctorByOwner(Doctor* head, const char* ownerUsername);     //按绑定账号查找

//挂号
void doctorViewWaitingList(Registration* regHead, char* doctorEmpNo);   //医生查看候诊列表
void doctorCallNextPatient(Registration** regHead, Registration** regTail,
    char* doctorEmpNo);                                                 //医生叫号入口

//医生接诊检查
int canAcceptPatient(Doctor* d);                                        //医生是否还能接诊（1=能, 0=不能）
void incrementPatientCount(Doctor* d);                                  //挂号成功后接诊数+1
void resetDailyPatients(Doctor* head);                                  //重置每日接诊数
void getDeptStats(Doctor * head, char* dept, char* date, int* count);   //科室统计
void listAllDoctors(Doctor* head);                                      //列表显示

#endif 