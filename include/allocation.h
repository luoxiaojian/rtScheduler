
#include "taskset.h"

class allocation
{
	public:
		allocation(const taskset *sample);
		~allocation();
		void setAlloc(int time, int pno, int value);
		void outputAlloc();
		bool checkSchedule(const taskset *sample);

	private:
		int **alloc;
		int processor_num, lcm_period, task_num;
}

