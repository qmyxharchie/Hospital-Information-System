//---------------
//药品模块
//---------------


#ifndef _MEDICINE_H_
#define _MEDICINE_H_

//药品信息
typedef struct MedicineData {
    char medNo[20];              // 药品编号（如MED001）
    char genericName[50];        // 通用名
    char brandName[50];          // 商品名
    char spec[50];              // 规格
    double price;                // 单价
    int stock;                   // 当前库存
    int minStock;                // 最低库存预警值
} Medicine;

struct MedicineNode {
    Medicine data;
    struct Medicine* next;
    struct Medicine* pre;
};

// 购药记录
typedef struct PurchaseData {
    char purNo[20];              // 购药单号
    char patientCardNo[20];      // 病人卡号
    char medNo[20];              // 药品编号
    int quantity;                 // 购买数量
    double totalCost;            // 总费用
    char date[20];               // 购药日期

    struct Purchase* next;
    struct Purchase* pre;
} Purchase;

struct PurchaseNode {
    Purchase data;
    struct Purchase* next;
    struct Purchase* pre;
};

checkLowStock(Medicine* head);//库存检查

purchaseMedicine(Medicine* head, char* medNo, int quantity);            //购药（扣减库存）

replenishStock(Medicine* head, char* medNo, int quantity);              //补充库存

addPurchaseRecord(Purchase** head, Purchase** tail, ...);               //购药记录（扣减药品库存）







#endif