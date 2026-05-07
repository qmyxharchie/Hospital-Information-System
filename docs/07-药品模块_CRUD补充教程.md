# 药品模块 - CRUD 补充教程

> 本文档补充 `medicine.c` 中缺失的**添加药品、删除药品、修改药品信息**三个核心功能。
> 假设你已阅读 `docs/07-药品模块_.md`，熟悉 `MedicineData` 结构体和现有查询/购药函数。

---

## 1. 模块当前状态

| 功能 | 状态 | 所在函数 |
|------|------|----------|
| 查询药品（按编号） | ✅ 已有 | `findMedicineByNo()` |
| 查询药品（按名称） | ✅ 已有 | `findMedicineByName()` |
| 购药扣库存 | ✅ 已有 | `purchaseMedicine()` |
| 补充库存 | ✅ 已有 | `replenishStock()` |
| 库存预警 | ✅ 已有 | `checkLowStock()` |
| 购药记录 | ✅ 已有 | `addPurchaseRecord()` |
| **添加药品** | ❌ **缺失** | 需补充 `addMedicine()` |
| **删除药品** | ❌ **缺失** | 需补充 `delMedicine()` |
| **修改药品信息** | ❌ **缺失** | 需补充 `modifyMedicine()` |

---

## 2. 头文件声明补充

在 `include/medicine.h` 中，现有声明之后追加以下内容：

```c
// 以下为新增声明 —— 添加/删除/修改药品
void addMedicine(Medicine** head, Medicine** tail,
    char genericName[], char brandName[], char spec[], double price, int stock, int minStock);
    // 添加新药品（自动生成 medNo）

void delMedicine(Medicine** head, Medicine** tail, char* medNo);
    // 删除指定编号的药品

int modifyMedicine(Medicine* head, char* medNo, MedicineData newData);
    // 修改药品信息，返回 1=成功，0=失败
```

> **命名说明**：沿用团队命名风格，`addXxx` / `delXxx` / `modifyXxx`，参数类型与 `patient.h` / `doctor.h` 保持一致。

---

## 3. 添加药品 — addMedicine()

### 3.1 实现思路

1. **分配内存**：`malloc(sizeof(Medicine))`
2. **初始化指针**：`next = pre = NULL`
3. **生成唯一编号**：`generateUniqueId("MED", id)`
4. **填充数据域**：复制通用名、商品名、规格、单价、库存、最低库存
5. **尾插法**插入链表
6. **写回文件**：`rebuildMedicineFile(*head)`
7. **输出成功提示**

### 3.2 完整代码

```c
//-------------------------
// 以下为添加药品函数
// 功能：向药品链表中添加一个新药品节点
// 参数：head - 指向链表头指针的指针，tail - 指向链表尾指针的指针
//      genericName - 通用名，brandName - 商品名，spec - 规格
//      price - 单价，stock - 初始库存，minStock - 最低库存预警值
//-------------------------
void addMedicine(Medicine** head, Medicine** tail,
    char genericName[], char brandName[], char spec[],
    double price, int stock, int minStock)
{
    // 1. 为新节点分配内存空间
    Medicine* newNode = (Medicine*)malloc(sizeof(Medicine));

    // 2. 初始化新节点的指针域，防止野指针
    newNode->next = newNode->pre = NULL;

    // 3. 生成唯一的药品编号
    char id[20];
    generateUniqueId("MED", id);                        // 生成以"MED"开头的唯一ID

    // 4. 保存药品具体信息到新节点的数据域
    strcpy(newNode->data.medNo, id);                    // 药品编号（自动生成）
    strcpy(newNode->data.genericName, genericName);       // 通用名
    strcpy(newNode->data.brandName, brandName);           // 商品名
    strcpy(newNode->data.spec, spec);                     // 规格
    newNode->data.price = price;                          // 单价
    newNode->data.stock = stock;                          // 当前库存
    newNode->data.minStock = minStock;                    // 最低库存预警值

    // 5. 将新节点插入到链表尾部
    if (*tail == NULL) {
        // 如果链表为空，则新节点既是头节点也是尾节点
        *head = newNode;
        *tail = newNode;
    }
    else {
        // 如果链表不为空，则将新节点连接到尾节点之后
        (*tail)->next = newNode;
        newNode->pre = *tail;
        *tail = newNode;
    }

    // 6. 将更新后的链表数据保存到文件
    rebuildMedicineFile(*head);

    // 7. 输出添加成功的提示信息
    printf("[OK] 药品添加成功，编号为 %s。\n", id);
}
```

