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
	int number;//������
	char name[20];//��������
	char description[500];//������
}Vertype;

//�ڽӾ���
typedef struct
{
	int arcs[MAXVER][MAXVER];//��
	Vertype vertex[MAXVER];//���㼯

	int vernum;//������
	int arcnum;//·����
	int weight;//·������
}AdjMatrix;

void Menu(AdjMatrix *G);//�������˵�
void ShowSpot(AdjMatrix *G);//չʾ��Ҫ����
void Display(AdjMatrix *G);//��ʾ������Ϣ
void Display_DFS(AdjMatrix *G);
void Display_BFS(AdjMatrix *G);
void Search(AdjMatrix *G);//��ѯ������Ϣ
void Search_num(AdjMatrix *G);
void Search_name(AdjMatrix *G);
void Add(AdjMatrix *G);//��Ӿ�����Ϣ
void Delete(AdjMatrix *G);//ɾ��·����Ϣ
void Delete_num(AdjMatrix *G);
void Delete_name(AdjMatrix *G);
void Path1(AdjMatrix *G);
void Path2(AdjMatrix *G);
void PathAll(AdjMatrix *G);//��������֮�������·��
void Dijkstra(AdjMatrix *G,int start,int end,int dist[],int path[][MAXVER]);
void ShortPath(AdjMatrix *G);//��������֮������·��
void Prim(AdjMatrix *G,int start);
void MiniSpanTree(AdjMatrix *G);//�����ͨ·��

//ȷ��λ��
int LocateVer(AdjMatrix *G,char name[])
{
	for(int i=1;i<=G->vernum;i++)
	{
		if(strcmp(G->vertex[i].name,name)==0)
			return i;
	}
 
    return -1; 
}

//������Ϣ�����ļ�
void WriteFile1(AdjMatrix *G,int num,char name[],char description[])
{
	FILE *fp;
	
	fp=fopen("sight.txt","at");//��׷�ӵķ�ʽ�򿪴�洢������Ϣ���ļ�
	if(fp==NULL)
	{
		printf("���ļ�ʧ��!���ļ����ܲ�����!");
		exit(0);
	}
	fprintf(fp,"%d %s %s\n",num,name,description);
	fclose(fp);

}

//·����Ϣ�����ļ�
void WriteFile2(AdjMatrix *G,char name1[],char name2[],int weight)
{
	FILE *fp;
	
	fp=fopen("road.txt","at");//��׷�ӵķ�ʽ�򿪴洢·�߳��ȵ��ļ�
	if(fp==NULL)
	{
		printf("���ļ�ʧ��!���ļ����ܲ�����!");
		exit(0);
	}
	fprintf(fp,"%s %s %d\n",name1,name2,weight);
	fclose(fp);

}

//���ļ��ж�ȡ��Ϣ�����ڽӾ���
void ReadFile(AdjMatrix *G)
{
	FILE *fp;
	int i=1,j;
	int n=1;
	char name1[20],name2[20];
	int weight;
	
	fp=fopen("sight.txt", "rt+");//�򿪴洢������Ϣ���ļ�
	if(fp==NULL)
	{
		printf("���ļ�ʧ��!���ļ����ܲ�����!\n");
		exit(0);
	}

	while(!feof(fp))
	{
		fscanf(fp,"%d%s%s\n",&G->vertex [i].number ,G->vertex [i].name ,G->vertex [i].description);
		i++;
	}
	G->vernum=i;
	fclose(fp);

	fp=fopen("road.txt","rt+");//�򿪴洢·����Ϣ���ļ�
	if(fp==NULL)
	{
		printf("�ļ���ʧ��!���ļ����ܲ�����!\n");
		exit(0);
	}

	//��ʼ��·������
	for(i=1;i<=G->vernum;i++)
		for(j=1;j<=G->vernum;j++ )
			G->arcs[i][j]=INFINITY;
  
	while (!feof(fp))
	{
		fscanf(fp,"%s%s%d",name1,name2,&weight);
	    i=LocateVer(G,name1);
		j=LocateVer(G,name2);
		G->arcs[i][j]=G->arcs[j][i]=weight;
		n++;
	}

	G->arcnum=n;
	fclose(fp);
}

