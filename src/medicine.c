#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medicine.h"
#include "file_io.h"
#include "utils.h"

//-------------------------
//以下为库存检查预警函数
// 功能：检查药品库存是否低于最低库存并发出预警
// 参数：head - 药品链表头指针
void checkLowStock(Medicine* head)
{
	Medicine* cur = head;
	int n = 0;
	while (cur != NULL)//遍历链表
	{
		if (cur->data.stock <= cur->data.minStock)//判断库存小于是否小于最低库存
		{
			n++;
			printf("药品名：%s   当前库存：%d   最低库存：%d\n", cur->data.medNo, cur->data.stock, cur->data.minStock);//输出相关信息	
		}
		cur = cur->next;
	}
	printf("预警药品数量  共 %d 种\n", n);//输出预警药品总数量
}
//-------------------------

//-------------------------
//以下为购药（扣减库存）函数
// 功能：购买药品并扣减相应库存
// 参数：head - 药品链表头指针，medNo - 药品编号，quantity - 购买数量
void purchaseMedicine(Medicine* head, char* medNo, int quantity)
{
	Medicine* cur = findMedicineByNo(head, medNo);//创建一个指针获得药品信息
	if (cur == NULL)//判断是否能够查询到药品信息
	{
		printf("没有查询到药品信息\n");
		return;
	}
	else {//能够查询到药品信息
		if (cur->data.stock < quantity)//判断药品库存是否足够进行扣减
		{
			printf("该药品库存不足\n");
			return;
		}
		else {//药品库存足够进行扣减
			cur->data.stock -= quantity;//扣减库存
			printf("购药成功\n");
			printf("当前库存：%d\n", cur->data.stock);
			if (cur->data.stock <= cur->data.minStock)
			{//如果扣减库存之后库存低于最低库存，进行预警
				checkLowStock(head);
			}
			rebuildMedicineFile(head);//保存购药记录
		}
	}
	return;
}
//-------------------------

//-------------------------
//以下为查询药品函数

//按照编号查询药品
// 功能：根据药品编号查找药品信息
// 参数：head - 药品链表头指针，medNo - 要查找的药品编号
// 返回值：找到的药品节点指针，未找到返回NULL
Medicine* findMedicineByNo(Medicine* head, char* medNo)
{
	Medicine* cur = head;//创建指针进行遍历
	while (cur != NULL)//遍历链表
	{
		if (strcmp(cur->data.medNo, medNo) == 0)
		{//比对输入的药品编号是否与药品库中的药品编号相同
			return cur;
		}
		cur = cur->next;
	}
	return NULL;//没有查询到药品信息，返回NULL
}

//按照药品名查询药品
// 功能：根据药品名称（通用名或商品名）查找药品信息
// 参数：head - 药品链表头指针，name - 要查找的药品名称
// 返回值：找到的药品节点指针，未找到返回NULL
Medicine* findMedicineByName(Medicine* head, char* name)
{
	Medicine* cur = head;//创建指针进行遍历
	while (cur!= NULL)//遍历链表
	{
		if (strcmp(cur->data.genericName, name) == 0 || strcmp(cur->data.brandName, *name) == 0)
		{//比对输入的药品名是否与通用名或商品名相同
			return cur;
		}
		cur = cur->next;
	}
	return NULL;//如果没有查询到药品信息，返回NULL
}
//-------------------------

//-------------------------
//以下为补充库存函数
// 功能：为指定药品补充库存
// 参数：head - 药品链表头指针，medNo - 药品编号，quantity - 补充数量
void replenishStock(Medicine* head, char* medNo, int quantity)
{
	Medicine* cur = findMedicineByNo(head, medNo);//创建指针获取药品信息
	if (cur == NULL)//判断是否查询到了药品
	{
		printf("没有查询到药品信息\n");
		return;
	}
	else {//补充库存
		cur->data.stock += quantity;
		printf("补充药品库存成功\n");
		rebuildMedicineFile(head);
	}
	return;
}
//-------------------------

//-------------------------
//以下为购药记录创建函数
// 功能：创建购药记录并保存到链表
// 参数：head - 指向购药记录链表头指针的指针，tail - 指向购药记录链表尾指针的指针
//      patientCardNo - 病人卡号，medNo - 药品编号，quantity - 购买数量
//      totalCost - 总费用，date - 购药日期
void addPurchaseRecord(Purchase** head, Purchase** tail,char patientCardNo[20],
	char medNo[20],int quantity,double totalCost,char date[20])
{
	Purchase* pur = (Purchase*)malloc(sizeof(Purchase));//创建新节点并分配内存
	if (!pur)
	{
		printf("内存分配失败\n");
		return;
	}
	pur->next = pur->pre = NULL;
	char id[20];
	generateUniqueId("PUR", id);//生成唯一ID
	//向节点中输入相关数据
	strcpy(pur->data.purNo, id);
	strcpy(pur->data.patientCardNo, patientCardNo);
	strcpy(pur->data.medNo,medNo );
	pur->data.quantity = quantity;
	pur->data.totalCost = totalCost;
	strcpy(pur->data.date, date);

	if (*tail == NULL)//利用尾插法添加节点
	{
		*head = pur;
		*tail = pur;  
	}
	else {
		(*tail)->next = pur;
		pur->pre = *tail;
		*tail = pur;
	}
	purchaseMedicine(*head, medNo, quantity);//购药时利用购药函数扣减库存
	rebuildPurchaseFile(*head);//保存购药记录
	return;
}
//-------------------------

//-------------------------
//以下为药品列表函数
// 功能：列出所有药品信息
// 参数：head - 药品链表头指针
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
//-------------------------

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