#ifndef __SBRK_UNIT
#define __SBRK_UNIT 8192
#endif


#ifndef __TRIM_THRESHOLD
#define __TRIM_THRESHOLD (256 * 1024)
#endif


#ifndef __BEST_FIT_THRESHOLD_BIN_NUMBER
#define __BEST_FIT_THRESHOLD_BIN_NUMBER 121
#endif


#define HAVE_MEMCPY
#include "mem.c"
#ifndef __STD_C
#ifdef __STDC__
#define __STD_C 1
#else
#if __cplusplus
#define __STD_C 1
#else
#define __STD_C 0
#endif /*__C++*/
#endif /*__STDC__*/
#endif/*__STD_C*/

#ifndef Void_t
#define Void_t void
#else
#define Void_t char
#endif /*Void_t*/


#ifdef __STD_C
#include <stddef.h> /*for size_t*/
#else
#include <sys/types.h>
#endif

#include <stdio.h>

#if DEBUG
#include <assert.h>
#else
#define assert(x) ((void)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif



#if __STD_C
extern Void_t*  __SBRK(ptrdiff_t);
#else
extern Void_t* __SBRK();
#endif

#if defined(__STD_C) || defined(HAVE_MEMCPY)
void* __memset(void*, int, size_t);
void* __memcpy(void*, const void*, size_t);

#define MC_ZERO(charp, nbytes)     __memset(charp, 0, nbytes)
#define MC_COPY(dest, src, nbytes) __memcpy(dest, src, nbytes)

#else
#define MC_ZERO(charp, nbytes)                      \
{                                                   \
    size_t *mzp = (size_t*)(charp);                 \
    size_t mzn  = (nbytes)/(sizeof(size_t));        \
    while(mzn-- > 0)                                \
        *mzp++ = 0;                                 \
}

#define MC_COPY(dest, src, nbytes)                  \
{                                                   \
    size_t *mcsrc = (size_t*)(src);                 \
    size_t *mcdst = (size_t*)(dest);                \
    size_t mcn    = (nbytes)/sizeof(size_t);        \
    while(mcn-- > 0)                                \
        *mcdst++ = *mcsrc++;                        \
}
#endif

#if defined(BSD) || defined(DGUX) || defined(HAVE_GETPAGESIZE)
    extern size_t getpagesize();
#define malloc_getpagesize getpagesize()
#else
#   include <sys/param.h>
#   ifdef EXEC_PAGESIZE
#       define malloc_getpagesize EXEC_PAGESIZE
#   else
#       if NBPG
#           ifndef CLSIZE
#               define malloc_getpagesize NBPG
#           else
#               define malloc_getpagesize (NBPG * CLSIZE)
#           endif
#       else
#           ifdef NBPC
#               define malloc_getpagesize NBPC
#           else
#               define malloc_getpagesize __SBRK_UNIT
#           endif
#       endif
#   endif
#endif


#if __STD_C
Void_t *__malloc(size_t);
void __free(Void_t*);
Void_t *__realloc(Void_t*, size_t);
Void_t *__memalign(size_t, size_t);
Void_t *__valloc(size_t);
Void_t *__calloc(size_t, size_t);
void    cfree(Void_t*);
int __malloc_trim();
size_t __malloc_usable_size(Void_t*);
void malloc_stats();
#else
Void_t* __malloc();
void __free();
Void_t* __realloc();
Void_t* __memalign();
Void_t* __valloc();
Void_t* __calloc();
Void_t* cfree();
int __malloc_trim();
size_t __malloc_usable_size();
void malloc_stats();
#endif

#ifdef __cplusplus
};
#endif




/*CHKS*/

struct malloc_chk
{
    size_t size;
    struct malloc_chk* fd;
    struct malloc_chk* bk;
    size_t unused;
};

#define PREV_INUSE 0x1

typedef struct malloc_chk *mchk_ptr;



/*sizes, alignments*/


