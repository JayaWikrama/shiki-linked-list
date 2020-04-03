/*
    lib info    : SHIKI_LIB_GROUP - LINKED_LIST
    ver         : 1.00.20.03.24
    author      : Jaya Wikrama, S.T.
    e-mail      : jayawikrama89@gmail.com
    Copyright (c) 2020 HANA,. Jaya Wikrama
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "shiki-linked-list.h"

#define var_name(var) #var

#define SHILINK_VER "1.00.20.03.24"

int8_t debug_mode_status = 0;

static void shilink_debug(const char *function_name, char *debug_type, char *debug_msg, ...){
	if (debug_mode_status == 1 || strcmp(debug_type, "INFO") != 0){
        struct tm *d_tm;
        struct timeval tm_debug;
        uint16_t msec = 0;
	    va_list aptr;
		
	    gettimeofday(&tm_debug, NULL);
	    d_tm = localtime(&tm_debug.tv_sec);
        msec = tm_debug.tv_usec/1000;
	
	    char* tmp_debug_msg;
        tmp_debug_msg = (char *) malloc(256*sizeof(char));
        if (tmp_debug_msg == NULL){
            printf("%02d-%02d-%04d %02d:%02d:%02d.%03i ERROR: %s: failed to allocate debug variable memory",
             d_tm->tm_mday, d_tm->tm_mon+1, d_tm->tm_year+1900, d_tm->tm_hour, d_tm->tm_min, d_tm->tm_sec, msec, __func__
            );
            return;
        }
	    va_start(aptr, debug_msg);
	    vsprintf(tmp_debug_msg, debug_msg, aptr);
	    va_end(aptr);
        #ifdef __linux__
            if (strcmp(debug_type, "INFO")==0)
                printf("\033[1;32m%02d-%02d-%04d %02d:%02d:%02d.%03d\033[1;34m SHILINK\033[1;32m %s: %s: %s\033[0m",
                 d_tm->tm_mday, d_tm->tm_mon+1, d_tm->tm_year+1900, d_tm->tm_hour, d_tm->tm_min, d_tm->tm_sec,
                 msec, debug_type, function_name, tmp_debug_msg
                );
    	    else if (strcmp(debug_type, "WARNING")==0)
                printf("\033[1;33m%02d-%02d-%04d %02d:%02d:%02d.%03d\033[1;34m SHILINK\033[1;33m %s: %s: %s\033[0m",
                 d_tm->tm_mday, d_tm->tm_mon+1, d_tm->tm_year+1900, d_tm->tm_hour, d_tm->tm_min, d_tm->tm_sec,
                 msec, debug_type, function_name, tmp_debug_msg
                );
    	    else if (strcmp(debug_type, "ERROR")==0)
                printf("\033[1;31m%02d-%02d-%04d %02d:%02d:%02d.%03d\033[1;34m SHILINK\033[1;31m %s: %s: %s\033[0m",
                 d_tm->tm_mday, d_tm->tm_mon+1, d_tm->tm_year+1900, d_tm->tm_hour, d_tm->tm_min, d_tm->tm_sec,
                 msec, debug_type, function_name, tmp_debug_msg
                );
            else if (strcmp(debug_type, "CRITICAL")==0)
                printf("\033[1;31m%02d-%02d-%04d %02d:%02d:%02d.%03d\033[1;34m SHILINK\033[1;31m %s: %s: %s\033[0m",
                 d_tm->tm_mday, d_tm->tm_mon+1, d_tm->tm_year+1900, d_tm->tm_hour, d_tm->tm_min, d_tm->tm_sec,
                 msec, debug_type, function_name, tmp_debug_msg
                );
	    #else
            printf("%02d-%02d-%04d %02d:%02d:%02d.%03d %s: %s: %s",
             d_tm->tm_mday, d_tm->tm_mon+1, d_tm->tm_year+1900, d_tm->tm_hour, d_tm->tm_min, d_tm->tm_sec,
             msec, debug_type, function_name, tmp_debug_msg
            );
        #endif
        free(tmp_debug_msg);
        tmp_debug_msg = NULL;
    }
}

static void shilink_get_var_name_from_struct(char *_struct_name, char *_full_varname, char *_varname_result){
    uint8_t idx_start = 0;
    uint8_t idx_result = 0;
    idx_start = strlen(_struct_name);

    char result[strlen(_full_varname) - idx_start + 1];
    memset(result, 0x00, sizeof(result));

    do {
        if (_full_varname[idx_start] != '-' && _full_varname[idx_start] != '>' && _full_varname[idx_start] != '.'){
            result[idx_result] = _full_varname[idx_start]; 
            idx_result++;
        }
        idx_start++;
    } while (idx_start < strlen(_full_varname));
    strcpy(_varname_result, result);
}

long shilink_get_version(char *_version){
    strcpy(_version, SHILINK_VER);
    long version_in_long = 0;
    uint8_t idx_ver = 0;
    uint8_t multiplier = 10;
    while(idx_ver < 13){
        if(SHILINK_VER[idx_ver] != '.' && SHILINK_VER[idx_ver] != 0x00){
            if (version_in_long == 0){
                version_in_long = SHILINK_VER[idx_ver] - '0';
            }
            else{
                version_in_long = (version_in_long*multiplier) + (SHILINK_VER[idx_ver] - '0');
            }
        }
        else if (SHILINK_VER[idx_ver] == 0x00){
            break;
        }
        idx_ver++;
    }
    return version_in_long;
}

void shilink_view_version(){
    shilink_debug(__func__, "VERSION", "%s\n", SHILINK_VER);
}

/* USER MODIFICATION PURPOSE START HERE */
void shilink_fill_data(SHLink *_target, SHLinkCustomData _data){
    (*_target)->sl_data.sl_key = _data.sl_key;
    (*_target)->sl_data.sl_value = _data.sl_value;
}

