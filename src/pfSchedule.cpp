#include "pfScheduler.h"

static int task_num, processor_num, lcm_period;
static int *sum;
static int **alpha, **alloc;
static int *period, *execute;

static int getSign(long x)
{
	if(x>0)
		return 1;
	if(x<0)
		return -1;
	return 0;
}

static void generateAlpha()
{
	for(int i=0; i<task_num; i++)
		for(int j=0; j<lcm_period; j++)
		{
			mid=(((long)(execute[i]))*((long)j))/((long)period[i]);
			ret=((long)execute[i])*((long)(j+1))-(mid+1l)*((long)period[i]);
			alpha[i][j]=getSign(ret);
		}
}

static void preprocess(const taskset *ts, allocation *al)
{
	task_num=ts->getTaskNum();
	processor_num=ts->getProcessorNum();
	lcm_period=ts->getLcmPeriod();

	period=(int *)malloc(sizeof(int)*task_num);
	execute=(int *)malloc(sizeof(int)*task_num);
	alpha=(int **)malloc(sizeof(int *)*task_num);
	for(int i=0; i<task_num; i++)
		alpha[i]=(int *)malloc(sizeof(int)*lcm_period);
	alloc=(int **)malloc(sizeof(int *)task_num);
	for(int i=0; i<task_num; i++)
		alloc[i]=(int *)malloc(sizeof(int)*lcm_period);
	sum=(int *)malloc(sizeof(int)*task_num);

	for(int i=0; i<task_num; i++)
		sum[i]=0;
	for(int i=0; i<processor_num; i++)
		for(int j=0; j<lcm_period; j++)
			alloc[i][j]=-1;
	for(i=0; i<task_num; i++)
	{
		period[i]=ts->getPeriods[i];
		execute[i]=ts->getExecute[i];
	}

	generateAlpha();
}

static int getLag(int tid, int t)
{
	int s=sum[tid];
	long mid=((long)t)*((long)execute[tid])-((long)s)*((long)period[tid]);
	int ret=getSign(mid);
	return ret;
}

static int isUrgent(int tid, int t)
{
	if((getLag(tid, t)>0)&&(alpha[tid][t]!=-1))
		return 1;
	return 0;
}

static int isTnegru(int tid, int t)
{
	if((getLag(tid, t)<0)&&(alpha[tid][t]!=1))
		return 1;
	return 0;
}

int compare(int tid1, int tid2, int t)
{
	int x=t+1;
	while((alpha[tid1][x]==alpha[tid2][x])&&(alpha[tid1][x]!=0))
		x++;
	if((alpha[tid1][x]==0)&&(alpha[tid2][x]==0))
	{
		if(tid1<tid2)
			return 1;
		return 0;
	}
	if(alpha[tid1][x]>=alpha[tid2][x])
		return 1;
	return 0;

}

void selectFromContending(int *contending, int contending_num, int t)
{
	for(int i=1; i<contending_num; i++)
		for(int j=0; j<contending_num-i; j++)
		{
			if(compare(contending[j], contending[j+1], t)==0)
			{
				int tmp=contending[j];
				contending[j]=contending[j+1];
				contending[j+1]=tmp;
			}
		}
}

static void pfAlgo()
{
	int *urgent, *tnegru, *contending;
	int urgent_num, tnegru_num, contending_num;
	urgent=(int *)malloc(sizeof(int)*task_num);
	tnegru=(int *)malloc(sizeof(int)*task_num);
	contending=(int *)malloc(sizeof(int)*task_num);
	for(int i=0; i<lcm_period; i++)
	{
		urgent_num=0;
		tnegru_num=0;
		contending_num=0;
		for(int j=0; j<task_num; j++)
		{
			if(isUrgent(j, i))	
			{
				urgent[urgent_num]=j;
				urgent_num++;
				continue;
			}
			if(isTnegru(j, i))
			{
				tnegru[tnegru_num]=j;
				tnegru_num++;
				continue;
			}
			contending[contending_num]=j;
			contending_num++;
		}
		int RU=processor_num-urgent_num;
		selectFromContending(contending, contending_num, i);
		int ind=0;
		for(int j=0; j<urgent_num; j++)
		{
			alloc[ind][i]=urgent[j];
			sum[urgent[j]]++;
			ind++;
		}
		for(int j=0; (j<RU)&&(j<contending_num); j++)
		{
			alloc[ind][i]=contending[j];
			sum[contending[j]]++;
			ind++;
		}
	}
}

void pfScheduler::schedule(const taskset *ts, allocation *al)
{
	preprocess(ts, al);
	pfAlgo();
	postprocess();
}
