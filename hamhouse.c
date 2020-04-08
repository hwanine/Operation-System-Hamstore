
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>
#include<math.h>

typedef struct            // �Խ�Ʈ ��� ����ü ����
{
	int guest;                // �մԹ�ȣ, �����ð�, �մ�Ÿ��, �߰�����, �������, �����Ÿ��, CPUŸ�� ���, �Է� �ð� ��������� �������	
	int a_time;
	char guest_type[5];
	int chicken_burger;
	int shrimp_burger;
	double arround_time;
	int c_num;        
	char t_num[5];
	struct QNode* link;         // ���� ��� ��ũ
}QNode;

typedef struct           // ������ ��� ����ü
{
	char page[8];               // ������ �̸�
	int page_num;               // ������ ��ȣ 0 ~ 6
	int page_count;             // ������ ����
	struct IONode* link;        // ���� ��� ��ũ
}IONode;

typedef struct    // ������ ��� ����ü
{
	int hour;
	int min;
	double s;      // ������ �̸�
	int pickle;      // ������ ��ȣ 0 ~ 6 (������ ���)
	int shrimp;
	int chicken;
	int bread;
	int lettuce;
	int cheeze;
	int tomato;
	struct ANode* link;            // ���� ��� ��ũ
}ANode;

typedef struct                // ������ ��� ����ü
{
	int hour;                // ����� ���� ��
	int min;                 // ��
	double s;                // ��
	int count;               // ������ ��ȣ 0 ~ 6
	int head;                // ������ ����
	double seektime;         // ã�� �ð�
	struct BNode* link;      // ���� ��� ��ũ
}BNode;

typedef struct      
{
	QNode* front, * rear;
}ReadyQueue;

typedef struct      
{
	IONode* front, * rear;
}IOQueue;

typedef struct     
{
	ANode* front, * rear;
}R_PQueue;

typedef struct     
{
	BNode* front, * rear;
}R_DQueue;

int temp = 0;                                     // �������� ����Ʈ�� �ε��� �� �������� ����
const int bringnum = 7;                     // const����� ������ ����� ������ ������ �� ����.

ReadyQueue* createLinkedQueue();
IOQueue* createLinkedQueue_IO();
R_PQueue* createLinkedQueue_RP();
R_DQueue* createLinkedQueue_RD();

void PrintMenu(void);                   // �޴� ���
void enQueue(ReadyQueue* Queue, int data1, int data2, char* data3, int data4, int data5, double data6, int data7, char* data8);         // ���μ��� ť ����
void deQueue(ReadyQueue* Queue);                         // ���μ��� ť ����
int Input(ReadyQueue* Queue, FILE* fp);                  // ���� �б�
void PrintGuest(ReadyQueue* Queue);                    // ��ü �Խ�Ʈ ���
void PrintResult(ReadyQueue* Queue, R_PQueue* R_PQueue, R_DQueue* R_DQueue, double total, FILE* fp2);                     // ��� �� ��ü ���
double Guest_info(ReadyQueue* Order, ReadyQueue* Making, ReadyQueue* Delivery, ReadyQueue* Result, int q, char* arr, 
	IOQueue * Page, IOQueue* Disk, R_PQueue* Abox, R_DQueue* Bbox);                       // �մԿ� ���� �Լ�. �ٽ��Լ��ν�, ��� ������ �� �Լ� ������ �ð����� �����ϰԵ�
void Sort(ReadyQueue* Queue, int num);                                              // ��������, �� ��ġ�� ����

void enQueue_IO(IOQueue* Queue, char* arr, int pagenum, int count);                     // ������ ����
void deQueue_IO(IOQueue* Queue);                                             // ������ ����
void Insert_Page(IOQueue* PQueue, int num);                            // ȣ��� �� ���� �ش� ��� ī��Ʈ ����
int PageSize_CK(IOQueue* PQueue);                                     // ������ ������ ũ�⸦ �ʰ��Ҷ� ������� ��Ḧ â��� ��
void Change(IONode* Page);                                              // LRU�˰��� ����

int PageCK(IOQueue* PQueue, IOQueue* DQueue, int ck);                   // ��ᰡ ���� ������ üũ
int Insert_Disk(IOQueue* Queue, char* arr, int num);                    // üũ�� ������ ��ũ ť�� ����
int SSTF(IOQueue* Queue, int count, int head);      // ��ũ ť�� head�� �������� �����ϰ� ù ť(���)�� ����
void Bring(IOQueue* Queue, int head);              // ���õ� ť(���)�� �����ͼ� ������ ��� ���� 7 ������Ŵ

void enQueue_RP(R_PQueue* Queue, int hour, int min, double s, int data1, int data2, int data3, int data4, int data5, int data6, int data7);       // ������ ����� ����
void enQueue_RD(R_DQueue* Queue, int hour, int min, double s, int count, int head, double seektime);         // ��ũ ����� ����
void Result_RP(IOQueue* Page, R_PQueue* Result, int hour, int min, double s);       // ��ũ ��� �� ��� �Լ�
double Round(double time, int position);           // �ݿø� �Լ�

