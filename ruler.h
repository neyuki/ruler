#ifndef __RULER_H
#define __RULER_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE
#endif

#include <xcb/xcb_ewmh.h>
#include <regex.h>

#define ATOM_WM_ROLE "WM_WINDOW_ROLE"
#define WINDOW_TYPE_STRING_LENGTH 110
#define REGEX_FLAGS REG_EXTENDED | REG_NOSUB

typedef char * command_t;

enum criterion {
	CRIT_CLASS,
	CRIT_INSTANCE,
	CRIT_TYPE,
	CRIT_NAME,
	CRIT_ROLE
};

struct descriptor {
	enum criterion criterion;
	char *str;
	regex_t *reg;
};

struct list {
	void *n;
	struct list *next;
	struct list *prev;
};

struct block {
	/* list of descriptors */
	struct list *d;
	command_t c;
};

struct win_props {
	char *class;
	char *instance;
	char *type;
	char *name;
	char *role;
};

void yyerror(const char *);
int yywrap(void);
int yylex(void);
int yyparse(void);

void print_usage(const char *);
char * strip_quotes(char *);

struct descriptor * new_descriptor(char *, char *);
void desc(char *, char *);
void descriptor_free(struct descriptor *);

void list_add(struct list **, void *node);
void list_free(struct list **);

command_t new_command(char *);
void comm(char *);

struct block * new_block(struct list *, command_t);
void block(void);
void block_free(struct block *);

struct win_props * new_win_props(void);
void free_win_props(struct win_props *);
void print_win_props(struct win_props *);

void init_ewmh(void);
xcb_atom_t get_atom(const char *);

char * window_type_to_string(xcb_ewmh_get_atoms_reply_t *);
char * get_string_prop(xcb_window_t, xcb_atom_t, int);

struct win_props * get_props(xcb_window_t);
int match_props(struct win_props *, struct list *);
struct block * find_matching_block(struct win_props *, struct list *);

void register_events(void);
void handle_events(void);

void cleanup(void);

#endif
