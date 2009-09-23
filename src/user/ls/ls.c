/**
 * \file ls.c
 */
#include "shell_command.h"
#include "string.h"
#include "rootfs.h"

#define COMMAND_NAME "ls"
#define COMMAND_DESC_MSG "list directory contents"
static const char *help_msg =
	#include "ls_help.inc"
	;
#define HELP_MSG help_msg

DECLARE_SHELL_COMMAND_DESCRIPTOR(COMMAND_NAME, exec, COMMAND_DESC_MSG, HELP_MSG);

static char available_keys[] = {
    'p', 'h'
};

static int exec(int argsc, char **argsv) {
    SHELL_KEY keys[MAX_SHELL_KEYS];
    char *key_value;
    int keys_amount;
    char *path = NULL;
    FSOP_DESCRIPTION *fsop;
    FS_FILE_ITERATOR iter_func;
    FILE_INFO file_info;


    keys_amount = parse_arg(COMMAND_NAME, argsc, argsv, available_keys,
            sizeof(available_keys), keys);

    if (keys_amount < 0) {
        show_help();
        return -1;
    }

    if (get_key('h', keys, keys_amount, &key_value)) {
        show_help();
        return 0;
    }

    if (get_key('p', keys, keys_amount, &key_value)) {
        path = key_value;
    } else {
        path = "/";
    }
    if (NULL == (fsop = rootfs_get_fsopdesc(path))){
        LOG_ERROR("can't find fs %s\n", path);
        return 0;
    }
    if (NULL == fsop->get_file_list_iterator){
        LOG_ERROR("wrong fs desc %s\n", path);
    }
    if (NULL == (iter_func = fsop->get_file_list_iterator())){
        LOG_ERROR("can't find iterator func for fs %s\n", path);
        return 0;
    }
    printf("%16s | %8s | %10s | %10s\n", "name", "mode", "size", "size on disk");
    while (NULL != iter_func(&file_info)){
        printf("%16s | %8X | %10d | %10d\n", file_info.file_name, file_info.mode, file_info.size_in_bytes, file_info.size_on_disk);
    }

    return 0;
}