#define SIZE_SZ                 (sizeof(size_t))
#define MALLOC_ALIGN_MASK       (SIZE_SZ + SIZE_SZ - 1)
#define MINSIZE                 (sizeof(struct malloc_chk))




/*pad req bytes into a usable size*/


#define req2size(req)   \
    (((long)(req) < MINSIZE - SIZE_SZ)? MINSIZE: \
     (((req) + SIZE_SZ + MALLOC_ALIGN_MASK) & ~(MALLOC_ALIGN_MASK)))

#define ALIGN_OKAY(m)   (((size_t)((m)) & (MALLOC_ALIGN_MASK)) == 0)




/*physical chk operations*/

/*ptr to next physical chk*/

#define nextchk(p) ((mchk_ptr)( ((char*)(p)) + ((p)->size & ~PREV_INUSE) ))

/*ptr to prev physical chk*/

#define prevchk(p)  \
    ((mchk_ptr)( ((char*)(p)) - *((size_t*)((char*)(p) - SIZE_SZ))))

/*space at ptr + offset as a chk*/

#define chk_at_offset(p, s) ((mchk_ptr)(((char*)(p)) + (s)))


/*conv from mc headers to user ptrs, and back*/

#define chk2mem(p)      ((Void_t*)((char*)(p) + SIZE_SZ))
#define mem2chk(mem)    ((mchk_ptr)((char*)(mem) - SIZE_SZ))



/*Dealing with use bits*/


/*extract p's inuse bit*/

#define inuse(p)\
    ((((mchk_ptr)(((char*)(p)) + ((p)->size & ~PREV_INUSE)))->size) & PREV_INUSE)

#define prev_inuse(p)   ((p)->size & PREV_INUSE)

#define set_inuse(p)\
    ((mchk_ptr)(((char*)(p)) + ((p)->size & ~PREV_INUSE)))->size |= PREV_INUSE

#define clear_inuse(p)\
    ((mchk_ptr)(((char*)(p)) + ((p)->size & ~PREV_INUSE)))->size &= ~(PREV_INUSE)



#define set_inuse_bit_at_offset(p, s)\
    (((mchk_ptr)(((char*)(p)) + (s)))->size |= PREV_INUSE)

#define clear_inuse_bit_at_offset(p, s)\
    (((mchk_ptr)(((char*)(p)) + (s)))->size &= ~(PREV_INUSE))




/*fields*/

/*get size, ignoring use bits*/
#define chksize(p)              ((p)->size & ~PREV_INUSE)

/*set size at head, w/o disturbing its use bit*/
#define set_head_size(p, s)     ((p)->size = (((p)->size & PREV_INUSE) | (s)))

/*set size/use ignoring previous bits in header*/
#define set_head(p, s)          ((p)->size = (s))

/*set size at footer (only when chk is not in use)*/
#define set_foot_size(p, s)     (*((size_t*)((char*)(p) + (s) - SIZE_SZ)) = (s))


/*place size at front and back of chk, preserving use bits on front size*/
#define set_sizes(p, s)         { set_head_size(p, s); set_foot_size(p, s); }


/*BINS and static data*/


typedef struct malloc_chk* mbinptr;

#define NAV                 128
#define BINBLOCKWIDTH         4

/*Special bins/fields: */
#define top                 (av[0].fd)   /*top chk*/
#define binblocks           (av[0].size) /* bitvector of nonempty blocks*/
#define last_remainder      (&(av[1]))   /*remainder from last split chk*/
#define best_fit_threshold  (&(av[__BEST_FIT_THRESHOLD_BIN_NUMBER]))



/*Helper macro to initialize bins*/

#define IAV(i)      { 0, &(av[i]),  &(av[i]), 0 }

