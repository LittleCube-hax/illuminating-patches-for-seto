#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ips.h>
#include <drivers.h>

int main()
{
	IPSRecord* records = load_ips("rpge.ips");
	if (records == NULL)
	{
		fprintf(stderr, "could not find `rpge.ips', quitting\n");
		return -1;
	}
	
	long table_filelen;
	char* table_buffer = read_file_buffer("ff5.tbl", &table_filelen);
	if (table_buffer == NULL)
	{
		fprintf(stderr, "ff5.tbl not found\n");
		return -1;
	}
	
	long table_bufpos = 0;
	
	char map[0x100][5];
	char byte[3];
	unsigned char idx;
	
	while (table_bufpos < table_filelen)
	{
		byte[0] = table_buffer[table_bufpos];
		byte[1] = table_buffer[table_bufpos + 1];
		byte[2] = '\0';
		idx = strtol(byte, NULL, 16);
		table_bufpos += 2;
		
		long i = 0;
		char c = table_buffer[table_bufpos];
		
		while (c != '\r' && c != '\n' && c != '\0')
		{
			if (c == '=')
			{
				table_bufpos += 1;
			}
			
			else
			{
				map[idx][i] = c;
				
				i += 1;
			}
			
			c = table_buffer[table_bufpos + i];
		}
		
		map[idx][i] = '\0';
		table_bufpos += i + 2;
	}
	
	close_ips(records);
	
	return 0;
}