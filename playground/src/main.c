#include <stdint.h>

volatile uint32_t global_kek = 0;

int main(void)
{
    volatile uint32_t a = 0;
    while (1)
    {
        a++;
        global_kek++;
    }

    return 0;
}

