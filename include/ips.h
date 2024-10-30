#pragma once

#include <stdbool.h>

#include <common.h>

typedef struct
{
	u32 offset;
	u16 size;
	u8* data;
	bool rle;
} IPSRecord;

IPSRecord* load_ips(const char* path);
void close_ips(IPSRecord* records);