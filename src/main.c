#include <stdio.h>
#include <stdlib.h>

#define MAX_DURATION 2000000
#define MAX_TASK_NUM 101
#define MAX_PROCESSOR_NUM 100

int **alloc;
int period[MAX_TASK_NUM], execute[MAX_TASK_NUM];
int task_num, processor_num, lcm_period;
int **alpha;
FILE *fin, *fout;
int sum[MAX_TASK_NUM];

int getSign(long x)
{
	if(x>0)
		return 1;
	if(x<0)
		return -1;
	return 0;
}

void generate_alpha()
{
	int i, j;
	long mid, ret;
	for(i=0; i<task_num; i++)
		for(j=0; j<lcm_period; j++)
		{
			mid=(((long)(execute[i]))*((long)j))/((long)period[i]);
			ret=((long)execute[i])*((long)(j+1))-(mid+1l)*((long)period[i]);
			alpha[i][j]=getSign(ret);
		}
}

int getLag(int tid, int t)
{
	int i, j, s;
	int ret;
	s=sum[tid];
	long mid=((long)t)*((long)execute[tid])-((long)s)*((long)period[tid]);
	ret=getSign(mid);
	return ret;
}

int isUrgent(int tid, int t)
{
	if((getLag(tid, t)>0)&&(alpha[tid][t]!=-1))
		return 1;
	return 0;
}

int isTnegru(int tid, int t)
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
	int i, j, tmp;
	for(i=1; i<contending_num; i++)
		for(j=0; j<contending_num-i; j++)
		{
			if(compare(contending[j], contending[j+1], t)==0)
			{
				tmp=contending[j];
				contending[j]=contending[j+1];
				contending[j+1]=tmp;
			}
		}
}

int checkSchedule()
{
	int i, j, k, m, count;
	for(i=0; i<task_num; i++)
	{
		for(j=0; j<lcm_period/period[i]; j++)
		{
			count=0;
			for(k=0; k<period[i]; k++)
			{
				for(m=0; m<processor_num; m++)
				{
					if(alloc[m][j*period[i]+k]==i)
						count++;
				}
			}
			if(count!=execute[i])
			{
				printf("task%d, job%d, from %d to %d.\n", i, j, j*period[i], (j+1)*period[i]-1);
				return -1;
			}
		}
	}
	for(i=0; i<lcm_period; i++)
	{
		for(j=0; j<processor_num-1; j++)
			for(k=j+1; k<processor_num; k++)
				if(alloc[j][i]==alloc[k][i])
				{
					printf("time%d.\n", i);
					return -1;
				}
	}
	return 1;
}

void generate_alloc()
{
	int i, j, RU, ind, k;
	int urgent[MAX_TASK_NUM], tnegru[MAX_TASK_NUM], urgent_num, tnegru_num, contending[MAX_TASK_NUM], contending_num;
	for(i=0; i<lcm_period; i++)
	{
		urgent_num=0;
		tnegru_num=0;
		contending_num=0;
		for(j=0; j<task_num; j++)
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
		RU=processor_num-urgent_num;
		selectFromContending(contending, contending_num, i);
		ind=0;
		for(j=0; j<urgent_num; j++)
		{
			alloc[ind][i]=urgent[j];
			sum[urgent[j]]++;
			ind++;
		}
		for(j=0; (j<RU)&&(j<contending_num); j++)
		{
			alloc[ind][i]=contending[j];
			sum[contending[j]]++;
			ind++;
		}
	}
}

int algo()
{
	int i, j, preemption;
	fscanf(fin, "%d%d%d", &task_num, &processor_num, &lcm_period);
	for(i=0; i<task_num; i++)
		fscanf(fin, "%d", &period[i]);
	for(i=0; i<task_num; i++)
		fscanf(fin, "%d", &execute[i]);
	alpha=(int **)malloc(sizeof(int *)*task_num);
	for(i=0; i<task_num; i++)
		alpha[i]=(int *)malloc(sizeof(int)*lcm_period);
	alloc=(int **)malloc(sizeof(int *)*processor_num);
	for(i=0; i<processor_num; i++)
		alloc[i]=(int *)malloc(sizeof(int)*lcm_period);
	for(i=0; i<processor_num; i++)
		for(j=0; j<lcm_period; j++)
			alloc[i][j]=-1;
	for(i=0; i<task_num; i++)
		sum[i]=0;
	generate_alpha();
	generate_alloc();
	for(i=0; i<processor_num; i++)
	{
		for(j=0; j<lcm_period; j++)
			fprintf(fout, "%d ", (int)alloc[i][j]);
		fprintf(fout, "\n");
	}
	if(checkSchedule()==1)
		printf("scheduling successfully.\n");
	else
		exit(0);
	for(i=0; i<task_num; i++)
		free(alpha[i]);
	free(alpha);
	for(i=0; i<processor_num; i++)
		free(alloc[i]);
	free(alloc);
	return 0;

}

int main(int argc, char *argv[])
{
	int i, taskset_num;

	if(argc<4)
	{
		printf("usage: ./pfAlgo <taskset_filename> <target_filename> <taskset_num>\n");
		return 0;
	}
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	taskset_num=atoi(argv[3]);

	for(i=0; i<taskset_num; i++)
	{
		printf("%d:\t", i);
		algo();
	}

	fflush(fout);
	fclose(fin);
	fclose(fout);
	return 0;


}
