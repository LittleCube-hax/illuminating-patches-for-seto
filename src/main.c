#include <stdio.h>
#include <stdlib.h>

#include <ips.h>
#include <drivers.h>

int main()
{
	IPSRecord r;
	r.offset = 25165824;
	r.size = 3;
	
	//~ printf("offset: 0x%06X, size: 0x%04X\n", r.offset, r.size);
	
	IPSRecord* records = load_ips("rpge.ips");
	if (records == NULL)
	{
		fprintf(stderr, "could not find `rpge.ips', quitting\n");
		return -1;
	}
	
	//~ create_folder("rpge");
	
	close_ips(records);
	
	return 0;
}