static void shilink_print_data(SHLink _data){
    /*
    char var_key[strlen(var_name(_data->sl_data.sl_key)) - strlen(var_name(_data->sl_data)) + 1];
    char var_value[strlen(var_name(_data->sl_data.sl_value)) - strlen(var_name(_data->sl_data)) + 1];
    shilink_get_var_name_from_struct(var_name(_data->sl_data), var_name(_data->sl_data.sl_key), var_key);
    shilink_get_var_name_from_struct(var_name(_data->sl_data), var_name(_data->sl_data.sl_value), var_value);


    printf("%s: %s\n", var_key, _data->sl_data.sl_key);
    printf("%s: %s\n", var_value, _data->sl_data.sl_value);
    */
    printf("key = %s; value = %s\n", _data->sl_data.sl_key, _data->sl_data.sl_value);
}

int8_t shilink_fill_custom_data(SHLinkCustomData *_data, char *_key, char *_value){
    _data->sl_key = NULL;
    _data->sl_value = NULL;
    
    _data->sl_key = (char *) malloc(strlen(_key) + 1);
    if (_data->sl_key == NULL){
        shilink_debug(__func__, "ERROR", "failed to allocate memory. process aborted!\n");
        return -1;
    }
    _data->sl_value = (char *) malloc(strlen(_value) + 1);
    if (_data->sl_value == NULL){
        shilink_debug(__func__, "ERROR", "failed to allocate memory. process aborted!\n");
        free(_data->sl_key);
        _data->sl_key = NULL;
        return -1;
    }

    strcpy(_data->sl_key, _key);
    strcpy(_data->sl_value, _value);
    return 0;
}

void shilink_free_custom_data(SHLinkCustomData *_data){
    free(_data->sl_key);
    free(_data->sl_value);

    _data->sl_key = NULL;
    _data->sl_value = NULL;
}

int8_t shilink_search_data_by_position(SHLink _target, char *_key, int8_t _pos, SHLinkCustomData *_data){
    int8_t idx_pos = -1;
    while (idx_pos < _pos){
        while (_target != NULL){
            if(strcmp(_target->sl_data.sl_key, _key) == 0){
                _data->sl_key = _target->sl_data.sl_key;
                _data->sl_value = _target->sl_data.sl_value;
                idx_pos++;
                _target = _target->sh_next;
                break;
            }
            _target = _target->sh_next;
        }
        if (idx_pos == _pos){
            shilink_debug(__func__, "INFO", "data found as: %s\n", _data->sl_value);
            return 0;
        }
        if (_target == NULL){
            break;
        }
    }
    if (idx_pos == -1){
        shilink_debug(__func__, "ERROR", "data not found\n");
        return -1;
    }
    shilink_debug(__func__, "WARNING", "data found with invalid position\n");
    return 1;
}

int8_t shilink_search_data_by_prev_cond(SHLink _target, char *_key, SHLinkCustomData *_prev_cond_data, SHLinkCustomData *_data){
    while (_target != NULL){
        while (_target != NULL){
            if(strcmp(_target->sl_data.sl_key, _prev_cond_data->sl_key) == 0 && strcmp(_target->sl_data.sl_value, _prev_cond_data->sl_value) == 0){
                _target = _target->sh_next;
                break;
            }
            _target = _target->sh_next;
        }
        shilink_free_custom_data(_prev_cond_data);
        if (_target == NULL){
            shilink_debug(__func__, "ERROR", "condition not found\n");
            return -1;
            break;
        }
        while (_target != NULL){
            if(strcmp(_target->sl_data.sl_key, _key) == 0){
                _data->sl_key = _target->sl_data.sl_key;
                _data->sl_value = _target->sl_data.sl_value;
                shilink_debug(__func__, "INFO", "data found as: %s\n", _data->sl_value);
                return 0;
            }
            _target = _target->sh_next;
        }
    }
    shilink_debug(__func__, "WARNING", "data not found\n");
    return -1;
}
/* USER MODIFICATION PURPOSE END HERE */

int8_t shilink_push(SHLink *_target, SHLinkCustomData _data){
    SHLink new_data = NULL;
    new_data = (SHLink) malloc(sizeof(SHLink));
    if (new_data == NULL){
        shilink_debug(__func__, "ERROR", "failed to allocate memory\n");
        return -1;
    }
    shilink_fill_data(&new_data, _data);
    new_data->sh_next = (*_target);
    (*_target) = new_data;
    return 0;
}

int8_t shilink_append(SHLink *_target, SHLinkCustomData _data){
    SHLink new_data = NULL;
    new_data = (SHLink) malloc(sizeof(SHLink));
    if (new_data == NULL){
        shilink_debug(__func__, "ERROR", "failed to allocate memory\n");
        return -1;
    }
    shilink_fill_data(&new_data, _data);
    new_data->sh_next = NULL;

    if ((*_target) == NULL){
        (*_target) = new_data;
        return 0;
    }

    SHLink end_of_target = (*_target);

    while (end_of_target->sh_next != NULL){
        end_of_target = end_of_target->sh_next;
    }

    end_of_target->sh_next = new_data;
    return 0;
}

void shilink_print(SHLink _target){
    while (_target != NULL){
        shilink_print_data(_target);
        _target = _target->sh_next;
    }
}

void shilink_free(SHLink *_target){
    SHLink sh_tmp = NULL;
    while ((*_target) != NULL){
        sh_tmp = (*_target);
        *(_target) = (*_target)->sh_next;
        shilink_free_custom_data(&(sh_tmp->sl_data));
        free(sh_tmp);
    }
    *(_target) = NULL;
}