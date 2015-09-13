#include "taskset.h"


class pfScheduler
{
	public:
		pfScheduler(taskset sample);
		void setTaskset(taskset sample);
		taskset *getTaskset();
		bool schedule();
		int **getAlpha();
	private:
		taskset sample;
		allocation result;
		int **alpha;
}
