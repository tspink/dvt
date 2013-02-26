/*
 * context.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

struct dvt *dvt_create_context_file(const char *file)
{
	struct stat stat;
	struct dvt *dvt;
	void *base;
	int fd, rc;
	
	fd = open(file, O_RDONLY);
	if (fd < 0) {
		return NULL;
	}
	
	rc = fstat(fd, &stat);
	if (rc < 0) {
		close(fd);
		return NULL;
	}
	
	base = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	
	if (!base) {
		return NULL;
	}
	
	dvt = dvt_create_context_memory(base, stat.st_size);
	if (dvt) {
		dvt->mapped = 1;
	}
	
	return dvt;
}

struct dvt *dvt_create_context_memory(void *base, unsigned int size)
{
	struct dvt *dvt;
	
	dvt = dvt_alloc(sizeof(*dvt), DVT_ALLOC_NONE);
	if (!dvt)
		return NULL;
	
	dvt->raw.base = base;
	dvt->raw.size = size;
	dvt->mapped = 0;
	
	return dvt;
}

void dvt_destroy(struct dvt *dvt)
{
	if (dvt->mapped) {
		munmap(dvt->raw.base, dvt->raw.size);
	}
	
	free(dvt);
}