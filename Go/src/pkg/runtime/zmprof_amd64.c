// auto generated by go tool dist
// goos=windows goarch=amd64

#include "runtime.h"
#include "arch_GOARCH.h"
#include "malloc.h"
#include "defs_GOOS_GOARCH.h"
#include "type.h"

#line 543 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
static Lock proflock; 
#line 547 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
typedef struct Bucket Bucket; 
struct Bucket 
{ 
Bucket *next; 
Bucket *allnext; 
uintptr allocs; 
uintptr frees; 
uintptr alloc_bytes; 
uintptr free_bytes; 
uintptr recent_allocs; 
uintptr recent_frees; 
uintptr recent_alloc_bytes; 
uintptr recent_free_bytes; 
uintptr hash; 
uintptr nstk; 
uintptr stk[1]; 
} ; 
enum { 
BuckHashSize = 179999 , 
} ; 
static Bucket **buckhash; 
static Bucket *buckets; 
static uintptr bucketmem; 
#line 572 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
static Bucket* 
stkbucket ( uintptr *stk , int32 nstk , bool alloc ) 
{ 
int32 i; 
uintptr h; 
Bucket *b; 
#line 579 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
if ( buckhash == nil ) { 
buckhash = runtime·SysAlloc ( BuckHashSize*sizeof buckhash[0] ) ; 
mstats.buckhash_sys += BuckHashSize*sizeof buckhash[0]; 
} 
#line 585 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
h = 0; 
for ( i=0; i<nstk; i++ ) { 
h += stk[i]; 
h += h<<10; 
h ^= h>>6; 
} 
h += h<<3; 
h ^= h>>11; 
#line 594 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
i = h%BuckHashSize; 
for ( b = buckhash[i]; b; b=b->next ) 
if ( b->hash == h && b->nstk == nstk && 
runtime·mcmp ( ( byte* ) b->stk , ( byte* ) stk , nstk*sizeof stk[0] ) == 0 ) 
return b; 
#line 600 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
if ( !alloc ) 
return nil; 
#line 603 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
b = runtime·mallocgc ( sizeof *b + nstk*sizeof stk[0] , FlagNoProfiling , 0 , 1 ) ; 
bucketmem += sizeof *b + nstk*sizeof stk[0]; 
runtime·memmove ( b->stk , stk , nstk*sizeof stk[0] ) ; 
b->hash = h; 
b->nstk = nstk; 
b->next = buckhash[i]; 
buckhash[i] = b; 
b->allnext = buckets; 
buckets = b; 
return b; 
} 
#line 616 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
void 
runtime·MProf_GC ( void ) 
{ 
Bucket *b; 
#line 621 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
runtime·lock ( &proflock ) ; 
for ( b=buckets; b; b=b->allnext ) { 
b->allocs += b->recent_allocs; 
b->frees += b->recent_frees; 
b->alloc_bytes += b->recent_alloc_bytes; 
b->free_bytes += b->recent_free_bytes; 
b->recent_allocs = 0; 
b->recent_frees = 0; 
b->recent_alloc_bytes = 0; 
b->recent_free_bytes = 0; 
} 
runtime·unlock ( &proflock ) ; 
} 
#line 643 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
typedef struct AddrHash AddrHash; 
typedef struct AddrEntry AddrEntry; 
#line 646 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
struct AddrHash 
{ 
AddrHash *next; 
uintptr addr; 
AddrEntry *dense[1<<13]; 
} ; 
#line 653 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
struct AddrEntry 
{ 
AddrEntry *next; 
uint32 addr; 
Bucket *b; 
} ; 
#line 660 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
enum { 
AddrHashBits = 12 
} ; 
static AddrHash *addrhash[1<<AddrHashBits]; 
static AddrEntry *addrfree; 
static uintptr addrmem; 
#line 672 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
enum { 
HashMultiplier = 2654435769U 
} ; 
#line 677 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
static void 
setaddrbucket ( uintptr addr , Bucket *b ) 
{ 
int32 i; 
uint32 h; 
AddrHash *ah; 
AddrEntry *e; 
#line 685 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
h = ( uint32 ) ( ( addr>>20 ) *HashMultiplier ) >> ( 32-AddrHashBits ) ; 
for ( ah=addrhash[h]; ah; ah=ah->next ) 
if ( ah->addr == ( addr>>20 ) ) 
goto found; 
#line 690 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
ah = runtime·mallocgc ( sizeof *ah , FlagNoProfiling , 0 , 1 ) ; 
addrmem += sizeof *ah; 
ah->next = addrhash[h]; 
ah->addr = addr>>20; 
addrhash[h] = ah; 
#line 696 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
found: 
if ( ( e = addrfree ) == nil ) { 
e = runtime·mallocgc ( 64*sizeof *e , FlagNoProfiling , 0 , 0 ) ; 
addrmem += 64*sizeof *e; 
for ( i=0; i+1<64; i++ ) 
e[i].next = &e[i+1]; 
e[63].next = nil; 
} 
addrfree = e->next; 
e->addr = ( uint32 ) ~ ( addr & ( ( 1<<20 ) -1 ) ) ; 
e->b = b; 
h = ( addr>>7 ) & ( nelem ( ah->dense ) -1 ) ; 
e->next = ah->dense[h]; 
ah->dense[h] = e; 
} 
#line 713 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
static Bucket* 
getaddrbucket ( uintptr addr ) 
{ 
uint32 h; 
AddrHash *ah; 
AddrEntry *e , **l; 
Bucket *b; 
#line 721 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
h = ( uint32 ) ( ( addr>>20 ) *HashMultiplier ) >> ( 32-AddrHashBits ) ; 
for ( ah=addrhash[h]; ah; ah=ah->next ) 
if ( ah->addr == ( addr>>20 ) ) 
goto found; 
return nil; 
#line 727 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
found: 
h = ( addr>>7 ) & ( nelem ( ah->dense ) -1 ) ; 
for ( l=&ah->dense[h]; ( e=*l ) != nil; l=&e->next ) { 
if ( e->addr == ( uint32 ) ~ ( addr & ( ( 1<<20 ) -1 ) ) ) { 
*l = e->next; 
b = e->b; 
e->next = addrfree; 
addrfree = e; 
return b; 
} 
} 
return nil; 
} 
#line 742 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
void 
runtime·MProf_Malloc ( void *p , uintptr size ) 
{ 
int32 nstk; 
uintptr stk[32]; 
Bucket *b; 
#line 749 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
if ( m->nomemprof > 0 ) 
return; 
#line 752 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
m->nomemprof++; 
nstk = runtime·callers ( 1 , stk , 32 ) ; 
runtime·lock ( &proflock ) ; 
b = stkbucket ( stk , nstk , true ) ; 
b->recent_allocs++; 
b->recent_alloc_bytes += size; 
setaddrbucket ( ( uintptr ) p , b ) ; 
runtime·unlock ( &proflock ) ; 
m->nomemprof--; 
} 
#line 764 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
void 
runtime·MProf_Free ( void *p , uintptr size ) 
{ 
Bucket *b; 
#line 769 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
if ( m->nomemprof > 0 ) 
return; 
#line 772 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
m->nomemprof++; 
runtime·lock ( &proflock ) ; 
b = getaddrbucket ( ( uintptr ) p ) ; 
if ( b != nil ) { 
b->recent_frees++; 
b->recent_free_bytes += size; 
} 
runtime·unlock ( &proflock ) ; 
m->nomemprof--; 
} 
#line 788 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
typedef struct Record Record; 
struct Record { 
int64 alloc_bytes , free_bytes; 
int64 alloc_objects , free_objects; 
uintptr stk[32]; 
} ; 
#line 796 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
static void 
record ( Record *r , Bucket *b ) 
{ 
int32 i; 
#line 801 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
r->alloc_bytes = b->alloc_bytes; 
r->free_bytes = b->free_bytes; 
r->alloc_objects = b->allocs; 
r->free_objects = b->frees; 
for ( i=0; i<b->nstk && i<nelem ( r->stk ) ; i++ ) 
r->stk[i] = b->stk[i]; 
for ( ; i<nelem ( r->stk ) ; i++ ) 
r->stk[i] = 0; 
} 
void
runtime·MemProfile(Slice p, bool include_inuse_zero, uint8, uint16, uint32, int32 n, bool ok)
{
#line 811 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"

	Bucket *b;
	Record *r;

	runtime·lock(&proflock);
	n = 0;
	for(b=buckets; b; b=b->allnext)
		if(include_inuse_zero || b->alloc_bytes != b->free_bytes)
			n++;
	ok = false;
	if(n <= p.len) {
		ok = true;
		r = (Record*)p.array;
		for(b=buckets; b; b=b->allnext)
			if(include_inuse_zero || b->alloc_bytes != b->free_bytes)
				record(r++, b);
	}
	runtime·unlock(&proflock);
	FLUSH(&n);
	FLUSH(&ok);
}