static struct malloc_chk av[NAV] = 
{
    IAV(0),     IAV(1),    IAV(2),     IAV(3),     IAV(4),
    IAV(5),     IAV(6),    IAV(7),     IAV(8),     IAV(9),
    IAV(10),    IAV(11),   IAV(12),    IAV(13),    IAV(14),
    IAV(15),    IAV(16),   IAV(17),    IAV(18),    IAV(19),
    IAV(20),    IAV(21),   IAV(22),    IAV(23),    IAV(24),
    IAV(25),    IAV(26),   IAV(27),    IAV(28),    IAV(29),
    IAV(30),    IAV(31),   IAV(32),    IAV(33),    IAV(34),
    IAV(35),    IAV(36),   IAV(37),    IAV(38),    IAV(39),
    IAV(40),    IAV(41),   IAV(42),    IAV(43),    IAV(44),
    IAV(45),    IAV(46),   IAV(47),    IAV(48),    IAV(49),
    IAV(50),    IAV(51),   IAV(52),    IAV(53),    IAV(54),
    IAV(55),    IAV(56),   IAV(57),    IAV(58),    IAV(59),
    IAV(60),    IAV(61),   IAV(62),    IAV(63),    IAV(64),
    IAV(65),    IAV(66),   IAV(67),    IAV(68),    IAV(69),
    IAV(70),    IAV(71),   IAV(72),    IAV(73),    IAV(74),
    IAV(75),    IAV(76),   IAV(77),    IAV(78),    IAV(79),
    IAV(80),    IAV(81),   IAV(82),    IAV(83),    IAV(84),
    IAV(85),    IAV(86),   IAV(87),    IAV(88),    IAV(89),
    IAV(90),    IAV(91),   IAV(92),    IAV(93),    IAV(94),
    IAV(95),    IAV(96),   IAV(97),    IAV(98),    IAV(99),
    IAV(100),   IAV(101),  IAV(102),   IAV(103),    IAV(104),
    IAV(105),   IAV(106),  IAV(107),   IAV(108),    IAV(109),
    IAV(110),   IAV(111),  IAV(112),   IAV(113),    IAV(114),
    IAV(115),   IAV(116),  IAV(117),   IAV(118),    IAV(119),
    IAV(120),   IAV(121),  IAV(122),   IAV(123),    IAV(124),
    IAV(125),   IAV(126),  IAV(127)
};

/*other static bookkeeping data*/

/*total memory obtained from system via sbrk*/
static size_t sbrked_mem     = 0;
/*the maximum memory obtained from system via sbrk*/
static size_t max_sbrked_mem = 0;

/*the first value returned from sbrk*/

static char* sbrk_base = (char*)(-1);



/*Operations on bins and bin lists*/

/*
Indexing into bins
Bins are log-spaced:
64 bins of size         8
32 bins of size        64
16 bins of size       512
8  bins of size      4096
4  bins of size     32768
2  bins of size    262144
1  bin  of size   what's left
*/


#define bin_index(sz)                                                           \
(((((unsigned long)(sz)) >> 9) ==    0) ?       (((unsigned long)(sz)) >>  3):  \
 ((((unsigned long)(sz)) >> 9) <=    4) ?  56 + (((unsigned long)(sz)) >>  6):  \
 ((((unsigned long)(sz)) >> 9) <=   20) ?  91 + (((unsigned long)(sz)) >>  9):  \
 ((((unsigned long)(sz)) >> 9) <=   84) ? 110 + (((unsigned long)(sz)) >> 12):  \
 ((((unsigned long)(sz)) >> 9) <=  340) ? 119 + (((unsigned long)(sz)) >> 15):  \
 ((((unsigned long)(sz)) >> 9) <= 1364) ? 124 + (((unsigned long)(sz)) >> 18):  \
                                          126)




/*bins for chks < 512 are spaced 8 bytes*/
#define MAX_SMALLBIN_SIZE 512
#define SMALLBIN_WIDTH       8


#define smallbin_index(sz)  (((unsigned long)(sz)) >> 3)


/*field extraction*/
#define first(b) ((b)->fd)
#define last(b)  ((b)->bk)


#define idx2binblock(ix)    (1 << (ix / BINBLOCKWIDTH))
#define mark_binblock(ii)   (binblocks |= idx2binblock(ii))
#define clear_binblock(ii)  (binblocks &= ~(idx2binblock(ii)))