//�������˵�
void Menu(AdjMatrix *G)
{
	int choice;
	printf("\t************************��ӭʹ��У԰����ͼϵͳ*******************\n\n");
    printf("\t\t-------------------------------------------------\n\n");
	printf("\t\t|                                               |\n");
    printf("\t\t|            1.��ʾ��Ҫ����                     |\n");
	printf("\t\t|            2.��ʾ���о�����Ϣ                 |\n");
    printf("\t\t|            3.��ѯĳ��������Ϣ                 |\n");
    printf("\t\t|            4.��Ӿ�����Ϣ                     |\n");
    printf("\t\t|            5.ɾ��������Ϣ                     |\n");
    printf("\t\t|            6.��ѯ��������֮�������·��       |\n");
    printf("\t\t|            7.��ѯ��������֮������·��       |\n");
    printf("\t\t|            8.��ѯ�����ͨ·��                 |\n");
    printf("\t\t|            0.�˳�ϵͳ                         |\n\n");
    printf("\t\t|                                               |\n");
	printf("\t\t-------------------------------------------------\n\n");
	
    printf("\t����������ѡ��(0~8):");
    scanf("%d",&choice);
    switch(choice)
    {
      case 1:ShowSpot(G);break; 
	  case 2:Display(G);break;
      case 3:Search(G);break;
      case 4:Add(G);break;
      case 5:Delete(G);break;
	  case 6:PathAll(G);break;
	  case 7:ShortPath(G);break;
      case 8:MiniSpanTree(G);break;
    }
	
}

//��ʾ��Ҫ����
void ShowSpot(AdjMatrix *G)
{
    printf("\t***********************************************************************\n");
	printf("\t*                          ��Ҫ�����б�                               *\n");
	printf("\t***********************************************************************\n\n");
	printf("\t      ----------------------------------------------------------\n");	
	printf("\t      |                           ����                         |\n");
	printf("\t      |     <1>.��������                        <2>.����¥     |\n");
	printf("\t      |     <3>.��ѧ¥                          <4>.ʵ��¥     |\n");
	printf("\t      |     <5>.��ѧ�������                  <6>.ͼ���     |\n");
	printf("\t      |     <7>.���ʺ�                                         |\n");
	printf("\t      |     <8>.ϴԡ����                        <9>.ҽ������   |\n");
	printf("\t      |     <10>.���ٳ�                         <11>.��ʳ�㳡  |\n");
	printf("\t      |     <12>.������                         <13>.�ﾶ��    |\n");
	printf("\t      |     <14>.����Է                         <15>.��ѧ����Ԣ|\n");
	printf("\t      |     <16>.�������ڴ�Ժ                   <17>.��������  |\n");
	printf("\t      ----------------------------------------------------------\n");
	printf("\t      |                        <18>��������                    |\n");

	printf("\t      ----------------------------------------------------------\n");	
	printf("\t      |                            ����                        |\n");
	printf("\t      |     <19>.����������                     <20>.����¥    |\n");
	printf("\t      |     <21>.��������                       <22>.ԡ��      |\n");
	printf("\t      |     <23>.��ѧ����Ԣ                     <24>.�ۺϽ�ѧ¥|\n");
	printf("\t      |     <25>.����������                                    |\n");
	printf("\t      ----------------------------------------------------------\n\n");

	getchar();
	printf("\t��������������˵�!");
	getc(stdin);
	Menu(G);

} 


//��ʾ�˵�
void DisplayMenu(AdjMatrix *G)
{   
    int  choice;

	printf("\t\t\t------------------------------------------------\n\n");
	printf("\t\t\t|               ��ӭ������ʾ�˵�               |\n");
    printf("\t\t\t|                                              |\n");
	printf("\t\t\t|                1.DFS������ʾ                  |\n");
	printf("\t\t\t|                2.BFS������ʾ                 |\n");
	printf("\t\t\t|                0.�������˵�                       |\n");
	printf("\t\t\t|                                              |\n");
	printf("\t\t\t------------------------------------------------\n\n");

    printf("\t����������ѡ��(0~2):");
	scanf("%d",&choice);
	switch(choice)
	{
	case 1:Display_DFS(G);break;
	case 2:Display_BFS(G);break;
	case 0:Menu(G);break;
	}
}

