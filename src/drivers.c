#include <stdio.h>
#include <stdlib.h>

#include <drivers.h>

#if defined(_MSC_VER)
//  Microsoft

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <fileapi.h>
#include <tchar.h>

int create_folder(const char* path)
{
	return CreateDirectory(path, NULL);
}

long get_file_length(const char* path)
{
	HANDLE file_handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD filelen = GetFileSize(file_handle, NULL);
	CloseHandle(file_handle);
	
	return (long) filelen;
}

char* read_file_buffer(const char* path)
{
	HANDLE file_handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	
	DWORD filelen = GetFileSize(file_handle, NULL);
	DWORD nread = 1;
	char* file_buffer = malloc(filelen + 1);
	size_t bufpos = 0;
	
	while (nread > 0)
	{
		ReadFile(file_handle, &file_buffer[bufpos], filelen, &nread, NULL);
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

long get_file_length(const char* path)
{
	FILE* file = fopen(path, "r");
	
	fseek(file, 0, SEEK_END);
	long filelen = ftell(file);
	
	fclose(file);
	
	return filelen;
}

char* read_file_buffer(const char* path)
{
	FILE* file = fopen(path, "r");
	
	if (file == NULL)
	{
		return NULL;
	}
	
	fseek(file, 0, SEEK_END);
	long filelen = ftell(file);
	rewind(file);
	
	char* file_buffer = malloc(filelen);
	size_t nread;
	size_t bufpos = 0;
	
	while ((nread = fread(&file_buffer[bufpos], 1, filelen, file)) > 0)
	{
		bufpos += nread;
	}
	
	fclose(file);
	
	return file_buffer;
}

#endif