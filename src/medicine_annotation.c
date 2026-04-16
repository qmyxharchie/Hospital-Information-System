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
	while (cur->next != NULL)//遍历链表
	{
		if (strcmp(cur->data.genericName, *name) == 0 || strcmp(cur->data.brandName, *name) == 0)
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
	strcpy(pur->data.quantity,quantity );
	strcpy(pur->data.totalCost,totalCost);
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
void listAllMedicines(MedicineNode* head)
{
	Medicine* cur = head;//创建指针以遍历链表
	printf("===药品列表===\n");
	printf("%-20s  %-50s %-50s %-50s %-15s %-15s %-15s\n ",
		"药品编号", "通用名", "商品名", "规格", "单价（元）", "当前库存", "库存预警值");
	while (cur->next != NULL)//输出药品信息
	{
		printf("%-20s %-50s %-50s %-50s %-15f %-15d %-15d",
			cur->data.medNo,
			cur->data.genericName,
			cur->data.brandName,
			cur->data.spec,
			cur->data.price,
			cur->data.stock,
			cur->data.minStock);
		cur = cur->next;
	}
	return;
}
//-------------------------