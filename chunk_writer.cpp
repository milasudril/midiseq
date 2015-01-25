#ifdef __WAND__
target[name[chunk_writer.o] type[object]]
#endif

#include "chunk_writer.h"
#include "file_header.h"
#include <herbs/exceptionmissing/exceptionmissing.h>
#include <herbs/fileout/fileout.h>
#include <herbs/cpuinfo/cpuinfo.h>
#include <cstring>

MIDISeq::ChunkWriter::ChunkWriter(Herbs::FileOut& dest
	,const FileHeader& header):
	m_dest(dest)
	{
	position=0;
	m_dest.modeBufferedOn();
	const char* magic="MThd";
	while(*magic!='\0')
		{
		m_dest.bytePut(*magic);
		++magic;
		++position;
		}

	uint32_t size_header=sizeof(header);
	if(!Herbs::CPUInfo::bigEndianIs())
		{size_header=__builtin_bswap32(size_header);}

	auto n_written=m_dest.write(&size_header,sizeof(size_header));
	if(n_written != sizeof(size_header))
		{
		m_dest.modeBufferedOff();
		throw Herbs::ExceptionMissing(___FILE__,__LINE__);
		}
	position+=n_written;
	
	auto header_copy=header;
	if(!Herbs::CPUInfo::bigEndianIs())
		{
		header_copy.format=__builtin_bswap16(header.format);
		header_copy.n_tracks=__builtin_bswap16(header.n_tracks);
		header_copy.time_division=__builtin_bswap16(header.time_division);
		}
	
	n_written=m_dest.write(&header_copy,sizeof(header_copy));
	if(n_written != sizeof(header_copy))
		{
		m_dest.modeBufferedOff();
		throw Herbs::ExceptionMissing(___FILE__,__LINE__);
		}
	position+=n_written;
	}

MIDISeq::ChunkWriter::~ChunkWriter()
	{
	m_dest.modeBufferedOff();
	}

bool MIDISeq::ChunkWriter::headerWrite(const Herbs::Stringbase<char>& name)
	{
	unsigned int k=0;
	auto ptr_name=name.begin();
	while(ptr_name!=name.end() && k!=4)
		{
		m_dest.bytePut(*ptr_name);
		++k;
		++ptr_name;
		++position;
		}

	size_chunk=0;
	offset_chunk=position;

	auto n_written=m_dest.write(&size_chunk,sizeof(size_chunk));
	if(n_written != sizeof(size_chunk))
		{return 0;}
	position+=n_written;
	return 1;
	}

size_t MIDISeq::ChunkWriter::dataWrite(const void* buffer,size_t buffer_size)
	{
	auto n_written=m_dest.write(buffer,buffer_size);
	size_chunk+=n_written;
	position+=n_written;
	return n_written;
	}

bool MIDISeq::ChunkWriter::bytePut(uint8_t byte)
	{
	m_dest.bytePut(byte);
	++size_chunk;
	++position;
	return 1;
	}

bool MIDISeq::ChunkWriter::varfieldPut(size_t x)
	{
	size_t buffer=x&0x7f;
	while( x>>=7 )
		{
		buffer<<=8;
		buffer|=( (x&0x7f) | 0x80 );
		}
	
	while(1)
		{
		if(!bytePut(buffer&0xff))
			{return 0;}
		if(buffer & 0x80)
			{buffer>>=8;}
		else
			{return 1;}
		}
	}
	
bool MIDISeq::ChunkWriter::chunkEnd()
	{
	m_dest.seek(offset_chunk);
	if(!Herbs::CPUInfo::bigEndianIs())
		{size_chunk=__builtin_bswap32(size_chunk);}
	m_dest.write(&size_chunk,sizeof(size_chunk));
	m_dest.seek(position);
	return 1;
	}
