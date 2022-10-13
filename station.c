#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>

#define VMAX 512
#define MAXLEN 16
#define INFINITY 233333

typedef struct station
{
	char sname[MAXLEN];//车站名
	char exflag;//是否为换乘站
}Station;
typedef struct edge
{
	int weight;
	int linenum;
}Edge;

void Init_Graph(void);
int get_vertex_order(char *name,int exflag);
void Dijkstra(int v0,int v1);
int Adjust_order(int *tmp,int v0,int v1);

Station BGvertex[VMAX];//站点信息 名字和换乘标志(这个貌似没啥用)
int vertexindex;//index,顾名思义
Edge BGedge[VMAX][VMAX];//边的信息,位于几号线,以及权重(默认为1)
int VN,EN;//总站数和总边数
/*
**下面是Dijkstra算法需要的数据结构
*/
int wfound[VMAX];//确定最短路径标志位
int Spath[VMAX];//记录前驱结点
int Sweight[VMAX];//顶点到起点的最短距离 初试化为 到起点的距离

int main()
{
	Init_Graph();
	char s0[MAXLEN],s1[MAXLEN];
	scanf("%s%s",s0,s1);
	int v0=get_vertex_order(s0,0),v1=get_vertex_order(s1,0);
	//printf("%d %d",v0,v1);
	//printf("%s %s",v0,v1);
	//printf("%d %d",EN,VN);
	Dijkstra(v0,v1);
	
	int tmp[VMAX],tmpnum;
	tmpnum=Adjust_order(tmp,v0,v1);
	//printf("%d %s\n",v,BGvertex[v].sname);
	int exchangenum=0;
	printf("%s",BGvertex[tmp[0]].sname);
	for(int i=1;i<tmpnum;i++)
	{
		if(i==tmpnum-1 || BGedge[tmp[i]][tmp[i+1]].linenum!=BGedge[tmp[i]][tmp[i-1]].linenum)
		{
			exchangenum++;
			printf("-%d(%d)",BGedge[tmp[i]][tmp[i-1]].linenum,exchangenum);
			exchangenum=0;
			printf("-%s",BGvertex[tmp[i]].sname);
		}
		else exchangenum++;
	}
	system("pause");
	return 0;
}

int Adjust_order(int *tmp,int v0,int v1)
{
	int tmpindex=0,v=v1;
	tmp[tmpindex++]=v1;
	while(v!=v0)
	{
		tmp[tmpindex++]=Spath[v];
		v=Spath[v];
	}
	
	int front=0,last=tmpindex-1,temp;
	while(front<last)
	{
		temp=tmp[front];
		tmp[front]=tmp[last];
		tmp[last]=temp;
		front++; last--;
	}
	return tmpindex;
}

void Dijkstra(int v0,int v1)
{
	//初始化
	for(int i=0;i<VN;++i)
	{
		Sweight[i]=BGedge[v0][i].weight;
		Spath[i]=v0;
	}
	wfound[v0]=1; Sweight[v0]=0;
	
	int minweight,v;
	for(int i=1;i<VN;i++)
	{
		minweight=INFINITY;
		for(int j=0;j<VN;++j)
		{
			if(wfound[j]==1) continue;
			if(minweight>Sweight[j])
			{
				minweight=Sweight[j];
				v=j;
			}	
		}
		wfound[v]=1;
		for(int j=0;j<VN;++j)
		{
			if(wfound[j]==1) continue;
			if(Sweight[j]>Sweight[v]+BGedge[v][j].weight)
			{
				Sweight[j]=Sweight[v]+BGedge[v][j].weight;
				Spath[j]=v;
			}
		}
	}
}

void Init_Graph(void)
{
	for(int i=0;i<VMAX;++i)
		for(int j=0;j<VMAX;++j)
			BGedge[i][j].weight=INFINITY;
	
	FILE *in;
	in=fopen("bgstations.txt","r");
	
	int linetotal;
	fscanf(in,"%d",&linetotal); 
	
	int linenum,stationtotal;
	Station tmpst;
	int v1,v2;
	for(int i=1;i<=linetotal;i++)
	{
		fscanf(in,"%d%d",&linenum,&stationtotal);
		//printf("%d %d\n",linenum,stationtotal);
		v1=v2=-1;
		for(int j=1;j<=stationtotal;j++)
		{
			fscanf(in,"%s %d",tmpst.sname,&tmpst.exflag);
			//printf("%s %d\n",tmpst.sname,tmpst.exflag);
			v2=get_vertex_order(tmpst.sname,tmpst.exflag);
			if(v1!=-1)
			{
				EN++;
				BGedge[v1][v2].weight=BGedge[v2][v1].weight=1;
				BGedge[v1][v2].linenum=BGedge[v2][v1].linenum=linenum;
			}
			v1=v2;
			VN++;
		}
	}
	fclose(in);
}

int get_vertex_order(char *name,int exflag)
{
	int i;
	for(i=0;i<vertexindex;i++)
	{
		if(strcmp(name,BGvertex[i].sname)==0)
			return i;
	}
	strcpy(BGvertex[vertexindex].sname,name);
	BGvertex[vertexindex++].exflag=exflag;
	return i;
}