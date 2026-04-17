//---------------
//床位模块
//---------------

/*
**床位管理**：
床位增删改查
病区管理
床位状态跟踪（空闲/占用/维修）
*/

#ifndef _BED_H_
#define _BED_H_


typedef struct BedData {
    char ward[30];           // 病区
    char bedNo[20];          // 床位号
    char patientCardNo[20];  // 病人卡号（空=无病人）
    char patientName[50];    // 病人姓名
    char status[20];         // 状态（空闲/占用/维修）
} BedData;

// 先声明结构体标签
typedef struct BedNode {
    BedData data;
    // 用 struct BedNode* 声明指针，而不是直接用 Bed*
    Bed* next;
    Bed* pre;
} Bed; // 最后再 typedef 成 Bed

char* allocateBed(Bed* head, char* patientCardNo, char* patientName);    //自动分配床位
void freeBed(Bed* b);                                                    //释放床位
void addBed(BedNode** head, BedNode** tail, char* ward, char* bedNo);    // 添加床位
void delBed(Bed** head, Bed** tail, Bed* b);                                //删除床位
//查找床位
Bed* findBedByNo(Bed* head, char* bedNo);              // 按床位号精确查找
Bed* findBedsByWard(Bed* head, char* ward);            // 按病区查找（返回第一个匹配）
Bed* findAvailableBeds(Bed* head);                      // 查找第一个空闲床位
void getWardStats(BedNode* head, char* ward, int* total, int* occupied);//病区统计
//床位列表
void listAllBeds(Bed** head, Bed** tail, char* ward, char* bedNo);      //床位信息列表
void listBedsByWard(Bed* head, char* ward);                  // 指定病区床位列表


#endif