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

struct ll {
	struct ll *next;
	struct ll *prev;
	void *p;
};

struct dex_file {
	struct memv raw;
};

struct dvt_class;
struct dvt {
	struct dex_file *files;
	int nr_files;

	struct ll *classes;
};

enum dvt_alloc_flags {
	DVT_ALLOC_NONE		= 0x0,
	DVT_ALLOC_ZERO		= 0x1,
};

extern void *dvt_alloc(unsigned int size, enum dvt_alloc_flags flags);
extern void dvt_free(void *ptr);

static inline void ll_insert_head(struct ll **head, void *ent_p)
{
	struct ll *head_ent = *head;
	struct ll *ent;

	ent = dvt_alloc(sizeof(struct ll), DVT_ALLOC_NONE);
	if (!ent) {
		return;
	}

	if (head_ent)
		head_ent->prev = ent;

	ent->next = head_ent;
	ent->prev = NULL;
	ent->p = ent_p;

	*head = ent;
}

#define foreach(v, coll) struct ll *__iter_##v = coll; \
	for (__iter_##v = coll, \
		v = (__iter_##v ? __iter_##v->p : NULL); \
		__iter_##v; \
		__iter_##v = __iter_##v->next, \
		v = (__iter_##v ? __iter_##v->p : NULL))

extern struct dvt *dvt_create_context(void);
extern void dvt_destroy(struct dvt *dvt);

extern int dvt_dex_load_file(struct dvt *dvt, const char *path);

struct dvt_method;
struct dvt_class {
	struct dvt *ctx;
	struct dvt_class *base;
	char *name;

	struct ll *methods;
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
