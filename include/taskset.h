#include <vector>
#include <string>
#include <fstream>
class taskset
{
	public:
		taskset(const taskset *sam);
		~taskset();
		void setTaskNum(int tn);
		void setProcessorNum(int pn);
		void setLcmPeriod(int lcm);
		void setPeriod(int *period, int tn);
		void setExecute(int *execute, int tn);
		int getTaskNum();
		int getProcessorNum();
		int getLcmPeriod();
		int getPeriod(int tno);
		int getExecute(int tno);
		int *getPeriods();
		int *getExecutes();

	private:
		int task_num, processor_num, lcm_period;
		int *period;
		int *execute;
};

