/*
 * garbagefinder - Recursively searches current directory for files containing
 * only the null character (0x00) and reports those files' names to stdout.
 * 
 *
 * I created this quick hack after I had a hard drive crash. At first glance, I
 * was able to recover all files. But many of them turned out to be full of 0x00
 * and nothing else. So I wrote this tool to recover all the lost space on my new
 * disk, because storing gigabytes worth of zeroes is somewhat pointless.
 * 
 * Just make it and then run it from any directory. It will search the files in
 * the current directory and all subdirectories recursively, and print out one
 * line per garbage file found, relative to the current directory.
 * 
 *
 * Copyright (C) 2010 Crazor <crazor@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

void scan_dir(char *name);

int main(int argc, char argv[])
{

	
	scan_dir(".");
		
	return 0;
}

void scan_dir(char name[])
{
	DIR *d;
	struct dirent *e;
	FILE *f;
	bool garbage;
	int c;
	int size;
	
	if (chdir(name) != 0)
	{
	   fprintf(stderr, "Failed to chdir() to %s!\n", name);
	   return;
	}

	d = opendir(".");
	if (d == NULL)
	{
		fprintf(stderr, "Failed to opendir() %s!\n", getwd(NULL));
		return;
	}

	while (e = readdir(d))
	{
		if (e->d_type == DT_REG)
		{
			f = fopen(e->d_name, "rb");
			if (f == NULL)
			{
				fprintf(stderr, "Failed to fopen %s/%s\n", getwd(NULL), e->d_name);
			}
			else
			{
				garbage = true;
				size = 0;

				while(true)
				{
					c = fgetc(f);
					if (c != EOF)
					{
						size++;
						if (c != '\0')
						{
							garbage = false;
							break;
						}
					}
					else
					{
						break;
					}
				}

				if (garbage == true && size > 0)
				{
					printf("%s/%s\n", getwd(NULL), e->d_name);
				}

				fclose(f);
			}
		}
		if (e->d_type == DT_DIR)
		{
			if (strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0)
			{
				scan_dir(e->d_name);
			}
		}
	}

	closedir(d);
	if (chdir("..") != 0)
	{
		fprintf(stderr, "Failed to chdir() to ..!\n");
	}
}
