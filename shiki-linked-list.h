#ifndef __SHIKI_LINKED_LIST__
#define __SHIKI_LINKED_LIST__

#include <stdint.h>

/* USER MODIFICATION PURPOSE START HERE */
struct shilink_custom_data{
    char *sl_key;
    char *sl_value;
};

typedef struct shilink_custom_data SHLinkCustomData;
/* USER MODIFICATION PURPOSE END HERE */

struct shilink_var{
    SHLinkCustomData sl_data;
    struct shilink_var *sh_next;
};

typedef struct shilink_var * SHLink;

long shilink_get_version(char *_version);
void shilink_view_version();

/* USER MODIFICATION PURPOSE START HERE */
void shilink_fill_data(SHLink *_target, SHLinkCustomData _data);
int8_t shilink_fill_custom_data(SHLinkCustomData *_data, char *_key, char *_value);
void shilink_free_custom_data(SHLinkCustomData *_data);
int8_t shilink_search_data_by_position(SHLink _target, char *_key, int8_t _pos, SHLinkCustomData *_data);
int8_t shilink_search_data_by_prev_cond(SHLink _target, char *_key, SHLinkCustomData *_prev_cond_data, SHLinkCustomData *_data);
/* USER MODIFICATION PURPOSE END HERE */

int8_t shilink_push(SHLink *_target, SHLinkCustomData _data);
int8_t shilink_append(SHLink *_target, SHLinkCustomData _data);
void shilink_print(SHLink _target);
void shilink_free(SHLink *_target);

#endif