//������ȱ���
int visited1[MAXVER]={0};
void DFS(AdjMatrix *G,int vi)
{
	int vj;
	printf("%d %s %s\n",G->vertex[vi].number,G->vertex[vi].name,G->vertex[vi].description);
	visited1[vi]=1;//�ѷ��ʹ���ֵΪ1
	
	vj=1;
    while(vj<=G->vernum)
	{
		if(!visited1[vj]&&G->arcs[vi][vj]!=INFINITY)//vj��δ������vi-vj�ߴ���
			DFS(G,vj);//����vj
		vj++;
	}
}

//������ȱ���������ͨͼ��
int visited3[MAXVER];
void UDFS(AdjMatrix *G)     
{
    int i;
    for (i=1; i<=G->vernum; i++)
    {
        visited3[i]=0;
    }
    for (i=1; i<G->vernum; i++)
    {
        if (visited3[i]==0)
            DFS(G,i);
    }
}

typedef struct QueueNode//ѭ�����н�㶨��
{
	datatype data[MAXVER];
	int front,rear;
}CSqQueue;

void InitQueue(CSqQueue *sq)//ѭ�����г�ʼ��
{
	sq->front =sq->rear =0;
}

int  IsEmpty(CSqQueue *sq)//ѭ�������п�
{
	if(sq->front ==sq->rear )
		return TRUE;
	else 
		return FALSE;
}
 
int IsFull(CSqQueue *sq)//ѭ����������
{
	if((sq->rear +1)%MAXVER==sq->front)
		return TRUE;
	else 
		return FALSE;
}

int InQueue(CSqQueue *sq,int x)//���
{
	if(IsFull(sq))
	{
		printf("����!\n");
		return FALSE;
	}

	else
	{
		sq->data [sq->rear ]=x;
		sq->rear =(sq->rear+1)%MAXVER;
      	return TRUE;
	}
}

int OutQueue(CSqQueue *sq,int *x)
{
	if(IsEmpty(sq))
	{
		printf("�ӿ�!\n");
		return FALSE;
	}
	else
	{
		*x=sq->data[sq->front];
		sq->front=(sq->front+1)%MAXVER;
		return TRUE;
	}
}

//������ȱ���
int visited2[MAXVER]={0};
void BFS(AdjMatrix *G,int va)
{
	int v, vb;//vb�Ƕ����������е��±�
	CSqQueue *Q;
    Q=(CSqQueue *)malloc(sizeof(CSqQueue));
	InitQueue(Q);
    printf("%d %s %s\n",G->vertex[va].number,G->vertex[va].name,G->vertex[va].description);
	visited2[va]=1;//�ѷ��ʹ��ĸ�ֵ1
    InQueue(Q,va);//�ѷ��ʹ��ĵ����

	while(!IsEmpty(Q))
	{
		OutQueue(Q,&v);//��ͷ���ӣ�������ͷԪ�ظ���v
		for(vb=1;vb<=G->vernum;vb++)
		{
			if((!visited2[vb])&&(G->arcs[v][vb]!=INFINITY))//vjδ������v-vb�����ߴ���
			{
				printf("%d %s %s\n",G->vertex[vb].number,G->vertex[vb].name,G->vertex[vb].description);
                visited2[vb]=1;
			    InQueue(Q,vb);	
			}
		}
	}
}

//������ȱ���������ͨͼ��
int visited4[MAXVER];
void UBFS(AdjMatrix *G)     
{
    int i;
    for (i=1; i<=G->vernum; i++)
    {
        visited4[i]=0;
    }
    for (i=1; i<G->vernum; i++)
    {
        if (visited4[i]==0)
            BFS(G,i);
    }
}


