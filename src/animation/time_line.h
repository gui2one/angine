#ifndef TIME_LINE_H
#define TIME_LINE_H

#include "../pch.h"

class TimeLine
{
	public:
		TimeLine();
		
		void update();
		void play();
		void pause();
		void stop();
		
		int start = 1;
		int end = 100;
		float frame_rate = 25.0;
		int current_frame = 1;
		int temp_frame = 0;
	
		float chrono_start = 0.0;
		
	private:
		bool is_playing = false;
		/* add your private declarations */
};

#endif /* TIME_LINE_H */ 
