#include "nt_handler.h"
#include "ntshell.h"
#include "ntlibc.h"
#include "usart.h"

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


static int func_callback(const char *text, void *extobj)
{
    ntshell_t *ntshell = (ntshell_t *)extobj;
    char *buff1 = "User input text:'";
    char *buff2 = "'\r\n";

    if (ntlibc_strlen(text) > 0) {
        // g_usart_object.usart_ops.usart_write_buffer(buff1, ntlibc_strlen(text));
        // g_usart_object.usart_ops.usart_write_buffer(text, ntlibc_strlen(text));
        // g_usart_object.usart_ops.usart_write_buffer(buff2, ntlibc_strlen(text));

        g_usart_object.usart_ops.usart_put_string("User input text:'");
        g_usart_object.usart_ops.usart_put_string(text);
        g_usart_object.usart_ops.usart_put_string("'\r\n");
    }
    return 0;
}

void init_nt_shell(void)
{
    ntshell_init(
        &ntshell,
        func_read,
        func_write,
        func_callback,
        (void *)&ntshell);

    ntshell_set_prompt(&ntshell, "ntshell>");
    ntshell_execute(&ntshell);
}