//DFS��ʾ���о�����Ϣ
void Display_DFS(AdjMatrix *G)
{
    int i=1;	
	printf("������Ϣ����:\n");
		UDFS(G);

	getchar();
	printf("\t�������������ʾ�˵�!");
    getc(stdin);
	DisplayMenu(G);
}

//BFS��ʾ���о�����Ϣ
void Display_BFS(AdjMatrix *G)
{
    int i=1;	
	printf("������Ϣ����:\n");
		UBFS(G);

	getchar();
	printf("\t�������������ʾ�˵�!");
    getc(stdin);
	DisplayMenu(G);
}

//��ʾ���о�����Ϣ
void Display(AdjMatrix *G)
{
	DisplayMenu(G);
}

//��ѯ�˵�
void SearchMenu(AdjMatrix *G)
{   
    int  choice;

	printf("\t\t\t------------------------------------------------\n\n");
	printf("\t\t\t|               ��ӭ�����ѯ�˵�               |\n");
    printf("\t\t\t|                                              |\n");
	printf("\t\t\t|                1.����Ų�ѯ                  |\n");
	printf("\t\t\t|                2.�����Ʋ�ѯ                  |\n");
	printf("\t\t\t|                0.�������˵�                       |\n");
	printf("\t\t\t|                                              |\n");
	printf("\t\t\t------------------------------------------------\n\n");

    printf("\t����������ѡ��(0~2):");
	scanf("%d",&choice);
	switch(choice)
	{
	case 1:Search_num(G);break;
	case 2:Search_name(G);break;
	case 0:Menu(G);break;
	}
}

//�þ����Ų�ѯ
void Search_num(AdjMatrix *G)
{
	int i,j,num;
	char c;
    
	ReadFile(G);
	while(1)
	{
		printf("��������Ҫ���ҵľ�����:\n");
		scanf("%d",&num);
		for(i=1;i<=G->vernum;i++)
		{
			if(num==G->vertex[i].number)
			{
				printf("\t��Ҫ���Ҿ�����Ϣ����:\n");
				printf("\t����:%-20s\n",G->vertex[i].name );
				printf("\t���:%-20s\n",G->vertex[i].description);
	            for(j=1;j<=G->vernum;j++)
					if(G->arcs[i][j]!=INFINITY)
						printf("\t%s<--->%s:%dm\n",G->vertex[i].name,G->vertex[j].name,G->arcs [i][j]);
					break;
			}
		}
		if(i>G->vernum)
			printf("\n\t�޸þ�����Ϣ!\n");

		getchar();
		printf("\t�Ƿ������ѯ(y/n)?");
		scanf("%c",&c);
		if(c=='n')
		break;
	}

	getchar();
	printf("\t����������ز�ѯ�˵�!");
    getc(stdin);
	SearchMenu(G);
}

//�þ������Ʋ�ѯ
void Search_name(AdjMatrix *G)
{
	int i,j;
	char c,name[20];

	while(1)
	{
		printf("��������Ҫ���ҵľ�������:\n");
	    scanf("%s",name);
		for(i=1;i<=G->vernum;i++)
		{
			if(strcmp(name,G->vertex[i].name )==0)
			{
				printf("\t��Ҫ���Ҿ�����Ϣ����:\n");
			    printf("\t���:%-5d\n",G->vertex [i].number );
			    printf("\t���:%-20s\n",G->vertex[i].description);
				for(j=1;j<=G->vernum;j++)
					if(G->arcs[i][j]!=INFINITY)
						printf("\t%s<--->%s:%dm\n",G->vertex[i].name,G->vertex[j].name,G->arcs [i][j]);
					break;
			}
		}
	    if(i>G->vernum)
			printf("\t�޸þ�����Ϣ!\n");

       	getchar();
		printf("\t�Ƿ������ѯ(y/n)?");
		scanf("%c",&c);
		if(c=='n')
		break;
	}

	getchar();
	printf("\n\t����������ز�ѯ�˵�!");
    getc(stdin);
	SearchMenu(G);
}


//��ѯ������Ϣ
void Search(AdjMatrix *G)
{
	SearchMenu(G);
}


