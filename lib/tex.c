#include <assert.h>
#include <string.h>
#include <ctype.h>

#include <bib/bib.h>
#include <bib/tex.h>

#define WHITE " \n\t\r\v\f"

static const struct {
	const char *from;
	const char *to;
} a[] = {
	{ "---", "\xE2\x80\x94" },
	{ "--",  "\xE2\x80\x93" },
	{ "``",  "\xE2\x80\x9C" },
	{ "''",  "\xE2\x80\x9D" },
	{ "`",   "\xE2\x80\x98" },
	{ "'",   "\xE2\x80\x99" },
	{ "~",   "\xC2\xA0"     },

/* TODO: flag for {"} vs \"
	{ "\\\\", "\\" },
	{ "\\'",  "'"  },
	{ "\\\"", "\"" },
*/
	{ "\\$",  "$"  },
	{ "\\{",  "{"  },
	{ "\\}",  "}"  },

	/* TODO: more */
	{ "\\'e",  "\xC3\xA9" },
	{ "\\\"e", "\xC3\xAB" },
	{ "\\\"u", "\xC3\xBC" }
};

char *
tex_escape(char *dst, const char *src, int normalisecase)
{
	const char *p;
	char *q;
	int depth;
	size_t i;

	assert(src != NULL);
	assert(dst != NULL);

	depth = 0;

	p = src;
	q = dst;

	while (*p != '\0') {
		if (isspace((unsigned char) *p)) {
			p += strspn(p, WHITE);
			if (q != dst && *p != '\0') {
				*q++ = ' ';
			}
			continue;
		}

		if (*p == '{') { depth++; p++; continue; }
		if (*p == '}') { depth--; p++; continue; }

		/* TODO: state for $$ math mode */

		for (i = 0; i < sizeof a / sizeof *a; i++) {
			if (0 == strncmp(p, a[i].from, strlen(a[i].from))) {
				strcpy(q, a[i].to);
				p += strlen(a[i].from);
				q += strlen(a[i].to);
				break;
			}
		}

		if (i < sizeof a / sizeof *a) {
			continue;
		}

		if (normalisecase) {
			if (q == dst) {
				*q = toupper(*p);
			} else if (depth == 0) {
				*q = tolower(*p);
			} else {
				*q = *p;
			}
		} else {
			*q = *p;
		}

		p++;
		q++;
	}

	*q = '\0';

	return dst;
}

char *
tex_delim(const char *s, const char *delim)
{
	const char *p;
	int depth;
	size_t i;

	assert(s != NULL);
	assert(delim != NULL);

	depth = 0;

	p = s;

	while (*p != '\0') {
		if (*p == '{') { depth++; p++; continue; }
		if (*p == '}') { depth--; p++; continue; }
		if (*p == '$') { depth++; p++; continue; }
		if (*p == '$') { depth--; p++; continue; }

		for (i = 0; i < sizeof a / sizeof *a; i++) {
			if (0 == strncmp(p, a[i].from, strlen(a[i].from))) {
				p += strlen(a[i].from);
				break;
			}
		}

		if (i < sizeof a / sizeof *a) {
			continue;
		}

		if (depth == 0) {
			if (0 == strncmp(p, delim, strlen(delim))) {
				return p;
			}
		}

		p++;
	}

	return NULL;
}
