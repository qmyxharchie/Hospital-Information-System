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
typedef struct DoctorData {
    char empNo[20];              // 工号（如DOC001）
    char name[50];               // 姓名
    char dept[50];               // 科室
    char schedule[100];          // 出诊时间
    int maxPatients;             // 每日最大接诊数
    int currentPatients;         // 今日已接诊数
} DoctorData;
typedef struct Doctor {
    DoctorData data;
    struct Doctor* next;
    struct Doctor* pre;
} Doctor;

void addDoctor(Doctor* head, Doctor* tail,
    char name[], char dept[], char schedule[], int maxPatients);        //添加医生
int canAcceptPatient(Doctor d);                                         //医生接诊检查
void incrementPatientCount(Doctor d);
void resetDailyPatients(Doctor head);                                   //重置每日接诊数
void getDeptStats(Doctor head, char* dept, int* count);                 //科室统计

#endif // !_DOCTOR_H_