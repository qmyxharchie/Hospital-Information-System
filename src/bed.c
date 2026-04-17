#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "utils.h"
#include "bed.h"

//-------------------------
//以下为自动分配床位函数
char* allocateBed(Bed* head, char* patientCardNo, char* patientName)
{
	Bed* cur = head;
	while (cur != NULL)
	{
		if (strcmp(cur->data.status, "空闲") == 0)
		{
			strcpy(cur->data.patientCardNo, patientCardNo);
			strcpy(cur->data.patientName, patientName);
			strcpy(cur->data.status, "占用");
			printf("分配床位成功");
			return cur-data.bedNo;
		}
	}
	printf("暂无空闲床位");
	return "NULL";
}
//-------------------------

//-------------------------
//以下为释放床位函数
void freeBed(Bed* b)
{
	if (b == NULL)
	{
		return;
	}
	strcpy(b->data.patientCardNo, "空");
	strcpy(b->data.patientName, "空");
	strcpy(b->data.status, "空闲");
	printf("床位释放成功");
	return;
}
//-------------------------

//-------------------------
//以下为床位添加/删除函数
//添加床位
void addBed(Bed** head, Bed** tail, char* ward, char* bedNo)
{
	Bed* b = (Bed*)malloc(sizeof(Bed));
	if (!b)//创建新节点
	{
		printf("内存分配失败\n");
		return;
	}
	b->data.patientCardNo = "空";
	b->data.status = "空闲";
	b->next = b->pre = NULL:
	if (*tail == NULL)//利用尾插法添加节点
	{
		*head = b;
		*tail = b;
	}
	else {
		(*tail)->next = b;
		b->pre = *tail;
		*tail = b;
	}
	rebuildBedFile(*head);
	return;
}
//删除函数
void delBed(Bed** head, Bed** tail, Bed* b)
{
	// 1. 边界检查：空链表或要删除的节点为空，直接返回
	if (*head == NULL || b == NULL) {
		return;
	}

	// 2. 占用状态的床位不能删除
	if (strcmp(b->data.status, "占用") == 0) {
		printf("错误：床位【%s】处于占用状态，无法删除！\n", b->data.bedNo);
		return;
	}

	// 3. 情况1：要删除的是头节点
	if (b == *head) {
		*head = b->next;        // 头指针后移
		if (*head != NULL) {
			(*head)->pre = NULL; // 新头节点的前驱置空
		}
		else {
			// 链表删空了，尾指针也要置空
			*tail = NULL;
		}
	}
	// 4. 情况2：要删除的是尾节点
	else if (b == *tail) {
		*tail = b->pre;         // 尾指针前移
		(*tail)->next = NULL;   // 新尾节点的后继置空
	}
	// 5. 情况3：要删除的是中间节点
	else {
		b->pre->next = b->next; // 前驱节点的后继指向当前节点的后继
		b->next->pre = b->pre;  // 后继节点的前驱指向当前节点的前驱
	}

	// 6. 释放节点内存
	free(b);
	rebuildBedFile(head);
}
//-------------------------

//-------------------------
//以下为查询函数
//按床位号精确查询
Bed* findBedByNo(Bed* head, char* bedNo)
{
	Bed* b = head;
	while (b != = NULL) {
		if (strcmp(b->data.bedNo, bedNo) == 0)
		{
			return h;
		}
		b = b->next;
	}
	return NULL;
}

//以下为按照病区查找
Bed* findBedsByWard(Bed* head, char* ward)
{
	Bed* resultHead = NULL;  // 结果链表头
	Bed* resultTail = NULL;  // 结果链表尾
	Bed* p = head;

	while (p != NULL)
	{
		// 匹配病区
		if (strcmp(p->data.ward, ward) == 0)
		{
			// 创建新节点，复制数据
			Bed* newNode = (Bed*)malloc(sizeof(Bed));
			newNode->data = p->data;  // 复制数据
			newNode->next = NULL;
			newNode->pre = NULL;

			// 加入结果链表
			if (resultHead == NULL)
			{
				resultHead = newNode;
				resultTail = newNode;
			}
			else
			{
				resultTail->next = newNode;
				newNode->pre = resultTail;
				resultTail = newNode;
			}
		}
		p = p->next;
	}

	return resultHead;  // 返回所有匹配的链表
}

//查找第一个空床位
Bed* findAvailableBeds(Bed* head)
{
	Bed* b = head;
	while (b != = NULL) {
		if (strcmp(b->data.status, "空") == 0)
		{
			return h;
		}
		b = b->next;
	}
	return NULL;
}
//-------------------------

//-------------------------
//以下为病区统计函数
void getWardStats(Bed* head, char* ward, int* total, int* occupied)
{
	*total = 0;
	*occupied = 0;
	Bed* b = head;
	while (b != NULL)
	{
		if (strcmp(b->data.ward, ward) == 0)
		{
			(*total)++;
			if (strcmp(b->data.status, "占用") == 0)
			{
				(*occupied)++;
			}
		}
		b = b->next;
	}
	return;
}
//-------------------------

//-------------------------
//以下为列表显示函数
//所有床位列表
void listAllBeds(Bed* head)
{
	Bed* b = head;
	printf("===床位显示列表===\n");
	printf("%-30s %-20s %-20s %-50s %-20s",
		"病区", "床位号", "病人卡号", "病人姓名", "床位状态");
	while (b->next != NULL)
	{
		printf("%-30s %-20s %-20s %-50s %-20s\n",
			b->data.ward,
			b->data.bedNo,
			b->data.patientCardNo,
			b->data.patientName,
			b->data.status);
		b = b->next;
	}
	return;
}
//指定病区床位列表
void listBedsByWard(Bed* head, char* ward)
{
	printf("===指定病区床位显示列表===\n");
	printf("%-30s %-20s %-20s %-50s %-20s\n",
		"病区", "床位号", "病人卡号", "病人姓名", "床位状态");
	Bed* b = findBedsByWard(head, ward);
	while (b->next != NULL)
	{
		printf("%-30s %-20s %-20s %-50s %-20s\n",
			b->data.ward,
			b->data.bedNo,
			b->data.patientCardNo,
			b->data.patientName,
			b->data.status);
		b = b->next;
	}
	return;
}
//-------------------------