#ifdef __WAND__
target[name[track_reader.o] type[object]]
#endif

#include "track_reader.h"
#include "chunk_reader.h"
#include "event.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

bool MIDISeq::TrackReader::eventNextGet(Event& event)
	{
	if(m_reader.eoc())
		{return 0;}

	memset(&event,0,sizeof(event));

	time_running+=m_reader.varfieldGet();
	event.time=time_running;
	if(m_reader.eoc())
		{return 0;}

	event.data[0]=m_reader.byteGet();
	
	if(m_reader.eoc())
		{return 0;}

	switch(event.data[0])
		{
		case 0xff:
			{
			event.data[1]=m_reader.byteGet();
			if(m_reader.eoc())
				{return 0;}
			
			size_t length=m_reader.varfieldGet();
			if(length==0)
				{return 1;}
			if(m_reader.eoc())
				{return 0;}
			auto n=std::min(length,sizeof(event.data)-2);
			auto n_read=m_reader.dataRead(event.data+2,n);
			if(n_read!=n)
				{return 0;}
			length-=n_read;
			if(!m_reader.skip(length))
				{return 0;}
			event.size=n_read;
			return 1;
			}
	
		case 0xf7:
		case 0xf0:
			{
			size_t length=m_reader.varfieldGet();
			if(length==0)
				{return 1;}
			if(m_reader.eoc())
				{return 0;}
			auto n=std::min(length,sizeof(event.data)-1);
			auto n_read=m_reader.dataRead(event.data+1,n);
			if(n_read!=n)
				{return 0;}
			length-=n_read;
			if(!m_reader.skip(length))
				{return 0;}
			event.size=n_read;
			return 1;
			}
			
		default:
		//	We got a new status message
			if(event.data[0]&0x80)
				{
				if(m_reader.eoc())
					{return 0;}
				event.data[1]=m_reader.byteGet();
				}
			else //We did not. This is running status.
				{
			//	Duplicate the old status, and take the current "status" byte
			//	as the first data byte.
				event.data[1]=event.data[0];
				event.data[0]=status_prev;
				}

		//	Not all MIDI status messages are 3 bytes. Some are only two
			if((event.data[0]&0xf0)!=0xc0 && (event.data[0]&0xf0)!=0xd0)
				{
				event.data[2]=m_reader.byteGet();
				if(m_reader.eoc())
					{return 0;}
				}

			status_prev=event.data[0];
			break;
		}
	return 1;
	}
