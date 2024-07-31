void* __memset(void* ptr, int b, unsigned long size)
{
    unsigned long* p = (unsigned long*)ptr;
    unsigned long s  = (size) / sizeof(unsigned long);
    while(s-- > 0)
    {
        *p++ = 0;
    }
}

void* __memcpy(void* dst, const void* src, unsigned long __s)
{
    typedef unsigned long size_t;
    size_t *mcsrc = (size_t*)src;
    size_t *mcdst = (size_t*)dst;
    size_t mcn    = (__s)/(sizeof(size_t));

    while(mcn--  > 0)
    {
        *mcdst++ = *mcsrc++;
    }
}
