#include <math.h>
static int temp = -1;
void progressbar2(double currentiter, double fullsize)
{  
 double seekpercent = ((double)currentiter/(double)fullsize)*100;
if(((int)floor(seekpercent))%10==0 && (int)floor(seekpercent)!= temp)
  {
	//std::cerr << "\nProcess " << (int)seekpercent << "% complete..";
    std::cerr<<"\r[";
	for(int i=0; i<100; i=i+10)
	{
		if(i<(int)(seekpercent)) std::cerr << "=";
		else std::cerr << " ";
	}
    std::cerr << "]";
    std::cerr << " Status: " << (int)seekpercent << " % complete.." << std::flush;

	temp = (int)floor(seekpercent);
  }

}
