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
	char patientCardNo[20],char patientName[50],double prepay)
{
	//分配床位
	char* bedNo1 = allocateBed(g_bedHead, patientCardNo, patientName);
	if (strcmp(bedNo1, "NULL") == 0)
	{
		printf("[ERROR] 床位不足");
		return;
	}
	Hospitalization* h = (Hospitalization*)malloc(sizeof(Hospitalization));
	if (!h)//分配新节点
	{
		printf("[ERROR] 内存申请失败\n");
		return;
	}
	char id[20];
	generateUniqueId("HOS", id);//生成住院编号
	h->next = h->pre = NULL;
	h->data.totalCost = 0;//将住院信息填充到结构体的各个字段中去
	strcpy(h->data.bedNo, bedNo1);
	strcpy(h->data.patientCardNo, patientCardNo);
	strcpy(h->data.patientName, patientName);
	h->data.prepay = prepay;
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
	printf("[OK] 入院登记成功！住院单号与床位已自动分配。\n");
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
		printf("补交费用 %f 元\n", (-1) * returnCost);
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
Hospitalization* findCurrentHospitalizations(Hospitalization* head)
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

```c
//-------------------------
// 以下为追加预交金函数
// 功能：为指定住院记录追加预交金
// 参数：h - 住院记录节点指针，amount - 追加金额（必须 > 0）
//-------------------------
void addPrepay(Hospitalization* h, double amount)
{
	// 1. 边界检查
	if (h == NULL) {
		printf("[ERROR] 住院记录为空，无法追加预交金。\n");
		return;
	}

	// 2. 检查追加金额合法性
	if (amount <= 0) {
		printf("[ERROR] 追加金额必须大于 0。\n");
		return;
	}

	// 3. 检查患者是否仍在院
	if (strcmp(h->data.status, "住院") != 0) {
		printf("[ERROR] 该患者已出院，无法追加预交金。\n");
		return;
	}

	// 4. 累加预交金
	h->data.prepay += amount;

	// 5. 写回文件（通过全局头指针）
	extern Hospitalization* g_hosHead;
	rebuildHospitalizationFile(g_hosHead);

	// 6. 输出提示
	printf("[OK] 预交金追加成功！\n");
	printf("  本次追加：%.2f 元\n", amount);
	printf("  当前预交金总额：%.2f 元\n", h->data.prepay);
}

//-------------------------
// 以下为修改住院信息函数
// 功能：修改指定住院记录（转床 + 追加预交金）
// 参数：head - 住院链表头指针
//      recordNo - 住院单号
//      newBedNo - 新床位号（为空字符串则不转床）
//      addPrepayAmount - 追加预交金金额（为 0 则不追加）
// 返回值：1 - 成功，0 - 失败（未找到或已出院）
//-------------------------
int modifyHospitalization(Hospitalization* head, char* recordNo,
	char* newBedNo, double addPrepayAmount)
{
	// 1. 按住院单号查找记录
	Hospitalization* h = findHospitalizationByNo(head, recordNo);

	// 2. 检查是否找到
	if (h == NULL) {
		printf("[ERROR] 未找到住院单号为 %s 的记录。\n", recordNo);
		return 0;
	}

	// 3. 检查是否已出院
	if (strcmp(h->data.status, "住院") != 0) {
		printf("[ERROR] 该患者已出院，无法修改住院信息。\n");
		return 0;
	}

	// 4. 显示当前信息
	printf("当前住院信息：\n");
	printf("  住院单号：%s\n", h->data.recordNo);
	printf("  患者姓名：%s\n", h->data.patientName);
	printf("  当前床位：%s\n", h->data.bedNo);
	printf("  当前预交金：%.2f 元\n", h->data.prepay);

	// 5. 转床处理（如果 newBedNo 非空）
	if (newBedNo != NULL && strlen(newBedNo) > 0) {
		// 释放旧床位
		Bed* oldBed = findBedByNo(g_bedHead, h->data.bedNo);
		if (oldBed != NULL) {
			freeBed(oldBed);
			printf("  [OK] 旧床位 %s 已释放。\n", h->data.bedNo);
		}

		// 分配新床位
		Bed* newBed = findBedByNo(g_bedHead, newBedNo);
		if (newBed == NULL) {
			printf("[ERROR] 床位 %s 不存在。\n", newBedNo);
			return 0;
		}
		if (strcmp(newBed->data.status, "空闲") != 0) {
			printf("[ERROR] 床位 %s 非空闲状态，无法分配。\n", newBedNo);
			return 0;
		}

		// 占用新床位
		strcpy(newBed->data.patientCardNo, h->data.patientCardNo);
		strcpy(newBed->data.patientName, h->data.patientName);
		strcpy(newBed->data.status, "占用");
		strcpy(h->data.bedNo, newBedNo);

		printf("  [OK] 已成功转至床位 %s。\n", newBedNo);

		// 写回床位文件
		rebuildBedFile(g_bedHead);
	}

	// 6. 追加预交金（如果金额 > 0）
	if (addPrepayAmount > 0) {
		h->data.prepay += addPrepayAmount;
		printf("  [OK] 预交金追加 %.2f 元，当前总额 %.2f 元。\n",
			addPrepayAmount, h->data.prepay);
	}

	// 7. 写回住院记录文件
	rebuildHospitalizationFile(head);

	printf("[OK] 住院信息修改成功。\n");
	return 1;
}