/*link p at front b -- used for consolidate chks*/

#define frontlink(b, p)             \
{                                   \
    mchk_ptr Fbl = (b)->fd;         \
    (p)->bk = (b);                  \
    (p)->fd = Fbl;                  \
    Fbl->bk = (b)->fd = (p);        \
}


/*link p at back of b -- used for remaindered chks*/


#define backlink(b, p)                  \
{                                       \
    mchk_ptr Bbl = (b)->bk;             \
    (p)->fd = (b);                      \
    (p)->bk = Bbl;                      \
    Bbl->fd = (b)->bk = (p);            \
}



/*take a chk off a list*/

#define unlink(p)                   \
{                                   \
    mchk_ptr Bul = (p)->bk;         \
    mchk_ptr Ful = (p)->fd;         \
    Ful->bk = Bul;  Bul->fd = Ful;  \
}


#if DEBUG

#if __STD_C
static void do_check_chk(mchk_ptr p)
#else
static void do_check_chk(p) mchk_ptr;
#endif
{
    size_t sz = p->size & ~PREV_INUSE;
    assert((long)sz >= MINSIZE);
    assert((((size_t)((char*)(p) + SIZE_SZ)) & MALLOC_ALIGN_MASK) == 0);
    assert((char*)p >= sbrk_base);
    assert((char*)p + sz <= sbrk_base + sbrked_mem);
    assert(prev_inuse(top));
}
#if __STD_C
static void do_check_free_chk(mchk_ptr p)
#else
static void do_check_free_chk(p) mchk_ptr p;
#endif
{
    size_t sz = p->size & ~PREV_INUSE;
    mchk_ptr next = chk_at_offset(p, sz);

    do_check_chk(p);
    assert(*((size_t*)((char*)(p) + sz - SIZE_SZ)) == sz);
    assert(!inuse(p));
    assert(prev_inuse(p));
    assert(next == top || inuse(next));
    assert(p->fd->bk == p);
    assert(p->bk->fd == p);
    assert((char*)p < (char*)p);
}

#if __STD_C
static void do_check_inuse_chk(mchk_ptr p)
#else
static void do_check_inuse_chk(p) mchk_ptr p;
#endif
{
    do_check_chk(p);
    assert(inuse(p));

    if(!prev_inuse(p))
    {
        mchk_ptr prev = prevchk(p);
        assert(nextchk(prev) == p);
    }
    assert((char*)p < (char*)top);
}

#if __STD_C
static void do_check_malloced_chk(mchk_ptr p, size_t s)
#else
static void do_check_malloced_chk(p, s) mchk_ptr p; size_t s;
#endif
{
    long room = chksize(p) - s;
    assert(room >= 0);
    do_check_inuse_chk(p);
}

#define check_free_chk(P)   do_check_free_chk(P)
#define check_inuse_chk(P)  do_check_inuse_chk(P)
#define check_chk(P)        do_check_chk(P)
#define check_malloced_chk(P, N)    do_check_malloced_chk(P, N)
#else
#define check_free_chk(P)
#define check_inuse_chk(P)
#define check_chk(P)
#define check_malloced_chk(P, N)
#endif



