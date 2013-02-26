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
	
	printf("DVT - Dalvik Translator\n");
	printf("Copyright (C) University of Edinburgh 2013\n\n");
	
	if (argc != 2) {
		printf("usage: %s <dex-file>\n", argv[0]);
		return -1;
	}
	
	context = dvt_create_context_file(argv[1]);
	if (!context) {
		return -1;
	}
	
	dvt_destroy(context);
	return 0;
}
