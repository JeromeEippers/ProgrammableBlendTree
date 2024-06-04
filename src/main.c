#include <stdio.h>

#include "python/python.h"
#include "log.h"

int main(void)
{
    pbt_log_set_level(0);
    pbt_log_trace("hello world");

    bool status = pbt_init_python();
    assert(status);

    pbt_deinit_python();
    
    return 0;
}