int main(int argc, char* argv[])
{
	FILE* fp = fopen(argv[1], "rt");
	if (fp == NULL) {
		printf("File Open fail!\n");
		printf("You need input_file.\n");
		return -1;
	}
	FILE* fp2 = fopen("result.txt", "a");
	if (fp2 == NULL) {
		printf("File Open fail!\n");
		printf("New file is being created.\n");
		printf("Please run it again.\n");
		return -1;
	}
	char arr[7][8] = { "pickle", "shrimp", "chicken", "bread", "lettuce", "cheeze", "tomato" };
	int selectNum;
	int q;
	int i = 0;
	double total;
	ReadyQueue* Order = createLinkedQueue();    // ����ť ����
	ReadyQueue* Making = createLinkedQueue();
	ReadyQueue* Delivery = createLinkedQueue();
	ReadyQueue* Result = createLinkedQueue();
	IOQueue* Page = createLinkedQueue_IO();
	IOQueue* Disk = createLinkedQueue_IO();
	R_PQueue* Abox = createLinkedQueue_RP();
	R_DQueue* Bbox = createLinkedQueue_RD();
	q = Input(Order, fp);
	PrintMenu();
	
	for (int i = 0; i < 7; i++)    // �ʱ� ����غ� �� ��Ḧ 7���� ����
		enQueue_IO(Page, arr[i], i, 7);
	while (i != 1) {
		selectNum = 0;
		printf("select num? ");
		scanf("%d", &selectNum);
		printf("\n");
		switch (selectNum)
		{
		case 1:
			if (Order->front == NULL && Order->rear == NULL)      // ����ť�� ����� break
			{
				printf("No Guest!\n");
				break;
			}
			printf("Excuting.... wait plz\n\n");
			Sort(Order, 0);                   // ���ķ� �����ð����� �������� ����
			total = Guest_info(Order, Making, Delivery, Result, q, *arr, Page, Disk, Abox, Bbox);                // ���� �����ϰ� total�� arround_time ���� ����.
			Sort(Result, 1);                                       // ��ȣ ������ �� ����
			PrintResult(Result, Abox, Bbox, total, fp2);              // ��� �� ���
			i = 1;
			break; 	
		case 2:
			PrintGuest(Order);
			break;
		case 3:
			printf("Goodbye~ \n");
			i = 1;
			break;
		default:
			printf("Wrong Input!\n");
			while (getchar() != '\n');     // �Է¹��۸� ���
			break;
		}
	}
	fclose(fp);      //����� ��� ��Ʈ��, ť�� ���
	fclose(fp2);
	free(Order);
	free(Making);
	free(Delivery);
	free(Result);
	free(Page);
	free(Disk);
	free(Abox);
	free(Bbox);
	return 0;
}

void PrintMenu(void) {
	printf("\n**********  Changwon HamHouse  **********");
	printf("\n********** 00:00 AM ~ 23:59 PM **********");
	printf("\n\n\n");
	printf("<< Main Menu >>\n\n");
	printf("* Open The HamHouse                  (1)\n");
	printf("* Print File                         (2)\n");
	printf("* Exit                               (3)\n");
	printf("\n");
	printf("********** Input choice number **********\n\n");
}

ReadyQueue* createLinkedQueue()            // ����ť ���� �Լ�
{
	ReadyQueue* Queue;
	Queue = (ReadyQueue*)malloc(sizeof(ReadyQueue));
	Queue->front = NULL;
	Queue->rear = NULL;
	return Queue;
}

IOQueue* createLinkedQueue_IO()            // ����ť ���� �Լ�
{
	IOQueue* Queue;
	Queue = (IOQueue*)malloc(sizeof(IOQueue));
	Queue->front = NULL;
	Queue->rear = NULL;
	return Queue;
}

R_PQueue* createLinkedQueue_RP()            // ����ť ���� �Լ�
{
	R_PQueue* Queue;
	Queue = (R_PQueue*)malloc(sizeof(R_PQueue));
	Queue->front = NULL;
	Queue->rear = NULL;
	return Queue;
}

R_DQueue* createLinkedQueue_RD()            // ����ť ���� �Լ�
{
	R_DQueue* Queue;
	Queue = (R_DQueue*)malloc(sizeof(R_DQueue));
	Queue->front = NULL;
	Queue->rear = NULL;
	return Queue;
}
void enQueue(ReadyQueue * Queue, int data1, int data2, char* data3, int data4, int data5, double data6, int data7, char* data8)       //�Խ�Ʈ ť ��� ������ ������ �����ϴ� �Լ�
{
	QNode* newNode = (QNode*)malloc(sizeof(QNode));
	newNode->guest = data1;	
	newNode->a_time = data2;
	strcpy(newNode->guest_type, data3);
	newNode->chicken_burger = data4;
	newNode->shrimp_burger = data5;	
	newNode->arround_time = data6;
	newNode->c_num = data7;
	strcpy(newNode->t_num, data8);

	newNode->link = NULL;
	if (Queue->front == NULL)
	{
		Queue->front = newNode;
		Queue->rear = newNode;
	}
	else
	{
		Queue->rear->link = newNode;
		Queue->rear = newNode;
	}	
}

void enQueue_IO(IOQueue* Queue, char* arr, int pagenum, int count)       //IO ť ��� ������ ������ �����ϴ� �Լ�
{
	IONode* newNode = (IONode*)malloc(sizeof(IONode));
	strcpy(newNode->page, arr);
	newNode->page_num = pagenum;
	newNode->page_count = count;

	newNode->link = NULL;
	if (Queue->front == NULL)
	{
		Queue->front = newNode;
		Queue->rear = newNode;
	}
	else
	{
		Queue->rear->link = newNode;
		Queue->rear = newNode;
	}
}

