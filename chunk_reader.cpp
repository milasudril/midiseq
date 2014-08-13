#ifdef __WAND__
target[name[chunk_reader.o] type[object]]
#endif

#include "chunk_reader.h"
#include "file_header.h"
#include <herbs/exceptionmissing/exceptionmissing.h>
#include <herbs/streamin/streamin.h>
#include <herbs/cpuinfo/cpuinfo.h>
#include <cstring>

MIDISeq::ChunkReader::ChunkReader(Herbs::StreamIn& source
	,FileHeader& header):
	m_source(source)
	{
	m_source.modeBufferedOn();
	auto n_eaten=m_source.offsetGet();
	char buffer_magic[4];
	unsigned int k=0;
	while(!m_source.eof() && k!=4)
		{
		buffer_magic[k]=m_source.byteGet();
		++k;
		}

	uint32_t size_header=0;
	auto n_read=m_source.read(&size_header,sizeof(size_header));
	if(!Herbs::CPUInfo::bigEndianIs())
		{size_header=__builtin_bswap32(size_header);}

	if(strncmp(buffer_magic,"MThd",4) || size_header!=6
		|| n_read!=sizeof(size_header))
		{
		m_source.offsetRestore(n_eaten);
		m_source.modeBufferedOff();
		throw Herbs::ExceptionMissing(___FILE__,__LINE__);
		}

	auto n_read_0=std::min(sizeof(header),(decltype(sizeof(header)))size_header);
	n_read=m_source.read(&header,n_read_0);
	if(n_read!=n_read_0)
		{throw Herbs::ExceptionMissing(___FILE__,__LINE__);}
	if(!Herbs::CPUInfo::bigEndianIs())
		{
		header.format=__builtin_bswap16(header.format);
		header.n_tracks=__builtin_bswap16(header.n_tracks);
		header.time_division=__builtin_bswap16(header.time_division);
		}
	}

MIDISeq::ChunkReader::~ChunkReader()
	{
	m_source.modeBufferedOff();
	}

bool MIDISeq::ChunkReader::headerRead(Herbs::Stringbase<char>& name)
	{
	name.clear();
	unsigned int k=0;
	while(!m_source.eof() && k!=4)
		{
		name.append(m_source.byteGet() );
		++k;
		}
	if(k!=4)
		{return 0;}

	uint32_t size_chunk_tmp;
	auto n_read=m_source.read(&size_chunk_tmp,sizeof(size_chunk_tmp));
	if(n_read!=sizeof(size_chunk_tmp))
		{return 0;}

	if(Herbs::CPUInfo::bigEndianIs())
		{size_chunk=size_chunk_tmp;}
	else
		{size_chunk=__builtin_bswap32(size_chunk_tmp);}

	return 1;
	}

size_t MIDISeq::ChunkReader::dataRead(void* buffer,size_t buffer_size)
	{
	auto n_read=m_source.read(buffer,std::min(buffer_size,size_chunk));
	size_chunk-=n_read;
	return n_read;
	}

bool MIDISeq::ChunkReader::skip()
	{
	if(size_chunk==0)
		{return 0;}
	uint8_t buffer[4096];
	while(size_chunk!=0)
		{
		auto n_read=std::min(size_chunk,(decltype(size_chunk))4096);
		auto ret=m_source.read(buffer,n_read);
		if(ret!=n_read)
			{return 0;}
		size_chunk-=ret;
		}
	return 1;
	}

bool MIDISeq::ChunkReader::skip(size_t n_bytes)
	{
	if(size_chunk==0)
		{return 0;}
	uint8_t buffer[4096];
	while(n_bytes!=0 && size_chunk!=0)
		{
		auto n_read=std::min(n_bytes
			,std::min(size_chunk,(decltype(size_chunk))4096));
		auto ret=m_source.read(buffer,n_read);
		if(ret!=n_read)
			{return 0;}
		size_chunk-=ret;
		n_bytes-=ret;
		}
	return 1;
	}

uint8_t MIDISeq::ChunkReader::byteGet()
	{
	uint8_t ret=m_source.byteGet();
	--size_chunk;
	return ret;
	}

size_t MIDISeq::ChunkReader::varfieldGet()
	{
	size_t ret=0;
	while(!m_source.eof() && !eoc())
		{
		uint8_t in=m_source.byteGet();
		--size_chunk;
		ret = (ret << 7) | (size_t)(in & 0x7f);
		if( !( in & 0x80) )
			{return ret;}
		}
	return ret;
	}