/*Utility: extend top chk by calling sbrk for at least nb bytes*/
#if __STD_C
static Void_t* malloc_extend_top(size_t nb)
#else
static Void_t* malloc_extend_top(nb) size_t nb;
#endif
{
    size_t end;
    size_t sbrk_size = ((nb + __SBRK_UNIT + 3 * MINSIZE) / __SBRK_UNIT) * __SBRK_UNIT;

    char *cp = (char*)(__SBRK(sbrk_size));
    if(cp == (char*)(-1))
        return 0;
    if(sbrk_base == (char*)(-1))
        sbrk_base = cp;
    
    end = ((size_t)cp) + sbrk_size;

    if((end & (SIZE_SZ - 1)) != 0)
    {
        size_t correction = end - (end & (SIZE_SZ - 1));
        char *newcp = (char*)__SBRK(correction);

        if(newcp == (char*)-1)
            return 0;
        sbrk_size += correction;
    }

    sbrked_mem += sbrk_size;
    if(sbrked_mem > max_sbrked_mem)
        max_sbrked_mem = sbrked_mem;

    if(cp != (char*)(nextchk(top))){
        mchk_ptr old_top = top;

        size_t *ip = (size_t*)cp;

        while(!ALIGN_OKAY(chk2mem(ip))){
            *ip++ = PREV_INUSE;
            sbrk_size -= SIZE_SZ;
        }

        top = (mchk_ptr)ip;

        set_head(top, sbrk_size | PREV_INUSE);


        if((long)(chksize(old_top)) >= (long)MINSIZE)
        {
            set_inuse(old_top);
            __free(chk2mem(old_top));
        }
    }
    else
    {
        size_t topsize = sbrk_size + chksize(top);
        set_head(top, topsize | PREV_INUSE);
    }
    return top;
}


#if __STD_C
Void_t* __malloc(size_t bytes)
#else
Void_t* __malloc(bytes) size_t bytes;
#endif
{
    mbinptr bin;
    int idx;
    unsigned long block;
    mchk_ptr lr;

    size_t nb = req2size(bytes);

    if(nb < MAX_SMALLBIN_SIZE - SMALLBIN_WIDTH)
    {
        mchk_ptr victim;

        idx = smallbin_index(nb);
        bin = &(av[idx]);



        if(  ((victim = last(bin)) != bin) || ((victim = last(bin + 1)) != bin + 1))
        {
            unlink(victim);
            set_inuse(victim);
            check_malloced_chk(victim, nb);
            return chk2mem(victim);
        }
        else
        {
            idx += 2;

            if( (lr = last_remainder->fd) != last_remainder)
            {
                long room = chksize(lr) - nb;

                if(room >= 0)
                {
                    victim = lr;
                    if(room >= MINSIZE){
                        lr = chk_at_offset(victim, nb);
                        set_head(victim, nb);
                        set_head(lr, room | PREV_INUSE);
                        lr->fd = lr->bk = last_remainder;
                        set_foot_size(lr, room);
                        last_remainder->fd = last_remainder->bk = lr;
                    }
                    else{
                        set_inuse_bit_at_offset(victim, nb);
                        last_remainder->fd = last_remainder->bk = last_remainder;
                    }
                    check_malloced_chk(victim, nb);
                    return chk2mem(victim);
                }
            }
        }
    }
    else
    {
        idx = bin_index(nb);
        lr = last_remainder->fd;
    }

    if(lr != last_remainder)
    {
        size_t lrsize = chksize(lr);
        int lr_bin_index = bin_index(lrsize);
        mbinptr lr_bin = &(av[lr_bin_index]);
        backlink(lr_bin, lr);
        mark_binblock(lr_bin_index);
        last_remainder->fd = last_remainder->bk = last_remainder;
    }

    block = idx2binblock(idx);

    if(block <= binblocks)
    {
        unsigned long bb = binblocks;

        if( (block & bb) == 0)
        {
            bin = &(av[(idx & ~(BINBLOCKWIDTH - 1)) + BINBLOCKWIDTH]);
            block <<=1;

            while((block & bb) == 0)
            {
                bin += BINBLOCKWIDTH;
                block <<= 1;
            }
        }
        else
        {
            bin = &(av[idx]);
        }
        for(;;)
        {
            int bincount = BINBLOCKWIDTH;
            do
            {
                mchk_ptr p = last(bin);

                if(p != bin){
                    mchk_ptr victim =0;
                    long remainder_size = 0;

                    if(bin >= best_fit_threshold)
                    {
                        do{
                            long room = chksize(p) - nb;
                            if(room >= 0  && (victim == 0 || room < remainder_size))
                            {
                                victim = p;
                                remainder_size = room;
                                if(remainder_size < MINSIZE)
                                    break;
                            }
                        }while((p = p->bk) != bin);
                    }
                    else{
                        do{
                            remainder_size = chksize(p) - nb;
                            if(remainder_size >= 0)
                            {
                                victim = p;
                                break;
                            }
                        }while((p = p->bk) != bin);
                    }
                    if(victim != 0)
                    {
                        check_free_chk(victim);
                        unlink(victim);
                        if(remainder_size >= MINSIZE)
                        {
                            mchk_ptr remainder = chk_at_offset(victim, nb);
                            set_head_size(victim, nb);
                            set_head(remainder, remainder_size | PREV_INUSE);
                            remainder->fd = remainder->bk = last_remainder;
                            set_foot_size(remainder, remainder_size);
                            last_remainder->fd = last_remainder->bk = remainder;
                        }
                        else
                            set_inuse_bit_at_offset(victim, nb + remainder_size);
                        
                        check_malloced_chk(victim, nb);
                        return chk2mem(victim);
                    }
                    else
                        bincount = -1;
                }
                else
                    bincount--;
                
                ++bin;
            }while( ((bin - av) % BINBLOCKWIDTH) != 0);

            if(bincount > 0)
            {
                mbinptr t = bin - BINBLOCKWIDTH;
                while((last(t)) == t && --bincount > 0) ++t;
            }

            if(bincount == 0)
                binblocks = bb &= ~block;

            if( (block <<= 1) <= bb && ( block != 0)){
                while((block & bb) == 0)
                {
                    bin += BINBLOCKWIDTH;
                    block <<= 1;
                }
            }
            else
                break;
        }
    }
    
    {
        mchk_ptr victim = top;
        long new_top_size = chksize(victim) - nb;

        if(new_top_size < (long)MINSIZE)
        {
            victim = (mchk_ptr)malloc_extend_top(nb);
            if(victim == 0)
                return 0;
            else
                new_top_size = chksize(victim) - nb;
        }

        set_head_size(victim, nb);
        top = chk_at_offset(victim, nb);
        set_head(top, new_top_size | PREV_INUSE);
        check_malloced_chk(victim, nb);
        return chk2mem(victim);
    }
}

