#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
#define MAXVER 30
#define INFINITY 32767
typedef int datatype;

typedef struct
{
	int number;//景点编号
	char name[20];//景点名称
	char description[500];//景点简介
}Vertype;

//邻接矩阵
typedef struct
{
	int arcs[MAXVER][MAXVER];//边
	Vertype vertex[MAXVER];//顶点集

	int vernum;//景点数
	int arcnum;//路径数
	int weight;//路径长度
}AdjMatrix;

void Menu(AdjMatrix *G);//主操作菜单
void ShowSpot(AdjMatrix *G);//展示主要景点
void Display(AdjMatrix *G);//显示景点信息
void Display_DFS(AdjMatrix *G);
void Display_BFS(AdjMatrix *G);
void Search(AdjMatrix *G);//查询景点信息
void Search_num(AdjMatrix *G);
void Search_name(AdjMatrix *G);
void Add(AdjMatrix *G);//添加景点信息
void Delete(AdjMatrix *G);//删除路径信息
void Delete_num(AdjMatrix *G);
void Delete_name(AdjMatrix *G);
void Path1(AdjMatrix *G);
void Path2(AdjMatrix *G);
void PathAll(AdjMatrix *G);//两个景点之间的所有路径
void Dijkstra(AdjMatrix *G, int start, int end, int dist[], int path[][MAXVER]);
void ShortPath(AdjMatrix *G);//两个景点之间的最短路径
void Prim(AdjMatrix *G, int start);
void MiniSpanTree(AdjMatrix *G);//最短连通路径

//确定位置
int LocateVer(AdjMatrix *G, char name[])
{
	for(int i = 1; i <= G->vernum; i++)
	{
		if(strcmp(G->vertex[i].name,name) == 0)
			return i;
	}
 
        return -1; 
}

//景点信息存入文件
void WriteFile1(AdjMatrix *G, int num, char name[], char description[])
{
	FILE *fp;
	fp = fopen("sight.txt", "at");//以追加的方式打开存存储景点信息的文件
	if(fp == NULL)
	{
		printf("打开文件失败!该文件可能不存在!");
		exit(0);
	}
	fprintf(fp, "%d %s %s\n", num, name, description);
	fclose(fp);
}

//路径信息存入文件
void WriteFile2(AdjMatrix *G, char name1[], char name2[], int weight)
{
	FILE *fp;
	fp = fopen("road.txt", "at");//以追加的方式打开存储路线长度的文件
	if(fp == NULL)
	{
		printf("打开文件失败!该文件可能不存在!");
		exit(0);
	}
	fprintf(fp,"%s %s %d\n", name1, name2, weight);
	fclose(fp);
}

//从文件中读取信息存入邻接矩阵
void ReadFile(AdjMatrix *G)
{
	FILE *fp;
	int i = 1, j;
	int n = 1;
	char name1[20], name2[20];
	int weight;
	
	fp = fopen("sight.txt", "rt+");//打开存储景点信息的文件
	if(fp == NULL)
	{
		printf("打开文件失败!该文件可能不存在!\n");
		exit(0);
	}

	while(!feof(fp))
	{
		fscanf(fp,"%d%s%s\n", &G->vertex[i].number, G->vertex[i].name, G->vertex[i].description);
		i++;
	}
	G->vernum = i;
	fclose(fp);

	fp = fopen("road.txt", "rt+");//打开存储路径信息的文件
	if(fp == NULL)
	{
		printf("文件打开失败!该文件可能不存在!\n");
		exit(0);
	}

	//初始化路径长度
	for(i = 1; i <= G->vernum; i++)
		for(j = 1; j <= G->vernum; j++ )
			G->arcs[i][j] = INFINITY;
  
	while (!feof(fp))
	{
		fscanf(fp, "%s%s%d", name1, name2, &weight);
	        i = LocateVer(G, name1);
		j = LocateVer(G, name2);
		G->arcs[i][j] = G->arcs[j][i] = weight;
		n++;
	}

	G->arcnum = n;
	fclose(fp);
}

