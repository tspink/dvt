/*
 * mem.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>
#include <malloc.h>

void *dvt_alloc(unsigned int size, enum dvt_alloc_flags flags)
{
	if (flags & DVT_ALLOC_ZERO) {
		return calloc(1, size);
	} else {
		return malloc(size);
	}
}

void dvt_free(void *ptr)
{
	free(ptr);
}
