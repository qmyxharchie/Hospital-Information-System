#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospitalization.h"  // 住院模块头文件
#include "file_io.h"          // 文件输入输出模块
#include "utils.h"            // 工具函数模块
#include "bed.h"              // 床位模块

//-------------------------
//函数名：addHospitalization
//功能：为患者办理住院手续，自动分配床位
//参数：head - 指向住院记录链表头指针的指针
//     tail - 指向住院记录链表尾指针的指针  
//     recordNo - 住院号
//     patientCardNo - 患者卡号
//     patientName - 患者姓名
//     prepay - 预付款
//返回值：无
void addHospitalization(Hospitalization** head, Hospitalization** tail,
	char recordNo[20],char patientCardNo[20],char patientName[50],
	double prepay)
{
	//分配床位
	char* bedNo1 = allocateBed(g_bedHead, patientCardNo, patientName);
	if (strcmp(bedNo1, "NULL") == 0)
	{
		printf("床位不足");
		return;
	}
	Hospitalization* h = (Hospitalization*)malloc(sizeof(Hospitalization));
	if (!h)//分配新节点
	{
		printf("内存申请失败\n");
		return;
	}
	char id[20];
	generateUniqueId("HOS", id);//生成住院编号
	h->next = h->pre = NULL;
	h->data.totalCost = 0;//将住院信息填充到结构体的各个字段中去
	strcpy(h->data.bedNo, bedNo1);
	strcpy(h->data.patientCardNo, patientCardNo);
	strcpy(h->data.patientName, patientName);
	strcpy(h->data.prepay, prepay);
	strcpy(h->data.recordNo, id);
	strcpy(h->data.status, "住院");
	int year, month, day;
	getCurrentTime(&year, &month, &day);
	sprintf(h->data.admissionDate, "%04d-%02d-%02d", year, month, day);
	if (*tail == NULL)//如果尾指针为空则插入节点
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
//函数名：dischargePatient
//功能：为患者办理出院手续，进行费用结算
//参数：h - 住院记录节点指针
//     totalCost - 总费用
//返回值：无
void dischargePatient(Hospitalization* h, double totalCost)
{
	if (h == NULL)//边界判断
	{
		return;
	}
	h->data.totalCost = totalCost;//设置总费用
	strcpy(h->data.status, "已出院");
	double returnCost = h->data.prepay - h->data.totalCost;//计算应退还的费用
	if (returnCost < 0)//如果应退还的费用为负数，说明患者还需补交住院费
	{
		printf("补交费用 %f 元", (-1) * returnCost);
		returnCost = 0;
	}
	printf("====出院信息====\n");
	printf("%-20s %-50s %-20s %20s %-20s %-20s\n", 
		"住院编号", "姓名", "入院日期", "总费用", "预付款", "退款");
	printf("%-20s %-50s %-20s %20f %-20f %-20f\n",
		h->data.recordNo,
		h->data.patientName,
		h->data.admissionDate,
		h->data.totalCost,
		h->data.prepay,
		returnCost);
	Bed* b = findBedByNo(g_bedHead, h->data.bedNo);
	freeBed(b);
	return;
}
//-------------------------


//-------------------------
//函数名：findHospitalizationByCardNo
//功能：根据患者卡号查询住院记录
//参数：head - 住院记录链表头指针
//     cardNo - 患者卡号
//返回值：返回找到的住院记录节点指针，未找到返回NULL
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
//-------------------------


//-------------------------
//函数名：findHospitalizationByNo
//功能：根据住院编号查询住院记录
//参数：head - 住院记录链表头指针
//     recordNo - 住院编号
//返回值：返回找到的住院记录节点指针，未找到返回NULL
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
//-------------------------


//-------------------------
//函数名：findAllCurrentHospitalizations
//功能：查找当前住院患者
//参数：head - 住院记录链表头指针
//返回值：返回当前住院患者链表头指针
Hospitalization* findAllCurrentHospitalizations(Hospitalization* head)
{
	Hospitalization* h = head;
	Hospitalization* resultHead = NULL;  // 结果链表头
	Hospitalization* resultTail = NULL;  // 结果链表尾

	while (h != NULL)
	{
		if (strcmp(h->data.status, "住院") == 0)
		{
			// 创建新节点
			Hospitalization* newNode = (Hospitalization*)malloc(sizeof(Hospitalization));
			*newNode = *h;
			newNode->next = NULL;
			newNode->pre = NULL;

			if (resultHead == NULL)
			{
				resultHead = resultTail = newNode;
			}
			else
			{
				resultTail->next = newNode;
				newNode->pre = resultTail;
				resultTail = newNode;
			}
		}
		h = h->next;
	}

	return resultHead;
}
//-------------------------


//-------------------------
//函数名：listAllHospitalizations
//功能：显示所有住院记录列表
//参数：head - 住院记录链表头指针
//返回值：无
void listAllHospitalizations(Hospitalization* head)
{
	if(head == NULL)
	{
		printf("暂无住院记录\n");
		return;
	}
	
	Hospitalization* h = head;//遍历指针用于遍历链表
	printf("===住院记录列表===\n");
	printf("%-20s %-20s %-50s %-20s %-15s %-15s %-20s %-20s\n",
		"住院编号", "患者卡号", "姓名", "床位号", "预付款", "总费用", "入院日期","住院状态");
	while (h != NULL)
	{
		printf("%-20s %-20s %-50s %-20s %-15f %-15f %-20s %-20s\n",
			h->data.recordNo,
			h->data.patientCardNo,
			h->data.patientName,
			h->data.bedNo,
			h->data.prepay,
			h->data.totalCost,
			h->data.admissionDate,
			h->data.status);
		h = h->next;
	}
	return;
}
//-------------------------