#ifndef TIME_LINE_H
#define TIME_LINE_H

#include "../pch.h"

class TimeLine
{
	public:
		TimeLine();
		
		int start = 0;
		int end = 100;
		int frame_rate = 25;
		int current_frame = 0;
			
	private:
		/* add your private declarations */
};

#endif /* TIME_LINE_H */ 
