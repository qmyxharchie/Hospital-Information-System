#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file_io.h"
#include "utils.h"
#include "patient.h"
#include "doctor.h"
#include "registration.h"
#include "medicine.h"
#include "hospitalization.h"
#include "bed.h"

//-----------------
//	读取文件
void buildPatientChain(Patient** head, Patient** tail) {
	FILE* fp = fopen(PATIENT_FILE, "r");
	if (fp == NULL) {
		fopen(PATIENT_FILE, "w");
		fprintf(fp, "-1\n");
		fclose(fp);

		*head = NULL;
		*tail = NULL;
		return;
	}
	
	*head = NULL;
	*tail = NULL;
	Patient* pre = NULL;

	int opt;					//循环读取文件
	while (1) {
		fscanf(fp, "%d", &opt);

		if (opt == -1) 
			break;

		if (opt == 1) {			//创建节点
			Patient* node = (Patient*)malloc(sizeof(Patient));
			node->next = NULL;
			node->pre = NULL;
			fscanf(fp, "%s", node->data.cardNo);
			fscanf(fp, "%s", node->data.name);
			fscanf(fp, "%d", &node->data.age);
			fscanf(fp, "%s", node->data.gender);
			fscanf(fp, "%s", node->data.idCard);
			fscanf(fp, "%s", node->data.phone);
			fscanf(fp, "%d", &node->data.isActive);

			if (*head == NULL) {
				*head = node;
			}
			else {
				pre->next = node;
				node->pre = pre;
			}
			pre = node;
		}
	}
	*tail = pre;
	fclose(fp);
}

void buildDoctorChain(Doctor** head, Doctor** tail) {
	FILE* fp = fopen(DOCTOR_FILE, "r");
	if (fp == NULL) {
		fopen(DOCTOR_FILE, "w");
		fprintf(fp, "-1\n");
		fclose(fp);

		*head = NULL;
		*tail = NULL;
		return;
	}

	*head = NULL;
	*tail = NULL;
	Doctor* pre = NULL;

	int opt;					
	while (1) {
		fscanf(fp, "%d", &opt);

		if (opt == -1)
			break;

		if (opt == 1) {			
			Doctor* node = (Doctor*)malloc(sizeof(Doctor));
			node->next = NULL;
			node->pre = NULL;
			fscanf(fp, "%s", node->data.empNo);
			fscanf(fp, "%s", node->data.name);
			fscanf(fp, "%s", node->data.dept);
			fscanf(fp, "%s", node->data.schedule);
			fscanf(fp, "%d", &node->data.maxPatients);
			fscanf(fp, "%d", &node->currentPatients);

			if (*head == NULL) {
				*head = node;
			}
			else {
				pre->next = node;
				node->pre = pre;
			}
			pre = node;
		}
	}
	*tail = pre;
	fclose(fp);
}

void buildMedicineChain(Medicine** head, Medicine** tail) {
	FILE* fp = fopen(MEDICINE_FILE, "r");
	if (fp == NULL) {
		fopen(MEDICINE_FILE, "w");
		fprintf(fp, "-1\n");
		fclose(fp);

		*head = NULL;
		*tail = NULL;
		return;
	}

	*head = NULL;
	*tail = NULL;
	Medicine* pre = NULL;

	int opt;					
	while (1) {
		fscanf(fp, "%d", &opt);

		if (opt == -1)
			break;

		if (opt == 1) {			
			Medicine* node = (Medicine*)malloc(sizeof(Medicine));
			node->next = NULL;
			node->pre = NULL;
			fscanf(fp, "%s", node->data.medNo);
			fscanf(fp, "%s", node->data.genericName);
			fscanf(fp, "%s", node->data.brandName);
			fscanf(fp, "%s", node->data.spec);
			fscanf(fp, "%lf", &node->data.price);
			fscanf(fp, "%d", &node->currentPatients);
			fscanf(fp, "%d", &node->minStock);

			if (*head == NULL) {
				*head = node;
			}
			else {
				pre->next = node;
				node->pre = pre;
			}
			pre = node;
		}
	}
	*tail = pre;
	fclose(fp);
}

