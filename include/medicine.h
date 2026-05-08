//---------------
//药品模块
//---------------



/*
药品模块负责管理药品的库存信息：
- 添加新药品
- 删除药品
- 修改药品信息
- 补充库存
- 库存预警检查
- 购药（扣减库存）
- 购药记录管理
*/

#ifndef _MEDICINE_H_
#define _MEDICINE_H_

//药品信息
typedef struct MedicineData {
    char medNo[20];     // 药品编号（唯一ID，如"MED001"）
    char genericName[50];  // 通用名（如"阿莫西林"）
    char brandName[50];    // 商品名（如"阿莫西林胶囊"）
    char spec[50];         // 规格（如"0.5g*24粒"）
    double price;          // 单价（元）
    int stock;             // 当前库存
    int minStock;          // 最低库存预警值
} MedicineData;   // 注：团队头文件中 Medicine 即为 MedicineData

typedef struct Medicine {
    MedicineData data;          // 药品数据
    struct Medicine* next;
    struct Medicine* pre;
} Medicine;

//购药记录
typedef struct PurchaseData {
    char purNo[20];         // 购药单号
    char patientCardNo[20];  // 病人卡号
    char medNo[20];         // 药品编号
    int quantity;           // 购买数量
    double totalCost;       // 总费用
    char date[20];          // 购药日期
} PurchaseData;

typedef struct Purchase {
    PurchaseData data;
    struct Purchase* next;
    struct Purchase* pre;
} Purchase;

void checkLowStock(Medicine* head);                                          //库存检查
void purchaseMedicine(Medicine* head, char* medNo, int quantity);            //购药（扣减库存）
void replenishStock(Medicine* head, char* medNo, int quantity);              //补充库存
void addPurchaseRecord(Purchase** head, Purchase** tail, char patientCardNo[20],
    char medNo[20], int quantity, double totalCost, char date[20]);   //购药记录（扣减药品库存）
//查询药品
Medicine* findMedicineByNo(Medicine* head, char* medNo);       // 按药品编号查找
Medicine* findMedicineByName(Medicine* head, char* name);     // 按药品名查找
void listAllMedicines(Medicine* head);    //列出所有药品
void addMedicine(Medicine** head, Medicine** tail, char genericName[], char brandName[],
    char spec[], double price, int stock, int minStock);//添加药品
void delMedicine(Medicine** head, Medicine** tail, char* medNo);//删除指定编号的药品
int modifyMedicine(Medicine* head, char* medNo, MedicineData newData); // 修改药品信息，返回 1=成功，0=失败

#endif