//主操作菜单
void Menu(AdjMatrix *G)
{
	int choice;
	printf("\t************************欢迎使用校园导游图系统*******************\n\n");
        printf("\t\t-------------------------------------------------\n\n");
	printf("\t\t|                                               |\n");
        printf("\t\t|            1.显示主要景点                     |\n");
	printf("\t\t|            2.显示所有景点信息                 |\n");
        printf("\t\t|            3.查询某个景点信息                 |\n");
        printf("\t\t|            4.添加景点信息                     |\n");
        printf("\t\t|            5.删除景点信息                     |\n");
        printf("\t\t|            6.查询两个景点之间的所有路径       |\n");
        printf("\t\t|            7.查询两个景点之间的最短路径       |\n");
        printf("\t\t|            8.查询最短连通路径                 |\n");
        printf("\t\t|            0.退出系统                         |\n\n");
        printf("\t\t|                                               |\n");
	printf("\t\t-------------------------------------------------\n\n");
	
        printf("\t请输入您的选择(0~8):");
        scanf("%d", &choice);
        switch(choice)
        {
        	case 1:ShowSpot(G); break; 
	        case 2:Display(G); break;
                case 3:Search(G); break;
                case 4:Add(G); break;
                case 5:Delete(G); break;
	        case 6:PathAll(G); break;
	        case 7:ShortPath(G); break;
                case 8:MiniSpanTree(G); break;
        }
}

//显示主要景点
void ShowSpot(AdjMatrix *G)
{
        printf("\t***********************************************************************\n");
	printf("\t*                          主要景点列表                               *\n");
	printf("\t***********************************************************************\n\n");
	printf("\t      ----------------------------------------------------------\n");	
	printf("\t      |                           西区                         |\n");
	printf("\t      |     <1>.西区北门                        <2>.行政楼     |\n");
	printf("\t      |     <3>.教学楼                          <4>.实验楼     |\n");
	printf("\t      |     <5>.大学生活动中心                  <6>.图书馆     |\n");
	printf("\t      |     <7>.西邮湖                                         |\n");
	printf("\t      |     <8>.洗浴中心                        <9>.医疗中心   |\n");
	printf("\t      |     <10>.西操场                         <11>.美食广场  |\n");
	printf("\t      |     <12>.体育馆                         <13>.田径场    |\n");
	printf("\t      |     <14>.旭日苑                         <15>.西学生公寓|\n");
	printf("\t      |     <16>.基建后勤大院                   <17>.西区东门  |\n");
	printf("\t      ----------------------------------------------------------\n");
	printf("\t      |                        <18>西邮天桥                    |\n");

	printf("\t      ----------------------------------------------------------\n");	
	printf("\t      |                            东区                        |\n");
	printf("\t      |     <19>.东区西南门                     <20>.家属楼    |\n");
	printf("\t      |     <21>.餐厅超市                       <22>.浴室      |\n");
	printf("\t      |     <23>.东学生公寓                     <24>.综合教学楼|\n");
	printf("\t      |     <25>.东区西北门                                    |\n");
	printf("\t      ----------------------------------------------------------\n\n");

	getchar();
	printf("\t按任意键返回主菜单!");
	getc(stdin);
	Menu(G);
} 


//显示菜单
void DisplayMenu(AdjMatrix *G)
{   
        int  choice;

	printf("\t\t\t------------------------------------------------\n\n");
	printf("\t\t\t|               欢迎进入显示菜单               |\n");
        printf("\t\t\t|                                              |\n");
	printf("\t\t\t|                1.DFS搜索显示                  |\n");
	printf("\t\t\t|                2.BFS搜索显示                 |\n");
	printf("\t\t\t|                0.返回主菜单                       |\n");
	printf("\t\t\t|                                              |\n");
	printf("\t\t\t------------------------------------------------\n\n");

        printf("\t请输入您的选择(0~2):");
	scanf("%d", &choice);
	switch(choice)
	{
		case 1:Display_DFS(G); break;
		case 2:Display_BFS(G); break;
		case 0:Menu(G); break;
	}
}