#line 832 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
typedef struct TRecord TRecord; 
struct TRecord { 
uintptr stk[32]; 
} ; 
void
runtime·ThreadCreateProfile(Slice p, int32 n, bool ok)
{
#line 837 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"

	TRecord *r;
	M *first, *m;
	
	first = runtime·atomicloadp(&runtime·allm);
	n = 0;
	for(m=first; m; m=m->alllink)
		n++;
	ok = false;
	if(n <= p.len) {
		ok = true;
		r = (TRecord*)p.array;
		for(m=first; m; m=m->alllink) {
			runtime·memmove(r->stk, m->createstack, sizeof r->stk);
			r++;
		}
	}
	FLUSH(&n);
	FLUSH(&ok);
}
void
runtime·Stack(Slice b, bool all, uint8, uint16, uint32, int32 n)
{
#line 856 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"

	byte *pc, *sp;
	
	sp = runtime·getcallersp(&b);
	pc = runtime·getcallerpc(&b);

	if(all) {
		runtime·semacquire(&runtime·worldsema);
		m->gcing = 1;
		runtime·stoptheworld();
	}

	if(b.len == 0)
		n = 0;
	else{
		g->writebuf = (byte*)b.array;
		g->writenbuf = b.len;
		runtime·goroutineheader(g);
		runtime·traceback(pc, sp, 0, g);
		if(all)
			runtime·tracebackothers(g);
		n = b.len - g->writenbuf;
		g->writebuf = nil;
		g->writenbuf = 0;
	}
	
	if(all) {
		m->gcing = 0;
		runtime·semrelease(&runtime·worldsema);
		runtime·starttheworld(false);
	}
	FLUSH(&n);
}

