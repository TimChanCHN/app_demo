#ifndef __LETTER_HANDLER_H
#define __LETTER_HANDLER_H

#include "shell.h"

void letter_shell_init(void);
Shell *get_shell(void);
void letter_shell_loop_task(void);

#endif

