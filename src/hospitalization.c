#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospitalization.h"
#include "file_io.h"
#include "utils.h"
#include "bed.h"

//-------------------------
//以下为住院登记函数
void addHospitalization(Hospitalization** head, Hospitalization** tail,
	char recordNo[20],char patientCardNo[20],char patientName[50],
	double prepay)
{
	//分配床位
	char bedNo1 = allocateBed(BedNode * head, char* patientCardNo, char* patientName);
	if (strcmp(bedNo1, "NULL") == 0)
	{
		printf("床位不足");
		return;
	}
	Hospitalization* h = (Hospitalization*)malloc(sizeof(Hospitalization));
	if (!h)//创建新节点
	{
		printf("内存分配失败\n");
		return;
	}
	generateUniqueId("HOS", id);//生成住院单号
	h->next = h->pre = NULL;
	h->data.totalCost = 0;//将住院信息填充到结构体的各个数据中去
	strcpy(h->data.bedNo,bedNo1);
	strcpy(h->data.patientCardNo,patientCardNo );
	strcpy(h->data.patientName, patientName);
	strcpy(h->data.prepay,prepay );
	strcpy(h->data.recordNo, id);
	strcpy(h->data.status, "在院");
	int year, month, day;
	getCurrentTime(&year, &month, &day);
	sprintf(h->data.admissionDate, "%04d-%02d-%02d", year, month, day);
	if (*tail == NULL)//利用尾插法添加节点
	{
		*head = h;
		*tail = h;
	}
	else {
		(*tail)->next =h;
		h->pre = *tail;
		*tail = h;
	}
	rebuildHospitalizationFile(*head);
	return;
}
//-------------------------


//-------------------------
//以下为出院结算函数
void dischargePatient(HospitalizationNode* h, double totalCost)
{
	if (h == NULL)//判断边界
	{
		return;
	}
	h->data.totalCost = totalCost;//填充总费用
	strcpy(h->data.status, "已出院");
	double returnCost = h->data.prepay - h->data.totalCost;//计算应返还的费用
	if (returnCost < 0)//若应返还的费用为负数，则提醒病人补缴住院费
	{
		printf("请补缴费用 %d 元", (-1) * returnCost);
		returnCost = 0;
	}
	printf("====出院结算====");
	printf("%-20s %-50s %-20s %20s %-20s %-20s", 
		"住院单号", "姓名", "入院日期", "总费用", "预交金额", "退款金额");
	printf("%-20s %-50s %-20s %20f %-20f %-20f",
		h->data.recordNo,
		h->data.patientName,
		h->data.admissionDate,
		h->data.prepay,
		h->data.totalCost,
		returnCost);
	Bed* b = findBedByNo(g_bedHead, h->data.bedNo);
	freeBed(b);
	return;
}

//-------------------------
//以下为查询函数
//按照病人卡号查询住院记录
Hospitalization* findHospitalizationByCardNo(Hospitalization* head,char* cardNo)
{
	Hospitalization* h = head;
	while (h != NULL)
	{
		if (strcmp(h->data.patientCardNo, cardNo) == 0) 
		{
			return h;
		}
		h = h->next;
	}
	return NULL;
 }
//按照住院单号进行查询
Hospitalization* findHospitalizationByNo(Hospitalization* head, char* recordNo)
{
	Hospitalization* h = head;
	while (h != NULL)
	{
		if (strcmp(h->data.recordNo, recordNo) == 0)
		{
			return h;
		}
		h = h->next;
	}
	return NULL;
}
//查找当前住院病人
Hospitalization* findAllCurrentHospitalizations(Hospitalization* head)
{
	Hospitalization* h = head;
	Hospitalization* resultHead = NULL;  // 新链表头
	Hospitalization* tail = NULL;

	while (h != NULL)
	{
		if (strcmp(h->data.status, "在院") == 0)
		{
			// 创建新节点
			Hospitalization* newNode = (Hospitalization*)malloc(sizeof(Hospitalization));
			*newNode = *h;
			newNode->next = NULL;

			if (resultHead == NULL)
			{
				resultHead = tail = newNode;
			}
			else
			{
				tail->next = newNode;
				tail = newNode;
			}
		}
		h = h->next;
	}

	return resultHead;
}
//---------------------

//---------------------
//以下为住院列表函数
void listAllHospitalizations(Hospitalization* head)
{
	Hospitalization* h= head;//创建指针以遍历链表
	printf("===住院记录列表===\n");
	printf("%-20s  %-20s %-50s %-20s %-15s %-15s %-20s %-20s\n ",
		"住院单号", "病人卡号", "姓名", "床位号", "预交金额", "总费用", "入院日期","住院状态");
	while (h->next != NULL)
	{
		printf("%-20s %-20s %-50s %-20s %-15f %-15f %-20s %-20s\n",
			h->data.admissionDate,
			h->data.bedNo,
			h->data.patientCardNo,
			h->data.patientName,
			h->data.prepay,
			h->data.recordNo,
			h->data.status,
			h->data.totalCost);
		h = h->next;
	}
	return;
}