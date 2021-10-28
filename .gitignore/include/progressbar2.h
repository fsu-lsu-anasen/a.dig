#include <math.h>
static int temp = -1;
void progressbar2(double currentiter, double fullsize)
{  
 double seekpercent = ((double)currentiter/(double)fullsize)*100;
if(((int)floor(seekpercent))%10==0 && (int)floor(seekpercent)!= temp)
  {
	//cerr << "\nProcess " << (int)seekpercent << "% complete..";
    cerr<<"\r[";
	for(int i=0; i<100; i=i+10)
	{
		if(i<(int)(seekpercent)) cerr << "=";
		else cerr << " ";
	}
    cerr << "]";
    cerr << " Status: " << (int)seekpercent << " % complete.." << std::flush;

	temp = (int)floor(seekpercent);
  }

}
