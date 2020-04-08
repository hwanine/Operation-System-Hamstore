
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>
#include<math.h>

typedef struct            // 게스트 노드 구조체 설정
{
	int guest;                // 손님번호, 도착시간, 손님타입, 닭고기버거, 새우버거, 어라운드타임, CPU타임 백업, 입력 시간 출력을위한 백업변수	
	int a_time;
	char guest_type[5];
	int chicken_burger;
	int shrimp_burger;
	double arround_time;
	int c_num;        
	char t_num[5];
	struct QNode* link;         // 다음 노드 링크
}QNode;

typedef struct           // 페이지 노드 구조체
{
	char page[8];               // 페이지 이름
	int page_num;               // 페이지 번호 0 ~ 6
	int page_count;             // 페이지 스택
	struct IONode* link;        // 다음 노드 링크
}IONode;

typedef struct    // 페이지 노드 구조체
{
	int hour;
	int min;
	double s;      // 페이지 이름
	int pickle;      // 페이지 번호 0 ~ 6 (각각의 재료)
	int shrimp;
	int chicken;
	int bread;
	int lettuce;
	int cheeze;
	int tomato;
	struct ANode* link;            // 다음 노드 링크
}ANode;

typedef struct                // 페이지 노드 구조체
{
	int hour;                // 출력을 위한 시
	int min;                 // 분
	double s;                // 초
	int count;               // 페이지 번호 0 ~ 6
	int head;                // 페이지 스택
	double seektime;         // 찾는 시간
	struct BNode* link;      // 다음 노드 링크
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

int temp = 0;                                     // 전역변수 리스트의 인덱스 값 가상으로 설정
const int bringnum = 7;                     // const상수로 가져올 재료의 갯수를 지정할 수 있음.

ReadyQueue* createLinkedQueue();
IOQueue* createLinkedQueue_IO();
R_PQueue* createLinkedQueue_RP();
R_DQueue* createLinkedQueue_RD();

void PrintMenu(void);                   // 메뉴 출력
void enQueue(ReadyQueue* Queue, int data1, int data2, char* data3, int data4, int data5, double data6, int data7, char* data8);         // 프로세스 큐 삽입
void deQueue(ReadyQueue* Queue);                         // 프로세스 큐 삭제
int Input(ReadyQueue* Queue, FILE* fp);                  // 파일 읽기
void PrintGuest(ReadyQueue* Queue);                    // 전체 게스트 출력
void PrintResult(ReadyQueue* Queue, R_PQueue* R_PQueue, R_DQueue* R_DQueue, double total, FILE* fp2);                     // 결과 값 전체 출력
double Guest_info(ReadyQueue* Order, ReadyQueue* Making, ReadyQueue* Delivery, ReadyQueue* Result, int q, char* arr, 
	IOQueue * Page, IOQueue* Disk, R_PQueue* Abox, R_DQueue* Bbox);                       // 손님에 대한 함수. 핵심함수로써, 모든 동작이 이 함수 내에서 시간마다 동작하게됨
void Sort(ReadyQueue* Queue, int num);                                              // 버블정렬, 값 대치로 구현

void enQueue_IO(IOQueue* Queue, char* arr, int pagenum, int count);                     // 페이지 삽입
void deQueue_IO(IOQueue* Queue);                                             // 페이지 삭제
void Insert_Page(IOQueue* PQueue, int num);                            // 호출될 때 마다 해당 재료 카운트 감소
int PageSize_CK(IOQueue* PQueue);                                     // 페이지 사이즈 크기를 초과할때 냉장고에서 재료를 창고로 뺌
void Change(IONode* Page);                                              // LRU알고리즘 정렬

int PageCK(IOQueue* PQueue, IOQueue* DQueue, int ck);                   // 재료가 없는 페이지 체크
int Insert_Disk(IOQueue* Queue, char* arr, int num);                    // 체크된 페이지 디스크 큐에 삽입
int SSTF(IOQueue* Queue, int count, int head);      // 디스크 큐를 head를 기준으로 정렬하고 첫 큐(재료)를 선택
void Bring(IOQueue* Queue, int head);              // 선택된 큐(재료)를 가져와서 페이지 재료 수를 7 증가시킴

void enQueue_RP(R_PQueue* Queue, int hour, int min, double s, int data1, int data2, int data3, int data4, int data5, int data6, int data7);       // 페이지 결과값 삽입
void enQueue_RD(R_DQueue* Queue, int hour, int min, double s, int count, int head, double seektime);         // 디스크 결과값 삽입
void Result_RP(IOQueue* Page, R_PQueue* Result, int hour, int min, double s);       // 디스크 결과 값 출력 함수
double Round(double time, int position);           // 반올림 함수

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
	ReadyQueue* Order = createLinkedQueue();    // 공백큐 생성
	ReadyQueue* Making = createLinkedQueue();
	ReadyQueue* Delivery = createLinkedQueue();
	ReadyQueue* Result = createLinkedQueue();
	IOQueue* Page = createLinkedQueue_IO();
	IOQueue* Disk = createLinkedQueue_IO();
	R_PQueue* Abox = createLinkedQueue_RP();
	R_DQueue* Bbox = createLinkedQueue_RD();
	q = Input(Order, fp);
	PrintMenu();
	
	for (int i = 0; i < 7; i++)    // 초기 재료준비 각 재료를 7개씩 보관
		enQueue_IO(Page, arr[i], i, 7);
	while (i != 1) {
		selectNum = 0;
		printf("select num? ");
		scanf("%d", &selectNum);
		printf("\n");
		switch (selectNum)
		{
		case 1:
			if (Order->front == NULL && Order->rear == NULL)      // 공백큐면 부재로 break
			{
				printf("No Guest!\n");
				break;
			}
			printf("Excuting.... wait plz\n\n");
			Sort(Order, 0);                   // 정렬로 도착시간별로 오름차순 정렬
			total = Guest_info(Order, Making, Delivery, Result, q, *arr, Page, Disk, Abox, Bbox);                // 연산 진행하고 total에 arround_time 합을 구함.
			Sort(Result, 1);                                       // 번호 순으로 재 졍렬
			PrintResult(Result, Abox, Bbox, total, fp2);              // 결과 값 출력
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
			while (getchar() != '\n');     // 입력버퍼를 비움
			break;
		}
	}
	fclose(fp);      //종료시 모든 스트림, 큐를 비움
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

ReadyQueue* createLinkedQueue()            // 공백큐 생성 함수
{
	ReadyQueue* Queue;
	Queue = (ReadyQueue*)malloc(sizeof(ReadyQueue));
	Queue->front = NULL;
	Queue->rear = NULL;
	return Queue;
}

IOQueue* createLinkedQueue_IO()            // 공백큐 생성 함수
{
	IOQueue* Queue;
	Queue = (IOQueue*)malloc(sizeof(IOQueue));
	Queue->front = NULL;
	Queue->rear = NULL;
	return Queue;
}

R_PQueue* createLinkedQueue_RP()            // 공백큐 생성 함수
{
	R_PQueue* Queue;
	Queue = (R_PQueue*)malloc(sizeof(R_PQueue));
	Queue->front = NULL;
	Queue->rear = NULL;
	return Queue;
}

R_DQueue* createLinkedQueue_RD()            // 공백큐 생성 함수
{
	R_DQueue* Queue;
	Queue = (R_DQueue*)malloc(sizeof(R_DQueue));
	Queue->front = NULL;
	Queue->rear = NULL;
	return Queue;
}
void enQueue(ReadyQueue * Queue, int data1, int data2, char* data3, int data4, int data5, double data6, int data7, char* data8)       //게스트 큐 노드 각각에 데이터 삽입하는 함수
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

void enQueue_IO(IOQueue* Queue, char* arr, int pagenum, int count)       //IO 큐 노드 각각에 데이터 삽입하는 함수
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

void enQueue_RP(R_PQueue* Queue, int hour, int min, double s, int data1, int data2, int data3, int data4, int data5,int data6, int data7)    // 창고A 결과값 출력을 위한 큐에 대한 삽입함수
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

void enQueue_RD(R_DQueue* Queue, int hour, int min, double s, int count, int head, double seektime)       // 창고B 결과값 출력을 위한 큐에 대한 삽입함수
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

void deQueue(ReadyQueue * Queue)         // 큐 삭제 함수
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

void deQueue_IO(IOQueue* Queue)         // 큐 삭제 함수
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

int Input(ReadyQueue * Queue, FILE * fp)                // 프로세스 read 
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
	char ham[1000];     // 읽어올 단어 저장변수, 입력하는 한 주문에 햄버거를 약 100개 이상 주문은 불가능하다고 가정한다.
	char chicken[7] = "닭고기";
	char shrimp[5] = "새우";

	ret = fscanf(fp, "%s", qt);
	ret = fscanf(fp, "%s\t%s\t%s\t%s",ham,ham,ham,ham);
	ptr = strtok(qt, "=");       // Q=5 에서 =를 기준으로 문자를 분러함
	while (ptr != NULL)
	{
		q = atoi(ptr);
		ptr = strtok(NULL, "=");
	}
	while (1)
	{
		ret = fscanf(fp, "%d\t%s\t%s\t%s", &a, time, c, ham);  // 입력파일을 한줄씩 읽어서 초기화
		int i = 0;

		strcpy(timecp, time);  // 입력시간 보관
	
		ptr = strtok(time, ":");  // :를 기준으로 문장 분리하고 1초당 1의 값인 숫자로 변환
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
		while (ptr != NULL)                 // 입력파일에서 닭고기 / 새우를 찾아서 갯수를 구함 
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
	
		if (b > 86400)            // 24시간이 지난 입력값을 받으면 종료
		{
			printf("File Open fail!\n");
			printf("ArivalTime < 86400 (00:00am ~ 23:59pm) \n");
			fclose(fp);
			exit(-1);
		}
		if (ret == EOF)
			break;
		enQueue(Queue, a, b, c, d*100, e*110, 0, 0, timecp);      // 큐에 삽입
		temp = temp + 1;
		count++;
	}
	if (count < 20 || count > 50)            // 주문 수가 20~50이 아니면 종료
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

void PrintGuest(ReadyQueue * Queue)                               // 큐 출력
{
	int i = 0;
	char gt[6];
	QNode* node = Queue->front;
	printf("Guest    Arival_time    Guest_type    Chicken_burger    Shrimp_burger\n");
	printf("----------------------------------------------------------------------\n");
	while (node)
	{
		if (strcmp(node->guest_type, "방문") == 0) strcpy(gt, "Visit");               // 방문이면  Visit, 전화면 Call
		else if (strcmp(node->guest_type, "전화") == 0) strcpy(gt, "Call");
		printf("%-11d %-13d %-17s %-17d %d\n", node->guest, node->a_time, gt, node->chicken_burger/100, node->shrimp_burger/110);
		node = node->link;
		i++;
	}
	printf("\nAll Guest Print [ %d ]\n", i);
}

void PrintResult(ReadyQueue * Queue, R_PQueue* R_PQueue, R_DQueue* R_DQueue, double total, FILE* fp2)                         // 알고리즘을 통과한 직후 결과 값을 저장한 몇 개의 
{                                                                                                                            // 큐를 읽어가면서 결과 값 출력, 출력 파일 기록
	int i = 0;
	char gt[6];
	char arr[7][8] = { "pickle", "shrimp", "chicken", "bread", "lettuce", "cheeze", "tomato" };        // 재료 값
	QNode* node = Queue->front;
	ANode* anode = R_PQueue->front;
	BNode* bnode = R_DQueue->front;
	printf("Guest    Arival_time    Guest_type    C_time    Arround_time\n");
	printf("--------------------------------------------------------------------------\n");
	fprintf(fp2, "%s\t%s\t%s\t%s\t%s\r\n", "번호", "도착시간", "주문종류", "서비스시간", "turnaround time");
	while (node)
	{
		if (strcmp(node->guest_type, "방문") == 0) strcpy(gt, "Visit");
		else if (strcmp(node->guest_type, "전화") == 0) strcpy(gt, "Call");
		printf("%-11d %-13s %-13s %-13d %-13.2f\n", node->guest, node->t_num, gt, node->c_num, Round(node->arround_time, 3));
		fprintf(fp2, "%d\t%s\t%s\t%d\t%.2f\r\n", node->guest, node->t_num, node->guest_type, node->c_num, Round(node->arround_time, 3));
		node = node->link;
		i++;
	}
	fprintf(fp2, "%s\t[%.2f]\r\n", "<Average Arround Time> :" , Round(total / temp, 3));
	fprintf(fp2, "%s\r\n", "<냉장고 상태> :");	
	fprintf(fp2, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n", "시간(시/분/초)", "피클", "새우", "닭고기", "빵", "상추", "치즈", "토마토");
	while (anode)
	{
		fprintf(fp2, "%02d:%02d:%05.2f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n", anode->hour, anode->min, anode->s, anode->pickle, anode->shrimp, anode->chicken, anode->bread, anode->lettuce, anode->cheeze, anode->tomato);
		anode = anode->link;
	}
	fprintf(fp2, "%s\r\n", "<디스크 변화 > :");
	fprintf(fp2, "%s\t%s\t%s\t%s\t%s\t\r\n", "시간(시/분/초)", "재료", "수량", "헤드", "탐색 시간");
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

double Guest_info(ReadyQueue * Order, ReadyQueue * Making, ReadyQueue * Delivery, ReadyQueue* Result, int q, char* arr, IOQueue* Page, IOQueue* Disk, R_PQueue* Abox, R_DQueue* Bbox)                      // FCFS 선입선출 방식
{
	int absolute_time = 0;          // 절대시간                                                                     // 주문과 배달의 경우 도중에 퀀텀을 가지고 중지할 수 없으므로 무조건
	int relative_time_o = 0;        // 주문, 제조, 배달의 각 큐의 상대시간                                          // 실행되어야 하기 때문에 서브가 되는 주문, 배달큐는 FCFS,
	int relative_time_m = 0;                                                                                        // 핵심 큐인 제조큐는 RR으로 구현
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
	int dak[5] = { 0, 2, 3, 4, 5 };      // 닭고기버거의 인덱스 값 저장 배열
	int saewoo[5] = { 0, 1, 3, 4, 6 };   // 새우버거의 인덱스 값 저장 배열
	double arround_time = 0;
	double arround_total = 0;
	QNode* node_o = Order->front;        // 주문 노드
	QNode* node_m;                       // 제조 노드
	QNode* node_d;                       // 배달 노드
	int head = 0;      // 초기헤드 0
	while (1)
	{                                                            // 시간 개념을 변수 absoulte_time, relative_time으로 구현
		
		if ((node_o->a_time <= absolute_time) && (i < temp))                // 커다란 while문 안의 실제 동작하는시간을 absolute_time, 각 큐마다 동작 시간을 체크하는 상대시간
		{
			if (strcmp(node_o->guest_type, "방문") == 0) type = 10;        // 방문주문 시간 5 + 계산시간 5      (선 결제 시스템)
			else if (strcmp(node_o->guest_type, "전화") == 0) type = 8;    // 전화주문 시간 3 + 계산시간 5
			
			if (type == relative_time_o)         // 상대시간이 계산완료시간이 되면 삭제 후 제조 큐 삽입
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
			if (node_m->chicken_burger == 0 && node_m->shrimp_burger == 0)             // 닭고기버거, 치킨버거 cputime이 둘다 소진될 경우 큐 이동
			{
				relative_time_m = -1;
				if (strcmp(node_m->guest_type, "방문") == 0)               // 방문일 경우 종료하고 결과 큐 삽입
				{
					arround_time = absolute_time - node_m->a_time + node_m->arround_time;
					arround_total += arround_time;
					enQueue(Result, node_m->guest, node_m->a_time, node_m->guest_type, node_m->chicken_burger, node_m->shrimp_burger, arround_time,
						10 + node_m->c_num, node_m->t_num);
					tp = tp + 1;
					j++;
				}
				else if (strcmp(node_m->guest_type, "전화") == 0)           // 전화일 경우 종료하고 배달 큐 삽입
					enQueue(Delivery, node_m->guest, node_m->a_time, node_m->guest_type, node_m->chicken_burger, node_m->shrimp_burger, node_m->arround_time, node_m->c_num, node_m->t_num);
				deQueue(Making);
				if (Making->front != NULL)
				{
					node_m = Making->front;// 큐 삽입, 삭제 후 첫 노드를 front값 지정
				}
			}
			else if (relative_time_m == q) // 상대시간이 퀀텀사이즈 q값과 같아질 경우 큐의 맨 뒤로 이동 
			{
				enQueue(Making, node_m->guest, node_m->a_time, node_m->guest_type, node_m->chicken_burger, node_m->shrimp_burger, node_m->arround_time, node_m->c_num, node_m->t_num); // 퀀텀사이즈 오버하면 남은 프로세스 마지막 큐에 삽입
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
			if ((node_m->chicken_burger % 100 == 0) && (node_m->chicken_burger != 0))            // 닭고기버거 제조 소요시간이 100초이기 때문에 mod 100이 0이되면 새로 Making 시작
			{
				count = PageCK(Page, Disk, 0);                      // 페이지를 체크하여 남은재료 갯수가 0이면 디스크 큐 삽입함수를 호출하고 카운트 값을 증가시켜 리턴
				hour = (int)(absolute_time + timetemp + gettime) / 3600;                   // 현재 시간을 시/분/초로 변환
				min = ((int)(absolute_time + timetemp + gettime) % 3600) / 60;
				s = Round((((((int)absolute_time + a + gettime) % 3600) % 60) + b), 3);
				while (count >= 1)                         // 디스크 큐에 1개 이상의 큐가 있을 때
				{					
					headtp = SSTF(Disk, count, head);       //SSTF 알고리즘을 실행하고 참조되는 헤드 리턴	
					Result_RP(Page, Abox, hour, min, s);      // 중간중간 페이지 결과큐 삽입
					if (PageSize_CK(Page) == 1)
					    Result_RP(Page, Abox, hour, min, s);
					Bring(Page, headtp);                   // 큐에서 한 종류 재료를 7개 가져옴
				
					gettime += bringnum;       // 재료를 가져오는 시간 변수
					timetemp += abs(head - headtp) * 0.1;       // head를 이동하는데 시간 변수
					node_m->arround_time += bringnum + (abs(head - headtp) * 0.1);
					hour = (int)(absolute_time + (int)timetemp + gettime) / 3600;
					min = ((int)(absolute_time + (int)timetemp + gettime) % 3600) / 60;
					a = (int)timetemp;
					b = timetemp - a;
					s =  Round((((((int)absolute_time + a + gettime) % 3600) % 60) + b), 3);

					enQueue_RD(Bbox, hour, min, s, bringnum, headtp, Round(abs(head - headtp) * 0.1, 3));      // 중간중간 디스크 결과 큐에 삽입
					head = headtp;						
					count--;
					p++;					
				}
				if(p > 0)               
				    Result_RP(Page, Abox, hour, min, s);
				for (int t=0; t < 5; t++)        // 부족한 재료를 다 가져왔으면 해당 재료를 참조하며 감소시키고 making 시작
				{
					Insert_Page(Page, dak[t]);     // 닭고기버거 인덱스를 매개변수로 사용					
				}
				Result_RP(Page, Abox, hour, min, s);
			}
			else if ((node_m->shrimp_burger % 110 == 0) && (node_m->shrimp_burger != 0) && (node_m->chicken_burger == 0))    // 닭고기버거를 모두 만들었으면 다음으로 새우버거를 만듦 
			{                                                                                                                // 항상 닭고기 -> 새우 순으로 제조
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
					Insert_Page(Page, saewoo[t]);      // 새우버거 인덱스를 매개변수로 사용	
				}
				Result_RP(Page, Abox, hour, min, s);
			}
			if (node_m->chicken_burger != 0)             // 시간이 다 되면 다음 연산을 위해 치킨버거에서 닭고기버거 순서로 cputime을 1초씩 감소시킴
				node_m->chicken_burger -= 1;
			else if (node_m->shrimp_burger != 0)
				node_m->shrimp_burger -= 1;		
			relative_time_m++;					// 상대시간은 증가
		}	
		if (Delivery->front != NULL)            
		{
			node_d = Delivery->front;		
			if (200 == relative_time_d)                  // 배달 노드 상대시간이 200이 되면 종료 후 결과큐 삽입
			{
				relative_time_d = -1;
				arround_time = absolute_time - node_d->a_time + node_d->arround_time;
				arround_total += arround_time;
				enQueue(Result, node_d->guest, node_d->a_time, node_d->guest_type, node_d->chicken_burger, node_d->shrimp_burger, 
					arround_time, 208 + node_d->c_num, node_d->t_num);
				deQueue(Delivery);	
				if (Delivery->front != NULL)
					node_d = Delivery->front;// 큐 삽입, 삭제 후 첫 노드를 front값 지정
				k++;
				tp = tp + 1;
			}
			relative_time_d++;					
		}			
		if (j+k == temp )
			break;	
		else if ((absolute_time + gettime + timetemp) >= 86400)       // 모든 주문과 배달은 하루내에만 이루어지기 때문에 절대시간 + I/O로 소모한시간이 24시가 되면 break
		{
			temp = tp;
			break;
		}
		absolute_time++;	 // while문 끝에 절대시간 1씩 증가
	}
	return arround_total;       // arround_time의 총합 반환
}

void Insert_Page(IOQueue* PQueue, int num)          // 페이지 테이블의 맨 위의 페이지는 가장 오래 재료, 최근에 사용된 페이지 일수록 아랫값이됨.
{
	IONode* node = PQueue->front;
	int count;
	
	for (int i = 0; i < 7; i++)       // 사이즈가 재료의 종류인 7만큼 페이지 체크
	{
		if (node && (node->page_num == num) && (node->page_count > 0))          // 기존의 페이지와 같을경우 카운터를 감소시키고 해당 페이지는 맨 뒤로 삽입, 맨 위의 가장 오래 사용안된 노드는 삭제 --> 큐 자료구조의 원리 이용
		{
			if (i == 0)                                 // 첫번째 페이지와 같을 경우, 해당 페이지를 삭제하고 카운트를 감소하여 다시 삽입함으로 써
			{                                           // 맨 위의 페이지가 맨 아래로 내려가게됨. 따라서 가장 최근에 사용된 페이지가되고 
				count = node->page_count - 1;           // 해당 페이지는 맨 아래쪽에 위치.
				char b[8];
				strcpy(b, node->page);
				deQueue_IO(PQueue);
				enQueue_IO(PQueue, b, num, count);
			}
			else if (i > 0 && i < 6 && (node->page_count > 0))             // 가운데 페이지와 같을 경우, 해당 페이지 카운트를 감소시키고 맨 뒤로 보냄.
			{	                                        // Change 함수는 해당 페이지를 마지막 큐 노드로 위치를 변경시킴
				count = node->page_count - 1;           // --> 역시 가장 최근에 사용된 페이지는 맨 아래쪽에 위치하게됨.
				node->page_count = count;
				Change(node);
			}
			else if (i == 6 && (node->page_count > 0))                            // 마지막 페이지와 같을 경우, 해당 페이지는 가장 최근의 페이지이기 때문에 위치 변화없이 카운터만 감소시킴
				node->page_count -= 1;
			break;
		}
		node = node->link;
	}
}

int PageCK(IOQueue* PQueue, IOQueue* DQueue, int ck)           // 페이지 체크 함수
{
	IONode* Page = PQueue->front;
	int count = 0;	
	for (int i = 0; i < 7; i++)
	{
		if (Page->page_count <= 0)          // 페이지 카운터가 0일 경우
		{
			if (ck == 0)
			{
				if (Page->page_num == 0 || Page->page_num == 2 || Page->page_num == 3 || Page->page_num == 4 || Page->page_num == 5)  // 닭고기버거일 경우 해당 재료를 디스크 큐에 삽입
					count = Insert_Disk(DQueue, Page->page, Page->page_num);		
			}
			else
			{
				if (Page->page_num == 0 || Page->page_num == 1 || Page->page_num == 3 || Page->page_num == 4 || Page->page_num == 6)  // 새우버거일 경우 해당 재료를 디스크 큐에 삽입
					count = Insert_Disk(DQueue, Page->page, Page->page_num);
			}
		}
		Page = Page->link;
	}	
	return count;
}
int PageSize_CK(IOQueue* PQueue)          // 메모리 크기가 50인데 재료를 가져와서 넣을 공간이 없을 경우, 메모리에서 lru 알고리즘에 의해서 가장 오래된 재료를 1개씩 창고B로 뺌.
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

	while(size > 50 - bringnum)          // 한 가지 재료씩 bringnum 변수 값에 저장된 갯수만큼 가져오기 때문에 size > 50 - bringnum 에서만 동작
	{
		node = PQueue->front;
		for (int i = 0; i < 7; i++)       // 사이즈가 재료의 종류인 7만큼 페이지 체크
		{
			if (node && (node->page_count > 1))          // 필요없는 재료를 빼더라도 효율을 위해 최소한 1개는 남김
			{
				if (i == 0)                                 // 첫번째 페이지와 같을 경우, 해당 페이지를 삭제하고 카운트를 감소하여 다시 삽입함
				{                                           // 맨 위의 페이지가 맨 아래로 내려가게됨. 따라서 가장 최근에 사용된 페이지가되고 
					count = node->page_count - 1;           // 해당 페이지는 맨 아래쪽에 위치.
					char b[8];
					int n = node->page_num;
					strcpy(b, node->page);
					deQueue_IO(PQueue);
					enQueue_IO(PQueue, b, n, count);
				}
				else if (i > 0 && i < 6 && (node->page_count > 1))             // 가운데 페이지와 같을 경우, 해당 페이지 카운트를 감소시키고 맨 뒤로 보냄.
				{	                                       
					count = node->page_count - 1;           // --> 역시 가장 최근에 사용된 페이지는 맨 아래쪽에 위치하게됨.
					node->page_count = count;
					Change(node);                            // Change 함수는 해당 페이지를 마지막 큐 노드로 위치를 변경시킴
				}
				else if (i == 6 && (node->page_count > 1))                            // 마지막 페이지와 같을 경우, 해당 페이지는 가장 최근의 페이지이기 때문에 위치 변화없이 카운터만 감소시킴
					node->page_count -= 1;
				size--;                         // 사이즈 감소
				ck = 1;
				break;
			}			
			node = node->link;
		}
	}
	return ck;
}

void Result_RP(IOQueue* Page, R_PQueue* Result, int hour, int min, double s)               // 페이지 결과 값 저장
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

int Insert_Disk(IOQueue* DQueue, char* arr, int num)          // 디스크 큐 삽입 함수
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
	return count;        // 큐에 있는 노드 수를 반환함
}

void Change(IONode * Page)                   // 참조되는 페이지가 페이지테이블 가운데에 위치한 경우 뒤의 값들과 대치하는 함수
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

void Sort(ReadyQueue * Queue, int num)                     // 버블정렬 사용
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
				if (temp1->a_time > temp2->a_time)               // 리스트를 서로 변경하는것이 아닌 데이터를 교환하는 정렬방식
				{                                                // num = 0이면 도착시간 정렬
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
				if (temp1->guest > temp2->guest)                      // num != 0 이면 게스트번호 정렬
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

int SSTF(IOQueue* Queue, int count, int head)                   // 디스크 대기 큐를 head와 가까운 페이지 순으로 정렬한 후 해당 재료를 이동
{
	int b, c;
	int hd = 0;
	char a[8];
	IONode* temp1 = Queue->front;
	IONode* temp2; 	
	for (int i = 0; i < count-1; i++)
	{		
		temp2 = temp1->link;		
		if (abs(head-1 - temp1->page_num) > abs(head-1 - temp2->page_num))               // head - page_num 으로 절대갑을 구하여 작은순으로 정렬
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
		deQueue_IO(Queue);         // 해당 큐를 보내고 페이지 번호와 재료들 각각의 인덱스가 같기 때문에 참조된 페이지번호를 다음 루프 헤드로 사용하게끔 반환
		return hd;	
	}
	else
	{
		deQueue_IO(Queue);		
		return head;
	}
}
void Bring(IOQueue* Queue, int head)      // SSTF에서 지워진 해당 페이지 번호를 매개변수로 받아서 창고A로 가져옴. 1번 채워질 때 마다 각각의 가져오는 재료의 갯수는 7개로 고정
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

double Round(double time, int position) // I/O 발생시 시간의 오차를 줄이기위한 반올림 함수
{
	return ((int)((time + 5 / pow(10, position)) * pow(10, position - 1)) / pow(10, position - 1));
}
