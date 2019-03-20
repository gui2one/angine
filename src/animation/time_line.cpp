#include "time_line.h"


TimeLine::TimeLine()
{
	//~ printf("Time Line : \n\tstart : %d\n\tend : %d\n\tframerate : %d\n", start, end, frame_rate);
}

void TimeLine::update(){
	if(is_playing)
	{
		int offset = (int)((glfwGetTime() - chrono_start) * frame_rate);
		
		current_frame = (offset + temp_frame)  % (end-start+1);
		current_frame += start;
	}else{
		
	}
}

void TimeLine::play(){
	
	chrono_start = glfwGetTime();
	is_playing = true;
}

void TimeLine::pause(){
	temp_frame = current_frame;
	is_playing = false;
}

void TimeLine::stop(){
	is_playing = false;
	current_frame = start;
	temp_frame = current_frame;
}


