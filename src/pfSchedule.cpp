#include "pfScheduler.h"
#include <cstring.h>

static int getSign(long x)
{
	if(x>0)
		return 1;
	if(x<0)
		return -1;
	return 0;
}

static void generateAlpha(int *period, int *execute, int **alpha, int tn, int lcm)
{
	int i, j;
	long mid, ret;
	for(i=0; i<tn; i++)
		for(j=0; j<lcm; j++)
		{
			mid=(((long)(execute[i]))*((long)j))/((long)period[i]);
			ret=((long)execute[i])*((long)(j+1))-(mid+1l)*((long)period[i]);
			alpha[i][j]=getSign(ret);
		}
}

pfScheduler::pfScheduler(taskset sample1)
{
	int *period, *execute;
	int **alpha;
	this->sample=new taskset(sample1);
	this->result=new allocation(sample1);
	period=this->sample.getPeriods();
	execute=this->sample.getExecutes();
	this->alpha=(int **)malloc(sizeof(int *)*this->sample.getTaskNum());
	for(int i=0; i<this->sample.getTaskNum(); i++)
		this->alpha[i]=(int *)malloc(sizeof(int)*this->sample.getLcmPeriod());
	alpha=this->alpha;
	generateAlpha(period, execute, alpha, sample.getTaskNum(), sample.getLcmPeriod());
}

pfScheduler::~pfScheduler()
{
	delete(sample);
	delete(result);
}

void pfScheduler::setTaskset(taskset sample1)
{
	memcpy(&(this->sample), &sample1, sizeof(taskset));
}


taskset *pfScheduler::getTaskset()
{
	return &(this->sample);
}

static getLag(int tid, int t, int **alpha, int *sum, int task_num)
{

}

bool pfScheduler::schedule()
{
	int lcm_period=this->sample.getLcmPeriod();
	int task_num=this->sample.getTaskNum();
	int *urgent, *tnegru, *contending;
	urgent=(int *)malloc(sizeof(int)*task_num);
	tnegru=(int *)malloc(sizeof(int)*task_num);
	contending=(int *)malloc(sizeof(int)*task_num);
	for(int i=0; i<lcm_period; i++)	
	{
		int urgent_num=0, tnegru_num=0, contending_num=0;
		for(int j=0; j<task_num; j++)
		{
			if()
		}
	}
}
