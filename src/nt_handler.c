#include "nt_handler.h"
#include "ntshell.h"
#include "ntlibc.h"
#include "usart.h"
#include "ntshell_usrcmd.h"

static ntshell_t ntshell;

static int func_read(char *buf, int cnt, void *extobj)
{
    g_usart_object.usart_ops.usart_read_buffer(buf, cnt);

    return cnt;
}

static int func_write(const char *buf, int cnt, void *extobj)
{
    int i = 0;
    for (i = 0; i < cnt; i++)
    {
        g_usart_object.usart_ops.usart_write(buf[i]);
    }

    return cnt;
}


void init_nt_shell(void)
{
    // 1. 初始化命令列表
    extern const unsigned int _shell_command_start;
    extern const unsigned int _shell_command_end;

    uint16_t i = 0;
    // uint16_t cmd_count = ((unsigned int)(&_shell_command_end)
    //                         - (unsigned int)(&_shell_command_start)) / sizeof(cmd_table_t);
    uint16_t cmd_count =  ((unsigned int)(&_shell_command_end)
                                - (unsigned int)(&_shell_command_start))
                                / sizeof(cmd_table_t);

    for (i = 0; i < cmd_count; i++)
    {

    }

    // 2. 初始化nt-shell
    ntshell_init(
        &ntshell,
        func_read,
        func_write,
        ntshell_usrcmd_execute,
        (void *)&g_usart_object);

    ntshell_set_prompt(&ntshell, "ntshell>");
    ntshell_execute(&ntshell);
}