void buildPurchaseChain(Purchase** head, Purchase** tail) {
	FILE* fp = fopen(PURCHASE_FILE, "r");
	if (fp == NULL) {
		fopen(PURCHASE_FILE, "w");
		fprintf(fp, "-1\n");
		fclose(fp);

		*head = NULL;
		*tail = NULL;
		return;
	}

	*head = NULL;
	*tail = NULL;
	Purchase* pre = NULL;

	int opt;
	while (1) {
		fscanf(fp, "%d", &opt);

		if (opt == -1)
			break;

		if (opt == 1) {
			Purchase* node = (Purchase*)malloc(sizeof(Purchase));
			node->next = NULL;
			node->pre = NULL;
			fscanf(fp, "%s", node->data.purNo);
			fscanf(fp, "%s", node->data.patientCardNo);
			fscanf(fp, "%s", node->data.medNo);
			fscanf(fp, "%d", &node->data.quantity);
			fscanf(fp, "%lf", &node->data.totalCost);
			fscanf(fp, "%s", node->date);

			if (*head == NULL) {
				*head = node;
			}
			else {
				pre->next = node;
				node->pre = pre;
			}
			pre = node;
		}
	}
	*tail = pre;
	fclose(fp);
}

void buildHospitalizationChain(Hospitalization** head, Hospitalization** tail) {
	FILE* fp = fopen(HOSPITAL_FILE, "r");
	if (fp == NULL) {
		fopen(HOSPITAL_FILE, "w");
		fprintf(fp, "-1\n");
		fclose(fp);

		*head = NULL;
		*tail = NULL;
		return;
	}

	*head = NULL;
	*tail = NULL;
	Hospitalization* pre = NULL;

	int opt;
	while (1) {
		fscanf(fp, "%d", &opt);

		if (opt == -1)
			break;

		if (opt == 1) {
			Hospitalization* node = (Hospitalization*)malloc(sizeof(Hospitalization));
			node->next = NULL;
			node->pre = NULL;
			fscanf(fp, "%s", node->data.recordNo);
			fscanf(fp, "%s", node->data.patientCardNo);
			fscanf(fp, "%s", node->data.patientName);
			fscanf(fp, "%s", node->data.bedNo);
			fscanf(fp, "%lf", &node->data.totalCost);
			fscanf(fp, "%s", node->admissionDate);
			fscanf(fp, "%s", node->status);

			if (*head == NULL) {
				*head = node;
			}
			else {
				pre->next = node;
				node->pre = pre;
			}
			pre = node;
		}
	}
	*tail = pre;
	fclose(fp);
}

void buildBedChain(Bed** head, Bed** tail) {
	FILE* fp = fopen(BED_FILE, "r");
	if (fp == NULL) {
		fopen(BED_FILE, "w");
		fprintf(fp, "-1\n");
		fclose(fp);

		*head = NULL;
		*tail = NULL;
		return;
	}

	*head = NULL;
	*tail = NULL;
	Bed* pre = NULL;

	int opt;
	while (1) {
		fscanf(fp, "%d", &opt);

		if (opt == -1)
			break;

		if (opt == 1) {
			Bed* node = (Bed*)malloc(sizeof(Bed));
			node->next = NULL;
			node->pre = NULL;
			fscanf(fp, "%s", node->data.ward);
			fscanf(fp, "%s", node->data.bedNo);
			fscanf(fp, "%s", node->data.patientCardNo);
			fscanf(fp, "%s", node->data.patientName);
			fscanf(fp, "%s", node->status);

			if (*head == NULL) {
				*head = node;
			}
			else {
				pre->next = node;
				node->pre = pre;
			}
			pre = node;
		}
	}
	*tail = pre;
	fclose(fp);
}

