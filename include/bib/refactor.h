#ifndef BIB_REFACTOR_H
#define BIB_REFACTOR_H

int
bib_split(struct bib_field *f, const char *delim);

void
bib_merge(struct bib_field *p);

void
bib_cull(struct bib_entry *e);

#endif
