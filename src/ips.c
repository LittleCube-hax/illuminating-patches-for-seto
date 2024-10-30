#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ips.h>

#define BE_CVT_3(x) (((x & 0xFF) << 16) | (x & 0xFF00) | ((x & 0xFF0000) >> 16))
#define BE_CVT_2(x) ((x & 0xFF) | ((x & 0xFF00) >> 8))

IPSRecord* load_ips(const char* ips_path)
{
	FILE* ips_file = fopen(ips_path, "r");
	if (ips_file == NULL)
	{
		return NULL;
	}
	
	IPSRecord* records = malloc(sizeof(IPSRecord));
	
	size_t len = 1;
	
	// read past "PATCH"
	fseek(ips_file, 5, SEEK_SET);
	
	size_t records_i = 0;
	
	IPSRecord* record;
	
	while (!feof(ips_file))
	{
		record = &records[records_i];
		
		fread(&record->offset, 3, 1, ips_file);
		record->offset = BE_CVT_3(record->offset);
		
		printf("offset: 0x%06X\n", record->offset);
		
		fread(&record->size, 2, 1, ips_file);
		record->size = BE_CVT_2(record->size);
		
		printf("size: 0x%04X\n", record->size);
		
		record->data = malloc(record->size);
		
		fread(record->data, record->size, 1, ips_file);
		
		printf("data: ");
		
		for (size_t i = 0; i < record->size; ++i)
		{
			printf("%02X ", (u8) record->data[i]);
		}
		
		printf("\n");
		
		records_i += 1;
		
		if (records_i >= len)
		{
			len <<= 1;
			IPSRecord* new_records = malloc(len*sizeof(IPSRecord));
			memcpy(new_records, records, len >> 1);
			free(records);
			records = new_records;
		}
	}
	
	fclose(ips_file);
	
	records[records_i].offset = 0x1000000;
	
	return records;
}

void close_ips(IPSRecord* records)
{
	size_t records_i = 0;
	
	while (records[records_i].offset < 0x1000000)
	{
		free(records[records_i].data);
	}
	
	free(records);
}