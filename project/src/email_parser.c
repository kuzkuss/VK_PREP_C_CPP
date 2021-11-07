#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "email_parser.h"

typedef enum {
    L_EMPTY_STR,
    L_SPACE,
    L_TITLE_TO,
    L_TITLE_FROM,
    L_TITLE_DATE,
    L_TITLE_CONTENT_TYPE,
    L_OTHER_TITLE,
    L_COUNT
} lexem_t;

typedef enum {
    S_BEGIN,
    S_TITLE,
    S_VALUE,
    S_COUNT,
    S_END,
    S_ERR
} state_t;

typedef int (*action_t)(const char *s, char **src_str);

typedef struct {
    state_t state;
    action_t action;
} rule_t;

static lexem_t get_lexem(char *s, size_t len);
static int get_value(const char *s, char **src_str);
static int save_data(const char *s, char **src_str);
static int is_multipart(char *s, char **boundary);
static void count_parts(FILE *f, char *type, char **boundary, char *str, int *parts);
static char *get_line(FILE *f);


static rule_t syntax[S_COUNT][L_COUNT] = {
//		   L_EMPTY_STR, L_SPACE, L_TITLE_TO, L_TITLE_FROM, L_TITLE_DATE, L_TITLE_CONTENT_TYPE, L_OTHER_TITLE
/*S_BEGIN*/	{{S_ERR, NULL},	 {S_ERR, NULL},	 {S_TITLE, NULL},  {S_TITLE, NULL},
                       {S_TITLE, NULL},           {S_TITLE, NULL},	         {S_TITLE, NULL}},
/*S_TITLE*/	{{S_END, NULL},  {S_ERR, NULL},  {S_VALUE, get_value},  {S_VALUE, get_value},
                       {S_VALUE, get_value},      {S_VALUE, get_value},     {S_VALUE, NULL}},
/*S_VALUE*/	{{S_END, NULL},   {S_VALUE, save_data},	 {S_TITLE, NULL},  {S_TITLE, NULL},
                       {S_TITLE, NULL},           {S_TITLE, NULL},	         {S_TITLE, NULL}},
};

static lexem_t get_lexem(char *s, size_t len) {
    if (len == 0)
        return L_EMPTY_STR;
    if (isspace(s[0]))
        return L_SPACE;

    int i = 0;
    while (s[i] != ':') {
        s[i] = tolower(s[i]);
        i++;
    }

    if (strstr(s, "to") == s)
        return L_TITLE_TO;
    else if (strstr(s, "from") == s)
        return L_TITLE_FROM;
    else if (strstr(s, "date") == s)
        return L_TITLE_DATE;
    else if (strstr(s, "content-type") == s)
        return L_TITLE_CONTENT_TYPE;
    else
        return L_OTHER_TITLE;
}

static int get_value(const char *s, char **src_str) {
    char *begin_str = strchr(s, ':');
    if (!begin_str)
        return ERROR;
    begin_str++;
    while (isspace(*begin_str))
        begin_str++;
    size_t len = strlen(begin_str);
    *src_str = malloc((len + 1) * sizeof(char));
    if (*src_str)
        strncpy(*src_str, begin_str, len + 1);
    return OK;
}

static int save_data(const char *s, char **src_str) {
    while (isspace(*s))
        s++;
    s--;
    size_t len = strlen(s) + 1;
    char *tmp = realloc(*src_str, (strlen(*src_str) + len) * sizeof(char));
    if (tmp) {
        strncat(tmp, s, len);
        *src_str = tmp;
        return OK;
    }
    return ERROR;
}

static int is_multipart(char *s, char **boundary) {
    int index = 0;
    while (s[index] != '=' && s[index]) {
        s[index] = tolower(s[index]);
        index++;
    }
    if (strstr(s, "multipart")) {
        *boundary = strstr(s, "boundary");
        if (*boundary && (isspace(*(*boundary - 1)) || *(*boundary - 1) == ';')
            && (*(*boundary + 8) == '=')) {
            *boundary = *boundary + 9;
            if (**boundary == '"') {
                ++(*boundary);
                int i = 0;
                while ((*boundary)[i] != '"')
                    ++i;
                (*boundary)[i] = '\0';
            } else {
                int i = 0;
                while (!isspace((*boundary)[i]) && (*boundary)[i])
                    ++i;
                (*boundary)[i] = '\0';
            }
            *boundary -= 2;
            (*boundary)[0] = '-';
            (*boundary)[1] = '-';
            return TRUE;
        } else {
            return FALSE;
        }
    }
    return FALSE;
}

