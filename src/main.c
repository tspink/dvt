/*
 * main.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 * 
 */
#include <stdio.h>
#include <dvt.h>

int main(int argc, char **argv)
{
	struct dvt *context;
	int rc = 0;
	
	printf("DVT - Dalvik Translator\n");
	printf("Copyright (C) University of Edinburgh 2013\n\n");
	
	if (argc != 2) {
		err("usage: %s <dex-file>\n", argv[0]);
		return -1;
	}
	
	dbg("creating context...\n");
	context = dvt_create_context_file(argv[1]);
	if (!context) {
		err("unable to create dvt context\n");
		return -1;
	}
	
	dbg("parsing dex file...\n");
	rc = dvt_parse_dex(context);
	if (rc) {
		err("error whilst parsing dex file\n");
	}
	
	dbg("destroying context...\n");
	dvt_destroy(context);
	return rc;
}