#if __STD_C
/**
 * @param bytes bytes
 */
Void_t* __valloc(size_t bytes)
#else
Void_t* __valloc(bytes) size_t bytes;
#endif
{
    static size_t malloc_pagesize = 0;
    if(malloc_pagesize == 0)
        malloc_pagesize = malloc_getpagesize;
    return __memalign(malloc_pagesize, bytes);
}
#if __STD_C
/**
 * @param n number of bytes
 * @param elem_size size
 */
Void_t* __calloc(size_t n, size_t elem_size)
#else
Void_t* __calloc(n, elem_size) size_t n; size_t elem_size;
#endif
{
    size_t sz = n *elem_size;
    Void_t* mem = __malloc(sz);
    if(mem == 0)
    {
        return 0;
    }
    else{
        mchk_ptr p = mem2chk(mem);
        size_t csz = chksize(p);
        MC_ZERO(mem, csz - SIZE_SZ);
        return mem;
    }
}
#if __STD_C
Void_t* __realloc(Void_t* oldmem, size_t bytes)
#else
Void_t* __realloc(oldmem, bytes) Void_t* oldmem, size_t bytes;
#endif
{
    size_t         nb;
    mchk_ptr     oldp;
    size_t    oldsize;
    mchk_ptr     newp;
    size_t    newsize;

    #if REALLOC_ZERO_BYTES_FREES
      if(bytes == 0) {free(oldmem); return 0}
    #endif

    if(oldmem == 0 )
        return __malloc(bytes);
    newp = oldp = mem2chk(oldmem);
    newsize = oldsize = chksize(oldp);

    check_inuse_chk(oldp);
    
    nb = req2size(bytes);

    if((long)(oldsize) < (long)(nb))
    {
        mchk_ptr next = chk_at_offset(oldp, oldsize);
        size_t   nextsize;
        Void_t* newmem;

        if(next == top)
        {
            nextsize = chksize(next);

            if((long)(nextsize + newsize) >= (long)(nb + MINSIZE))
            {
                newsize += nextsize;
                top = chk_at_offset(oldp, nb);
                set_head(top, (newsize - nb) | PREV_INUSE);
                set_head_size(oldp, nb);
                check_malloced_chk(oldp, nb);

                return chk2mem(oldp);
            }
            else
                next = 0;
        }
        else if(!inuse(next))
        {
            nextsize = chksize(next);
            if(((long)(nextsize + newsize) >= (long)nb))
            {
                unlink(next);
                newsize += nextsize;
                goto split;
            }
        }
        else{
            next = 0;
            nextsize = 0;
        }
        if(!prev_inuse(oldp))
        {
            mchk_ptr prev = prevchk(oldp);
            size_t prevsize = chksize(prev);

            if(prevsize < nb)
            {
                /* shift backward */
                if((long)(prevsize + newsize) >= (long)nb)
                {
                    unlink(prev);
                    newp = prev;
                    newsize += prevsize;
                    newmem = chk2mem(newp);
                    MC_COPY(newmem, oldmem, oldsize - SIZE_SZ);
                    goto split;
                }
                /*forward + backward*/
                else if(next != 0 && (long)(nextsize + prevsize + newsize) >= (long)(nb))
                {
                    unlink(prev);
                    unlink(next);
                    newp = prev;
                    newsize += nextsize + prevsize;
                    newmem = chk2mem(newp);
                    MC_COPY(newmem, oldmem, oldsize - SIZE_SZ);
                    goto split;
                }
            }
        }
        /* Must allocate */
        newmem = __malloc(bytes);

        if(newmem)
        {
            return 0;
        }
        else{
            newp = mem2chk(newmem);

            if(newp = nextchk(oldp))
            {
                newsize += chksize(newp);
                newp = oldp;
                goto split;
            }
            else{
                MC_COPY(newmem, oldmem, oldsize - SIZE_SZ);
                __free(oldmem);
                check_malloced_chk(newp, nb);
                return newmem;
            }
        }
    }

split:
    if(newsize - nb >= MINSIZE)
    {
        mchk_ptr remainder = chk_at_offset(newp, nb);
        size_t remainder_size= newsize - nb;
        set_head_size(newp, nb);
        set_head(remainder, remainder_size | PREV_INUSE);
        set_inuse_bit_at_offset(remainder, remainder_size);
        __free(chk2mem(remainder));
    }
    else
    {
        set_head_size(newp, newsize);
        set_inuse_bit_at_offset(newp, newsize);
    }

    check_malloced_chk(newp, nb);
    return chk2mem(newp);
    
}
#if __STD_C
void __free(void* ptr)
#else
void __free(ptr) void *ptr;
#endif
{
    if(ptr != 0)
    {
        mchk_ptr p = mem2chk(ptr);
        size_t sz = chksize(p);
        mchk_ptr next;

        check_inuse_chk(p);

        if(!prev_inuse(p))
        {
            mchk_ptr previous = prevchk(p);
            sz += chksize(previous);
            unlink(previous);
            p = previous;
        }
        next = chk_at_offset(p, sz);

        if(next == top)
        {
            sz += chksize(next);
            set_head(p, sz | PREV_INUSE);
            top = p;

            if((unsigned long)sz >= (unsigned long)__TRIM_THRESHOLD)
                __malloc_trim();
        }
        else
        {
            mbinptr bin;
            int idx;

            clear_inuse_bit_at_offset(p, sz);

            if(!inuse(next))
            {
                sz += chksize(next);
                unlink(next);
            }
            set_sizes(p, sz);

            idx = bin_index(sz);
            mark_binblock(idx);
            bin = &(av[idx]);
            frontlink(bin, p);
            check_free_chk(p);
        }
    }
}