---

## 4. 删除药品 — delMedicine()

### 4.1 实现思路

1. **空链表检查**：`if (*head == NULL)` 直接返回
2. **按编号查找**：遍历链表，比较 `medNo`
3. **找不到则报错**
4. **删除节点**：分四种情况（唯一节点 / 头节点 / 尾节点 / 中间节点）
5. **`free(cur)`** 释放内存
6. **写回文件**：`rebuildMedicineFile(*head)`

> **注意**：删除药品前，应确保该药品**没有被任何购药记录引用**。本实现中简化为直接删除，实际生产环境需增加外键检查。

### 4.2 完整代码

```c
//-------------------------
// 以下为删除药品函数
// 功能：从药品链表中删除指定编号的药品节点
// 参数：head - 指向链表头指针的指针，tail - 指向链表尾指针的指针
//      medNo - 要删除的药品编号
//-------------------------
void delMedicine(Medicine** head, Medicine** tail, char* medNo)
{
    // 1. 检查链表是否为空
    if (*head == NULL) {
        printf("暂无药品数据。\n");
        return;
    }

    // 2. 遍历链表寻找要删除的节点
    Medicine* cur = *head;
    Medicine* pre = NULL;
    while (cur != NULL) {
        if (strcmp(cur->data.medNo, medNo) == 0) {      // 当药品编号匹配时
            break;                                      // 跳出循环
        }
        pre = cur;                                      // 记录前一个节点
        cur = cur->next;                                // 移动到下一个节点
    }

    // 3. 检查是否找到要删除的节点
    if (cur == NULL) {
        printf("未找到编号为 %s 的药品，无法删除。\n", medNo);
        return;
    }

    // 4. 根据要删除节点的位置执行不同的删除操作
    if (cur == *head && cur == *tail) {
        *head = NULL;                                   // 1. 链表只有一个节点
        *tail = NULL;
    }
    else if (cur == *head) {
        *head = cur->next;                              // 2. 删除头节点
        (*head)->pre = NULL;
    }
    else if (cur == *tail) {
        *tail = cur->pre;                               // 3. 删除尾节点
        (*tail)->next = NULL;
    }
    else {
        pre->next = cur->next;                          // 4. 删除中间节点
        cur->next->pre = pre;
    }

    // 5. 释放被删除节点的内存
    free(cur);

    // 6. 输出删除成功的提示信息
    printf("[OK] 药品 %s 已删除。\n", medNo);

    // 7. 将更新后的链表数据保存到文件
    rebuildMedicineFile(*head);
}
```

---

## 5. 修改药品信息 — modifyMedicine()

### 5.1 实现思路

1. **按编号查找**：调用 `findMedicineByNo()`
2. **找不到则返回 0**
3. **显示当前信息**
4. **更新数据域**：用 `safeStringCopy()` 复制字符串字段，数值字段直接赋值
5. **写回文件**：`rebuildMedicineFile(head)`
6. **返回 1**

### 5.2 完整代码

```c
//-------------------------
// 以下为修改药品信息函数
// 功能：修改指定药品编号的信息
// 参数：head - 链表头指针，medNo - 要修改的药品编号，newData - 新的药品数据
// 返回值：1 - 修改成功，0 - 修改失败（未找到）
//-------------------------
int modifyMedicine(Medicine* head, char* medNo, MedicineData newData)
{
    Medicine* target = NULL;

    // 1. 根据药品编号查找要修改的节点
    target = findMedicineByNo(head, medNo);

    // 2. 检查是否找到要修改的药品
    if (target == NULL) {
        printf("未找到编号为 %s 的药品。\n", medNo);
        return 0;                                       // 返回 0 表示修改失败
    }

    // 3. 显示当前药品信息
    printf("当前药品信息为：\n");
    printf("药品编号：%s\n", target->data.medNo);
    printf("通用名：%s\n", target->data.genericName);
    printf("商品名：%s\n", target->data.brandName);
    printf("规格：%s\n", target->data.spec);
    printf("单价：%.2f\n", target->data.price);
    printf("当前库存：%d\n", target->data.stock);
    printf("最低库存：%d\n", target->data.minStock);

    // 4. 更新药品信息（编号 medNo 保持不变）
    safeStringCopy(target->data.genericName, newData.genericName, 50);
    safeStringCopy(target->data.brandName, newData.brandName, 50);
    safeStringCopy(target->data.spec, newData.spec, 50);
    target->data.price = newData.price;
    target->data.stock = newData.stock;
    target->data.minStock = newData.minStock;

    // 5. 将更新后的链表数据保存到文件
    rebuildMedicineFile(head);

    // 6. 输出修改成功的提示信息
    printf("[OK] 药品 %s 信息修改成功！\n", medNo);

    return 1;                                           // 返回 1 表示修改成功
}
```