void enQueue_RP(R_PQueue* Queue, int hour, int min, double s, int data1, int data2, int data3, int data4, int data5,int data6, int data7)    // â��A ����� ����� ���� ť�� ���� �����Լ�
{
	ANode* newNode = (ANode*)malloc(sizeof(ANode));
	newNode->hour = hour;
	newNode->min = min;
	newNode->s = s;
	newNode->pickle = data1;
	newNode->shrimp = data2;
	newNode->chicken = data3;
	newNode->bread = data4;
	newNode->lettuce = data5;
	newNode->cheeze = data6;
	newNode->tomato = data7;
	newNode->link = NULL;
	if (Queue->front == NULL)
	{
		Queue->front = newNode;
		Queue->rear = newNode;
	}
	else
	{
		Queue->rear->link = newNode;
		Queue->rear = newNode;
	}
}

void enQueue_RD(R_DQueue* Queue, int hour, int min, double s, int count, int head, double seektime)       // â��B ����� ����� ���� ť�� ���� �����Լ�
{
	BNode* newNode = (BNode*)malloc(sizeof(BNode));
	newNode->hour = hour;
	newNode->min = min;
	newNode->s = s;
	newNode->count = count;
	newNode->head = head;
	newNode->seektime = seektime;

	newNode->link = NULL;
	if (Queue->front == NULL)
	{
		Queue->front = newNode;
		Queue->rear = newNode;
	}
	else
	{
		Queue->rear->link = newNode;
		Queue->rear = newNode;
	}
}

void deQueue(ReadyQueue * Queue)         // ť ���� �Լ�
{
	QNode* old = Queue->front;
	if (Queue->front == NULL)
	{
		printf("Data is not exist\n");
		return;
	}
	else
	{
		Queue->front = Queue->front->link;
		if (Queue->front == NULL)
			Queue->rear = NULL;
		free(old);
	}
}

void deQueue_IO(IOQueue* Queue)         // ť ���� �Լ�
{
	IONode* old = Queue->front;
	if (Queue->front == NULL)
	{
		printf("Data is not exist\n");
		return;
	}
	else
	{
		Queue->front = Queue->front->link;
		if (Queue->front == NULL)
			Queue->rear = NULL;
		free(old);
	}
}

int Input(ReadyQueue * Queue, FILE * fp)                // ���μ��� read 
{
	int a, b, d, e;
	int count = 0;
	int ret;
	int hour = 0;
	int min = 0;
	int q = 0;
	char time[6];
	char timecp[6];
	char c[5];
	char qt[10];
	char* ptr;
	char ham[1000];     // �о�� �ܾ� ���庯��, �Է��ϴ� �� �ֹ��� �ܹ��Ÿ� �� 100�� �̻� �ֹ��� �Ұ����ϴٰ� �����Ѵ�.
	char chicken[7] = "�߰��";
	char shrimp[5] = "����";

	ret = fscanf(fp, "%s", qt);
	ret = fscanf(fp, "%s\t%s\t%s\t%s",ham,ham,ham,ham);
	ptr = strtok(qt, "=");       // Q=5 ���� =�� �������� ���ڸ� �з���
	while (ptr != NULL)
	{
		q = atoi(ptr);
		ptr = strtok(NULL, "=");
	}
	while (1)
	{
		ret = fscanf(fp, "%d\t%s\t%s\t%s", &a, time, c, ham);  // �Է������� ���پ� �о �ʱ�ȭ
		int i = 0;

		strcpy(timecp, time);  // �Է½ð� ����
	
		ptr = strtok(time, ":");  // :�� �������� ���� �и��ϰ� 1�ʴ� 1�� ���� ���ڷ� ��ȯ
		while (ptr != NULL) 
		{
			if (i == 0)
				hour = 3600 * atoi(ptr);
			else
				min = 60 * atoi(ptr);
			i++;
			ptr = strtok(NULL, ":");
		}
		b = hour + min;	
		d = 0;
		e = 0;
		ptr = strstr(ham, chicken);         
		while (ptr != NULL)                 // �Է����Ͽ��� �߰�� / ���츦 ã�Ƽ� ������ ���� 
		{
			d++;
			ptr = strstr(ptr + 1, chicken);        
		}
		ptr = strstr(ham, shrimp);
		while (ptr != NULL)
		{
			e++;
			ptr = strstr(ptr + 1, shrimp);
		}
	
		if (b > 86400)            // 24�ð��� ���� �Է°��� ������ ����
		{
			printf("File Open fail!\n");
			printf("ArivalTime < 86400 (00:00am ~ 23:59pm) \n");
			fclose(fp);
			exit(-1);
		}
		if (ret == EOF)
			break;
		enQueue(Queue, a, b, c, d*100, e*110, 0, 0, timecp);      // ť�� ����
		temp = temp + 1;
		count++;
	}
	if (count < 20 || count > 50)            // �ֹ� ���� 20~50�� �ƴϸ� ����
	{
		printf("File Open fail!\n");
		printf("Guest count must be 20 ~ 50 \n");
		fclose(fp);
		exit(-1);
	}
	if (ret == EOF)
	{
		printf("File Open Success\n");
	}
	return q;
}