//��Ӿ���
void  Add(AdjMatrix *G)
{
	int i,num;
	int count,end;
	char name1[20],name2[20],description[200];
	int length;
	ReadFile(G);

	printf("�������¾���ı��:\n");
	scanf("%d",&num);
	printf("�������¾��������:\n");
	scanf("%s",name1);
	printf("�������¾���ļ��:\n");
	scanf("%s",description);

	strcpy(G->vertex[num].name,name1);
	strcpy(G->vertex[num].description,description);
	G->vertex[num].number=num;

	WriteFile1(G,num,name1,description);

	printf("���������¾����ڽӵľ������:\n");
	scanf("%d",&count);

	for(i=1;i<=count;i++)
	{
		printf("������No.%d�ڽӾ�����:\n",i);
		scanf("%s",name2);
		end=LocateVer(G,name2);

		printf("������·������:\n");
		scanf("%d",&length);
		G->arcs [num][end]=	G->arcs [num][end]=length;
	
	}

	for(i=1;i<=count;i++)
	WriteFile2(G,name1,name2,length);

	G->vernum++;
	G->arcnum +=count;



	getchar();
	printf("\t��������������˵�!");
	getc(stdin);
	Menu(G);
}

//ɾ��·��
void Delete(AdjMatrix *G)
{
	char name1[20],name2[20];
	int start,end;

	printf("������Ҫɾ��·�ߵ���ʼ��������:\n");
	scanf("%s",name1);
	start=LocateVer(G,name1);
	printf("��ֹ��������:\n");
	scanf("%s",name2);
	end=LocateVer(G,name2);

	G->arcs [start][end]=G->arcs [end][start]=INFINITY;

	WriteFile2(G,name1,name2,INFINITY);

	getchar();
	printf("\t��������������˵�!");
	getc(stdin);
	Menu(G);

}

int count =0,visited[MAXVER]={0};
void Path1(AdjMatrix *G,int v1,int v2,int path[],int length)
{
	int vi;
	if(v1==v2)
	{
		printf("��%d��:",++count);

		int  i;
		for(i=0;i<length;i++)
			printf("%s--->",G->vertex[path[i]].name);
		printf("%s",G->vertex[v1].name);
		printf("\n");
	}

	else
	{
		visited[v1]=1;
		path[length]=v1;//v1��path����
		for(vi=1;vi<=G->vernum;vi++)
		{
			if(!visited[vi]&&G->arcs[v1][vi]!=INFINITY)
			{
				visited[vi]=1;
				Path1(G,vi,v2,path,length+1);
				visited[vi]=0;
			}
		}
	}
}

void path2(AdjMatrix *G,char name1[],char name2[])
{
    int path[MAXVER];
	int i,j,k;
	for(k=1;k<=G->vernum;k++)
		visited[k]=0;
	i=LocateVer(G,name1);
	j=LocateVer(G,name2);
    Path1(G,i,j,path,0);
}

//�������������·��
void PathAll(AdjMatrix *G)
{

	char name1[20],name2[20];
	printf("����������������:\n");
	flushall();
	scanf("%s %s",name1,name2);
	path2(G,name1,name2);

    getchar();
	printf("\n\t��������������˵�!");
    getc(stdin);
	Menu(G);
}

