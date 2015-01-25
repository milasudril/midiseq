#ifdef __WAND__
target[name[track_writer.o] type[object]]
#endif

#include "track_writer.h"
#include "chunk_writer.h"
#include "event.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

bool MIDISeq::TrackWriter::eventWrite(const Event& event)
	{
	auto delta=event.time-time_prev;
	m_writer.varfieldPut(delta);
	switch(event.data[0])
		{
		case 0xff:
			if(!m_writer.bytePut(event.data[0]))
				{return 0;}
			if(!m_writer.bytePut(event.data[1]))
				{return 0;}
			if(!m_writer.varfieldPut(event.size))
				{return 0;}
			if(m_writer.dataWrite(event.data+2,event.size)!=event.size)
				{return 0;}
			return 1;
		
		case 0xf7:
		case 0xf0:
			if(!m_writer.bytePut(event.data[0]))
				{return 0;}
			if(!m_writer.varfieldPut(event.size))
				{return 0;}
			if(m_writer.dataWrite(event.data+1,event.size)!=event.size)
				{return 0;}
			return 1;
			
		default:
			if(event.data[0]!=status_prev)
				{
				if(!m_writer.bytePut(event.data[0]))
					{return 0;}
				status_prev=event.data[0];
				}
				
			if(!m_writer.bytePut(event.data[1]))
				{return 0;}
			if((event.data[0]&0xf0)!=0xc0 && (event.data[0]&0xf0)!=0xd0)
				{
				if(!m_writer.bytePut(event.data[2]))
					{return 0;}
				}
		}
	time_prev=event.time;
	return 1;
	}