//深度优先遍历
int visited1[MAXVER] = {0};
void DFS(AdjMatrix *G, int vi)
{
	int vj;
	printf("%d %s %s\n", G->vertex[vi].number, G->vertex[vi].name, G->vertex[vi].description);
	visited1[vi] = 1;//已访问过则赋值为1
	
	vj = 1;
        while(vj <= G->vernum)
	{
		if(!visited1[vj] && G->arcs[vi][vj] != INFINITY)//vj点未访问且vi-vj边存在
			DFS(G,vj);//访问vj
		vj++;
	}
}

//深度优先遍历（非连通图）
int visited3[MAXVER];
void UDFS(AdjMatrix *G)     
{
    int i;
    for (i = 1; i <= G->vernum; i++)
    {
        visited3[i]=0;
    }
    for (i = 1; i <G ->vernum; i++)
    {
        if (visited3[i] == 0)
            DFS(G,i);
    }
}

typedef struct QueueNode//循环队列结点定义
{
	datatype data[MAXVER];
	int front, rear;
}CSqQueue;

void InitQueue(CSqQueue *sq)//循环队列初始化
{
	sq->front = sq->rear = 0;
}

int  IsEmpty(CSqQueue *sq)//循环队列判空
{
	if(sq->front == sq->rear )
		return TRUE;
	else 
		return FALSE;
}
 
int IsFull(CSqQueue *sq)//循环队列判满
{
	if((sq->rear + 1) % MAXVER == sq->front)
		return TRUE;
	else 
		return FALSE;
}

int InQueue(CSqQueue *sq, int x)//入队
{
	if(IsFull(sq))
	{
		printf("队满!\n");
		return FALSE;
	}
	else
	{
		sq->data[sq->rear] = x;
		sq->rear = (sq->rear + 1) % MAXVER;
      	        return TRUE;
	}
}

int OutQueue(CSqQueue *sq, int *x)
{
	if(IsEmpty(sq))
	{
		printf("队空!\n");
		return FALSE;
	}
	else
	{
		*x = sq->data[sq->front];
		sq->front = (sq->front + 1) % MAXVER;
		return TRUE;
	}
}

//广度优先遍历
int visited2[MAXVER] = {0};
void BFS(AdjMatrix *G, int va)
{
	int v, vb;//vb是顶点在数组中的下标
	CSqQueue *Q;
        Q=(CSqQueue *)malloc(sizeof(CSqQueue));
	InitQueue(Q);
        printf("%d %s %s\n", G->vertex[va].number, G->vertex[va].name, G->vertex[va].description);
	visited2[va] = 1;//已访问过的赋值1
        InQueue(Q, va);//已访问过的点入队

	while(!IsEmpty(Q))
	{
		OutQueue(Q, &v);//对头出队，并将队头元素赋给v
		for(vb = 1; vb <= G->vernum; vb++)
		{
			if((!visited2[vb]) && (G->arcs[v][vb] != INFINITY))//vj未访问且v-vb这条边存在
			{
				printf("%d %s %s\n", G->vertex[vb].number, G->vertex[vb].name, G->vertex[vb].description);
                                visited2[vb] = 1;
			        InQueue(Q, vb);	
			}
		}
	}
}

//广度优先遍历（非连通图）
int visited4[MAXVER];
void UBFS(AdjMatrix *G)     
{
	int i;
        for (i = 1; i <= G->vernum; i++)
        {
        	visited4[i] = 0;
        }
        for (i = 1; i < G->vernum; i++)
        {
        	if (visited4[i] == 0)
                BFS(G,i);
        }
}