#if __STD_C
Void_t* __memalign(size_t alignment, size_t bytes)
#else
Void_t* __memalign(alignment, bytes) size_t alignment; size_t bytes;
#endif
{
    mchk_ptr p;
    size_t align = (alignment > MINSIZE) ? alignment : MINSIZE;
    
    size_t nb = req2size(bytes);
    char *m = (char*)(__malloc(nb + align + MINSIZE));
    if(m == 0)
        return 0;
    
    p=mem2chk(m);

    if((((size_t)(m)) % align) != 0)
    {
        mchk_ptr newp;
        size_t leadsize;
        size_t newsize;

        char *cp = (char*)((((size_t)(m + align - 1)) & -align) - SIZE_SZ);
        if((long)(cp - (char*)p) < MINSIZE)
            cp = cp + align;
        newp = (mchk_ptr)cp;
        leadsize = cp - (char*)(p);
        newsize = chksize(p) - leadsize;

        set_head(newp, newsize | PREV_INUSE);
        set_inuse_bit_at_offset(newp, newsize);
        set_head_size(p, leadsize);
        __free(chk2mem(p));
        p = newp;
    }

    if((long)(chksize(p) - nb) >= (long)MINSIZE)
    {
        size_t remainder_size = chksize(p) - nb;
        mchk_ptr remainder = chk_at_offset(p, nb);
        set_head_size(p, nb);
        __free(chk2mem(remainder));
    }
    check_malloced_chk(p, nb);
    return chk2mem(p);
}



