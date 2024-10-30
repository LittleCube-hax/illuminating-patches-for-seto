#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ips.h>
#include <drivers.h>

#define BE_CVT_3(x) (((x & 0xFF) << 16) | (x & 0xFF00) | ((x & 0xFF0000) >> 16))
#define BE_CVT_2(x) (((x & 0xFF) << 8) | ((x & 0xFF00) >> 8))

#define READ_3(buf, off) BE_CVT_3(*((u32*) (ips_buffer + buffer_i)))
#define READ_2(buf, off) BE_CVT_2(*((u16*) (ips_buffer + buffer_i)))

IPSRecord* load_ips(const char* ips_path)
{
	char* ips_buffer = read_file_buffer(ips_path);
	
	if (ips_buffer == NULL)
	{
		return NULL;
	}
	
	long filelen = get_file_length(ips_path);
	
	IPSRecord* records = malloc(sizeof(IPSRecord));
	
	size_t len = 1;
	
	size_t records_i = 0;
	
	IPSRecord* record;
	
	size_t buffer_i = 5;
	
	while (buffer_i < filelen)
	{
		printf("at pos %zu (0x%08zX)\n", buffer_i, buffer_i);
		
		record = &records[records_i];
		
		record->offset = READ_3(ips_buffer, buffer_i);
		buffer_i += 3;
		
		if (record->offset == 0x454F46)
		{
			printf("\ngot eof\n");
			break;
		}
		
		printf("offset: 0x%06X\n", record->offset);
		
		record->size = READ_2(ips_buffer, buffer_i);
		buffer_i += 2;
		
		printf("size: 0x%04X\n", record->size);
		
		if (record->size == 0)
		{
			record->rle = true;
			
			record->size = READ_2(ips_buffer, buffer_i);
			buffer_i += 2;
			
			printf("RLE size: 0x%04X\n", record->size);
			
			record->data = malloc(1);
			record->data[0] = ips_buffer[buffer_i];
			buffer_i += 1;
			
			printf("data: %02X", record->data[0]);
			
			if (record->size == 0)
			{
				printf("\nRLE with size 0 is broken (silly rpge), skipping...");
				continue;
			}
		}
		
		else
		{
			record->data = malloc(record->size);
			memcpy(record->data, &ips_buffer[buffer_i], record->size);
			buffer_i += record->size;
			
			printf("data: ");
			
			for (size_t i = 0; i < record->size; ++i)
			{
				printf("%02X ", (u8) record->data[i]);
			}
		}
		
		printf("\n\n");
		
		records_i += 1;
		
		if (records_i >= len)
		{
			len <<= 1;
			IPSRecord* new_records = malloc(len*sizeof(IPSRecord));
			memcpy(new_records, records, (len >> 1)*sizeof(IPSRecord));
			free(records);
			records = new_records;
		}
	}
	
	printf("ended at pos %zu (0x%08zX)\n", buffer_i, buffer_i);
	
	records[records_i].offset = 0x1000000;
	
	return records;
}

void close_ips(IPSRecord* records)
{
	size_t records_i = 0;
	
	while (records[records_i].offset < 0x1000000)
	{
		free(records[records_i].data);
		records_i += 1;
	}
	
	free(records);
}