//DFS显示所有景点信息
void Display_DFS(AdjMatrix *G)
{
	int i = 1;	
        printf("景点信息如下:\n");
        UDFS(G);

	getchar();
	printf("\t按任意键返回显示菜单!");
        getc(stdin);
	DisplayMenu(G);
}

//BFS显示所有景点信息
void Display_BFS(AdjMatrix *G)
{
        int i = 1;	
	printf("景点信息如下:\n");
	UBFS(G);

	getchar();
	printf("\t按任意键返回显示菜单!");
        getc(stdin);
	DisplayMenu(G);
}

//显示所有景点信息
void Display(AdjMatrix *G)
{
	DisplayMenu(G);
}

//查询菜单
void SearchMenu(AdjMatrix *G)
{
	int  choice;

	printf("\t\t\t------------------------------------------------\n\n");
	printf("\t\t\t|               欢迎进入查询菜单               |\n");
        printf("\t\t\t|                                              |\n");
	printf("\t\t\t|                1.按编号查询                  |\n");
	printf("\t\t\t|                2.按名称查询                  |\n");
	printf("\t\t\t|                0.返回主菜单                       |\n");
	printf("\t\t\t|                                              |\n");
	printf("\t\t\t------------------------------------------------\n\n");

        printf("\t请输入您的选择(0~2):");
	scanf("%d", &choice);
	switch(choice)
	{
		case 1:Search_num(G); break;
		case 2:Search_name(G); break;
		case 0:Menu(G); break;
	}
}

//用景点编号查询
void Search_num(AdjMatrix *G)
{
	int i, j, num;
	char c;
    
	ReadFile(G);
	while(1)
	{
		printf("请输入您要查找的景点编号:\n");
		scanf("%d", &num);
		for(i = 1; i <= G->vernum; i++)
		{
			if(num == G->vertex[i].number)
			{
				printf("\t您要查找景点信息如下:\n");
				printf("\t名称:%-20s\n", G->vertex[i].name );
				printf("\t简介:%-20s\n", G->vertex[i].description);
	                 	for(j = 1; j <= G->vernum; j++)
					if(G->arcs[i][j] != INFINITY)
						printf("\t%s<--->%s:%dm\n", G->vertex[i].name, G->vertex[j].name, G->arcs [i][j]);
				        break;
			}
		}
		if(i > G->vernum)
			printf("\n\t无该景点信息!\n");

		getchar();
		printf("\t是否继续查询(y/n)?");
		scanf("%c", &c);
		if(c == 'n')
			break;
	}

	getchar();
	printf("\t按任意键返回查询菜单!");
        getc(stdin);
	SearchMenu(G);
}

//用景点名称查询
void Search_name(AdjMatrix *G)
{
	int i, j;
	char c, name[20];

	while(1)
	{
		printf("请输入您要查找的景点名称:\n");
	        scanf("%s", name);
		for(i = 1; i <= G->vernum; i++)
		{
			if(strcmp(name, G->vertex[i].name) == 0)
			{
				printf("\t您要查找景点信息如下:\n");
			        printf("\t编号:%-5d\n", G->vertex [i].number );
			        printf("\t简介:%-20s\n", G->vertex[i].description);
				for(j = 1; j <= G->vernum; j++)
					if(G->arcs[i][j] != INFINITY)
						printf("\t%s<--->%s:%dm\n", G->vertex[i].name, G->vertex[j].name, G->arcs [i][j]);
					break;
			}
		}
	        if(i > G->vernum)
			printf("\t无该景点信息!\n");

       	        getchar();
		printf("\t是否继续查询(y/n)?");
		scanf("%c", &c);
		if(c == 'n')
			break;
	}

	getchar();
	printf("\n\t按任意键返回查询菜单!");
        getc(stdin);
	SearchMenu(G);
}


//查询景点信息
void Search(AdjMatrix *G)
{
	SearchMenu(G);
}


