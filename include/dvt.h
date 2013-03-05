#ifndef __DVT_H__
#define __DVT_H__

#include <stdio.h>

#define dbg(a...) fprintf(stderr, "debug: " a)
#define info(a...) fprintf(stderr, "info: " a)
#define warn(a...) fprintf(stderr, "warning: " a)
#define err(a...) fprintf(stderr, "error: " a)

struct memv {
	void *base;
	unsigned int size;
};

struct dex_file {
	struct memv raw;
};

struct dvt {
	struct dex_file *files;
	int nr_files;
};

enum dvt_alloc_flags {
	DVT_ALLOC_NONE		= 0x0,
	DVT_ALLOC_ZERO		= 0x1,
};

extern void *dvt_alloc(unsigned int size, enum dvt_alloc_flags flags);
extern void dvt_free(void *ptr);

extern struct dvt *dvt_create_context(void);
extern void dvt_destroy(struct dvt *dvt);

extern int dvt_dex_load_file(struct dvt *dvt, const char *path);

struct dvt_class {
	struct dvt *ctx;
};

struct dvt_method {
	struct dvt_class *class;
};

typedef enum _find_flags {
	F_NONE			= 0,
	F_STATIC		= 1,
	F_PUBLIC		= 2,
	F_PRIVATE		= 4,
} find_flags_t;

extern struct dvt_class *dvt_find_class(struct dvt *dvt, const char *name);
extern struct dvt_method *dvt_find_method(struct dvt_class *class, const char *sig, find_flags_t flags);
extern void *dvt_invoke(struct dvt_method *method, void **args);

#endif
