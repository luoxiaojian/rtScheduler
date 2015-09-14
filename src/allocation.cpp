#include "allocation.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

alloctaion::allocation(const taskset *sample)
{
	int pnumber, lcm;
	pnumber=sample->getProcessorNum();
	lcm=sample->getLcmPeriod();
	this->processor_num=pnumber;
	this->lcm_period=lcm;
	this->task_num=sample->getTaskNum();
	alloc=(int **)malloc(sizeof(int *)*pnumber);
	for(int i=0; i<pnumber; i++)
		alloc[i]=(int *)malloc(sizeof(int)*lcm_period);
	for(int i=0; i<pnumber; i++)
		for(int j=0; j<lcm_period; j++)
			alloc[i][j]=-1;
}

allocation::~allocation()
{
	int pnumber=this->processor_num;
	for(int i=0; i<pnumber; i++)
		free(this->alloc[i]);
	free(this->alloc);
}

allocation::void setAlloc(int time, int pno, int value)
{
	if(time>=this->lcm_period || time<0)
	{
		cout<<"Error alloc: wrong time"<<endl;
		exit(0);
	}
	if(pno>=this->processor_num || pno<0)
	{
		cout<<"Error alloc: wrong processor no."<<endl;
		exit(0);
	}
	if(value>=this->task_num || value<0)
	{
		cout<<"Erro alloc: wrong task no."<<endl;
		exit(0);
	}
	this->alloc[pno][time]=value;
}

allocation::void outputAlloc()
{
	int lcm, pnumber;
	lcm=this->lcm_period;
	pnumber=this->processor_num;
	for(int i=0; i<pnumber; i++)
	{
		cout<<"p"<<i+1<<":\t";
		for(int j=0; j<lcm; j++)
			cout<<this->alloc[i][j]<<"  ";
		cout<<endl;
	}
}

allocation::bool checkSchedule(const taskset *sample)
{
	int *period=sample->getPeriods();
	int *execute=sample->getExecutes();
	int tn=sample->getTaskNum();
	int pn=sample->getProcessorNum();
	int lcm=sample->getLcmPeriod();

	for(int i=0; i<tn; i++)
	{
		for(int j=0; j<lcm/period[i]; j++)
		{
			int count=0;
			for(int k=0; k<period[i]; k++)
			{
				for(int m=0; m<pn; m++)
					if(this->alloc[m][j*period[i]+k]==i)
						count++;
			}
			if(count!=execute[i])
			{
				cout<<"Schedule error1."<<endl;
				return false;
			}
		}
	}

	for(int i=0; i<lcm; i++)
	{
		for(int j=0; j<pn-1; j++)
			for(int k=j+1; k<pn; k++)
				if(alloc[j][i]==alloc[k][i])
				{
					cout<<"Schedule error2."<<endl;
					return false;
				}
	}
	return true;
}
