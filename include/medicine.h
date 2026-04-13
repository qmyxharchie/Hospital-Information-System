//---------------
//药品模块
//---------------


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

typedef struct MedicineNode {
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

typedef struct PurchaseNode {
    Purchase data;
    struct Purchase* next;
    struct Purchase* pre;
} Purchase;

void checkLowStock(Medicine* head);                                      //库存检查

void purchaseMedicine(Medicine* head, char* medNo, int quantity);            //购药（扣减库存）

void replenishStock(Medicine* head, char* medNo, int quantity);              //补充库存

void addPurchaseRecord(Purchase** head, Purchase** tail, ...);               //购药记录（扣减药品库存）


//查询函数
Medicine* findMedicineByNo(Medicine* head, char* medNo);       // 按药品编号查找

Medicine* findMedicineByName(Medicine* head, char* name);     // 按药品名查找

void listAllMedicines(Medicine* head);    //列出所有药品

#endif