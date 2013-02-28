/*
 * dex/file.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

extern int dvt_parse_dex(struct dvt *dvt, struct dex_file *file);

int dvt_dex_load_file(struct dvt *dvt, const char *path)
{
	struct dex_file *file;
	struct stat stat;
	int fd, rc;
	
	file = dvt_alloc(sizeof(*file), DVT_ALLOC_NONE);
	if (!file) {
		err("dex: unable to allocate memory for file descriptor\n");
		return -1;
	}

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		dvt_free(file);
		err("dex: unable to open input file\n");
		return -1;
	}
	
	rc = fstat(fd, &stat);
	if (rc < 0) {
		dvt_free(file);
		close(fd);
		err("dex: unable to stat input file\n");
		return -1;
	}
	
	file->raw.size = stat.st_size;
	file->raw.base = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	
	if (!file->raw.base) {
		dvt_free(file);
		err("dex: unable to mmap input file\n");
		return -1;
	}
	
	rc = dvt_parse_dex(dvt, file);
	if (rc) {
		munmap(file->raw.base, file->raw.size);
		dvt_free(file);
		err("dex: unable to parse input file\n");
		return -1;
	}
	
	return 0;
}