void buildRegistrationChain(Registration** head, Registration** tail) {
	FILE* fp = fopen(REGISTRATION_FILE, "r");
	if (fp == NULL) {
		fopen(REGISTRATION_FILE, "w");
		fprintf(fp, "-1\n");
		fclose(fp);

		*head = NULL;
		*tail = NULL;
		return;
	}

	*head = NULL;
	*tail = NULL;
	Registration* pre = NULL;

	int opt;
	while (1) {
		fscanf(fp, "%d", &opt);

		if (opt == -1)
			break;

		if (opt == 1) {
			Registration* node = (Registration*)malloc(sizeof(Registration));
			node->next = NULL;
			node->pre = NULL;
			fscanf(fp, "%s", node->data.regNo);
			fscanf(fp, "%s", node->data.patientCardNo);
			fscanf(fp, "%s", node->data.patientName);
			fscanf(fp, "%s", node->data.doctorEmpNo);
			fscanf(fp, "%s", node->data.doctorName);
			fscanf(fp, "%s", node->data.dept);
			fscanf(fp, "%s", node->data.date);
			fscanf(fp, "%s", node->data.status);

			if (*head == NULL) {
				*head = node;
			}
			else {
				pre->next = node;
				node->pre = pre;
			}
			pre = node;
		}
	}
	*tail = pre;
	fclose(fp);
}

void buildUserChain(User** head, User** tail) {
	FILE* fp = fopen(USER_FILE, "r");
	if (fp == NULL) {
		fopen(USER_FILE, "w");
		fprintf(fp, "-1\n");
		fclose(fp);

		*head = NULL;
		*tail = NULL;
		return;
	}

	*head = NULL;
	*tail = NULL;
	User* pre = NULL;

	int opt;
	while (1) {
		fscanf(fp, "%d", &opt);

		if (opt == -1)
			break;

		if (opt == 1) {
			User* node = (User*)malloc(sizeof(User));
			node->next = NULL;
			node->pre = NULL;
			fscanf(fp, "%s", node->data.username);
			fscanf(fp, "%s", node->data.password);
			fscanf(fp, "%s", node->data.role);
			fscanf(fp, "%d", &node->data.lastLogin);

			if (*head == NULL) {
				*head = node;
			}
			else {
				pre->next = node;
				node->pre = pre;
			}
			pre = node;
		}
	}
	*tail = pre;
	fclose(fp);
}
//-----------------

