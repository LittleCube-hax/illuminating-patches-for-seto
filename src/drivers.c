#include <stdio.h>
#include <stdlib.h>

#include <drivers.h>

#if defined(_MSC_VER)
//  Microsoft

#include <windows.h>
#include <fileapi.h>
#include <tchar.h>

int create_folder(const char* path)
{
	return CreateDirectory(path, NULL);
}

char* read_file_buffer(const char* path, long* filelen_out)
{
	HANDLE file_handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	
	*filelen_out = GetFileSize(file_handle, NULL);
	DWORD nread = 1;
	char* file_buffer = malloc(*filelen_out + 1);
	size_t bufpos = 0;
	
	while (nread > 0)
	{
		ReadFile(file_handle, &file_buffer[bufpos], *filelen_out, &nread, NULL);
		bufpos += nread;
	}
	
	CloseHandle(file_handle);
	
	return file_buffer;
}

#elif defined(__GNUC__)
// GCC

#include <sys/stat.h>
#include <sys/types.h>

int create_folder(const char* path)
{
	return mkdir(path, 0777) == 0;
}

char* read_file_buffer(const char* path, long* filelen_out)
{
	FILE* file = fopen(path, "r");
	
	if (file == NULL)
	{
		return NULL;
	}
	
	if (filelen_out != NULL)
	{
		fseek(file, 0, SEEK_END);
		*filelen_out = ftell(file);
		rewind(file);
	}
	
	char* file_buffer = malloc(*filelen_out);
	size_t nread;
	size_t bufpos = 0;
	
	while ((nread = fread(&file_buffer[bufpos], 1, *filelen_out, file)) > 0)
	{
		bufpos += nread;
	}
	
	fclose(file);
	
	return file_buffer;
}

#endif