//添加景点
void  Add(AdjMatrix *G)
{
	int i, num;
	int count, end;
	char name1[20], name2[20], description[200];
	int length;
	ReadFile(G);

	printf("请输入新景点的编号:\n");
	scanf("%d", &num);
	printf("请输入新景点的名称:\n");
	scanf("%s", name1);
	printf("请输入新景点的简介:\n");
	scanf("%s", description);

	strcpy(G->vertex[num].name, name1);
	strcpy(G->vertex[num].description, description);
	G->vertex[num].number = num;

	WriteFile1(G, num, name1, description);

	printf("请输入与新景点邻接的景点个数:\n");
	scanf("%d", &count);

	for(i = 1; i <= count; i++)
	{
		printf("请输入No.%d邻接景点名:\n", i);
		scanf("%s", name2);
		end = LocateVer(G, name2);

		printf("请输入路径长度:\n");
		scanf("%d", &length);
		G->arcs[num][end] = G->arcs[num][end] = length;
	
	}

	for(i = 1; i <= count; i++)
	WriteFile2(G, name1, name2, length);

	G->vernum++;
	G->arcnum += count;



	getchar();
	printf("\t按任意键返回主菜单!");
	getc(stdin);
	Menu(G);
}

//删除路线
void Delete(AdjMatrix *G)
{
	char name1[20], name2[20];
	int start, end;

	printf("请输入要删除路线的起始景点名称:\n");
	scanf("%s", name1);
	start = LocateVer(G, name1);
	printf("终止景点名称:\n");
	scanf("%s", name2);
	end = LocateVer(G,name2);

	G->arcs[start][end] = G->arcs[end][start] = INFINITY;

	WriteFile2(G, name1, name2, INFINITY);

	getchar();
	printf("\t按任意键返回主菜单!");
	getc(stdin);
	Menu(G);

}

int count = 0, visited[MAXVER] = {0};
void Path1(AdjMatrix *G, int v1, int v2, int path[], int length)
{
	int vi;
	if(v1 == v2)
	{
		printf("第%d条:", ++count);

		int  i;
		for(i = 0; i < length; i++)
			printf("%s--->", G->vertex[path[i]].name);
		printf("%s", G->vertex[v1].name);
		printf("\n");
	}

	else
	{
		visited[v1] = 1;
		path[length] = v1;//v1入path数组
		for(vi = 1; vi <= G->vernum; vi++)
		{
			if(!visited[vi] && G->arcs[v1][vi] != INFINITY)
			{
				visited[vi] = 1;
				Path1(G, vi, v2, path, length+1);
				visited[vi] = 0;
			}
		}
	}
}

void path2(AdjMatrix *G,char name1[],char name2[])
{
        int path[MAXVER];
	int i, j, k;
	for(k = 1; k <= G->vernum; k++)
		visited[k] = 0;
	i = LocateVer(G,name1);
	j = LocateVer(G,name2);
        Path1(G, i, j, path, 0);
}

//两个景点的所有路径
void PathAll(AdjMatrix *G)
{

	char name1[20], name2[20];
	printf("请输入两个景点名:\n");
	flushall();
	scanf("%s %s", name1, name2);
	path2(G, name1, name2);

        getchar();
	printf("\n\t按任意键返回主菜单!");
        getc(stdin);
	Menu(G);
}