//����Dijkstra�㷨�����ʼ���㵽��������������·��
void Dijkstra(AdjMatrix *G,int start,int end,int dist[],int path[][MAXVER])
{
	//dist�����¼�������·�����ȣ�path�����¼��Ӧ·���ϵĸ�����
	int mindist;
	int i,j,k,t;
	for(i=1;i<=G->vernum ;i++)
	{
		dist[i]=G->arcs [start][i];
		if(G->arcs [start][i]!=INFINITY)
			path[i][1]=start;
	}

	path[start][0]=1;//start�����Ѽ��뵽����S��
	for(i=2;i<=G->vernum ;i++)//Ѱ�Ҹ������·��
	{
		mindist=INFINITY;
		for(j=1;j<=G->vernum ;j++)//ѡ��Ȩֵ��С�ı�
			if(!path[j][0]&&dist[j]<mindist)
			{
				k=j;//��k��¼��һ��ѡ�еľ���
				mindist=dist[j];
			}

			if(mindist==INFINITY)
				break;
			path[k][0]=1;//k�����Ѽ��뵽����S��
			for(j=1;j<=G->vernum ;j++)//�޸�·��
			{
				if(!path[j][1]&&G->arcs [k][j]<INFINITY&&dist[k]+G->arcs [k][j]<dist[j])
				{
					dist[j]=dist[k]+G->arcs [k][j];
					t=1;
					while(path[k][t]!=0)
					{
						path[j][t]=path[k][t];
						t++;
					}
					path[j][t]=k;
					path[j][t+1]=0;
				}
			}
	}

	for(i=1;i<=G->vernum ;i++)
		if(i==end)
			break;
			printf("%s<--->%s�����·��Ϊ:\n��%s",G->vertex [start].name,G->vertex [end].name,G->vertex [start].name);
		for(j=2;path[i][j]!=0;j++)
			printf("--->%s",G->vertex [path[i][j]].name);
		printf("--->%s,\n����Ϊ%dm\n",G->vertex [end].name,dist[i]);
}

//Ѱ�����·��
void ShortPath(AdjMatrix *G)
{
	char name[20];
	int start,end;
	int dist[MAXVER],path[MAXVER][MAXVER]={0};

	printf("��������ʼ��������:\n");
	scanf("%s",name);
	start=LocateVer(G,name);

	printf("��������ֹ��������:\n");
	scanf("%s",name);
	end=LocateVer(G,name);

	Dijkstra(G,start,end,dist,path);

	getchar();
	printf("\t��������������˵�!");
	getc(stdin);
	Menu(G);
}

//����prim�㷨�������ͨ·��
void Prim(AdjMatrix *G,int start)
{
	struct node
	{
		int adjvex;
		int lowcost;
	}closedge[MAXVER];//Ԫ���±��Ӧ������

	int i,e,k,m,min;
	closedge[start].lowcost=0;//��־����start�Ѽ��뵽��С��������
    
	//�Գ�����ʼ����������������ʼ����Ӧ��closedge����
	for(i=1;i<=G->vernum ;i++)
		if(i!=start)
		{
			closedge[i].adjvex=start;
			closedge[i].lowcost=G->arcs [start][i];
		}

       for(e=1;e<=G->vernum-2;e++)
	   {
		   //ѡ��Ȩֵ��С�ı�
           min=INFINITY;
		   for(k=1;k<=G->vernum ;k++)
		   {
			   if(closedge[k].lowcost!=0&&closedge[k].lowcost<min)
			   {
				   m=k;//��m��¼ѡ�е���һ������
				   min=closedge[k].lowcost;
			   }
		   }
		   printf("��%s<--->%s:%dm\n",G->vertex [closedge[m].adjvex].name,G->vertex [m].name,closedge[m].lowcost);
           closedge[m].lowcost=0;//��־����m�Ѽ��뵽��С��������

		   //���¶��������С�������󣬸���closedge������Ϣ
		   for(i=1;i<=G->vernum ;i++)
			   if(i!=m&&G->arcs[m][i]<closedge[i].lowcost)
			   //һ�����ָ�С��Ȩֵ�߳��֣����滻ԭ����Ϣ
			   {
				   closedge[i].lowcost=G->arcs [m][i];
				   closedge[i].adjvex=m;//������ı�
			   }
	   }
}

//��ѯ��ĳ�����㿪ʼ�������ͨ·��
void MiniSpanTree(AdjMatrix *G)
{
	char name[20];
	int start;

	printf("��������ʼ��������:\n");
	scanf("%s",name);
	start=LocateVer(G,name);
	Prim(G,start);

	getchar();
	printf("\t��������������˵�!");
	getc(stdin);
	Menu(G);
	
}

void main()
{   
	
	AdjMatrix *G;
	G=(AdjMatrix *)malloc(sizeof(AdjMatrix));
	ReadFile(G);
	Menu(G);
}

























