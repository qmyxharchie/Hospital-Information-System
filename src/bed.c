#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"  // 文件输入输出模块
#include "utils.h"    // 工具函数模块
#include "bed.h"      // 床位模块
#include "ui.h"

//-------------------------
//函数名：allocateBed
//功能：自动为患者分配一个空闲床位
//参数：head - 床位链表头指针
//     patientCardNo - 患者卡号
//     patientName - 患者姓名
//返回值：返回分配的床位号，若无可用床位则返回"NULL"
char* allocateBed(Bed* head, char* patientCardNo, char* patientName)
{
	Bed* cur = head;
	while (cur != NULL)
	{
		if (strcmp(cur->data.status, "空闲") == 0)  // 查找空闲床位
		{
			strcpy(cur->data.patientCardNo, patientCardNo);  // 登记患者卡号
			strcpy(cur->data.patientName, patientName);      // 登记患者姓名
			strcpy(cur->data.status, "占用");                // 修改状态为占用
			printf("分配床位成功\n");
			return cur->data.bedNo;  // 返回床位号
		}
		cur = cur->next;
	}
	printf("没有可用的床位\n");
	return "NULL";
}
//-------------------------


//-------------------------
//函数名：freeBed
//功能：将床位状态重置为空闲状态
//参数：b - 床位节点指针
//返回值：无
void freeBed(Bed* b)
{
	if (b == NULL)
	{
		return;
	}
	strcpy(b->data.patientCardNo, "无");      // 清除患者卡号
	strcpy(b->data.patientName, "无");        // 清除患者姓名
	strcpy(b->data.status, "空闲");           // 设置状态为空闲
	printf("床位释放成功\n");
	return;
}
//-------------------------