//采用Dijkstra算法求得起始景点到其它各景点的最短路径
void Dijkstra(AdjMatrix *G, int start, int end, int dist[], int path[][MAXVER])
{
	//dist数组记录各条最短路径长度，path数组记录对应路径上的各景点
	int mindist;
	int i,j,k,t;
	for(i = 1; i <= G->vernum; i++)
	{
		dist[i] = G->arcs[start][i];
		if(G->arcs[start][i] != INFINITY)
			path[i][1] = start;
	}

	path[start][0] = 1;//start景点已加入到集合S中
	for(i = 2; i <= G->vernum; i++)//寻找各条最短路径
	{
		mindist = INFINITY;
		for(j = 1; j <= G->vernum; j++)//选择权值最小的边
			if(!path[j][0] && dist[j] < mindist)
			{
				k = j;//用k记录下一个选中的景点
				mindist = dist[j];
			}

			if(mindist == INFINITY)
				break;
			path[k][0] = 1;//k景点已加入到集合S中
			for(j = 1; j <= G->vernum; j++)//修改路径
			{
				if(!path[j][1] && G->arcs[k][j] < INFINITY && dist[k] + G->arcs[k][j] < dist[j])
				{
					dist[j] = dist[k] + G->arcs[k][j];
					t = 1;
					while(path[k][t] != 0)
					{
						path[j][t] = path[k][t];
						t++;
					}
					path[j][t] = k;
					path[j][t+1] = 0;
				}
			}
	}

	for(i = 1; i <= G->vernum; i++)
		if(i == end)
			break;
		printf("%s<--->%s的最短路径为:\n从%s", G->vertex [start].name, G->vertex [end].name, G->vertex [start].name);
		for(j = 2; path[i][j] != 0; j++)
			printf("--->%s", G->vertex[path[i][j]].name);
		printf("--->%s,\n距离为%dm\n", G->vertex [end].name, dist[i]);
}

//寻找最短路径
void ShortPath(AdjMatrix *G)
{
	char name[20];
	int start,end;
	int dist[MAXVER], path[MAXVER][MAXVER] = {0};

	printf("请输入起始景点名称:\n");
	scanf("%s", name);
	start = LocateVer(G, name);

	printf("请输入终止景点名称:\n");
	scanf("%s", name);
	end = LocateVer(G, name);

	Dijkstra(G, start, end, dist, path);

	getchar();
	printf("\t按任意键返回主菜单!");
	getc(stdin);
	Menu(G);
}

//采用prim算法求最短连通路径
void Prim(AdjMatrix *G, int start)
{
	struct node
	{
		int adjvex;
		int lowcost;
	}closedge[MAXVER];//元素下标对应景点编号

	int i, e, k, m, min;
	closedge[start].lowcost = 0;//标志顶点start已加入到最小生成树中
    
	//对除了起始点以外的其他顶点初始化对应的closedge数组
	for(i = 1; i <= G->vernum; i++)
		if(i != start)
		{
			closedge[i].adjvex = start;
			closedge[i].lowcost = G->arcs[start][i];
		}

        for(e = 1; e <= G->vernum-2; e++)
	{
		//选择权值最小的边
       		min = INFINITY;
		for(k = 1; k <= G->vernum; k++)
		{
			if(closedge[k].lowcost != 0 && closedge[k].lowcost < min)
			{
		        	m = k;//用m记录选中的下一个顶点
				min = closedge[k].lowcost;
			}
		}
		printf("从%s<--->%s:%dm\n", G->vertex [closedge[m].adjvex].name, G->vertex [m].name, closedge[m].lowcost);
                closedge[m].lowcost = 0;//标志顶点m已加入到最小生成树种

		//当新顶点加入最小生成树后，更改closedge数组信息
		for(i = 1; i <= G->vernum; i++)
			if(i != m && G->arcs[m][i] < closedge[i].lowcost)
		        //一旦发现更小的权值边出现，则替换原有信息
			{
		        	closedge[i].lowcost = G->arcs[m][i];
				closedge[i].adjvex = m;//出发点改变
			}
	}
}

//查询从某个景点开始的最短连通路径
void MiniSpanTree(AdjMatrix *G)
{
	char name[20];
	int start;

	printf("请输入起始景点名称:\n");
	scanf("%s", name);
	start = LocateVer(G, name);
	Prim(G, start);

	getchar();
	printf("\t按任意键返回主菜单!");
	getc(stdin);
	Menu(G);
}

void main()
{   
	AdjMatrix *G;
	G = (AdjMatrix *)malloc(sizeof(AdjMatrix));
	ReadFile(G);
	Menu(G);
}

























