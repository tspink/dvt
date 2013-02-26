#ifndef __DVT_H__
#define __DVT_H__

#include <stdlib.h>

struct memv {
	void *base;
	unsigned int size;
};

struct dvt {
	struct memv raw;
	int mapped;
};

enum dvt_alloc_flags {
	DVT_ALLOC_NONE		= 0x0,
	DVT_ALLOC_ZERO		= 0x1,
};

extern void *dvt_alloc(unsigned int size, enum dvt_alloc_flags flags);
extern void dvt_free(void *ptr);

extern struct dvt *dvt_create_context_file(const char *file);
extern struct dvt *dvt_create_context_memory(void *base, unsigned int size);
extern void dvt_destroy(struct dvt *dvt);

#endif