//-------------------------
//函数名：addBed
//功能：添加床位
//参数：head - 指向床位链表头指针的指针
//     tail - 指向床位链表尾指针的指针
//     ward - 病房号
//     bedNo - 床位号
//返回值：无
void addBed(Bed** head, Bed** tail, char* ward)
{
	Bed* b = (Bed*)malloc(sizeof(Bed));
	if (!b)//分配新节点
	{
		printf("[ERROR] 床位申请失败\n");
		return;
	}
	strcpy(b->data.patientCardNo, "无");  // 初始化患者卡号为"无"
	strcpy(b->data.patientName, "无");    // 初始化患者姓名为"无"
	strcpy(b->data.status, "空闲");       // 初始化状态为空闲
	strcpy(b->data.ward, ward);           // 设置病房号
	char id[20];
	generateUniqueId("BED", id);
	strcpy(b->data.bedNo, id);         // 设置床位号
	b->next = b->pre = NULL;              // 初始化前后指针为空
	
	if (*tail == NULL)//如果尾指针为空则插入节点
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
//-------------------------


//-------------------------
//函数名：delBed
//功能：删除床位
//参数：head - 指向床位链表头指针的指针
//     tail - 指向床位链表尾指针的指针
//     b - 要删除的床位节点指针
//返回值：无
void delBed(Bed** head, Bed** tail, Bed* b)
{
	// 1. 边界检查：如果待删除的节点为NULL，直接返回
	if (*head == NULL || b == NULL) {
		return;
	}

	// 2. 占用状态的床位不能删除
	if (strcmp(b->data.status, "占用") == 0) {
		printf("[ERROR] 床位%s正处于占用状态，无法删除\n", b->data.bedNo);
		return;
	}

	// 3. 情况1：要删除的是头节点
	if (b == *head) {
		*head = b->next;        // 头指针后移
		if (*head != NULL) {
			(*head)->pre = NULL; // 新头节点的前驱置空
		}
		else {
			// 如果删除完了，尾指针也要置空
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
	rebuildBedFile(*head);
}
//-------------------------


//-------------------------
//函数名：findBedByNo
//功能：根据床位号精确查询
//参数：head - 床位链表头指针
//     bedNo - 床位号
//返回值：返回找到的床位节点指针，未找到返回NULL
Bed* findBedByNo(Bed* head, char* bedNo)
{
	Bed* b = head;
	while (b != NULL) {
		if (strcmp(b->data.bedNo, bedNo) == 0)
		{
			return b;
		}
		b = b->next;
	}
	return NULL;
}
//-------------------------


//-------------------------
//函数名：findBedsByWard
//功能：根据病房号查询床位
//参数：head - 床位链表头指针
//     ward - 病房号
//返回值：返回匹配的床位链表头指针
Bed* findBedsByWard(Bed* head, char* ward)
{
	Bed* resultHead = NULL;  // 结果链表头
	Bed* resultTail = NULL;  // 结果链表尾
	Bed* p = head;

	while (p != NULL)
	{
		// 匹配病房号
		if (strcmp(p->data.ward, ward) == 0)
		{
			// 创建新节点，复制数据
			Bed* newNode = (Bed*)malloc(sizeof(Bed));
			newNode->data = p->data;  // 复制数据
			newNode->next = NULL;
			newNode->pre = NULL;

			// 添加到结果链表
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

	return resultHead;  // 返回匹配的所有床位
}
//-------------------------


//-------------------------
//函数名：findAvailableBeds
//功能：查找所有空闲床位
//参数：head - 床位链表头指针
//返回值：返回空闲床位链表头指针
Bed* findAvailableBeds(Bed* head)
{
	Bed* b = head;
	Bed* resultHead = NULL;  // 结果链表头
	Bed* resultTail = NULL;  // 结果链表尾

	while (b != NULL) {
		if (strcmp(b->data.status, "空闲") == 0)
		{
			// 创建新节点，复制数据
			Bed* newNode = (Bed*)malloc(sizeof(Bed));
			newNode->data = b->data;  // 复制数据
			newNode->next = NULL;
			newNode->pre = NULL;

			// 添加到结果链表
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
		b = b->next;
	}
	return resultHead;
}
//-------------------------


//-------------------------
//函数名：getWardStats
//功能：病房统计
//参数：head - 床位链表头指针
//     ward - 病房号
//     total - 指向总数的整型指针
//     occupied - 指向已占用数的整型指针
//返回值：无
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
//函数名：listAllBeds
//功能：显示所有床位列表
//参数：head - 床位链表头指针
//返回值：无
void listAllBeds(Bed* head)
{
	Bed* b = head;
	if(b == NULL)
	{
		printf("暂无床位信息\n");
		return;
	}
	
	printf("===床位显示列表===\n");
	printPadded("病房", 10);     putchar(' ');
	printPadded("床位号", 18);   putchar(' ');
	printPadded("患者卡号", 20); putchar(' ');
	printPadded("患者姓名", 10); putchar(' ');
	printPadded("床位状态", 10); putchar('\n');

	while (b != NULL)
	{
		printPadded(b->data.ward, 10);            putchar(' ');
		printPadded(b->data.bedNo, 18);           putchar(' ');
		printPadded(b->data.patientCardNo, 20);   putchar(' ');
		printPadded(b->data.patientName, 10);     putchar(' ');
		printPadded(b->data.status, 10);          putchar('\n');
		b = b->next;
	}
	return;
}
//-------------------------


//-------------------------
//函数名：listBedsByWard
//功能：显示指定病房床位列表
//参数：head - 床位链表头指针
//     ward - 病房号
//返回值：无
void listBedsByWard(Bed* head, char* ward)
{
	if(head == NULL)
	{
		printf("暂无床位信息\n");
		return;
	}
	
	printf("===指定病房床位显示列表===\n");
	printPadded("病房", 10);     putchar(' ');
	printPadded("床位号", 18);   putchar(' ');
	printPadded("患者卡号", 20); putchar(' ');
	printPadded("患者姓名", 10); putchar(' ');
	printPadded("床位状态", 10); putchar('\n');

	Bed* b = head;  // 遍历指针
	while (b != NULL)
	{
		// 检查病房号是否匹配
		if (strcmp(b->data.ward, ward) == 0)
		{
			printPadded(b->data.ward, 10);            putchar(' ');
			printPadded(b->data.bedNo, 18);           putchar(' ');
			printPadded(b->data.patientCardNo, 20);   putchar(' ');
			printPadded(b->data.patientName, 10);     putchar(' ');
			printPadded(b->data.status, 10);          putchar('\n');
		}
		b = b->next;
	}
	return;
}
//-------------------------