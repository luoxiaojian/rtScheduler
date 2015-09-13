
#include "taskset.h"

class allocation
{
	public:
		allocation(taskset sample);
		~allocation();
		void setAlloc(int time, int pno, int value);
		void outputAlloc();
		bool checkSchedule();

	private:
		int **alloc;
		taskset sample;
}