---

## 6. UI 菜单联动修改

在 `docs/18-1-UI模块开发教程代码.md` 的 `showMedicineManagement()` 中，当前选项 1~6 为查询/补库存/购药/预警/列表。补齐 CRUD 后，建议将菜单改为：

```c
printf("\n-------- 药品管理 --------\n");
printf("1. 添加药品     5. 购药登记\n");
printf("2. 删除药品     6. 补充库存\n");
printf("3. 修改药品信息 7. 库存预警\n");
printf("4. 查询药品     8. 显示所有药品\n");
printf("0. 返回上级菜单\n");
```

并在 `switch` 中补充 `case 1` / `case 2` / `case 3`：

```c
case 1: {  // 添加
    char genericName[50], brandName[50], spec[50];
    double price;
    int stock, minStock;
    printf("通用名: ");     scanf("%49s", genericName);
    printf("商品名: ");     scanf("%49s", brandName);
    printf("规格: ");       scanf("%49s", spec);
    printf("单价: ");       scanf("%lf", &price);
    printf("初始库存: ");   scanf("%d", &stock);
    printf("最低库存: ");   scanf("%d", &minStock);
    addMedicine(&g_medHead, &g_medTail,
                genericName, brandName, spec, price, stock, minStock);
    break;
}
case 2: {  // 删除
    printf("请输入要删除的药品编号: ");
    scanf("%19s", medNo);
    delMedicine(&g_medHead, &g_medTail, medNo);
    break;
}
case 3: {  // 修改
    printf("请输入要修改的药品编号: ");
    scanf("%19s", medNo);
    Medicine* m = findMedicineByNo(g_medHead, medNo);
    if (m) {
        MedicineData newData;
        strcpy(newData.medNo, medNo);                   // 编号不变
        printf("新通用名: ");     scanf("%49s", newData.genericName);
        printf("新商品名: ");     scanf("%49s", newData.brandName);
        printf("新规格: ");       scanf("%49s", newData.spec);
        printf("新单价: ");       scanf("%lf", &newData.price);
        printf("新库存: ");       scanf("%d", &newData.stock);
        printf("新最低库存: ");   scanf("%d", &newData.minStock);
        modifyMedicine(g_medHead, medNo, newData);
    } else {
        printf("[ERROR] 未找到该药品！\n");
    }
    break;
}
```

---

## 7. 附：现有 listAllMedicines() 的两处 Bug

当前 `src/medicine.c` 中的 `listAllMedicines()` 存在以下问题，建议同步修复：

| Bug | 位置 | 说明 | 修复 |
|-----|------|------|------|
| 参数类型错误 | 函数签名 | `MedicineNode* head` 应为 `Medicine* head` | 改为 `Medicine* head` |
| 遍历漏尾节点 | `while` 条件 | `while (cur->next != NULL)` 会跳过最后一个节点 | 改为 `while (cur != NULL)` |
| `printf` 后缺少换行 | 输出语句 | 行尾 `%-15d` 后直接接 `cur = cur->next`，无 `\n` | 在格式串末尾加 `\n` |
| `printf` 末尾多余分号 | 第184行 | `cur->data.minStock;` 后 `cur = cur->next` 不在循环体内 | 补全 `{ }` 花括号 |

**修复后完整代码**：

```c
void listAllMedicines(Medicine* head)
{
    if (head == NULL) {
        printf("暂无药品信息\n");
        return;
    }
    Medicine* cur = head;
    printf("=== 药品列表 ===\n");
    printf("%-20s  %-50s %-50s %-50s %-15s %-15s %-15s\n",
        "药品编号", "通用名", "商品名", "规格", "单价（元）", "当前库存", "库存预警值");
    while (cur != NULL) {
        printf("%-20s %-50s %-50s %-50s %-15.2f %-15d %-15d\n",
            cur->data.medNo,
            cur->data.genericName,
            cur->data.brandName,
            cur->data.spec,
            cur->data.price,
            cur->data.stock,
            cur->data.minStock);
        cur = cur->next;
    }
}
```

---

> **验证方式**：补齐上述函数后，在 `main.c` 中调用 `addMedicine()` → `listAllMedicines()` → `modifyMedicine()` → `delMedicine()`，观察 `data/medicine.txt` 是否正确增删改。