void PrintGuest(ReadyQueue * Queue)                               // ť ���
{
	int i = 0;
	char gt[6];
	QNode* node = Queue->front;
	printf("Guest    Arival_time    Guest_type    Chicken_burger    Shrimp_burger\n");
	printf("----------------------------------------------------------------------\n");
	while (node)
	{
		if (strcmp(node->guest_type, "�湮") == 0) strcpy(gt, "Visit");               // �湮�̸�  Visit, ��ȭ�� Call
		else if (strcmp(node->guest_type, "��ȭ") == 0) strcpy(gt, "Call");
		printf("%-11d %-13d %-17s %-17d %d\n", node->guest, node->a_time, gt, node->chicken_burger/100, node->shrimp_burger/110);
		node = node->link;
		i++;
	}
	printf("\nAll Guest Print [ %d ]\n", i);
}

void PrintResult(ReadyQueue * Queue, R_PQueue* R_PQueue, R_DQueue* R_DQueue, double total, FILE* fp2)                         // �˰����� ����� ���� ��� ���� ������ �� ���� 
{                                                                                                                            // ť�� �о�鼭 ��� �� ���, ��� ���� ���
	int i = 0;
	char gt[6];
	char arr[7][8] = { "pickle", "shrimp", "chicken", "bread", "lettuce", "cheeze", "tomato" };        // ��� ��
	QNode* node = Queue->front;
	ANode* anode = R_PQueue->front;
	BNode* bnode = R_DQueue->front;
	printf("Guest    Arival_time    Guest_type    C_time    Arround_time\n");
	printf("--------------------------------------------------------------------------\n");
	fprintf(fp2, "%s\t%s\t%s\t%s\t%s\r\n", "��ȣ", "�����ð�", "�ֹ�����", "���񽺽ð�", "turnaround time");
	while (node)
	{
		if (strcmp(node->guest_type, "�湮") == 0) strcpy(gt, "Visit");
		else if (strcmp(node->guest_type, "��ȭ") == 0) strcpy(gt, "Call");
		printf("%-11d %-13s %-13s %-13d %-13.2f\n", node->guest, node->t_num, gt, node->c_num, Round(node->arround_time, 3));
		fprintf(fp2, "%d\t%s\t%s\t%d\t%.2f\r\n", node->guest, node->t_num, node->guest_type, node->c_num, Round(node->arround_time, 3));
		node = node->link;
		i++;
	}
	fprintf(fp2, "%s\t[%.2f]\r\n", "<Average Arround Time> :" , Round(total / temp, 3));
	fprintf(fp2, "%s\r\n", "<����� ����> :");	
	fprintf(fp2, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n", "�ð�(��/��/��)", "��Ŭ", "����", "�߰��", "��", "����", "ġ��", "�丶��");
	while (anode)
	{
		fprintf(fp2, "%02d:%02d:%05.2f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n", anode->hour, anode->min, anode->s, anode->pickle, anode->shrimp, anode->chicken, anode->bread, anode->lettuce, anode->cheeze, anode->tomato);
		anode = anode->link;
	}
	fprintf(fp2, "%s\r\n", "<��ũ ��ȭ > :");
	fprintf(fp2, "%s\t%s\t%s\t%s\t%s\t\r\n", "�ð�(��/��/��)", "���", "����", "���", "Ž�� �ð�");
	while (bnode) 
	{
		fprintf(fp2, "%02d:%02d:%05.2f\t%s\t%d\t%d\t%.2f\r\n", bnode->hour, bnode->min, bnode->s, arr[bnode->head-1], bnode->count, bnode->head, bnode->seektime);
		bnode = bnode->link;
	}	
	fprintf(fp2, "%s\r\n", "-------------------------------------------------------------------");
	printf("\n");
	printf("Average arround time: [ %.2f ]\n", Round(total / temp, 3));
	printf("Number of sales completion: [ %d ]\n", temp);
	printf("\n*** This time is 24:00.   ***\n"); 
	printf("*** This hamhouse needs to be closed now.   ***\n"); 
	printf("*** If you haven't received the burger yet, we'll give you a refund.   ***\n");
	printf("*** Thank you...   ***\n");
}

double Guest_info(ReadyQueue * Order, ReadyQueue * Making, ReadyQueue * Delivery, ReadyQueue* Result, int q, char* arr, IOQueue* Page, IOQueue* Disk, R_PQueue* Abox, R_DQueue* Bbox)                      // FCFS ���Լ��� ���
{
	int absolute_time = 0;          // ����ð�                                                                     // �ֹ��� ����� ��� ���߿� ������ ������ ������ �� �����Ƿ� ������
	int relative_time_o = 0;        // �ֹ�, ����, ����� �� ť�� ���ð�                                          // ����Ǿ�� �ϱ� ������ ���갡 �Ǵ� �ֹ�, ���ť�� FCFS,
	int relative_time_m = 0;                                                                                        // �ٽ� ť�� ����ť�� RR���� ����
	int relative_time_d = 0;
	int a = 0;
	double b = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int type = 0;
	int tp = 0;
	int gettime = 0;
	double timetemp = 0;
	int dak[5] = { 0, 2, 3, 4, 5 };      // �߰������� �ε��� �� ���� �迭
	int saewoo[5] = { 0, 1, 3, 4, 6 };   // ��������� �ε��� �� ���� �迭
	double arround_time = 0;
	double arround_total = 0;
	QNode* node_o = Order->front;        // �ֹ� ���
	QNode* node_m;                       // ���� ���
	QNode* node_d;                       // ��� ���
	int head = 0;      // �ʱ���� 0
	while (1)
	{                                                            // �ð� ������ ���� absoulte_time, relative_time���� ����
		
		if ((node_o->a_time <= absolute_time) && (i < temp))                // Ŀ�ٶ� while�� ���� ���� �����ϴ½ð��� absolute_time, �� ť���� ���� �ð��� üũ�ϴ� ���ð�
		{
			if (strcmp(node_o->guest_type, "�湮") == 0) type = 10;        // �湮�ֹ� �ð� 5 + ���ð� 5      (�� ���� �ý���)
			else if (strcmp(node_o->guest_type, "��ȭ") == 0) type = 8;    // ��ȭ�ֹ� �ð� 3 + ���ð� 5
			
			if (type == relative_time_o)         // ���ð��� ���Ϸ�ð��� �Ǹ� ���� �� ���� ť ����
			{
				relative_time_o = -1;
				enQueue(Making, node_o->guest, node_o->a_time, node_o->guest_type, node_o->chicken_burger, node_o->shrimp_burger,0, 
					node_o->chicken_burger + node_o->shrimp_burger, node_o->t_num);
				deQueue(Order);

				if(Order->front != NULL)
				    node_o = Order->front;
				i++;
			}
			if (Order->front != NULL)
				relative_time_o++;
		}			

		if ((j < temp) && Making->front != NULL)
		{
			node_m = Making->front;		
			if (node_m->chicken_burger == 0 && node_m->shrimp_burger == 0)             // �߰�����, ġŲ���� cputime�� �Ѵ� ������ ��� ť �̵�
			{
				relative_time_m = -1;
				if (strcmp(node_m->guest_type, "�湮") == 0)               // �湮�� ��� �����ϰ� ��� ť ����
				{
					arround_time = absolute_time - node_m->a_time + node_m->arround_time;
					arround_total += arround_time;
					enQueue(Result, node_m->guest, node_m->a_time, node_m->guest_type, node_m->chicken_burger, node_m->shrimp_burger, arround_time,
						10 + node_m->c_num, node_m->t_num);
					tp = tp + 1;
					j++;
				}
				else if (strcmp(node_m->guest_type, "��ȭ") == 0)           // ��ȭ�� ��� �����ϰ� ��� ť ����
					enQueue(Delivery, node_m->guest, node_m->a_time, node_m->guest_type, node_m->chicken_burger, node_m->shrimp_burger, node_m->arround_time, node_m->c_num, node_m->t_num);
				deQueue(Making);
				if (Making->front != NULL)
				{
					node_m = Making->front;// ť ����, ���� �� ù ��带 front�� ����
				}
			}
			else if (relative_time_m == q) // ���ð��� ���һ����� q���� ������ ��� ť�� �� �ڷ� �̵� 
			{
				enQueue(Making, node_m->guest, node_m->a_time, node_m->guest_type, node_m->chicken_burger, node_m->shrimp_burger, node_m->arround_time, node_m->c_num, node_m->t_num); // ���һ����� �����ϸ� ���� ���μ��� ������ ť�� ����
				deQueue(Making);
				node_m = Making->front;
				relative_time_m = 0;
			}
			int count = 0;
			int headtp = 0;
			int p = 0;

			int hour = 0;
			int min = 0;
			double s = 0;
			double sd = 0;
			if ((node_m->chicken_burger % 100 == 0) && (node_m->chicken_burger != 0))            // �߰����� ���� �ҿ�ð��� 100���̱� ������ mod 100�� 0�̵Ǹ� ���� Making ����
			{
				count = PageCK(Page, Disk, 0);                      // �������� üũ�Ͽ� ������� ������ 0�̸� ��ũ ť �����Լ��� ȣ���ϰ� ī��Ʈ ���� �������� ����
				hour = (int)(absolute_time + timetemp + gettime) / 3600;                   // ���� �ð��� ��/��/�ʷ� ��ȯ
				min = ((int)(absolute_time + timetemp + gettime) % 3600) / 60;
				s = Round((((((int)absolute_time + a + gettime) % 3600) % 60) + b), 3);
				while (count >= 1)                         // ��ũ ť�� 1�� �̻��� ť�� ���� ��
				{					
					headtp = SSTF(Disk, count, head);       //SSTF �˰����� �����ϰ� �����Ǵ� ��� ����	
					Result_RP(Page, Abox, hour, min, s);      // �߰��߰� ������ ���ť ����
					if (PageSize_CK(Page) == 1)
					    Result_RP(Page, Abox, hour, min, s);
					Bring(Page, headtp);                   // ť���� �� ���� ��Ḧ 7�� ������
				
					gettime += bringnum;       // ��Ḧ �������� �ð� ����
					timetemp += abs(head - headtp) * 0.1;       // head�� �̵��ϴµ� �ð� ����
					node_m->arround_time += bringnum + (abs(head - headtp) * 0.1);
					hour = (int)(absolute_time + (int)timetemp + gettime) / 3600;
					min = ((int)(absolute_time + (int)timetemp + gettime) % 3600) / 60;
					a = (int)timetemp;
					b = timetemp - a;
					s =  Round((((((int)absolute_time + a + gettime) % 3600) % 60) + b), 3);

					enQueue_RD(Bbox, hour, min, s, bringnum, headtp, Round(abs(head - headtp) * 0.1, 3));      // �߰��߰� ��ũ ��� ť�� ����
					head = headtp;						
					count--;
					p++;					
				}
				if(p > 0)               
				    Result_RP(Page, Abox, hour, min, s);
				for (int t=0; t < 5; t++)        // ������ ��Ḧ �� ���������� �ش� ��Ḧ �����ϸ� ���ҽ�Ű�� making ����
				{
					Insert_Page(Page, dak[t]);     // �߰����� �ε����� �Ű������� ���					
				}
				Result_RP(Page, Abox, hour, min, s);
			}
			else if ((node_m->shrimp_burger % 110 == 0) && (node_m->shrimp_burger != 0) && (node_m->chicken_burger == 0))    // �߰����Ÿ� ��� ��������� �������� ������Ÿ� ���� 
			{                                                                                                                // �׻� �߰�� -> ���� ������ ����
				count = PageCK(Page, Disk, 1);		
				hour = (int)(absolute_time + timetemp + gettime) / 3600;
				min = ((int)(absolute_time + timetemp + gettime) % 3600) / 60;
				s = Round((((((int)absolute_time + a + gettime) % 3600) % 60) + b), 3);
				while (count >= 1)
				{					
					headtp = SSTF(Disk, count, head);
					Result_RP(Page, Abox, hour, min, s);
					if (PageSize_CK(Page) == 1)
					    Result_RP(Page, Abox, hour, min, s);
					Bring(Page, headtp);

					gettime += bringnum;
					timetemp += abs(head - headtp) * 0.1;
					node_m->arround_time += bringnum + (abs(head - headtp) * 0.1);
					hour = (int)(absolute_time + (int)timetemp + gettime) / 3600;
					min = ((int)(absolute_time + (int)timetemp + gettime) % 3600) / 60;
					a = (int)timetemp;
					b = timetemp - a;
					s = Round((((((int)absolute_time + a + gettime) % 3600) % 60) + b), 3);

					enQueue_RD(Bbox, hour, min, s, bringnum, headtp, Round(abs(head - headtp)* 0.1, 3));
					head = headtp;
					count--;	
					p++;
				}
				if (p > 0)
					Result_RP(Page, Abox, hour, min, s);
				for (int t=0; t < 5; t++)
				{
					Insert_Page(Page, saewoo[t]);      // ������� �ε����� �Ű������� ���	
				}
				Result_RP(Page, Abox, hour, min, s);
			}
			if (node_m->chicken_burger != 0)             // �ð��� �� �Ǹ� ���� ������ ���� ġŲ���ſ��� �߰����� ������ cputime�� 1�ʾ� ���ҽ�Ŵ
				node_m->chicken_burger -= 1;
			else if (node_m->shrimp_burger != 0)
				node_m->shrimp_burger -= 1;		
			relative_time_m++;					// ���ð��� ����
		}	
		if (Delivery->front != NULL)            
		{
			node_d = Delivery->front;		
			if (200 == relative_time_d)                  // ��� ��� ���ð��� 200�� �Ǹ� ���� �� ���ť ����
			{
				relative_time_d = -1;
				arround_time = absolute_time - node_d->a_time + node_d->arround_time;
				arround_total += arround_time;
				enQueue(Result, node_d->guest, node_d->a_time, node_d->guest_type, node_d->chicken_burger, node_d->shrimp_burger, 
					arround_time, 208 + node_d->c_num, node_d->t_num);
				deQueue(Delivery);	
				if (Delivery->front != NULL)
					node_d = Delivery->front;// ť ����, ���� �� ù ��带 front�� ����
				k++;
				tp = tp + 1;
			}
			relative_time_d++;					
		}			
		if (j+k == temp )
			break;	
		else if ((absolute_time + gettime + timetemp) >= 86400)       // ��� �ֹ��� ����� �Ϸ系���� �̷������ ������ ����ð� + I/O�� �Ҹ��ѽð��� 24�ð� �Ǹ� break
		{
			temp = tp;
			break;
		}
		absolute_time++;	 // while�� ���� ����ð� 1�� ����
	}
	return arround_total;       // arround_time�� ���� ��ȯ
}

void Insert_Page(IOQueue* PQueue, int num)          // ������ ���̺��� �� ���� �������� ���� ���� ���, �ֱٿ� ���� ������ �ϼ��� �Ʒ����̵�.
{
	IONode* node = PQueue->front;
	int count;
	
	for (int i = 0; i < 7; i++)       // ����� ����� ������ 7��ŭ ������ üũ
	{
		if (node && (node->page_num == num) && (node->page_count > 0))          // ������ �������� ������� ī���͸� ���ҽ�Ű�� �ش� �������� �� �ڷ� ����, �� ���� ���� ���� ���ȵ� ���� ���� --> ť �ڷᱸ���� ���� �̿�
		{
			if (i == 0)                                 // ù��° �������� ���� ���, �ش� �������� �����ϰ� ī��Ʈ�� �����Ͽ� �ٽ� ���������� ��
			{                                           // �� ���� �������� �� �Ʒ��� �������Ե�. ���� ���� �ֱٿ� ���� ���������ǰ� 
				count = node->page_count - 1;           // �ش� �������� �� �Ʒ��ʿ� ��ġ.
				char b[8];
				strcpy(b, node->page);
				deQueue_IO(PQueue);
				enQueue_IO(PQueue, b, num, count);
			}
			else if (i > 0 && i < 6 && (node->page_count > 0))             // ��� �������� ���� ���, �ش� ������ ī��Ʈ�� ���ҽ�Ű�� �� �ڷ� ����.
			{	                                        // Change �Լ��� �ش� �������� ������ ť ���� ��ġ�� �����Ŵ
				count = node->page_count - 1;           // --> ���� ���� �ֱٿ� ���� �������� �� �Ʒ��ʿ� ��ġ�ϰԵ�.
				node->page_count = count;
				Change(node);
			}
			else if (i == 6 && (node->page_count > 0))                            // ������ �������� ���� ���, �ش� �������� ���� �ֱ��� �������̱� ������ ��ġ ��ȭ���� ī���͸� ���ҽ�Ŵ
				node->page_count -= 1;
			break;
		}
		node = node->link;
	}
}

int PageCK(IOQueue* PQueue, IOQueue* DQueue, int ck)           // ������ üũ �Լ�
{
	IONode* Page = PQueue->front;
	int count = 0;	
	for (int i = 0; i < 7; i++)
	{
		if (Page->page_count <= 0)          // ������ ī���Ͱ� 0�� ���
		{
			if (ck == 0)
			{
				if (Page->page_num == 0 || Page->page_num == 2 || Page->page_num == 3 || Page->page_num == 4 || Page->page_num == 5)  // �߰������� ��� �ش� ��Ḧ ��ũ ť�� ����
					count = Insert_Disk(DQueue, Page->page, Page->page_num);		
			}
			else
			{
				if (Page->page_num == 0 || Page->page_num == 1 || Page->page_num == 3 || Page->page_num == 4 || Page->page_num == 6)  // ��������� ��� �ش� ��Ḧ ��ũ ť�� ����
					count = Insert_Disk(DQueue, Page->page, Page->page_num);
			}
		}
		Page = Page->link;
	}	
	return count;
}
int PageSize_CK(IOQueue* PQueue)          // �޸� ũ�Ⱑ 50�ε� ��Ḧ �����ͼ� ���� ������ ���� ���, �޸𸮿��� lru �˰��� ���ؼ� ���� ������ ��Ḧ 1���� â��B�� ��.
{
	IONode* node = PQueue->front;
	int count = 0;
	int size = 0;
	int ck = 0;
	while (node)
	{
		size += node->page_count;
		node = node->link;
	}

	while(size > 50 - bringnum)          // �� ���� ��᾿ bringnum ���� ���� ����� ������ŭ �������� ������ size > 50 - bringnum ������ ����
	{
		node = PQueue->front;
		for (int i = 0; i < 7; i++)       // ����� ����� ������ 7��ŭ ������ üũ
		{
			if (node && (node->page_count > 1))          // �ʿ���� ��Ḧ ������ ȿ���� ���� �ּ��� 1���� ����
			{
				if (i == 0)                                 // ù��° �������� ���� ���, �ش� �������� �����ϰ� ī��Ʈ�� �����Ͽ� �ٽ� ������
				{                                           // �� ���� �������� �� �Ʒ��� �������Ե�. ���� ���� �ֱٿ� ���� ���������ǰ� 
					count = node->page_count - 1;           // �ش� �������� �� �Ʒ��ʿ� ��ġ.
					char b[8];
					int n = node->page_num;
					strcpy(b, node->page);
					deQueue_IO(PQueue);
					enQueue_IO(PQueue, b, n, count);
				}
				else if (i > 0 && i < 6 && (node->page_count > 1))             // ��� �������� ���� ���, �ش� ������ ī��Ʈ�� ���ҽ�Ű�� �� �ڷ� ����.
				{	                                       
					count = node->page_count - 1;           // --> ���� ���� �ֱٿ� ���� �������� �� �Ʒ��ʿ� ��ġ�ϰԵ�.
					node->page_count = count;
					Change(node);                            // Change �Լ��� �ش� �������� ������ ť ���� ��ġ�� �����Ŵ
				}
				else if (i == 6 && (node->page_count > 1))                            // ������ �������� ���� ���, �ش� �������� ���� �ֱ��� �������̱� ������ ��ġ ��ȭ���� ī���͸� ���ҽ�Ŵ
					node->page_count -= 1;
				size--;                         // ������ ����
				ck = 1;
				break;
			}			
			node = node->link;
		}
	}
	return ck;
}

void Result_RP(IOQueue* Page, R_PQueue* Result, int hour, int min, double s)               // ������ ��� �� ����
{
	int i = 0;
	int a[7];
	IONode* node = Page->front;
	while (node)
	{
		if (node->page_num == i)
		{			
			a[i] = node->page_count;
			i = -1;
			node = node->link;
		}
		i++;
	}
	enQueue_RP(Result, hour, min, s, a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
}

int Insert_Disk(IOQueue* DQueue, char* arr, int num)          // ��ũ ť ���� �Լ�
{
	IONode* disk = DQueue->front;
	int count = 0;
	while (disk)
	{	
		count++;
		disk = disk->link;
	}
	enQueue_IO(DQueue, arr, num, 1);
	count++;	
	return count;        // ť�� �ִ� ��� ���� ��ȯ��
}

void Change(IONode * Page)                   // �����Ǵ� �������� ���������̺� ����� ��ġ�� ��� ���� ����� ��ġ�ϴ� �Լ�
{                                           
	char a[8];
	int b, c;
	IONode* Page2;
	Page2 = Page->link;
	while (Page2)
	{
		strcpy(a, Page2->page);
		b = Page2->page_num;
		c = Page2->page_count;

		strcpy(Page2->page, Page->page);
		Page2->page_num = Page->page_num;
		Page2->page_count = Page->page_count;

		strcpy(Page->page, a);
		Page->page_num = b;
		Page->page_count = c;

		Page = Page->link;
		Page2 = Page2->link;
	}
}

void Sort(ReadyQueue * Queue, int num)                     // �������� ���
{
	QNode* temp1;
	QNode* temp2;
	int a, b, d, e, g;
	double f;
	char c[5];
	char h[6];
	for (int j = temp - 1; j > 0; j--)
	{
		temp1 = Queue->front;
		temp2 = temp1->link;
		for (int i = 0; i < j; i++)
		{
			if (num == 0)
			{
				if (temp1->a_time > temp2->a_time)               // ����Ʈ�� ���� �����ϴ°��� �ƴ� �����͸� ��ȯ�ϴ� ���Ĺ��
				{                                                // num = 0�̸� �����ð� ����
					a = temp1->guest;			
					b = temp1->a_time;
					strcpy(c, temp1->guest_type);
					d = temp1->chicken_burger;
					e = temp1->shrimp_burger;
					strcpy(h, temp1->t_num);

					temp1->guest = temp2->guest;				
					temp1->a_time = temp2->a_time;
					strcpy(temp1->guest_type, temp2->guest_type);
					temp1->chicken_burger = temp2->chicken_burger;
					temp1->shrimp_burger = temp2->shrimp_burger;
					strcpy(temp1->t_num, temp2->t_num);

					temp2->guest = a;
					temp2->a_time = b;
					strcpy(temp2->guest_type, c);
					temp2->chicken_burger = d;
					temp2->shrimp_burger = e;
					strcpy(temp2->t_num, h);
				}
			}
			else
			{
				if (temp1->guest > temp2->guest)                      // num != 0 �̸� �Խ�Ʈ��ȣ ����
				{
					a = temp1->guest;				
					b = temp1->a_time;
					strcpy(c, temp1->guest_type);
					d = temp1->chicken_burger;
					e = temp1->shrimp_burger;
					f = temp1->arround_time;
					g = temp1->c_num;
					strcpy(h, temp1->t_num);

					temp1->guest = temp2->guest;				
					temp1->a_time = temp2->a_time;
					strcpy(temp1->guest_type, temp2->guest_type);
					temp1->chicken_burger = temp2->chicken_burger;
					temp1->shrimp_burger = temp2->shrimp_burger;
					temp1->arround_time = temp2->arround_time;
					temp1->c_num = temp2->c_num;
					strcpy(temp1->t_num, temp2->t_num);

					temp2->guest = a;				
					temp2->a_time = b;
					strcpy(temp2->guest_type, c);
					temp2->chicken_burger = d;
					temp2->shrimp_burger = e;
					temp2->arround_time = f;
					temp2->c_num = g;
					strcpy(temp2->t_num, h);
				}
			}
			temp1 = temp1->link;
			temp2 = temp2->link;
		}
	}
}

int SSTF(IOQueue* Queue, int count, int head)                   // ��ũ ��� ť�� head�� ����� ������ ������ ������ �� �ش� ��Ḧ �̵�
{
	int b, c;
	int hd = 0;
	char a[8];
	IONode* temp1 = Queue->front;
	IONode* temp2; 	
	for (int i = 0; i < count-1; i++)
	{		
		temp2 = temp1->link;		
		if (abs(head-1 - temp1->page_num) > abs(head-1 - temp2->page_num))               // head - page_num ���� ���방�� ���Ͽ� ���������� ����
		{
			strcpy(a, temp1->page);
			b = temp1->page_num;
			c = temp1->page_count;

			strcpy(temp1->page, temp2->page);
		    temp1->page_num = temp2->page_num;
			temp1->page_count = temp2->page_count;;

			strcpy(temp2->page, a);
			temp2->page_num = b;
			temp2->page_count = c;	
		}
		temp1 = temp1->link;
		temp2 = temp2->link;
	}
	temp1 = Queue->front;
	if (temp1)
	{		
		hd = temp1->page_num + 1;
		deQueue_IO(Queue);         // �ش� ť�� ������ ������ ��ȣ�� ���� ������ �ε����� ���� ������ ������ ��������ȣ�� ���� ���� ���� ����ϰԲ� ��ȯ
		return hd;	
	}
	else
	{
		deQueue_IO(Queue);		
		return head;
	}
}
void Bring(IOQueue* Queue, int head)      // SSTF���� ������ �ش� ������ ��ȣ�� �Ű������� �޾Ƽ� â��A�� ������. 1�� ä���� �� ���� ������ �������� ����� ������ 7���� ����
{
	IONode* node = Queue->front;
	while (node)
	{
		if (node->page_num == head-1)
		{
			node->page_count += bringnum;
			break;
		}
		node = node->link;
	}
}

double Round(double time, int position) // I/O �߻��� �ð��� ������ ���̱����� �ݿø� �Լ�
{
	return ((int)((time + 5 / pow(10, position)) * pow(10, position - 1)) / pow(10, position - 1));
}
