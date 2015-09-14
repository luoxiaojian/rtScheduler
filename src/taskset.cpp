#include "taskset.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

taskset::taskset(const taskset *sam)
{
	this->task_num=sam->getTaskNm();
	this->processor_num=sam->getProcessorNum();
	this->lcm_period=sam->getLcmPeriod();
	
	this->period=(int *)malloc(sizeof(int)*this->task_num);
	this->execute=(int *)malloc(sizeof(int)*this->task_num);

	for(int i=0; i<this->task_num; i++)
	{
		this->period[i]=sam->getPeriod[i];
		this->execute[i]=sam->getExecute[i];
	}
}

taskset::~taskset()
{
	free(this->period);
	free(this->execute);
}

void taskset::setTaskNum(int tn)
{
	this->task_num=tn;
}

void taskset::setProcessorNum(int pn)
{
	this->processor_num=pn;
}

void taskset::setLcmPeriod(int lcm)
{
	this->lcm_period=lcm;
}

void taskset::setPeriod(int *parr, int tn)
{
	if(this->task_num!=tn)
	{
		cout<<"disaccord task number."<<endl;
		exit(0);
	}
	this->period=(int *)malloc(sizeof(int)*tn);
	for(int i=0; i<tn; i++)
		this->period[i]=parr[i];
}

void taskset::setExecute(int *earr, int tn)
{
	if(this->task_num!=tn)
	{
		cout<<"disaccord task number."<<endl;
		exit(0);
	}
	this->execute=(int *)malloc(sizeof(int)*tn);
	for(int i=0; i<tn; i++)
		this->execute[i]=earr[i];
}

int getTaskNum()
{
	return this->task_num;
}

int getProcessorNum()
{
	return this->processor_num;
}

int getLcmPeriod()
{
	return this->lcm_period;
}

int getPeriod(int tno)
{
	if(tno>=this->task_num || tno<0)
	{
		cout<<"Error task number when fetch period."<<endl;
		exit(0);
	}
	return this->period[tno];
}

int getExecute(int tno)
{
	if(tno>=this->task_num || tno<0)
	{
		cout<<"Error task number when fetch execute."<<endl;
		exit(0);
	}
	return this->execute[tno];
}

int *getPeriods()
{
	return this->period;
}

int *getExecutes()
{
	return this->execute;
}
