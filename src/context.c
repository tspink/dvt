/*
 * context.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>

struct dvt *dvt_create_context()
{
	struct dvt *dvt;
	
	dvt = dvt_alloc(sizeof(*dvt), DVT_ALLOC_ZERO);
	if (!dvt) {
		err("context: unable to allocate storage for context object\n");
		return NULL;
	}
	
	return dvt;
}

void dvt_destroy(struct dvt *dvt)
{
	dvt_free(dvt);
}