#line 889 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
static void 
saveg ( byte *pc , byte *sp , G *g , TRecord *r ) 
{ 
int32 n; 
#line 894 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"
n = runtime·gentraceback ( pc , sp , 0 , g , 0 , r->stk , nelem ( r->stk ) ) ; 
if ( n < nelem ( r->stk ) ) 
r->stk[n] = 0; 
} 
void
runtime·GoroutineProfile(Slice b, int32 n, bool ok)
{
#line 899 "C:\Users\ADMINI~1\AppData\Local\Temp\2\bindist269497170\go\src\pkg\runtime\mprof.goc"

	byte *pc, *sp;
	TRecord *r;
	G *gp;
	
	sp = runtime·getcallersp(&b);
	pc = runtime·getcallerpc(&b);
	
	ok = false;
	n = runtime·gcount();
	if(n <= b.len) {
		runtime·semacquire(&runtime·worldsema);
		m->gcing = 1;
		runtime·stoptheworld();

		n = runtime·gcount();
		if(n <= b.len) {
			ok = true;
			r = (TRecord*)b.array;
			saveg(pc, sp, g, r++);
			for(gp = runtime·allg; gp != nil; gp = gp->alllink) {
				if(gp == g || gp->status == Gdead)
					continue;
				saveg(gp->sched.pc, gp->sched.sp, gp, r++);
			}
		}
	
		m->gcing = 0;
		runtime·semrelease(&runtime·worldsema);
		runtime·starttheworld(false);
	}
	FLUSH(&n);
	FLUSH(&ok);
}
