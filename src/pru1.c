#define PRU_SHAREDMEM 0x00012000

volatile register unsigned int __R31, __R30;

int main(void)
{
    __halt();
    return 0;
}