static void count_parts(FILE *f, char *type, char **boundary, char *str, int *parts) {
    if (!type) {
        *parts = 1;
        return;
    }
    if (is_multipart(type, boundary)) {
        int count_not_empty = 0;
        str = get_line(f);
        while (str) {
            size_t len = strlen(str);
            while ((str[len - 1] == '\n' || str[len - 1] == '\r')) {
                str[len - 1] = '\0';
                if (--len == 0)
                    break;
            }
            if (len > 0) {
                count_not_empty++;
                if (strcmp(str, *boundary) == 0)
                    (*parts)++;
            }
            free(str);
            str = get_line(f);
        }
        if (count_not_empty == 0)
            *parts = 0;
    } else {
        *parts = 1;
    }
}

static char *get_line(FILE *f) {
    int size = BUF_SIZE;
    char *buf = malloc(BUF_SIZE);
    if (!buf)
        return NULL;
    if (!fgets(buf, BUF_SIZE, f)) {
        free(buf);
        return NULL;
    }
    size_t len = strlen(buf);
    while (!feof(f) && buf[len - 1] != '\n') {
        size += BUF_SIZE;
        char *tmp = realloc(buf, size);
        if (!tmp) {
            free(buf);
            return NULL;
        }
        buf = tmp;
        if (!fgets(buf + len, BUF_SIZE, f)) {
            free(buf);
            return NULL;
        }
        len = strlen(buf);
    }
    return buf;
}

int parse(FILE *f, char **from, char **to, char **date, int *parts) {
    state_t state = S_BEGIN;
    char *str = NULL;
    char *type = NULL;
    char *boundary = NULL;
    int rc = OK;
    if (!(str = get_line(f)))
        return ERROR;
    size_t len = strlen(str);
    while ((str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        if (--len == 0)
            break;
    }
    lexem_t last_lexem = -1;
    while (!feof(f)) {
        lexem_t lexem = get_lexem(str, len);

        if (state == S_TITLE)
            last_lexem = lexem;

        rule_t rule = syntax[state][lexem];
        if (rule.state == S_ERR) {
            free(str);
            return ERROR;
        }
        if (rule.action != NULL) {
            if ((lexem == L_TITLE_TO) || (lexem == L_SPACE && last_lexem == L_TITLE_TO))
                rc = rule.action(str, to);
            else if ((lexem == L_TITLE_FROM) || (lexem == L_SPACE && last_lexem == L_TITLE_FROM))
                rc = rule.action(str, from);
            else if ((lexem == L_TITLE_DATE) || (lexem == L_SPACE && last_lexem == L_TITLE_DATE))
                rc = rule.action(str, date);
            else if ((lexem == L_TITLE_CONTENT_TYPE) ||
                     (lexem == L_SPACE && last_lexem == L_TITLE_CONTENT_TYPE))
                rc = rule.action(str, &type);
        }

        if (rc) {
            free(str);
            return ERROR;
        }

        state = rule.state;
        if (state == S_VALUE) {
            free(str);
            str = get_line(f);
            if (str) {
                len = strlen(str);
                while ((str[len - 1] == '\n' || str[len - 1] == '\r')) {
                    str[len - 1] = '\0';
                    if (--len == 0)
                        break;
                }
            }
        } else if (state == S_END) {
            break;
        }
    }
    count_parts(f, type, &boundary, str, parts);

    free(type);
    free(str);

    if (!(*from)) {
        *from = malloc(1);
        strncpy(*from, "\0", 1);
    }
    if (!(*to)) {
        *to = malloc(1);
        strncpy(*to, "\0", 1);
    }
    if (!(*date)) {
        *date = malloc(1);
        strncpy(*date, "\0", 1);
    }
    return OK;
}

