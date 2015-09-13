#include "allocation.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

alloctaion::allocation(taskset sample1)
{
	int pnumber, lcm_period;
	this->sample=new taskset(sample1);
	pnumber=this->sample.getProcessorNum();
	lcm_period=this->sample.getLcmPeriod();
	alloc=(int **)malloc(sizeof(int *)*pnumber);
	for(int i=0; i<pnumber; i++)
		alloc[i]=(int *)malloc(sizeof(int)*lcm_period);
	for(int i=0; i<pnumber; i++)
		for(int j=0; j<lcm_period; j++)
			alloc[i][j]=-1;
}

allocation::~allocation()
{
	int pnumber=this->sample.getProcessorNum();
	delete(this->sample);
	for(int i=0; i<pnumber; i++)
		free(this->alloc[i]);
	free(this->alloc);
}

allocation::void setAlloc(int time, int pno, int value)
{
	if(time>=lcm_period || time<0)
	{
		cout<<"Error alloc"<<endl;
		exit(0);
	}
	this->alloc[pno][time]=value;
}

allocation::void outputAlloc()
{
	int lcm, pnumber;
	lcm=this->sample.getLcmPeriod();
	pnumber=this->sample.getProcessorNum();
	for(int i=0; i<pnumber; i++)
	{
		cout<<"p"<<i+1<<":\t";
		for(int j=0; j<lcm; j++)
			cout<<this->alloc[i][j]<<"  ";
		cout<<endl;
	}
}

allocation::bool checkSchedule()
{
	int *period=this->sample.getPeriods();
	int *execute=this->sample.getExecutes();
	int tn=this->sample.getTaskNum();
	int pn=this->sample.getProcessorNum();
	int lcm=this->sample.getLcmPeriod();

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