//-----------------
//	写入文件
void rebuildPatientFile(Patient* head) {
	FILE* fp = fopen(PATIENT_FILE, "w");

	if (fp == NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	Patient* p = head;

	while (p != NULL) {
		fprintf(fp, "1\n");
		
		fprintf(fp, "%s\n",p->data.cardNo);
		fprintf(fp, "%s\n", p->data.name);
		fprintf(fp, "%d\n", p->data.age);
		fprintf(fp, "%s\n", p->data.gender);
		fprintf(fp, "%s\n", p->data.idCard);
		fprintf(fp, "%s\n", p->data.phone);
		fprintf(fp, "%d\n", p->data.isActive);

		fprintf(fp, "\n");
		p = p->next;
	}
	fprintf(fp, "-1\n");

	fclose(fp);
}

void rebuildDoctorFile(Doctor* head) {
	FILE* fp = fopen(DOCTOR_FILE, "w");

	if (fp == NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	Doctor* p = head;

	while (p != NULL) {
		fprintf(fp, "1\n");

		fprintf(fp, "%s\n", p->data.empNo);
		fprintf(fp, "%s\n", p->data.name);
		fprintf(fp, "%s\n", p->data.dept);
		fprintf(fp, "%s\n", p->data.schedule);
		fprintf(fp, "%d\n", p->data.maxPatients);
		fprintf(fp, "%d\n", p->data.currentPatients);

		fprintf(fp, "\n");
		p = p->next;
	}
	fprintf(fp, "-1\n");

	fclose(fp);
}

void rebuildMedicineFile(Medicine* head) {
	FILE* fp = fopen(MEDICINE_FILE, "w");

	if (fp == NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	Medicine* p = head;

	while (p != NULL) {
		fprintf(fp, "1\n");

		fprintf(fp, "%s\n", p->data.medNo);
		fprintf(fp, "%s\n", p->data.genericName);
		fprintf(fp, "%s\n", p->data.brandName);
		fprintf(fp, "%s\n", p->data.spec);
		fprintf(fp, "%lf\n", p->data.price);
		fprintf(fp, "%d\n", p->data.stock);
		fprintf(fp, "%d\n", p->data.minStock);

		fprintf(fp, "\n");
		p = p->next;
	}
	fprintf(fp, "-1\n");

	fclose(fp);
}

void rebuildPurchaseFile(Purchase* head) {
	FILE* fp = fopen(PURCHASE_FILE, "w");

	if (fp == NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	Purchase* p = head;

	while (p != NULL) {
		fprintf(fp, "1\n");

		fprintf(fp, "%s\n", p->data.purNo);
		fprintf(fp, "%s\n", p->data.patientCardNo);
		fprintf(fp, "%s\n", p->data.medNo);
		fprintf(fp, "%d\n", p->data.quantity);
		fprintf(fp, "%lf\n", p->data.totalCost);
		fprintf(fp, "%s\n", p->data.date);

		fprintf(fp, "\n");
		p = p->next;
	}
	fprintf(fp, "-1\n");

	fclose(fp);
}

void rebuildHospitalizationFile(Hospitalization* head) {
	FILE* fp = fopen(HOSPITAL_FILE, "w");

	if (fp == NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	Hospitalization* p = head;

	while (p != NULL) {
		fprintf(fp, "1\n");

		fprintf(fp, "%s\n", p->data.recordNo);
		fprintf(fp, "%s\n", p->data.patientCardNo);
		fprintf(fp, "%s\n", p->data.patientName);
		fprintf(fp, "%lf\n", p->data.prepay);
		fprintf(fp, "%lf\n", p->data.totalCost);
		fprintf(fp, "%s\n", p->data.admissionDate);
		fprintf(fp, "%s\n", p->data.status);

		fprintf(fp, "\n");
		p = p->next;
	}
	fprintf(fp, "-1\n");

	fclose(fp);
}

void rebuildBedFile(Bed* head) {
	FILE* fp = fopen(BED_FILE, "w");

	if (fp == NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	Bed* p = head;

	while (p != NULL) {
		fprintf(fp, "1\n");

		fprintf(fp, "%s\n", p->data.ward);
		fprintf(fp, "%s\n", p->data.bedNo);
		fprintf(fp, "%s\n", p->data.patientCardNo);
		fprintf(fp, "%s\n", p->data.patientName);
		fprintf(fp, "%s\n", p->data.status);

		fprintf(fp, "\n");
		p = p->next;
	}
	fprintf(fp, "-1\n");

	fclose(fp);
}

void rebuildRegistrationFile(Registration* head) {
	FILE* fp = fopen(REGISTRATION_FILE, "w");

	if (fp == NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	Registration* p = head;

	while (p != NULL) {
		fprintf(fp, "1\n");

		fprintf(fp, "%s\n", p->data.regNo);
		fprintf(fp, "%s\n", p->data.patientCardNo);
		fprintf(fp, "%s\n", p->data.patientName);
		fprintf(fp, "%s\n", p->data.doctorEmpNo);
		fprintf(fp, "%s\n", p->data.doctorName);
		fprintf(fp, "%s\n", p->data.dept);
		fprintf(fp, "%s\n", p->data.date);
		fprintf(fp, "%s\n", p->data.status);

		fprintf(fp, "\n");
		p = p->next;
	}
	fprintf(fp, "-1\n");

	fclose(fp);
}

void rebuildUserFile(User* head) {
	FILE* fp = fopen(USER_FILE, "w");

	if (fp == NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	User* p = head;

	while (p != NULL) {
		fprintf(fp, "1\n");

		fprintf(fp, "%s\n", p->data.username);
		fprintf(fp, "%s\n", p->data.password);
		fprintf(fp, "%d\n", p->data.role);
		fprintf(fp, "%s\n", p->data.lastLogin);
		fprintf(fp, "\n");
		p = p->next;
	}
	fprintf(fp, "-1\n");

	fclose(fp);
}
//-----------------

//-----------------
//	释放内存

void freePatientChain(Patient** head) {
	if (!head || !*head) return;
	Patient* p = *head;

	while (p != NULL) {
		Patient* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeDoctorChain(Doctor** head) {
	if (!head || !*head) return;
	Doctor* p = *head;

	while (p != NULL) {
		Doctor* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeMedicineChain(Medicine** head) {
	if (!head || !*head) return;
	Medicine* p = *head;

	while (p != NULL) {
		Medicine* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freePurchaseChain(Purchase** head) {
	if (!head || !*head) return;
	Purchase* p = *head;

	while (p != NULL) {
		Purchase* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeHospitalizationChain(Hospitalization** head) {
	if (!head || !*head) return;
	Hospitalization* p = *head;

	while (p != NULL) {
		Hospitalization* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeBedChain(Bed** head) {
	if (!head || !*head) return;
	Bed* p = *head;

	while (p != NULL) {
		Bed* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeRegistrationChain(Registration** head) {
	if (!head || !*head) return;
	Registration* p = *head;

	while (p != NULL) {
		Registration* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeUserChain(User** head) {
	if (!head || !*head) return;
	User* p = *head;

	while (p != NULL) {
		User* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeRegistrationResultChain(Registration** head) {
	if (!head || !*head) return;
	Registration* p = *head;

	while (p != NULL) {
		Registration* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeDoctorsResultChain(Doctor** head) {
	if (!head || !*head) return;
	Doctor* p = *head;

	while (p != NULL) {
		Doctor* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freePatientsResultChain(Patient** head) {
	if (!head || !*head) return;
	Patient* p = *head;

	while (p != NULL) {
		Patient* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeMedicineResultChain(Medicine** head) {
	if (!head || !*head) return;
	Medicine* p = *head;

	while (p != NULL) {
		Medicine* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}

void freeHospitalizationResultChain(Hospitalization** head){
	if (!head || !*head) return;
		Medicine* p = *head;

	while (p != NULL) {
		Medicine* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
	}

void freeBedResultChain(Bed** head) {
	if (!head || !*head) return;
	Bed* p = *head;

	while (p != NULL) {
		Bed* temp = p->next;
		free(p);
		p = temp;
	}
	*head = NULL;
}
//-----------------

//-----------------
//	复制单个文件
int copyFile(char* source, char* dest) {
	FILE* src, * dst;
	char buffer[1024];		 // 缓冲区
	size_t bytesRead;

	src = fopen(source, "r");
	if (src == NULL) {
		printf("无法打开源文件: %s\n", source);
		return 0;
	}

	dst = fopen(dest, "w");
	if (dst == NULL) {
		printf("无法创建目标文件: %s\n", dest);
		fclose(src);
		return 0;
	}

	while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) {	//读到继续循环
		fwrite(buffer, 1, bytesRead, dst);
	}

	fclose(src);			//关闭文件
	fclose(dst);
	return 1;
}
//-----------------

//-----------------
//	备份所有数据文件
int backupAllData() {
	int year, month, day;
	getCurrentTime(&year, &month, &day);

	char* files[] = {			//存放需要备份的文件
		PATIENT_FILE,
		DOCTOR_FILE,
		MEDICINE_FILE,
		PURCHASE_FILE,
		HOSPITAL_FILE,
		BED_FILE,
		REGISTRATION_FILE,
		USER_FILE
	};
	int fileCount = 8;

	char destPath[1024];			// 目标路径
	int successCount = 0;

	//循环必须包住 copyFile
	for (int i = 0; i < fileCount; i++) {		//循环备份

		const char* fileName = strrchr(files[i], '/');		//从右往左找最后一次出现的某个字符
		if (fileName) fileName++;				//变成纯文件名
		else fileName = files[i];

		sprintf(destPath, "backup/%s_%04d%02d%02d.txt",
			files[i], year, month, day);


		if (copyFile(files[i], destPath)) {
			printf("备份成功: %s -> %s\n", files[i], destPath);
			successCount++;
		}
		else {
			printf("备份失败: %s\n", files[i]);
		}
	}

	printf("总共成功备份 %d/%d 个文件\n", successCount, fileCount);
	return 1;
}