/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#ifndef BIB_H
#define BIB_H

struct bib_value {
	char *text;

	struct bib_value *next;
};

struct bib_field {
	char *name;
	struct bib_value *value;

	struct bib_field *next;
};

struct bib_entry {
	char *type;
	char *key;

	struct bib_field *field;

	char *zim;
	char *bib;

	struct bib_entry *next;
};

struct bib_value *
bib_new_value(char *text, int normalisecase);

struct bib_field *
bib_new_field(char *name, struct bib_value *value);

struct bib_entry *
bib_new_entry(char *type, char *key, struct bib_field *field);

#endif