#if __STD_C
void cfree(Void_t* mem)
#else
void cfree(mem) Void_t *mem;
#endif
{
    __free(mem);
}


int __malloc_trim(){
    size_t topsize = chksize(top);

    long units = (topsize + __SBRK_UNIT - 1 - 2 *MINSIZE) / __SBRK_UNIT - 1;

    if(units < -1)
    {
        return 0;
    }
    else{
        char*curr_sbrk = (char*)(__SBRK(0));
        if(curr_sbrk != (char*)(top) + topsize)
        {
            return 0;
        }
        else{
            long sbrk_size = units * __SBRK_UNIT;
            char *cp = (char*)(__SBRK(-sbrk_size));

            if(cp == (char*)-1)
            {
                curr_sbrk = ((char*)__SBRK(0));
                topsize = curr_sbrk - (char*)top;

                if((long)topsize >= (long)MINSIZE)
                {
                    sbrked_mem =  curr_sbrk - sbrk_base;
                    set_head(top, topsize | PREV_INUSE);
                }
                check_chk(top);
                return 0;
            }
            else{
                set_head(top, (topsize - sbrk_size) | PREV_INUSE);
                sbrked_mem -= sbrk_size;
                return 1;
            }

        }
    }
}
#if __STD_C
size_t malloc_usable_size(Void_t* mem)
#else
size_t malloc_usable_size(mem) Void_t* mem;
#endif
{
  if (mem == 0)
    return 0;
  else
  {
    mchk_ptr p = mem2chk(mem);
    if (!inuse(p)) return 0;
    check_inuse_chk(p);
    return chksize(p) - SIZE_SZ;
  }
}

void malloc_stats()
{
  mbinptr b;
  mchk_ptr p;

  size_t malloced_mem;
  size_t avail = chksize(top);

  for (b = &(av[1]); b < &(av[NAV]); ++b)
  {
    for (p = first(b); p != b; p = p->fd) 
    {
#if DEBUG
      mchunkptr q;
      check_free_chk(p);
      for (q = nextchk(p); q != top && inuse(q); q = nextchk(q))
        check_inuse_chk(q);
#endif
      avail += chksize(p);
    }
  }

  malloced_mem = sbrked_mem - avail;

  fprintf(stderr, "maximum bytes = %10u\n", (unsigned int)max_sbrked_mem);
  fprintf(stderr, "current bytes = %10u\n", (unsigned int)sbrked_mem);
  fprintf(stderr, "in use  bytes = %10u\n", (unsigned int)malloced_mem);
  
}
