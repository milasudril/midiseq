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

	uint8_t eventType=m_reader.byteGet();
	if(m_reader.eoc())
		{return 0;}

	switch(eventType)
		{
		case 0xff:
			{
			event.type=m_reader.byteGet();
			if(m_reader.eoc())
				{return 0;}
			size_t length=m_reader.varfieldGet();
			if(length==0)
				{break;}
			if(m_reader.eoc())
				{return 0;}

			auto n=std::min(length,sizeof(event.data));
			auto n_read=m_reader.dataRead(&event.data,n);
			if(n_read!=n)
				{return 0;}
			length-=n_read;

			if(!m_reader.skip(length))
				{return 0;}

			if(event.type==0x51)
				{event.data.dwords[0]=__builtin_bswap32(event.data.dwords[0])>>8;}
			}
			break;
		case 0xf7:
		case 0xf0:
			{
			size_t length=m_reader.varfieldGet();
			if(length!=0 && m_reader.eoc())
				{return 0;}
			if(!m_reader.skip(length))
				{return 0;}
			printf("Sysex message skipped\n");
			}
			break;
		default:
			event.type=(uint64_t)-1;

			if(eventType&0x80)
				{
				event.data.bytes[0]=eventType;
				if(m_reader.eoc())
					{return 0;}
				event.data.bytes[1]=m_reader.byteGet();
				}
			else
				{
				//Running mode
				event.data.bytes[0]=status_prev;
				event.data.bytes[1]=eventType;
				}

			if((event.data.bytes[0]&0xf0)!=0xc0
				&& (event.data.bytes[0]&0xf0)!=0xd0)
				{
				event.data.bytes[2]=m_reader.byteGet();
				if(m_reader.eoc())
					{return 0;}
				}

			status_prev=event.data.bytes[0];
			break;
		}
	return 1;
	}
