#pragma once

#include <common.h>

typedef struct
{
	u32 offset;
	u16 size;
	u8* data;
} IPSRecord;

IPSRecord* load_ips(const char* path);
void close_ips(IPSRecord* records);