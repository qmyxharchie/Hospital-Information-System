#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//学生数据结构体
typedef struct student{
    int id;
    char name[20];                      //不用*name,因为得单独申请地址
    int chinese;
    int math;
    int english;
    int physics;
    int chemistry;
    int total;
}Student;
//链表结构
typedef struct node{
    Student data;
    struct node *next;
}Node;
//计算总成绩
void calculateTotal(Student *s){
    if(s == NULL){
        return;
    }
    s->total=s->chinese+s->math+s->english+s->physics+s->chemistry;
}
//申请一个新节点
Node *createNode(Student s){
    Node *newNode;
    newNode=(Node *)malloc(sizeof(Node));   //todo free
    if(newNode==NULL){
        printf("内存申请失败\n");
        return NULL;
    }
    newNode->data=s;
    newNode->next=NULL;
    return newNode;
}
//尾插函数
Node *appendNode(Node *head,Node *newNode){
    
    if(newNode==NULL){
        return head;
    }
    if(head==NULL){
        head=newNode;
    }
    else{
        Node *rear=head;
        while(rear->next!=NULL){
            rear=rear->next;
        }
        rear->next=newNode;
    }
    return head;
}
//遍历显示函数
void printlist(Node *head){             //todo 记得优化表头和格式化
    if(head==NULL){
        printf("暂无学生记录\n");
        return;                         //!链表为空时结束函数
    }
    printf(" 学号     姓名 语文 数学 英语 物理 化学  总分\n");
    printf("-------------------------------------------------\n");
    Node *p;
    p=head;
    while(p!=NULL){
        printf("%5d %10s %4d %4d %4d %4d %4d %5d\n",
            p->data.id,p->data.name,p->data.chinese,
            p->data.math,p->data.english,p->data.physics,
            p->data.chemistry,p->data.total);
        p=p->next;
    }
}
//释放链表
void freelist(Node *head){
    Node *p=head;
    while(head!=NULL){
        head=head->next;
        free(p);
        p=head;
    }
    return;
}
//文件读入函数
Node *loadFromFile(char *filename){
    FILE *fp;
    fp=fopen(filename,"r");             //?有深刻教育意义[doge]
    if(fp==NULL){
        printf("文件打开失败\n");
        return NULL;
    }
    Student temp;
    Node *head=NULL;                    //!记得初始化
    while(fscanf(fp,"%d %s %d %d %d %d %d",
        &temp.id,temp.name,&temp.chinese,&temp.math,
        &temp.english,&temp.physics,&temp.chemistry)==7){  //!这个返回值是7
        calculateTotal(&temp);
        Node *newNode=createNode(temp);
        head=appendNode(head,newNode);        
    }
    fclose(fp);
    return head;
}
//保存到文件
void saveToFile(Node *head,char *filename){
    FILE *fp;
    fp=fopen(filename,"w");
    if(fp==NULL){                       //?笑死我了，咋能就打一个=
        printf("文件打开失败");
        return;
    }
    Node *p;
    p=head;
    Student temp;
    while(p!=NULL){
        temp=p->data;
        fprintf(fp,"%d %s %d %d %d %d %d \n",
        temp.id,temp.name,temp.chinese,temp.math,
        temp.english,temp.physics,temp.chemistry);//*可以不加total，毕竟可以算
        p=p->next;
    }
    fclose(fp);
}
//增加学生
Node *addStudent(Node *head){
    Student temp;
    printf("请输入：学号 姓名 语文 数学 英语 物理 化学\n");
    scanf("%d %19s %d %d %d %d %d",                     //限制%s长度（结构体中给的）
        &temp.id,temp.name,&temp.chinese,&temp.math,
        &temp.english,&temp.physics,&temp.chemistry);
    calculateTotal(&temp);
    Node *p=createNode(temp);           //要记得要接住函数返回值啊
    head=appendNode(head,p);
    return head;
}
//删除学生
Node *deleteStudent(Node *head,int id,char *name){
    Node *p,*q;
    p=head;
    q=NULL;
    int found=0;
    while(p!=NULL){
        if(id==p->data.id&&strcmp(name,p->data.name)==0){
            if(q==NULL){
                head=p->next;
                free(p);
                p=NULL;                   //防止野指针
            }
            else{
                q->next=p->next;
                free(p);
                p=NULL;
            }
            found=1;
            break;
        }
        else{
        q=p;
        p=p->next;    
        }        
    }
    if(found){
        printf("删除成功\n");
    }
    else{
        printf("未找到该学生\n");
    }
    return head;
}
//修改学生
Node *modifyStudent(Node *head,int id,char *name){
    Node *p;
    p=head;
    int modify=0;
    while(p!=NULL){
        if(id==p->data.id&&strcmp(name,p->data.name)==0){
            printf("请重新输入五门成绩\n");
            scanf("%d %d %d %d %d",&p->data.chinese,&p->data.math,
                &p->data.english,&p->data.physics,&p->data.chemistry);
            calculateTotal(&p->data);            
            modify=1;
            break;
            }
            else{
            p=p->next;    
        }        
    }
    if(modify){
        printf("您已修改成功");
    }
    else{
        printf("修改失败，请检查学号与姓名是否输入正确");
    }
    return head;
}
//排序学生(设计一个type)
int getValue(Student s, int type){
    switch (type){
        case 1:
            return s.id;
        case 2:
            return s.chinese;
        case 3:
            return s.math;
        case 4:
            return s.english;
        case 5:
            return s.physics;
        case 6:
            return s.chemistry;
        case 7:
            return s.total;
        default:
            return 0;
    }
}
//排序学生(主函数)
void sortStudent(Node *head,int type,int order){
    if(head==NULL||head->next==NULL){    //order==1升序，2降序 
        return;
    }
    int a,b;
    Node *p,*q;
    p=head;
    while(p!=NULL){
        q=p->next;
        while(q!=NULL){
            a=getValue(p->data,type);
            b=getValue(q->data,type);
            if((order==1&&a>b)||(order==2&&a<b)){
                Student temp=p->data;
                p->data=q->data;
                q->data=temp;
            }
            q=q->next;
        }
        p=p->next;
    }
}
//菜单控制系统
Node *runSystem(Node *head){
    int choice;
    while (1)
    {
        printf("\n===== 成绩管理系统 =====\n");
        printf("1. 显示所有学生\n");
        printf("2. 添加学生\n");
        printf("3. 删除学生\n");
        printf("4. 修改学生\n");
        printf("5. 排序学生\n");
        printf("6. 保存到文件\n");
        printf("0. 退出系统\n");
        printf("请选择: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:{// 显示所有学生
                if(head==NULL){
                    printf("暂无学生记录\n");
                    break;
                }
                printlist(head);
                break;
            }
            case 2:{// 添加学生
                head=addStudent(head);
                break;
            }
            case 3:{// 删除学生                         //!因为变量重复定义，
                int id;                                //!给每个case 单独加作用域{}
                char name[20];
                printf("请输入要删除的：学号 姓名\n");
                scanf("%d %s",&id,name);
                head=deleteStudent(head,id,name);
                break;
            }
            case 4:{// 修改学生
                int id;
                char name[20];
                printf("请输入要修改的：学号 姓名\n");
                scanf("%d %s",&id,name);
                head=modifyStudent(head,id,name);
                break;
            }
            case 5:{// 排序学生
                int type,order;
                printf("请输入排序对象(数字)\n");
                printf("1.学号\n");  
                printf("2.语文\n"); 
                printf("3.数学\n"); 
                printf("4.英语\n"); 
                printf("5.物理\n"); 
                printf("6.化学\n"); 
                printf("7.总分\n");   
                scanf("%d",&type);
                if(!(type>0&&type<8)){                  //todo用goto重新输入
                    printf("排序对象输入错误\n");
                    break;
                }
                printf("请输入排序方式(数字)：升序(1) 降序(2)\n");
                scanf("%d",&order);
                if(!(order==1||order==2)){
                    printf("排序方式输入错误\n");
                    break;
                }                               
                sortStudent(head,type,order);
                printf("排序完成\n"); 
                break;
            }
            case 6:{// 保存到文件
                char filename[100];
                printf("请输入保存文件名\n");
                while(getchar() != '\n');               //!一直读字符，直到读到换行符为止,即把缓冲区清空
                fgets(filename,sizeof(filename),stdin);
                filename[strcspn(filename,"\n")]=0;
                saveToFile(head,filename);
                printf("保存成功\n");
                break;
            }
            case 0:
                break;
            default:
                printf("输入错误，请重新选择\n");
        }
        if (choice == 0){
            break;
        }
    }
    return head;
}

int main(){
    char filename[100];
    Node *head;
    printf("请输入要读取的文件名\n");
    fgets(filename,sizeof(filename),stdin);
    filename[strcspn(filename,"\n")]='\0';   //因为fgets会读入'\n'，把它改成'\0'或0
    head=loadFromFile(filename);
    head=runSystem(head); 
    char choice;
    printf("是否保存数据？(y/n): ");
    while(getchar() != '\n');               // 清缓冲区（关键）
    scanf("%c", &choice);
    if(choice == 'y' || choice == 'Y'){
        printf("请输入保存文件名\n");
        while(getchar() != '\n');           // 再清一次（因为刚scanf了）
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename,"\n")] = 0;
        saveToFile(head, filename);
        printf("保存成功\n");
    }   
    freelist(head);
    return 0;
}


