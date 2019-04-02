#include <stdio.h>
#define ROADSTORE 50000000
#define MAXROW 5000
#define MAXTREEWIDTH 200000
/*
#define BITSWIDE  21
#define DEPTH     3
#define PERIOD    2
#define XMOVE     1
#define YMOVE     0
#define CLEARUP   -5
#define CLEARAT   1
#define BUFFERIGHT 4
#define NOSYM     1
#define EVENSYM   0
#define ODDSYM    0
#define FLIP      1
#define ASYMFORCE 0
#define ASYMROW   0
*/
#define MAXCONSTACK  16
#define MAXCPRSTACK  16
#define MAXPROSTACK  16
#define MAXDEPTH  100
#define MAXPERIOD 8
#define EXRIGHT   0
#define EXLEFT    0
#define FACTDEPTH 7
#define NUMFACTS  13
#define DN        ((PERIOD + 1)*NUMFACTS)
#define UP        (-DN)
#define UR        NUMFACTS
#define DL        (-UR)
#define RT        (DN + UR)
#define LT        (-RT)

main(argc, argv)
int argc;
char *argv[];
{
int BITSWIDE, DEPTH, PERIOD, XMOVE, YMOVE, CLEARUP, SLOWER, VERYSLOW;
int CLEARAT, BUFFERIGHT, NOSYM, EVENSYM, ODDSYM, FLIP, ASYMFORCE, ASYMROW;
int INTREE, TREEOUT, MEMCOMP, HYPEROW, FLOTROW, SLOW, UNIQUE;
static char line[500], ljunk[10];
unsigned c, d, xscreen, yscreen;
unsigned r0, cr0, xr0, xr0loc, checkend, clearsum, pass1, pass2, calced;
unsigned newval, oldval, valimit, valinc;
unsigned t0, t1, t2, t3, t4, t5, t6, t7, t8;
unsigned firsttry, numtries, numtriesb, numhits, rownum, trownum;
unsigned flipon, symflip;
unsigned noway3l, noway3r, noway0l, noway0r;
unsigned noway1l, noway1r, noway2l, noway2r;
unsigned gt1, gt2, lt2, lt1;
unsigned gt1l, gt1r, lt2l, lt2r;
unsigned gt1ll, gt1rr, lt2ll, lt2rr;
unsigned gt2l, gt2r, lt1l, lt1r;
unsigned nomid, nomidl, nomidr;
unsigned totnw4;
unsigned ostuff0, ostuff1, ostuff2, ostuff3, ostuff4, ostuff5, ostuff6;
unsigned contra, contries, contriesb, conmissq, conmissqb, contrits, contritsb, contramisses, contramissesb;
unsigned trims, inship, openship, inrealship, likelyflot, urows;
unsigned advtodo, props;
unsigned *forcntpnt;
unsigned *newfacts, *oldfacts, *noway, *sumnw;
unsigned *stackstat, *stackbase;
static unsigned statuses[(FACTDEPTH + 2)*(MAXPERIOD + 1)*NUMFACTS];
static unsigned factemplate[(MAXPERIOD + 1)*NUMFACTS];
static unsigned forbearcnt[MAXROW];
unsigned surrounds[8], sigs[4], carryin, carryout;
unsigned no5wayn[32], no5wayt[32], nowayt[8], noway2[4], noways[4], *no5way, anychange, mover, *make1, makec, makes;
unsigned make1_0r, make1_0l, make1_1r, make1_1l, make1_2r, make1_2l, make1_3r, make1_3l;
unsigned *rpt, *rpto, *curdepths;
unsigned statsa[32], statsb[32];
struct roads {
  unsigned value;
  struct roads *parent;
  };
static unsigned shiplist[MAXROW], flotlist[MAXROW];
static unsigned dpaths[ROADSTORE];
static unsigned reaches[ROADSTORE];
static struct roads allroads[ROADSTORE];
static struct roads *sortroads[ROADSTORE];
struct roads **sortop, **sonroad, **soffroad;
struct roads *onroad, *offroad, *nchild;
struct roads *ancestor, *tancestor, *firsthit;
struct depths {
  unsigned rows[MAXDEPTH];
  };
static struct depths alldepths[MAXTREEWIDTH];
struct forbears {
  struct roads *loc;
  unsigned rstuff[6*MAXPERIOD];
  unsigned facts[FACTDEPTH*(MAXPERIOD + 1)*NUMFACTS];
  unsigned checker;
  unsigned symmetric;
  unsigned lowper;
  unsigned prostack[3*MAXPROSTACK];
  unsigned protop;
  unsigned passtwo;
  unsigned firstpasstwo;
  unsigned onemask;
  unsigned knockout;
  unsigned numdives;
  unsigned lastdive;
  unsigned numddives;
  unsigned constack[2*(MAXCONSTACK + 2)];
  unsigned contop;
  unsigned cprstack[3*MAXCPRSTACK];
  unsigned cprtop;
  };
static struct forbears cglist[MAXDEPTH + 3*MAXPERIOD + 1];
struct forbears *gpt, *glist, *cpt, *oldcpt, *tempcpt;
struct todostuff {
  unsigned *where;
  unsigned what;
  unsigned *status;
  };
static struct todostuff todolist[FACTDEPTH*(MAXPERIOD + 1)*NUMFACTS];
struct todostuff *todo;

NOSYM = 0;
EVENSYM = 0;
ODDSYM = 0;
BUFFERIGHT = 0;
DEPTH = 5;
XMOVE = 1;
YMOVE = 0;
PERIOD = 5;
SLOWER = 0;
VERYSLOW = 0;
BITSWIDE = 9;
CLEARUP = 0;
CLEARAT = 2;
FLIP = 0;
INTREE = 0;
TREEOUT = 0;
MEMCOMP = 0;
ASYMFORCE = 0;
ASYMROW = 0;
HYPEROW = 0;
FLOTROW = 0;
UNIQUE = 0;
if (argc > 1) while(--argc > 0) {
  if ((*++argv)[0] == '-')
    switch ((*argv)[1]) {
    case 'w':
      --argc;
      sscanf(*++argv, "%d", &BITSWIDE);
      break;
    case 'x':
      --argc;
      sscanf(*++argv, "%d", &XMOVE);
      break;
    case 'h':
      --argc;
      sscanf(*++argv, "%d", &HYPEROW);
      break;
    case 'u':
      --argc;
      sscanf(*++argv, "%d", &UNIQUE);
      break;
    case 'a':
      --argc;
      ASYMFORCE = 1;
      sscanf(*++argv, "%d", &ASYMROW);
      break;
    case 'm':
      --argc;
      sscanf(*++argv, "%d", &MEMCOMP);
      break;
    case 'y':
      --argc;
      sscanf(*++argv, "%d", &YMOVE);
      break;
    case 'c':
      --argc;
      sscanf(*++argv, "%d", &CLEARUP);
      --argc;
      sscanf(*++argv, "%d", &CLEARAT);
      break;
    case 'p':
      --argc;
      sscanf(*++argv, "%d", &PERIOD);
      break;
    case 'd':
      --argc;
      sscanf(*++argv, "%d", &DEPTH);
      break;
    case 'b':
      --argc;
      sscanf(*++argv, "%d", &BUFFERIGHT);
      break;
    case 'n':
      if (!BUFFERIGHT) BUFFERIGHT = 8;
      NOSYM = 1;
      break;
    case 'e':
      EVENSYM = 1;
      break;
    case 'v':
      VERYSLOW = 1;
      break;
    case 'o':
      ODDSYM = 1;
      break;
    case 'f':
      if (!BUFFERIGHT) BUFFERIGHT = 8;
      FLIP = 1;
      NOSYM = 1;
      break;
    case 'i':
      INTREE = 1;
      break;
    case 't':
      TREEOUT = 1;
      break;
    }
  }
else {
  printf("\n");
  printf("Welcome to knight.\n");
  printf("\n");
  printf("This program searches for spaceships in Conway's\n");
  printf("game of life.  It is named for its initial intention\n");
  printf("to search for a knightship, a ship that moves up 2 and\n");
  printf("over 1.  It turns out that I don't have the patience to\n");
  printf("wait the ~1 million hours of CPU time required to find one,\n");
  printf("but the program can still find a lot of neat things.\n");
  printf("The command line options to control the search are as follows:\n");
  printf("\n");
  printf("-p <PERIOD>   : This sets the number of generations to wait\n");
  printf("              : before checking for isomorphism to the original state.\n");
  printf("\n");
  printf("-x <XMOVE>    : These parameters set the displacement of the initial\n");
  printf("-y <YMOVE>    : pattern for which to check isomorphism.  Since search\n");
  printf("              : time is a high exponential of WIDTH*(PERIOD - XMOVE)\n");
  printf("              : it is a good idea to place most (or all) of the movement\n");
  printf("              : in the XMOVE parameter.\n");
  printf("\n");
  printf("-w <BITSWIDE> : This sets the maximum width of the spaceships that will\n");
  printf("              : be searched for.  The program searches by trying to extend\n");
  printf("              : ships whose front parts meet the isomorphism criteria, and\n");
  printf("              : therefore the length of the ships found is essentially unlimited.\n");
  printf("\n");
  printf("-d <DEPTH>    : The search proceeds in a breadth first manner in the large\n");
  printf("              : sense.  For long searches this would require gigabytes of memory.\n");
  printf("              : This parameter sets the number of rows of valid extension of a\n");
  printf("              : prospective spaceship must be found before the first row of the\n");
  printf("              : extension is entered into the search tree.  This saves memory\n");
  printf("              : by reducing the number of entries into the search tree.\n");
  printf("\n");
  printf("-c <CLEARUP> <CLEARAT> : These parameters control amount of the search tree that\n");
  printf("              : will be pruned when a complete ship is found.\n");
  printf("\n");
  printf("-e            : Search for spaceships with even symmetry.\n");
  printf("-o            : Search for spaceships with odd symmetry.\n");
  printf("-f            : Set the isomorphism check to match to the initial state flipped.\n");
  printf("-n            : Search for spaceships with no symmetry.  This is the only option\n");
  printf("              :   that makes sense when YMOVE is nonzero.\n");
  printf("              : Only one of the previous four options should be used at a time.\n");
  printf("              : If none of the options are stated a type of odd symmetric ship\n");
  printf("              : with dead cells along the axis of symmetry are searched for.\n");
  printf("\n");
  printf("-b <BUFFERIGHT> : This sets the number of buffer bits to the right of the ship\n");
  printf("              : in the 32 bit word in which a row of the ship is stored.  This\n");
  printf("              : parameter is automatically set to a reasonable value.\n");
  printf("\n");
  printf("-t            : This option results in each row of the search tree database\n");
  printf("              : being printed to standard output as each row of the search is\n");
  printf("              : completed.  This is typically a lot of data.  When this option\n");
  printf("              : is chosen run the search in the background with the output piped\n");
  printf("              : to a file.  The more comprehensible aspects of the output can\n");
  printf("              : be picked off with -- grep -v N <outfile>.\n");
  printf("\n");
  printf("-i            : This option tells the program to expect to read in a partial\n");
  printf("              : search tree database on standard input.  This is useful for\n");
  printf("              : restarting interrupted searches.\n");
  printf("\n");
  printf("When complete spaceships are found they are printed to standard output in\n");
  printf("a format readable by Xlife and Alan Hensel's life1.6 for DOS.\n");
  printf("\n");
  printf("-Tim Coe      coe@vitsemi.com\n");
  printf("\n");
  printf("Press return to see the results of -- knightt -o -p 4 -d 32 -w 7: ");
  gets(line);
  ODDSYM = 1;
  PERIOD = 4;
  DEPTH = 32;
  BITSWIDE = 7;
  }

MEMCOMP = 4;
YMOVE = 0;
FLIP = 0;
HYPEROW = 0;
if (PERIOD > 3*XMOVE) SLOW = 1;
else SLOW = 0;
if (!FLOTROW) FLOTROW = 10*PERIOD;
forcntpnt = forbearcnt;
*forcntpnt = 0;
c = MAXROW;
while (c--) *forcntpnt++ = 0;
xscreen = 0;
yscreen = 0;
glist = cglist + DEPTH;
(glist + 2*PERIOD + 1)->symmetric = 1;
if (MEMCOMP & 4) (glist + 2*PERIOD + 1)->symmetric = 0;
(glist + 2*PERIOD + 1)->lowper = 1;
gpt = glist;
while (gpt < glist + 2*PERIOD + 1) gpt++->loc = NULL;
firsttry = 1;
onroad = allroads;
onroad->value = 0;
onroad->parent = allroads;
sortop = sortroads;
*sortop = onroad;
sortop++;
reaches[0] = 0;
shiplist[0] = 0;
dpaths[0] = 0;
/*
onroad++;
onroad->value = 0x008000;
onroad->parent = onroad - 1;
*/
rownum = onroad - allroads;
stackstat = statuses;
while (stackstat < statuses + 2*(PERIOD + 1)*NUMFACTS) *stackstat++ = 0;
while (stackstat < statuses + (FACTDEPTH + 1)*(PERIOD + 1)*NUMFACTS) *stackstat++ = 1;
while (stackstat < statuses + (FACTDEPTH + 2)*(PERIOD + 1)*NUMFACTS) *stackstat++ = 0;
stackbase = statuses + PERIOD*NUMFACTS + 2*(PERIOD + 1)*NUMFACTS;
while (stackbase < statuses + (FACTDEPTH + 1)*(PERIOD + 1)*NUMFACTS) {
  stackstat = stackbase;
  while (stackstat < stackbase + NUMFACTS) *stackstat++ = 0;
  stackbase += (PERIOD + 1)*NUMFACTS;
  }
stackbase = statuses + 2*(PERIOD + 1)*NUMFACTS;
while (stackbase < statuses + (FACTDEPTH + 1)*(PERIOD + 1)*NUMFACTS) {
  stackbase[10] = 2;
  stackbase += (PERIOD + 1)*NUMFACTS;
  }
stackbase = statuses + (PERIOD + 1)*NUMFACTS;
newfacts = factemplate;
t1 = ~(~0 << BUFFERIGHT);
t2 = -(1 << (BITSWIDE + BUFFERIGHT));
c = 8;
while (c--) nowayt[c] = 0;
c = 8;
while (c--) if (c & 2) nowayt[c] |= ~(((1 << BITSWIDE) - 1) << BUFFERIGHT);
//c = 8;
//while (c--) printf("template %d %x\n", c, nowayt[c]);
anychange = 1;
while (anychange) {
  anychange = 0;
  c = 4;
  while (c--) {
    mover = (nowayt[2*c] & nowayt[2*c+1]) << 1;
    anychange |= mover & ~nowayt[c];
    anychange |= mover & ~nowayt[c+4];
    no5wayn[c] = nowayt[c] | mover;
    no5wayn[c+4] = nowayt[c+4] | mover;
    }
  c = 8;
  while (c--) nowayt[c] = no5wayn[c];
  }
anychange = 1;
while (anychange) {
  anychange = 0;
  c = 4;
  while (c--) {
    mover = (nowayt[c] & nowayt[c+4]) >> 1;
    anychange |= mover & ~nowayt[2*c];
    anychange |= mover & ~nowayt[2*c+1];
    no5wayn[2*c] = nowayt[2*c] | mover;
    no5wayn[2*c+1] = nowayt[2*c+1] | mover;
    }
  c = 8;
  while (c--) nowayt[c] = no5wayn[c];
  }
c = 32;
while (c--) no5wayt[c] = 0;
c = 32;
while (c--) if (c & 4) no5wayt[c] |= ~(((1 << BITSWIDE) - 1) << BUFFERIGHT);
//c = 32;
//while (c--) printf("template %d %x\n", c, no5wayt[c]);
anychange = 1;
while (anychange) {
  anychange = 0;
  c = 16;
  while (c--) {
    mover = (no5wayt[2*c] & no5wayt[2*c+1]) << 1;
    anychange |= mover & ~no5wayt[c];
    anychange |= mover & ~no5wayt[c+16];
    no5wayn[c] = no5wayt[c] | mover;
    no5wayn[c+16] = no5wayt[c+16] | mover;
    }
  c = 32;
  while (c--) no5wayt[c] = no5wayn[c];
  }
anychange = 1;
while (anychange) {
  anychange = 0;
  c = 16;
  while (c--) {
    mover = (no5wayt[c] & no5wayt[c+16]) >> 1;
    anychange |= mover & ~no5wayt[2*c];
    anychange |= mover & ~no5wayt[2*c+1];
    no5wayn[2*c] = no5wayt[2*c] | mover;
    no5wayn[2*c+1] = no5wayt[2*c+1] | mover;
    }
  c = 32;
  while (c--) no5wayt[c] = no5wayn[c];
  }
  c = 8;
  while (c--) if (c & 1) nowayt[c] |= 0x1;
  c = 32;
  while (c--) if (c & 1) no5wayt[c] |= 3;
  c = 32;
  while (c--) if (c & 2) no5wayt[c] |= 1;

c = 4;
while (c--) {
  if (c) noways[c] = -(1 << (BITSWIDE + BUFFERIGHT - c + 2));
  else noways[c] = 0;
  }
c = 4;
if (NOSYM) while (c--) {
  if (c) noways[c] |= (1 << (BUFFERIGHT + c - 2)) - 1;
  }

//c = 32;
//while (c--) printf("template %d %x\n", c, no5wayt[c]);
//exit(0);
offroad = onroad + 1;
if (INTREE) while (gets(line) != NULL) {
  if ((line[1] == 'D') && (onroad != offroad)) {
    firsttry = 0;
    onroad = offroad;
    rownum++;
    oldfacts = glist->facts + (PERIOD + 1)*NUMFACTS;
    newfacts = glist->facts;
    while (oldfacts < glist->facts + FACTDEPTH*(PERIOD + 1)*NUMFACTS) *newfacts++ = *oldfacts++;
    oldfacts = factemplate;
    while (newfacts < glist->facts + FACTDEPTH*(PERIOD + 1)*NUMFACTS) *newfacts++ = *oldfacts++;
    }
  if (line[1] == 'N') {
    sscanf(line, "%s %x %d", ljunk, &t1, &t2);
    offroad->value = t1;
    offroad->parent = onroad - t2;
    if (offroad->parent < allroads) offroad->parent = allroads;
    offroad++;
    }
  if (line[1] == 'P') {
    sscanf(line, "%s %d %d\n", ljunk, &xscreen, &yscreen);
    if (NOSYM) xscreen += BITSWIDE + 6;
    else xscreen += 2*BITSWIDE + 6;
    if (xscreen > 300) {
      yscreen += rownum + 6;
      xscreen = 0;
      }
    }
  }
t0 = MAXTREEWIDTH - (offroad - onroad);
while (t0 < MAXTREEWIDTH) {
  curdepths = (alldepths + t0)->rows;
  c = DEPTH;
  while (c--) curdepths[c] = 0;
  t0++;
  }
openship = 0;
trims = 0;
nchild = offroad;
while (((onroad < offroad) || openship) && (rownum < MAXROW)) {
  contramisses = 0;
  contramissesb = 0;
  contries = 0;
  contriesb = 0;
  conmissq = 0;
  conmissqb = 0;
  contrits = 0;
  contritsb = 0;
  numtries = 0;
  numtriesb = 0;
  numhits = 0;
  c = 32;
  while (c--) statsa[c] = 0;
  c = 32;
  while (c--) statsb[c] = 0;
  if (onroad < offroad) firsthit = onroad;
  while (((onroad->parent - allroads) >= 100000000) && (onroad < offroad)) onroad++;
  while (onroad < offroad) {
    gpt = glist;
    gpt->loc = onroad;
    forcntpnt = forbearcnt;
    tancestor = allroads;
    while ((gpt->loc->parent != (gpt + 1)->loc) && (gpt < glist + 2*PERIOD)) {
      (*forcntpnt++)++;
      if ((gpt + 1)->loc != NULL) tancestor = (gpt + 1)->loc->parent;
      (gpt + 1)->loc = gpt->loc->parent;
      gpt++;
      if (gpt == glist + 2*PERIOD) {
        ancestor = gpt->loc->parent;
        while (ancestor != tancestor) {
          ancestor = ancestor->parent;
          tancestor = tancestor->parent;
          (*forcntpnt++)++;
          }
        }
      }
    while (gpt >= glist) {
      rpto = (gpt + PERIOD)->rstuff;
      rpt = gpt->rstuff;
      r0 = gpt->loc->value;
      t2 = r0;
      if (FLIP && !(((glist - gpt) + rownum) % (PERIOD))) {
        t1 = t2;
        t2 = 0;
        c = BITSWIDE;
        while (c--) {
          t2 <<= 1;
          t2 |= t1 & (1 << BUFFERIGHT);
          t1 >>= 1;
          }
        }
      gpt->checker = t2;
      if (YMOVE && !(((glist - gpt) + rownum) % (PERIOD))) gpt->checker <<= 1;
      if (!YMOVE && NOSYM && (gpt + 1)->symmetric) {
        t1 = r0;
        t2 = 0;
        c = BITSWIDE;
        while (c--) {
          t2 <<= 1;
          t2 |= (1 << BUFFERIGHT) & t1;
          t1 >>= 1;
          }
        if (t2 == r0) gpt->symmetric = 1;
	else gpt->symmetric = 0;
        }
      else gpt->symmetric = 0;
      make1 = gpt->facts;
      noway = gpt->facts + 4;
      no5way = gpt->facts + 12;
      while (rpt < gpt->rstuff + 6*1) {
        rpt[0] = r0;
        t1 = r0 >> 1;
        t2 = r0 << 1;
        if (ODDSYM) t2 |= t1 & 1;
        if (EVENSYM) t2 |= r0 & 1;
        t3 = t1 ^ t2;
        rpt[2] = (t1 & t2) ^ (r0 & t3);
        rpt[1] = r0 ^ t3;
        t1 = rpt[1] & rpto[1];
        rpt[3] = rpt[1] ^ rpto[1];
        t2 = rpt[2] & rpto[2];
        t3 = rpt[2] ^ rpto[2];
        rpt[5] = t2 ^ (t3 | t1);
        rpt[4] = ~(t3 ^ t1);
        r0 = (rpto[0] | (rpt[1] ^ rpto[3])) &
             (rpt[2] ^ rpto[4] ^ (rpt[1] | rpto[3])) &
             (rpto[5] ^ (rpt[2] & rpto[4]));
        make1[0] = ((rpt[0] & ~rpt[3] & rpt[4]) |
                    (rpt[3] & ~rpt[4])) & rpt[5];
        make1[1] = (rpt[0] | ~rpt[3]) & ~rpt[4] & rpt[5];
        make1[2] = (rpt[0] & ~rpt[3] & ~rpt[4] & rpt[5]) |
                   (rpt[3] & rpt[4] & ~rpt[5]);
        make1[3] = (rpt[0] | ~rpt[3]) & rpt[4] & ~rpt[5];
        make1 += 4;
        rpt += 6;
        rpto += 6;
        }
      make1 -= 4;
      if (HYPEROW && (gpt + 1)->lowper) {
        if (gpt >= (glist + 2*PERIOD - 1)) gpt->lowper = 1;
        else if (gpt->rstuff[6] == (gpt + 2)->rstuff[0]) gpt->lowper = 1;
        else gpt->lowper = 0;
        }
      else gpt->lowper = 0;
      make1 = (gpt + XMOVE)->facts;
//      r0 = gpt->checker;
      r0 = gpt->rstuff[0];
      noway[0] = make1[0] ^ r0;
      noway[1] = make1[1] ^ r0;
      noway[2] = noway[1];
      noway[3] = make1[2] ^ r0;
      noway[4] = noway[1];
      noway[5] = noway[3];
      noway[6] = noway[3];
      noway[7] = make1[3] ^ r0;
      if (EVENSYM) {
        c = 8;
        while (c) {
          c -= 2;
          if (c & 2) {
            noway[c] = (noway[c] & 0xfffffffe) | (noway[c+1] & 1);
            }
          }
        }
      else if (ODDSYM) {
        c = 8;
        while (c) {
          c -= 2;
          if (c & 4) {
            noway[c] = (noway[c] & 0xfffffffe) | (noway[c+1] & 1);
            }
          }
        }
      c = 8;
      while (c--) noway[c] |= nowayt[c];
      contra = ~0;
      c = 8;
      while (c--) contra &= noway[c];
      anychange = 1;
      while (anychange && !contra) {
        anychange = 0;
        contra = ~0;
        c = 4;
        while (c--) {
          mover = (noway[2*c] & noway[2*c+1]) << 1;
          anychange |= mover & ~noway[c];
          anychange |= mover & ~noway[c+4];
          no5wayn[c] = noway[c] | mover;
          no5wayn[c+4] = noway[c+4] | mover;
          contra &= no5wayn[c];
          contra &= no5wayn[c+4];
          }
        c = 8;
        while (c--) noway[c] = no5wayn[c];
        }
      c = 8;
      while (c--) {
//printf("noways %d %x %x %x\n", c, noway[c], r0, make1[c]);
        no5way[ 0 + 2*c + 0] = (noway[((c<<1)&7)  ] << 1) | noway[c] | (noway[  (c>>1)] >> 1);
        no5way[ 0 + 2*c + 1] = (noway[((c<<1)&7)+1] << 1) | noway[c] | (noway[  (c>>1)] >> 1);
        no5way[16 + 2*c + 0] = (noway[((c<<1)&7)  ] << 1) | noway[c] | (noway[4+(c>>1)] >> 1);
        no5way[16 + 2*c + 1] = (noway[((c<<1)&7)+1] << 1) | noway[c] | (noway[4+(c>>1)] >> 1);
        }
      (gpt + XMOVE)->cprtop = 0;
      no5way = (gpt + XMOVE)->facts + 12;
      c = 32;
//      while (c--) no5way[c+96] = no5way[c];
      while (c--) {
        no5way[c+96] = no5way[0+c] | (no5way[0+(c>>1)] >> 1);
        no5way[c+128] = no5way[0+c] | (no5way[0+16+(c>>1)] >> 1);
        }
      gpt->passtwo = 1;
      gpt->firstpasstwo = 0;
      gpt--;
      }
    t1 = MAXTREEWIDTH - (offroad - onroad);
    curdepths = (alldepths + t1)->rows;
    c = DEPTH;
    c--;
    (glist - c - 1)->rstuff[0] = 0;
    while (c--) {
      (glist - c - 1)->rstuff[0] = curdepths[c + 1];;
      }
    oldcpt = glist;
    cpt = glist - 1;
    newval = cpt->rstuff[0];
    valimit = 1 << (BITSWIDE + BUFFERIGHT);
    valinc = 1 << BUFFERIGHT;
    if (firsttry) {
      newval = valinc;
      firsttry = 0;
      }
    trownum = rownum;
    (cpt + PERIOD - XMOVE)->protop = 0;
    no5way = (cpt + PERIOD - XMOVE)->facts + 12;
    c = 64;
    while (c--) no5way[c+32] = no5way[c+96];
//    c = 32;
//    while (c--) {
//      no5way[c+32] = no5way[96+c] | (no5way[96+(c>>1)] >> 1);
//      no5way[c+64] = no5way[96+c] | (no5way[96+16+(c>>1)] >> 1);
//      }
    cpt->knockout = valimit;
//    cpt->knockout = valinc;
    cpt->numdives = 0;
    cpt->numddives = 0;
    cpt->contop = 0;
    while (cpt < glist) {
      no5way = (cpt + PERIOD - XMOVE)->facts + 12 + 32;
//c = 64;
//while (c--) printf("no5ways %d %x\n", c, no5way[c]);
      c = BITSWIDE + BUFFERIGHT - 1;
      r0 = valimit << 1;
      xr0 = 0;
//printf("calc a try %d %d %d %x\n", cpt - glist, c, xr0, newval);
      if (NOSYM) while ((c >= (BUFFERIGHT - 2)) && !xr0) {
        if (r0 & no5way[(newval >> c) & 63]) {
          if (c > BUFFERIGHT) newval = ((newval >> c) + 1) << c;
          else newval = ((newval >> BUFFERIGHT) + 1) << BUFFERIGHT;
          while (~(newval >> c) & 1) {
            r0 <<= 1;
            c++;
            }
          if (c >= (BITSWIDE + BUFFERIGHT)) xr0 = valinc;
//          if (newval >= valimit) xr0 = valinc;
          }
        else {
          c--;
          r0 >>= 1;
          }
        if ((c == (BUFFERIGHT - 2)) && (cpt + PERIOD/2)->cprtop) {
          t1 = (cpt + PERIOD/2)->cprtop;
          while (t1) {
            t1 -= 3;
            if (!((cpt + PERIOD/2)->cprstack[t1 + 1] & (newval ^ (cpt + PERIOD/2)->cprstack[t1 + 2]))) {
              c = (cpt + PERIOD/2)->cprstack[t1];
              if (c > BUFFERIGHT) newval = ((newval >> c) + 1) << c;
              else newval = ((newval >> BUFFERIGHT) + 1) << BUFFERIGHT;
              c = BITSWIDE + BUFFERIGHT - 1;
              r0 = valimit << 1;
              t1 = 0;
              }
            }
          }
        if ((c == (BUFFERIGHT - 2)) && (cpt + PERIOD/2)->protop) {
          t1 = (cpt + PERIOD/2)->protop;
          while (t1) {
            t1 -= 3;
            if (!((cpt + PERIOD/2)->prostack[t1 + 1] & (newval ^ (cpt + PERIOD/2)->prostack[t1 + 2]))) {
              c = (cpt + PERIOD/2)->prostack[t1];
              if (c > BUFFERIGHT) newval = ((newval >> c) + 1) << c;
              else newval = ((newval >> BUFFERIGHT) + 1) << BUFFERIGHT;
              c = BITSWIDE + BUFFERIGHT - 1;
              r0 = valimit << 1;
              t1 = 0;
              }
            }
          }
        }
      else while (r0 && !xr0) {
        if ((r0 > 2) && (r0 & no5way[(newval >> c) & 63])) {
          newval = ((newval >> c) + 1) << c;
          while (~(newval >> c) & 1) {
            r0 <<= 1;
            c++;
            }
          if (c >= (BITSWIDE + BUFFERIGHT)) xr0 = valinc;
          }
        else if ((r0 == 2) && (r0 & no5way[(newval << 1) & 63])) {
          newval++;
          r0 <<= 1;
          c++;
          while (~(newval >> c) & 1) {
            r0 <<= 1;
            c++;
            }
          if (c >= BITSWIDE) xr0 = valinc;
          }
        else if ((r0 == 1) && (r0 & no5way[(newval << 2) & 63])) {
          newval++;
          r0 <<= 1;
          c++;
          r0 <<= 1;
          c++;
          while (~(newval >> c) & 1) {
            r0 <<= 1;
            c++;
            }
          if (c >= BITSWIDE) xr0 = valinc;
          }
        else {
          c--;
          r0 >>= 1;
          }
        if (!r0 && (cpt + PERIOD/2)->cprtop) {
          t1 = (cpt + PERIOD/2)->cprtop;
          while (t1) {
            t1 -= 3;
            if (!((cpt + PERIOD/2)->cprstack[t1 + 1] & (newval ^ (cpt + PERIOD/2)->cprstack[t1 + 2]))) {
              c = (cpt + PERIOD/2)->cprstack[t1];
              if (!(c & 0x80000000)) newval = ((newval >> c) + 1) << c;
              else newval++;
              c = BITSWIDE - 1;
              r0 = valimit << 1;
              t1 = 0;
              }
            }
          }
        if (!r0 && (cpt + PERIOD/2)->protop) {
          t1 = (cpt + PERIOD/2)->protop;
          while (t1) {
            t1 -= 3;
            if (!((cpt + PERIOD/2)->prostack[t1 + 1] & (newval ^ (cpt + PERIOD/2)->prostack[t1 + 2]))) {
              c = (cpt + PERIOD/2)->prostack[t1];
              if (!(c & 0x80000000)) newval = ((newval >> c) + 1) << c;
              else newval++;
              c = BITSWIDE - 1;
              r0 = valimit << 1;
              t1 = 0;
              }
            }
          }
        }
//printf("a try %d %d %d %x\n", cpt - glist, c, xr0, newval);
      xr0loc = 32;
      pass1 = 0;
      pass2 = 0;
      calced = 0;
      while (!xr0 && (!pass1 || !pass2)) {
        if (!pass1 && !pass2) numtries++;
        if (SLOW && (cpt + PERIOD - 2*XMOVE)->passtwo && newval && (newval == (cpt + PERIOD - 2*XMOVE)->firstpasstwo)) pass2 = 1;
        statsa[glist - cpt]++;
        if ((glist - cpt) == 1) statsb[0] = 1;
        else statsb[0]++;
        rpto = (cpt + PERIOD)->rstuff;
        rpt = cpt->rstuff;
        r0 = newval;
/*
printf("%8x\n", r0);
*/
        make1 = cpt->facts;
        noway = cpt->facts + 4;
        no5way = cpt->facts + 12;
        if (!calced) while (rpt < cpt->rstuff + 6*1) {
          rpt[0] = r0;
          t1 = r0 >> 1;
          t2 = r0 << 1;
          if (ODDSYM) t2 |= t1 & 1;
          if (EVENSYM) t2 |= r0 & 1;
          t3 = t1 ^ t2;
          rpt[2] = (t1 & t2) ^ (r0 & t3);
          rpt[1] = r0 ^ t3;
          r0 = (rpto[0] | (rpt[1] ^ rpto[3])) &
               (rpt[2] ^ rpto[4] ^ (rpt[1] | rpto[3])) &
               (rpto[5] ^ (rpt[2] & rpto[4]));
          t1 = rpt[1] & rpto[1];
          rpt[3] = rpt[1] ^ rpto[1];
          t2 = rpt[2] & rpto[2];
          t3 = rpt[2] ^ rpto[2];
          rpt[5] = t2 ^ (t3 | t1);
          rpt[4] = ~(t3 ^ t1);
          make1[0] = ((rpt[0] & ~rpt[3] & rpt[4]) |
                      (rpt[3] & ~rpt[4])) & rpt[5];
          make1[1] = (rpt[0] | ~rpt[3]) & ~rpt[4] & rpt[5];
          make1[2] = (rpt[0] & ~rpt[3] & ~rpt[4] & rpt[5]) |
                     (rpt[3] & rpt[4] & ~rpt[5]);
          make1[3] = (rpt[0] | ~rpt[3]) & rpt[4] & ~rpt[5];
//c = 4;
//while (c--) printf("make1 and noway late %d %d %x %x %x %x %x %x\n", rpt - cpt->rstuff, c, rpt[0], rpto[0], make1[c], rpt[3], rpt[4], rpt[5]);
          make1 += 4;
          rpt += 6;
          rpto += 6;
          }
        calced = 1;
        props = 0;
//        if ((cpt + PERIOD - 2*XMOVE)->passtwo) {
        if ((XMOVE > 1) || pass2) {
          r0 = cpt->rstuff[0];
          make1 = (cpt + XMOVE)->facts;
          noway[0] = make1[0] ^ r0;
          noway[1] = make1[1] ^ r0;
          noway[2] = noway[1];
          noway[3] = make1[2] ^ r0;
          noway[4] = noway[1];
          noway[5] = noway[3];
          noway[6] = noway[3];
          noway[7] = make1[3] ^ r0;
          if (EVENSYM) {
            c = 8;
            while (c) {
              c -= 2;
              if (c & 2) {
                noway[c] = (noway[c] & 0xfffffffe) | (noway[c+1] & 1);
                }
              }
            }
          else if (ODDSYM) {
            c = 8;
            while (c) {
              c -= 2;
              if (c & 4) {
                noway[c] = (noway[c] & 0xfffffffe) | (noway[c+1] & 1);
                }
              }
            }
          c = 8;
          while (c--) noway[c] |= nowayt[c];
//c = 8;
//while (c--) printf("%d %x %x %x\n", c, r0, noway[c], make1[c]);
          props = 0;
          xr0 = ~0;
          c = 8;
          while (c--) xr0 &= noway[c];
          anychange = 1;
          while (anychange && !xr0) {
            props++;
            contrits++;
            anychange = 0;
            xr0 = ~0;
            c = 4;
            while (c--) {
              mover = (noway[2*c] & noway[2*c+1]) << 1;
              anychange |= mover & ~noway[c];
              anychange |= mover & ~noway[c+4];
              no5wayn[c] = noway[c] | mover;
              no5wayn[c+4] = noway[c+4] | mover;
              xr0 &= no5wayn[c];
              xr0 &= no5wayn[c+4];
              }
            c = 8;
            while (c--) noway[c] = no5wayn[c];
            }
//c = 8;
//while (c--) printf("noways %d %x %x %x %x\n", c, noway[c], r0, make1[c], xr0);
          xr0 >>= props;
          if (xr0) {
            t1 = xr0;
            c = 0;
            if (t1 & 0xffff0000) {
              c += 16;
              t1 &= 0xffff0000;
              }
            else {
              t1 &= 0x0000ffff;
              }
            if (t1 & 0xff00ff00) {
              c += 8;
              t1 &= 0xff00ff00;
              }
            else {
              t1 &= 0x00ff00ff;
              }
            if (t1 & 0xf0f0f0f0) {
              c += 4;
              t1 &= 0xf0f0f0f0;
              }
            else {
              t1 &= 0x0f0f0f0f;
              }
            if (t1 & 0xcccccccc) {
              c += 2;
              t1 &= 0xcccccccc;
              }
            else {
              t1 &= 0x33333333;
              }
            if (t1 & 0xaaaaaaaa) {
              c += 1;
              t1 &= 0xaaaaaaaa;
              }
            else {
              t1 &= 0x55555555;
              }
            if (1) {
//              }
//            else if (1) {
              no5way = (cpt + PERIOD - XMOVE)->facts + 12 + 32;
              if (props < 6) {
                t2 = t1 << 2;
                t4 = 1 << (1 + props);
                t5 = (newval >> (c - 0)) & (t4 - 1);
                t3 = 64;
//printf("props %d %d %x %x %x\n", props, c, t2, t5, newval);
                while (t3) {
                  t3 -= t4;
                  no5way[t3 + t5] |= t2;
                  }
                }
              }
if (c == 32) printf("really big early %d %d %x %x\n", props, cpt->contop, xr0, newval);
            if (1) {
              xr0 = t1;
              cpt->onemask |= (t1 << (props + 1)) - t1;
              }
            xr0loc = c;
            }
          else pass1 = 1;
          }
//printf("looking passive %d %x %x %d %d\n", glist - cpt, cpt->rstuff[0], xr0, pass2, (cpt + PERIOD - 2*XMOVE)->passtwo);
//        if (1 && !xr0) {
        if (1 && !xr0 && !pass2) {
          contries++;
          noway = noway2;
          make1 = (cpt + PERIOD - XMOVE)->facts;
          r0 = (cpt + PERIOD - 2*XMOVE)->checker;
          noway[0] = (make1[0] ^ r0) | noways[0];
          noway[1] = (make1[1] ^ r0) | noways[1];
          noway[2] = (make1[2] ^ r0) | noways[2];
          noway[3] = (make1[3] ^ r0) | noways[3];
          make1 = cpt->facts;
          make1_0r = make1[0] << 1;
          make1_0l = make1[0] >> 1;
          make1_1r = make1[1] << 1;
          make1_1l = make1[1] >> 1;
          make1_2r = make1[2] << 1;
          make1_2l = make1[2] >> 1;
          make1_3r = make1[3] << 1;
          make1_3l = make1[3] >> 1;
          if (EVENSYM) {
            make1_0r |= make1[0] & 1;
            make1_1r |= make1[1] & 1;
            make1_2r |= make1[2] & 1;
            make1_3r |= make1[3] & 1;
            }
          else if (ODDSYM) {
            make1_0r |= make1_0l & 1;
            make1_1r |= make1_1l & 1;
            make1_2r |= make1_2l & 1;
            make1_3r |= make1_3l & 1;
            }
          makec = (make1_0l & (make1[0] | make1_0r)) |
                              (make1[0] & make1_0r);
          makes =  make1_0l ^  make1[0] ^ make1_0r;
          no5way[ 0] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_0l & (make1[0] | make1_1r)) |
                              (make1[0] & make1_1r);
          makes =  make1_0l ^  make1[0] ^ make1_1r;
          no5way[ 1] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_0l & (make1[1] | make1_1r)) |
                              (make1[1] & make1_1r);
          makes =  make1_0l ^  make1[1] ^ make1_1r;
          no5way[ 2] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_0l & (make1[1] | make1_2r)) |
                              (make1[1] & make1_2r);
          makes =  make1_0l ^  make1[1] ^ make1_2r;
          no5way[ 3] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_1l & (make1[1] | make1_1r)) |
                              (make1[1] & make1_1r);
          makes =  make1_1l ^  make1[1] ^ make1_1r;
          no5way[ 4] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_1l & (make1[1] | make1_2r)) |
                              (make1[1] & make1_2r);
          makes =  make1_1l ^  make1[1] ^ make1_2r;
          no5way[ 5] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_1l & (make1[2] | make1_2r)) |
                              (make1[2] & make1_2r);
          makes =  make1_1l ^  make1[2] ^ make1_2r;
          no5way[ 6] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_1l & (make1[2] | make1_3r)) |
                              (make1[2] & make1_3r);
          makes =  make1_1l ^  make1[2] ^ make1_3r;
          no5way[ 7] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_1l & (make1[1] | make1_0r)) |
                              (make1[1] & make1_0r);
          makes =  make1_1l ^  make1[1] ^ make1_0r;
          no5way[ 8] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          no5way[ 9] = no5way[ 4];
          makec = (make1_1l & (make1[2] | make1_1r)) |
                              (make1[2] & make1_1r);
          makes =  make1_1l ^  make1[2] ^ make1_1r;
          no5way[10] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          no5way[11] = no5way[ 6];
          makec = (make1_2l & (make1[2] | make1_1r)) |
                              (make1[2] & make1_1r);
          makes =  make1_2l ^  make1[2] ^ make1_1r;
          no5way[12] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_2l & (make1[2] | make1_2r)) |
                              (make1[2] & make1_2r);
          makes =  make1_2l ^  make1[2] ^ make1_2r;
          no5way[13] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_2l & (make1[3] | make1_2r)) |
                              (make1[3] & make1_2r);
          makes =  make1_2l ^  make1[3] ^ make1_2r;
          no5way[14] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_2l & (make1[3] | make1_3r)) |
                              (make1[3] & make1_3r);
          makes =  make1_2l ^  make1[3] ^ make1_3r;
          no5way[15] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_1l & (make1[0] | make1_0r)) |
                              (make1[0] & make1_0r);
          makes =  make1_1l ^  make1[0] ^ make1_0r;
          no5way[16] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_1l & (make1[0] | make1_1r)) |
                              (make1[0] & make1_1r);
          makes =  make1_1l ^  make1[0] ^ make1_1r;
          no5way[17] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          no5way[18] = no5way[ 4];
          no5way[19] = no5way[ 5];
          makec = (make1_2l & (make1[1] | make1_1r)) |
                              (make1[1] & make1_1r);
          makes =  make1_2l ^  make1[1] ^ make1_1r;
          no5way[20] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_2l & (make1[1] | make1_2r)) |
                              (make1[1] & make1_2r);
          makes =  make1_2l ^  make1[1] ^ make1_2r;
          no5way[21] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          no5way[22] = no5way[13];
          makec = (make1_2l & (make1[2] | make1_3r)) |
                              (make1[2] & make1_3r);
          makes =  make1_2l ^  make1[2] ^ make1_3r;
          no5way[23] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_2l & (make1[1] | make1_0r)) |
                              (make1[1] & make1_0r);
          makes =  make1_2l ^  make1[1] ^ make1_0r;
          no5way[24] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          no5way[25] = no5way[20];
          no5way[26] = no5way[12];
          no5way[27] = no5way[13];
          makec = (make1_3l & (make1[2] | make1_1r)) |
                              (make1[2] & make1_1r);
          makes =  make1_3l ^  make1[2] ^ make1_1r;
          no5way[28] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_3l & (make1[2] | make1_2r)) |
                              (make1[2] & make1_2r);
          makes =  make1_3l ^  make1[2] ^ make1_2r;
          no5way[29] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_3l & (make1[3] | make1_2r)) |
                              (make1[3] & make1_2r);
          makes =  make1_3l ^  make1[3] ^ make1_2r;
          no5way[30] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          makec = (make1_3l & (make1[3] | make1_3r)) |
                              (make1[3] & make1_3r);
          makes =  make1_3l ^  make1[3] ^ make1_3r;
          no5way[31] = (noway[0] & ~makec & ~makes) |
                       (noway[1] & ~makec &  makes) |
                       (noway[2] &  makec & ~makes) |
                       (noway[3] &  makec &  makes);
          if (EVENSYM) {
            c = 32;
            while (c) {
              c -= 2;
              if (c & 2) {
                no5way[c] = (no5way[c] & 0xfffffffd) | (no5way[c+1] & 2);
                }
              else if ((c & 0xc) == 4) {
                no5way[c] = (no5way[c] & 0xfffffffe) | (no5way[c+2] & 1);
                }
              else if ((c & 0xc) == 8) {
                no5way[c] = (no5way[c] & 0xfffffffe) | (no5way[c+1] & 1);
                }
              else if ((c & 0xc) == 0xc) {
                no5way[c] = (no5way[c] & 0xfffffffe) | (no5way[c+3] & 1);
                }
              }
            }
          else if (ODDSYM) {
            c = 32;
            while (c) {
              c -= 2;
              if (c & 4) {
                no5way[c] = (no5way[c] & 0xfffffffd) | (no5way[c+1] & 2);
                }
              if ((c & 0x1b) == 8) {
                no5way[c] = (no5way[c] & 0xfffffffe) | (no5way[c+2] & 1);
                }
              else if ((c & 0x1b) == 0x10) {
                no5way[c] = (no5way[c] & 0xfffffffe) | (no5way[c+1] & 1);
                }
              else if ((c & 0x1b) == 0x18) {
                no5way[c] = (no5way[c] & 0xfffffffe) | (no5way[c+3] & 1);
                }
              }
            }
          c = 32;
          while (c--) no5way[c] |= no5wayt[c];
          xr0 = ~0;
          c = 32;
          while (c--) xr0 &= no5way[c];
//          if (!xr0) conmissq++;
          anychange = 1;
          t1 = 0;
          while (anychange && !xr0) {
            t1++;
            conmissq++;
            anychange = 0;
            xr0 = ~0;
            c = 16;
            while (c--) no5wayn[c] = (no5way[2*c] & no5way[2*c+1]) << 1;
            c = 16;
            while (c--) {
              anychange |= no5wayn[c] & ~no5way[c];
              anychange |= no5wayn[c] & ~no5way[c+16];
              no5way[c] = no5way[c] | no5wayn[c];
              no5way[c+16] = no5way[c+16] | no5wayn[c];
              xr0 &= no5way[c];
              xr0 &= no5way[c+16];
              }
            }
          props = t1;
          xr0 >>= props;
//          if (xr0) statsa[props]++;
//          else statsb[props]++;
          if (xr0) {
            t1 = xr0;
            c = 0;
            if (t1 & 0xffff0000) {
              c += 16;
              t1 &= 0xffff0000;
              }
            else {
              t1 &= 0x0000ffff;
              }
            if (t1 & 0xff00ff00) {
              c += 8;
              t1 &= 0xff00ff00;
              }
            else {
              t1 &= 0x00ff00ff;
              }
            if (t1 & 0xf0f0f0f0) {
              c += 4;
              t1 &= 0xf0f0f0f0;
              }
            else {
              t1 &= 0x0f0f0f0f;
              }
            if (t1 & 0xcccccccc) {
              c += 2;
              t1 &= 0xcccccccc;
              }
            else {
              t1 &= 0x33333333;
              }
            if (t1 & 0xaaaaaaaa) {
              c += 1;
              t1 &= 0xaaaaaaaa;
              }
            else {
              t1 &= 0x55555555;
              }
            if (NOSYM) {
              if (!props) {
                no5way = (cpt + PERIOD - XMOVE)->facts + 12 + 32;
                t2 = (newval >> (c - 2)) & 31;
                no5way[t2] |= t1;
                no5way[32 + t2] |= t1;
                no5way[64 + t2] |= t1;
                no5way[96 + t2] |= t1;
                }
              else if (props < 2) {
                no5way = (cpt + PERIOD - XMOVE)->facts + 12 + 32;
                t2 = (newval >> (c - 2)) & 63;
                no5way[t2] |= t1;
                no5way[64 + t2] |= t1;
                }
              else if (1) {
                }
              else if (props < 3) {
                if ((cpt + PERIOD/2)->protop < 3*MAXPROSTACK) {
                  (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop] = c - 2;
                  (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop + 1] = (1 << (c + props + 3)) - (1 << (c - 2));
                  (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop + 2] = newval;
                  (cpt + PERIOD/2)->protop += 3;
                  }
                }
              }
            else {
              if (!props) {
                no5way = (cpt + PERIOD - XMOVE)->facts + 12 + 32;
                if (c >= 2) {
                  t2 = (newval >> (c - 2)) & 31;
                  no5way[t2] |= t1;
                  no5way[32 + t2] |= t1;
                  no5way[64 + t2] |= t1;
                  no5way[96 + t2] |= t1;
                  }
                else {
                  t2 = (newval << (2 - c)) & 31;
                  no5way[t2] |= t1;
                  no5way[32 + t2] |= t1;
                  no5way[64 + t2] |= t1;
                  no5way[96 + t2] |= t1;
                  }
                }
              else if (props < 2) {
                no5way = (cpt + PERIOD - XMOVE)->facts + 12 + 32;
                if (c >= 2) {
                  t2 = (newval >> (c - 2)) & 63;
                  no5way[t2] |= t1;
                  no5way[64 + t2] |= t1;
                  }
                else {
                  t2 = (newval << (2 - c)) & 63;
                  no5way[t2] |= t1;
                  no5way[64 + t2] |= t1;
                  }
                }
              else if (1) {
                }
              else if (props < 3) {
                if ((cpt + PERIOD/2)->protop < 3*MAXPROSTACK) {
                  if (c >= 2) {
                    (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop] = c - 2;
                    (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop + 1] = (1 << (c + props + 3)) - (1 << (c - 2));
                    (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop + 2] = newval;
                    (cpt + PERIOD/2)->protop += 3;
                    }
                  else {
                    (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop] = 0;
                    (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop + 1] = (1 << (c + props + 3)) - 1;
                    (cpt + PERIOD/2)->prostack[(cpt + PERIOD/2)->protop + 2] = newval;
                    (cpt + PERIOD/2)->protop += 3;
                    }
                  }
                }
              }
if (c == 32) printf("really big early %d %d %x %x\n", props, cpt->contop, xr0, newval);
            if (1) {
              xr0 = t1;
              }
            if (xr0) {
              xr0 >>= 2;
              if (!xr0) xr0 = valinc;
              }
            xr0loc = c;
            if (FLIP && !(((glist - cpt) + rownum - 1) % (PERIOD))) cpt->constack[cpt->contop] = 2*BUFFERIGHT + BITSWIDE - c - props - 1;
            else cpt->constack[cpt->contop] = c;
            cpt->constack[cpt->contop+1] = props;
            if (cpt->contop < 2*MAXCONSTACK) cpt->contop += 2;
            }
          else {
            if (!(cpt + PERIOD - 2*XMOVE)->passtwo) (cpt + PERIOD - 2*XMOVE)->firstpasstwo = newval;
            (cpt + PERIOD - 2*XMOVE)->passtwo = 1;
            pass2 = 1;
            }
          }
        no5way = cpt->facts + 12;
        noway = cpt->facts + 4;
        c = 8;
        if (!xr0) while (c--) {
          no5way[ 0 + 2*c + 0] = (noway[((c<<1)&7)  ] << 1) | noway[c] | (noway[  (c>>1)] >> 1);
          no5way[ 0 + 2*c + 1] = (noway[((c<<1)&7)+1] << 1) | noway[c] | (noway[  (c>>1)] >> 1);
          no5way[16 + 2*c + 0] = (noway[((c<<1)&7)  ] << 1) | noway[c] | (noway[4+(c>>1)] >> 1);
          no5way[16 + 2*c + 1] = (noway[((c<<1)&7)+1] << 1) | noway[c] | (noway[4+(c>>1)] >> 1);
          }
        if (!xr0 && pass1 && pass2) contramisses++;
/*
printf("\n");
*/
        }
      if (HYPEROW && !xr0) {
        if ((cpt + 1)->lowper) {
          t1 = cpt->rstuff[6];
          t2 = 0;
          c = BITSWIDE;
          while (c--) {
            t2 <<= 1;
            t2 |= (1 << BUFFERIGHT) & t1;
            t1 >>= 1;
            }
          if (((cpt->rstuff[6] == (cpt + 2)->rstuff[0]) && (HYPEROW >= 2) && (trownum == HYPEROW)) ||
              ((cpt->rstuff[6] > (cpt + 2)->rstuff[0]) ||
               (NOSYM && (cpt + 3)->symmetric && (cpt->rstuff[6] != (cpt + 2)->rstuff[0]) && (t2 > (cpt + 2)->rstuff[0])))) {
            xr0 = valinc;
            cpt->knockout = valinc;
            cpt->numdives++;
            (cpt + 1)->numddives++;
            cpt->constack[cpt->contop] = BUFFERIGHT;
            cpt->constack[cpt->contop+1] = props;
            if (cpt->contop < 2*MAXCONSTACK) cpt->contop += 2;
            }
          else if (cpt->rstuff[6] == (cpt + 2)->rstuff[0]) cpt->lowper = 1;
          else cpt->lowper = 0;
          }
        else cpt->lowper = 0;
        }
      else cpt->lowper = 0;
      if (!xr0 && !YMOVE && NOSYM) {
	if ((cpt + 1)->symmetric) {
          t1 = newval;
          t2 = 0;
          c = BITSWIDE;
          while (c--) {
            t2 <<= 1;
            t2 |= (1 << BUFFERIGHT) & t1;
            t1 >>= 1;
            }
          if ((t2 < newval) || (ASYMFORCE && (t2 == newval) && (trownum == ASYMROW))) {
            xr0 = valinc;
            cpt->knockout = valinc;
            cpt->numdives++;
            (cpt + 1)->numddives++;
            cpt->constack[cpt->contop] = BUFFERIGHT;
            cpt->constack[cpt->contop+1] = props;
            if (cpt->contop < 2*MAXCONSTACK) cpt->contop += 2;
            }
	  else if (t2 == newval) cpt->symmetric = 1;
	  else cpt->symmetric = 0;
	  }
	else cpt->symmetric = 0;
        }
      if ((!xr0) && (cpt == glist - DEPTH)) {
        statsb[1] += statsb[0];
        numhits++;
        newval = (glist - 1)->rstuff[0];
        nchild->value = newval;
        nchild->parent = onroad;
        (glist - 1)->loc = nchild;
        t1 = nchild - offroad;
        curdepths = (alldepths + t1)->rows;
        c = DEPTH;
        while (c--) {
          curdepths[c] = (glist - c - 1)->rstuff[0];
          if (c) (glist - c - 1)->rstuff[0] = 0;
          }
        nchild++;
        if (nchild == onroad + MAXTREEWIDTH) exit(0);
        if (nchild == allroads + ROADSTORE) exit(0);
        if (!cpt->rstuff[0]) {
          checkend = 0;
          c = 2*PERIOD;
          while (c--) checkend |= (cpt + c)->rstuff[0];
//          c = PERIOD;
//          while (c--) checkend |= (cpt + c)->facts[12];
          checkend |= (rownum + CLEARUP) & (1 << 31);
          if (MEMCOMP & 4) {
            checkend = 0;
            c = DEPTH - 1;
            while (c--) checkend |= (cpt + c)->rstuff[0];
            if (!(glist - 1)->rstuff[0]) checkend = 1;
            }
          if (!checkend && !(MEMCOMP & 4)) {
            if (PERIOD < DEPTH - 1) {
              c = 1;
              while (c < DEPTH - PERIOD) {
                (nchild + c - 1)->value = (glist - c - 1)->rstuff[0];
                (nchild + c - 1)->parent = nchild + c - 2;
                (glist - c - 1)->loc = nchild + c - 1;
                c++;
                }
              }
            printf("#P %d %d\n", xscreen, yscreen);
            if (NOSYM) xscreen += BITSWIDE + 6;
            else xscreen += 2*BITSWIDE + 6;
            if (xscreen > 300) {
              yscreen += rownum/PERIOD + 8;
              xscreen = 0;
              }
            ancestor = (cpt + PERIOD)->loc;
            c = (DEPTH + rownum - 1) % (PERIOD);
            while (c--) ancestor = ancestor->parent;
            while (ancestor != allroads) {
              t1 = ancestor->value;
              if (!t1) putchar('.');
              if (NOSYM && t1) {
                t1 >>= (BUFFERIGHT - EXRIGHT);
                }
              else if (t1) {
                t2 = 1 << (BUFFERIGHT + BITSWIDE + EXLEFT - 1);
                while (t2 > 1) {
                  if (t2 & t1) putchar('*');
                  else putchar('.');
                  t2 >>= 1;
                  }
                if (EVENSYM) {
                  if (t2 & t1) putchar('*');
                  else putchar('.');
                  }
                else if (ODDSYM);
                else {
                  if (t2 & t1) putchar('*');
                  else putchar('.');
                  putchar('.');
                  }
                }
              while (t1) {
                if (t1 & 1) putchar('*');
                else putchar('.');
                t1 >>= 1;
                }
              putchar('\n');
              c = PERIOD;
              while (c--) ancestor = ancestor->parent;
              }
            fflush(stdout);
            if (!(MEMCOMP & 4)) {
              if (CLEARAT) clearsum = (rownum + CLEARUP)/CLEARAT;
              else clearsum = CLEARUP;
              c = clearsum;
              tancestor = nchild - 1;
              while (c--) tancestor = tancestor->parent;
              ancestor = tancestor;
              while (tancestor == ancestor) {
                nchild--;
                c = clearsum;
                ancestor = nchild - 1;
                while (c--) ancestor = ancestor->parent;
                }
              if (clearsum) ancestor = tancestor;
              while (tancestor == ancestor) {
                onroad++;
                c = clearsum - 1;
                ancestor = onroad;
                while (c--) ancestor = ancestor->parent;
                }
              if (clearsum) {
                onroad--;
                (glist - 1)->rstuff[0] = valimit;
                }
              }
            }
          }
        trownum = rownum;
        cpt = glist - 1;
/*
        tempcpt = cpt;
        if (PERIOD > 4) while (tempcpt < glist + 3) {
          tempcpt->cprtop = 0;
          no5way = tempcpt->facts + 12;
          c = 32;
          while (c--) no5way[c+96] = no5way[c];
          tempcpt->knockout = valinc;
          tempcpt++;
          }
*/
        oldcpt = cpt;
        oldval = cpt->rstuff[0];
        newval = oldval + valinc;
        if (newval >= valimit) cpt = glist;
        }
      else if (!xr0) {
        if (contries >= 1000000000) {
          contries -= 1000000000;
          contriesb++;
          }
        if (conmissq >= 1000000000) {
          conmissq -= 1000000000;
          conmissqb++;
          }
        if (contramisses >= 1000000000) {
          contramisses -= 1000000000;
          contramissesb++;
          }
        if (contrits >= 1000000000) {
          contrits -= 1000000000;
          contritsb++;
          }
        if (numtries >= 1000000000) {
          numtries -= 1000000000;
          numtriesb++;
          }
        trownum++;
        rpto = (cpt + 1)->rstuff;
        rpt = cpt->rstuff;
        t2 = *rpt;
        if (FLIP && !(((glist - cpt) + rownum) % (PERIOD))) {
          t1 = t2;
          t2 = 0;
          c = BITSWIDE;
          while (c--) {
            t2 <<= 1;
            t2 |= t1 & (1 << BUFFERIGHT);
            t1 >>= 1;
            }
          }
        cpt->checker = t2;
        if (YMOVE && !(((glist - cpt) + rownum) % (PERIOD))) cpt->checker <<= 1;
        while (rpt < cpt->rstuff + 6*2) {
          rpt += 6;
          rpto += 6;
          }
        if (0 && (cpt->numdives)) {
          no5way = (cpt + 1)->facts + 12;
          c = 32;
          while (c--) no5way[c+96] = no5way[c];
          cpt->knockout = valinc;
          cpt->numdives++;
          (cpt + 1)->numddives++;
          t1 = cpt->lastdive ^ newval;
          cpt->lastdive = newval;
          cpt--;
          while (cpt->knockout & t1) cpt->knockout <<= 1;
          cpt->numdives = 0;
          cpt->numddives = 0;
          newval = 0;
          (cpt + PERIOD)->protop = 0;
          no5way = (cpt + PERIOD)->facts + 12;
          c = 64;
          while (c--) no5way[c+32] &= ~(t1 | (t1 >> 1));
          c = 32;
          while (c--) {
            no5way[c+32] |= no5way[96+c] | (no5way[96+(c>>1)] >> 1);
            no5way[c+64] |= no5way[96+c] | (no5way[96+16+(c>>1)] >> 1);
            }
          }
        else {
          (cpt + XMOVE)->cprtop = 0;
          no5way = (cpt + XMOVE)->facts + 12;
          c = 32;
//          while (c--) no5way[c+96] = no5way[c];
          while (c--) {
            no5way[c+96] = no5way[0+c] | (no5way[0+(c>>1)] >> 1);
            no5way[c+128] = no5way[0+c] | (no5way[0+16+(c>>1)] >> 1);
            }
          cpt->knockout = valinc;
          cpt->numdives++;
          (cpt + 1)->numddives++;
          cpt->lastdive = newval;
          cpt->passtwo = 0;
          cpt--;
          cpt->knockout = valimit;
//          cpt->knockout = valinc;
          cpt->onemask = 0;
          cpt->numdives = 0;
          cpt->numddives = 0;
          cpt->contop = 0;
          newval = cpt->rstuff[0];
          if (newval) {
            cpt->knockout = valinc;
            (cpt + PERIOD - 2*XMOVE)->passtwo = 1;
            (cpt + PERIOD - 2*XMOVE)->firstpasstwo = 0;
            }
          else if (SLOW && (cpt + PERIOD - 2*XMOVE)->passtwo) newval = (cpt + PERIOD - 2*XMOVE)->firstpasstwo;
          (cpt + PERIOD - XMOVE)->protop = 0;
          no5way = (cpt + PERIOD - XMOVE)->facts + 12;
          c = 64;
          while (c--) no5way[c+32] = no5way[c+96];
//          c = 32;
//          while (c--) {
//            no5way[c+32] = no5way[96+c] | (no5way[96+(c>>1)] >> 1);
//            no5way[c+64] = no5way[96+c] | (no5way[96+16+(c>>1)] >> 1);
//            }
          }
        }
      else {
        if (SLOWER && (!(((glist - cpt) + rownum) % (PERIOD)) || (VERYSLOW && !(((glist - cpt) + rownum + 1) % (PERIOD)))))
          t1 = ((xr0 >> 1) | valinc) >> 1;
        else t1 = ((xr0 >> 0) | valinc) >> 1;
        t1 |= t1 >> 1;
        t1 |= t1 >> 2;
        t1 |= t1 >> 4;
        t1 |= t1 >> 8;
        t1 |= t1 >> 16;
        if (newval < valimit) {
          if (FLIP && !(((glist - cpt) + rownum - 1) % (PERIOD))) {
            xr0 = 1 << (2*BUFFERIGHT + BITSWIDE - xr0loc - props - 1);
//printf("xr0loc %d %d %x %x %x %x %x\n", ((glist - cpt) + rownum - 1), xr0loc, xr0, t1, cpt->knockout, (cpt + 1)->rstuff[0], newval);
            if (xr0loc == 32) xr0 = valinc;
//            xr0 = valinc;
            }
          if (YMOVE && !(((glist - cpt) + rownum - 1) % (PERIOD))) xr0 >>= 1;
          xr0 >>= 1;
          if (xr0 < valinc) cpt->knockout = valinc;
          if (SLOW) {
            if (!pass2) xr0 = valimit;
            }
          else xr0 = valinc;
          c = valinc;
          while (c < cpt->knockout) {
            if (c & xr0) cpt->knockout = c;
//            if (!(~c & xr0)) cpt->knockout = c;
            else {
              xr0 &= ~c;
              c <<= 1;
              }
            }
          }
        oldval = newval;
        newval = (oldval & ~t1) + t1 + 1;
        while ((newval >= valimit) && (cpt < glist)) {
          if (!(cpt + PERIOD - 2*XMOVE)->passtwo) {
            c = 1;
            while ((cpt + c < glist) && (cpt + c < cpt + PERIOD - 2*XMOVE)) {
              (cpt + c)->rstuff[0] = valimit;
              c++;
              }
            }
          if (1 && !(cpt + PERIOD - 2*XMOVE)->passtwo && cpt->contop) {
            no5way = (cpt + 2*PERIOD - 3*XMOVE)->facts + 12;
            oldval = (cpt + PERIOD - 2*XMOVE)->rstuff[0];
            if (cpt->contop < 2*MAXCONSTACK) {
              c = cpt->contop;
              t1 = 32;
              t2 = 0;
//printf("contop %d %d %x %x %x\n", cpt->contop, cpt->constack[cpt->contop - 1], cpt->constack[cpt->contop - 2], oldval, (cpt + 2)->rstuff[0]);
//printf("contop more %x %x %x %x\n", t1, t2, t3, oldval);
              t3 = 0;
              while (c) {
                c -= 2;
                if (cpt->constack[c] < t1) t1 = cpt->constack[c];
                if ((cpt->constack[c] + cpt->constack[c+1]) > t2) t2 = cpt->constack[c] + cpt->constack[c+1];
                t3 |= (1 << (cpt->constack[c] + cpt->constack[c+1] + 1)) - (1 << cpt->constack[c]);
                }
              if (0) (cpt + PERIOD - 2*XMOVE - 1)->knockout = 1 << t1;
              if ((t2 - t1) < 6) {
                t3 >>= t1;
                if (YMOVE && !(((glist - cpt) + rownum - 1) % (PERIOD))) {
                  oldval >>= t1 - 1;
                  t2 = 1 << (t1 + 1);
                  }
                else {
                  oldval >>= t1;
                  t2 = 1 << (t1 + 2);
                  }
                c = 64;
                while (c--) if (!(t3 & (c ^ oldval))) no5way[32 + c] |= t2;
                }
              else if (1) {
                }
              else if ((cpt + PERIOD + 1)->cprtop < 3*MAXCPRSTACK) {
                if (YMOVE && !(((glist - cpt) + rownum - 1) % (PERIOD))) {
                  t1--;
                  t3 >>= 1;
                  }
                (cpt + PERIOD + 1)->cprstack[(cpt + PERIOD + 1)->cprtop] = t1 - 2;
                (cpt + PERIOD + 1)->cprstack[(cpt + PERIOD + 1)->cprtop + 1] = t3;
                (cpt + PERIOD + 1)->cprstack[(cpt + PERIOD + 1)->cprtop + 2] = oldval;
                (cpt + PERIOD + 1)->cprtop += 3;
                }
              }
            }
          else if (1 && SLOW && !cpt->numdives) {
            t3 = cpt->onemask;
            t3 = ((t3 << 1) | t3 | (t3 >> 1)) & ~nowayt[2];
//            t3 = ((t3 << 1) | t3 | (t3 >> 1));
            t1 = 0;
            while (!(t3 & (1 << t1))) t1++;
            t2 = 31;
            while (!(t3 & (1 << t2))) t2--;
            if ((t2 - t1) < 6) {
              no5way = (cpt + PERIOD)->facts + 12;
              oldval = (cpt + XMOVE)->rstuff[0];
              t3 >>= t1;
              if (YMOVE && !(((glist - cpt) + rownum - 1) % (PERIOD))) {
                oldval >>= t1 - 1;
                t2 = 1 << (t1 + 1);
                }
              else {
                oldval >>= t1;
                t2 = 1 << (t1 + 2);
                }
              c = 64;
              while (c--) if (!(t3 & (c ^ oldval))) no5way[32 + c] |= t2;
              }
            }
          cpt->rstuff[0] = 0;
          xr0 = 0;
          cpt++;
          oldcpt = cpt;
          trownum--;
          oldval = cpt->rstuff[0];
          newval = (oldval & ~((cpt - 1)->knockout - 1)) + (cpt - 1)->knockout;
//          newval = oldval + valinc;
          }
        }
      }
    onroad++;
    while (((onroad->parent - allroads) >= 100000000) && (onroad < offroad)) onroad++;
    }
  tempcpt = glist + 2*PERIOD;
  while (tempcpt > glist) {
    tempcpt->loc = allroads;
    tempcpt--;
    tempcpt->loc = allroads;
    newfacts = (tempcpt + 1)->facts;
    oldfacts = tempcpt->facts;
    while (oldfacts < tempcpt->facts + FACTDEPTH*(PERIOD + 1)*NUMFACTS) *newfacts++ = *oldfacts++;
    }
/*
  oldfacts = glist->facts + (PERIOD + 1)*NUMFACTS;
  newfacts = glist->facts;
  while (oldfacts < glist->facts + FACTDEPTH*(PERIOD + 1)*NUMFACTS) *newfacts++ = *oldfacts++;
  oldfacts = factemplate;
  while (newfacts < glist->facts + FACTDEPTH*(PERIOD + 1)*NUMFACTS) *newfacts++ = *oldfacts++;
*/
/*
  d = 32;
  while (d && !statsa[--d]);
  printf("#D");
  c = 0;
  while (c <= d) printf(" %d", statsa[c++]);
  printf("\n");
  d = 32;
  while (d && !statsb[--d]);
  printf("#D");
  c = 0;
  while (c <= d) printf(" %d", statsb[c++]);
  printf("\n");
*/
  t0 = MAXTREEWIDTH;
  t1 = (nchild - offroad);
  while (t1--) {
    t0--;
    c = DEPTH;
    while (c--) (alldepths + t0)->rows[c] = (alldepths + t1)->rows[c];
    }
  offroad = nchild;
  forcntpnt = forbearcnt;
  while (*forcntpnt != 0) forcntpnt++;
  printf("#D");
  while (forcntpnt-- > forbearcnt) {
    printf(" %d", *forcntpnt);
    if ((forcntpnt > forbearcnt) && (*forcntpnt == *(forcntpnt - 1))) {
      t0 = *forcntpnt;
      t1 = 1;
      while ((forcntpnt > forbearcnt) && (t0 == *(forcntpnt - 1))) {
        *forcntpnt = 0;
        forcntpnt--;
        t1++;
        }
      printf("x%d", t1);
      }
    *forcntpnt = 0;
    }
  printf("\n");
  rownum++;
  if (contrits >= 1000000000) {
    contrits -= 1000000000;
    contritsb++;
    }
  if (numtries >= 1000000000) {
    numtries -= 1000000000;
    numtriesb++;
    }
  if ((onroad < offroad) && (MEMCOMP & 4)) {
    sonroad = sortop;
    t0 = offroad - onroad;
    soffroad = sonroad + t0;
    while (t0--) sonroad[t0] = onroad + t0;
    t0 = 1;
    while (t0 < (offroad - onroad)) {
      t1 = 0;
      while (t1 < (offroad - onroad)) {
        t5 = t1;
        t3 = t1;
        t1 += t0;
        if (t1 > (offroad - onroad)) t1 = offroad - onroad;
        t2 = t1;
        t4 = t1;
        t1 += t0;
        if (t1 > (offroad - onroad)) t1 = offroad - onroad;
        while (t5 < t1) {
          if (t4 == t1) {
            soffroad[t5] = sonroad[t3];
            t3++;
            }
          else if (t3 == t2) {
            soffroad[t5] = sonroad[t4];
            t4++;
            }
          else {
            ancestor = sonroad[t3];
            tancestor = sonroad[t4];
            c = 2*PERIOD - 1;
            while (c && (ancestor->value == tancestor->value)) {
              c--;
              ancestor = ancestor->parent;
              tancestor = tancestor->parent;
              }
            if ((ancestor->value < tancestor->value) || ((ancestor->value == tancestor->value) && (ancestor->parent < tancestor->parent))) {
              soffroad[t5] = sonroad[t3];
              t3++;
              }
            else {
              soffroad[t5] = sonroad[t4];
              t4++;
              }
            }
          t5++;
          }
        }
      t1 = 0;
      while (t1 < (offroad - onroad)) {
        sonroad[t1] = soffroad[t1];
        t1++;
        }
      t0 <<= 1;
      }
    if (NOSYM) {
      t0 = offroad - onroad;
      sonroad = sortop + t0;
      soffroad = sonroad + t0;
      while (t0--) sonroad[t0] = onroad + t0;
      t0 = 1;
      while (t0 < (offroad - onroad)) {
        t1 = 0;
        while (t1 < (offroad - onroad)) {
          t5 = t1;
          t3 = t1;
          t1 += t0;
          if (t1 > (offroad - onroad)) t1 = offroad - onroad;
          t2 = t1;
          t4 = t1;
          t1 += t0;
          if (t1 > (offroad - onroad)) t1 = offroad - onroad;
          while (t5 < t1) {
            if (t4 == t1) {
              soffroad[t5] = sonroad[t3];
              t3++;
              }
            else if (t3 == t2) {
              soffroad[t5] = sonroad[t4];
              t4++;
              }
            else {
              ancestor = sonroad[t3];
              tancestor = sonroad[t4];
              c = 2*PERIOD - 1;
              while (c && (ancestor->value == tancestor->value)) {
                c--;
                ancestor = ancestor->parent;
                tancestor = tancestor->parent;
                }
              t8 = ancestor->value;
              t6 = 0;
              c = BITSWIDE;
              while (c--) {
                t6 <<= 1;
                t6 |= (1 << BUFFERIGHT) & t8;
                t8 >>= 1;
                }
              t8 = tancestor->value;
              t7 = 0;
              c = BITSWIDE;
              while (c--) {
                t7 <<= 1;
                t7 |= (1 << BUFFERIGHT) & t8;
                t8 >>= 1;
                }
              if ((t6 < t7) || ((ancestor->value == tancestor->value) && (ancestor->parent < tancestor->parent))) {
                soffroad[t5] = sonroad[t3];
                t3++;
                }
              else {
                soffroad[t5] = sonroad[t4];
                t4++;
                }
              }
            t5++;
            }
          }
        t1 = 0;
        while (t1 < (offroad - onroad)) {
          sonroad[t1] = soffroad[t1];
          t1++;
          }
        t0 <<= 1;
        }
      t0 = offroad - onroad;
      sonroad = sortop;
      soffroad = sonroad + t0;
      t1 = sortop - sortroads;
      t2 = 0;
      t3 = 0;
      t4 = 0;
      while ((t2 < t1) && (t4 < t0)) {
        ancestor = sortroads[t2];
        tancestor = soffroad[t4];
        reaches[tancestor - allroads] = 0;
        t8 = tancestor->value;
        t7 = 0;
        c = BITSWIDE;
        while (c--) {
          t7 <<= 1;
          t7 |= (1 << BUFFERIGHT) & t8;
          t8 >>= 1;
          }
        c = 2*PERIOD - 1;
        while (c && (ancestor->value == t7)) {
          c--;
          ancestor = ancestor->parent;
          tancestor = tancestor->parent;
          t8 = tancestor->value;
          t7 = 0;
          d = BITSWIDE;
          while (d--) {
            t7 <<= 1;
            t7 |= (1 << BUFFERIGHT) & t8;
            t8 >>= 1;
            }
          }
        if (ancestor->value == t7) {
          ancestor = sortroads[t2];
          tancestor = soffroad[t4];
          c = 2*PERIOD - 1;
          while (c && (ancestor->value == tancestor->value)) {
            c--;
            ancestor = ancestor->parent;
            tancestor = tancestor->parent;
            }
          if (ancestor->value != tancestor->value) {
            ancestor = sortroads[t2];
            tancestor = soffroad[t4];
            reaches[tancestor - allroads] = ancestor - allroads;
            }
          t4++;
          }
        else if (ancestor->value < t7) t2++;
        else t4++;
        }
      t0 = offroad - onroad;
      sonroad = sortop;
      soffroad = sonroad + t0;
      t1 = sortop - sortroads;
      t2 = 0;
      t3 = 0;
      t4 = 0;
      while ((t3 < t0) && (t4 < t0)) {
        ancestor = sonroad[t3];
        tancestor = soffroad[t4];
        t8 = tancestor->value;
        t7 = 0;
        c = BITSWIDE;
        while (c--) {
          t7 <<= 1;
          t7 |= (1 << BUFFERIGHT) & t8;
          t8 >>= 1;
          }
        c = 2*PERIOD - 1;
        while (c && (ancestor->value == t7)) {
          c--;
          ancestor = ancestor->parent;
          tancestor = tancestor->parent;
          t8 = tancestor->value;
          t7 = 0;
          d = BITSWIDE;
          while (d--) {
            t7 <<= 1;
            t7 |= (1 << BUFFERIGHT) & t8;
            t8 >>= 1;
            }
          }
        if (ancestor->value == t7) {
          ancestor = sonroad[t3];
          tancestor = soffroad[t4];
          c = 2*PERIOD - 1;
          while (c && (ancestor->value == tancestor->value)) {
            c--;
            ancestor = ancestor->parent;
            tancestor = tancestor->parent;
            }
          if (ancestor->value != tancestor->value) {
            ancestor = sonroad[t3];
            tancestor = soffroad[t4];
//            if ((ancestor < tancestor) && !reaches[tancestor - allroads])
//              printf("a flip %x %x %d %d\n", ancestor->value, tancestor->value, ancestor - allroads, tancestor - allroads);
            if ((ancestor < tancestor) && !reaches[tancestor - allroads]) reaches[tancestor - allroads] = ancestor - allroads;
            }
          t4++;
          }
        else if (ancestor->value < t7) t3++;
        else t4++;
        }
/*
*/
      }
    t1 = soffroad - sortroads;
    t2 = ROADSTORE - t1;
    sonroad = sortroads + t2;
    while (t1--) sortroads[t2 + t1] = sortroads[t1];
    t5 = 0;
    t3 = 0;
    t2 = sortop - sortroads;
    t4 = sortop - sortroads;
    t1 = soffroad - sortroads;
    soffroad = sortroads;
    soffroad[t5] = sonroad[t3];
    t5++;
    t3++;
//printf("trims %d %d %d %d %d\n", t1, t2, t3, t4, t5);
    while ((t4 < t1) || (t3 < t2)) {
      if (t4 == t1) {
        soffroad[t5] = sonroad[t3];
        t3++;
        }
      else if (t3 == t2) {
        soffroad[t5] = sonroad[t4];
        t4++;
        }
      else {
        ancestor = sonroad[t3];
        tancestor = sonroad[t4];
        c = 2*PERIOD - 1;
        while (c && (ancestor->value == tancestor->value)) {
          c--;
          ancestor = ancestor->parent;
          tancestor = tancestor->parent;
          }
        if ((ancestor->value < tancestor->value) || ((ancestor->value == tancestor->value) && (ancestor->parent < tancestor->parent))) {
          soffroad[t5] = sonroad[t3];
          t3++;
          }
        else {
          soffroad[t5] = sonroad[t4];
          t4++;
          }
        }
      ancestor = soffroad[t5];
      tancestor = soffroad[t5 - 1];
//if ((ancestor - allroads > 34261) && (ancestor - allroads < 34270)) printf("trimmedst %d %d %x   %d %d %x   %d\n",
//                                        ancestor - allroads, ancestor->parent - allroads, ancestor->value,
//                                        tancestor - allroads, tancestor->parent - allroads, tancestor->value, t5);
//printf("sorted %x %x %x %x %x  %d %d %d\n", sortroads[t5]->value, sortroads[t5]->parent->value,
//       sortroads[t5]->parent->parent->value, sortroads[t5]->parent->parent->parent->value, sortroads[t5]->parent->parent->parent->parent->value,
//       t5, sortroads[t5] - allroads, sortroads[t5]->parent - allroads);
      c = 2*PERIOD - 1;
      while (c && (ancestor->value == tancestor->value)) {
        c--;
        ancestor = ancestor->parent;
        tancestor = tancestor->parent;
        }
      if (ancestor->value == tancestor->value) {
        ancestor = soffroad[t5];
        tancestor = soffroad[t5 - 1];
//printf("trimmed %d %d %x   %d %d %x\n", ancestor - allroads, ancestor->parent - allroads, ancestor->value,
//                                        tancestor - allroads, tancestor->parent - allroads, tancestor->value);
        ancestor->value = tancestor - allroads;
        ancestor->parent += 100000000;
//printf("trimmed %d %d %x   %d %d %x   %d\n", ancestor - allroads, ancestor->parent - allroads, ancestor->value,
//                                            tancestor - allroads, tancestor->parent - allroads, tancestor->value, t5);
fflush(stdout);
        }
      else if (NOSYM && (soffroad[t5] > onroad) && reaches[soffroad[t5] - allroads]) {
        ancestor = soffroad[t5];
        ancestor->value = reaches[ancestor - allroads];
        ancestor->parent += 150000000;
        }
      else t5++;
      }
    sortop = sortroads + t5;
//printf("trims %d %d %d %d %d\n", t1, t2, t3, t4, t5);
    trims = t1 - t5;
    }
  inship = 0;
  inrealship = 0;
  if (MEMCOMP & 4) {
    t1 = onroad - allroads;
    t0 = offroad - allroads;
    t2 = onroad - allroads;
    while (t2 < t0) {
      reaches[t2] = rownum;
      t2++;
      }
    anychange = 1;
    while (anychange) {
      inship = 0;
      inrealship = 0;
      openship = 0;
      anychange = 0;
      t2 = 1;
      while (t2 < t0) {
        if (reaches[t2] & 0x7fff0000) inship++;
        if (reaches[t2] & 0x8000) inrealship++;
        if (!(reaches[t2] & 0x7fff0000) && (((allroads + t2)->parent - allroads) >= 100000000)) {
          if ((allroads + t2)->value == 0) {
            t4 = 1;
            }
          else {
            t4 = (reaches[(allroads + t2)->value] & 0x7fff0000) >> 16;
            if (t4 && ((t4 + (reaches[t2] & 0x7fff)) > (rownum + 1))) openship++;
            if (!t4 || ((t4 + (reaches[t2] & 0x7fff)) > (rownum + 1))) t4 = 0;
            }
          if (t4) {
            anychange = 1;
            t3 = t2;
            reaches[t3] |= (t4 << 16);
            shiplist[reaches[t3] & 0x7fff] = t3;
            t5 = t3;
            symflip = 0;
            if (((allroads + t3)->parent - allroads) < 150000000) t3 = (allroads + t3)->parent - allroads - 100000000;
            else {
              if (((allroads + t3)->parent - allroads - 150000000) ==
                  ((allroads + (allroads + t3)->value)->parent - allroads)) symflip = 1;
              t3 = (allroads + t3)->parent - allroads - 150000000;
              }
            t4++;
            while (t3 && !(reaches[t3] & 0x7fff0000)) {
              reaches[t3] |= (t4 << 16);
              shiplist[reaches[t3] & 0x7fff] = t3;
              dpaths[t3] = t5;
              t5 = t3;
              t3 = (allroads + t3)->parent - allroads;
              t4++;
              }
            while (t3) {
              shiplist[reaches[t3] & 0x7fff] = t3;
              t3 = (allroads + t3)->parent - allroads;
              }
            likelyflot = 0;
            t3 = t2;
            while (t3) {
              if (((allroads + t3)->parent - allroads) >= 100000000) t3 = (allroads + t3)->value;
              t3 = dpaths[t3];
              if (t3 && ((reaches[t3] & 0x7fff) < FLOTROW) && !likelyflot) likelyflot = rownum + 1 - ((reaches[t3] & 0x7fff0000) >> 16);
              shiplist[rownum + 1 - ((reaches[t3] & 0x7fff0000) >> 16)] = t3;
              }
            if (likelyflot) {
              t4 = shiplist[likelyflot];
              t3 = likelyflot + 2*PERIOD;
              while (t3 > likelyflot) {
                flotlist[t3] = 0;
                t3--;
                }
              flipon = 0;
              while (t3) {
                if (((allroads + shiplist[t3])->parent - allroads) >= 100000000)
                  t1 = (allroads + (allroads + shiplist[t3])->value)->value;
                else t1 = (allroads + shiplist[t3])->value;
                if (flipon) {
                  t8 = t1;
                  t1 = 0;
                  d = BITSWIDE;
                  while (d--) {
                    t1 <<= 1;
                    t1 |= (1 << BUFFERIGHT) & t8;
                    t8 >>= 1;
                    }
                  }
                flotlist[t3] = (allroads + t4)->value ^ t1;
                t4 = (allroads + t4)->parent - allroads;
                if (((allroads + shiplist[t3])->parent - allroads) >= 150000000) {
                  if (flipon) flipon = 0;
                  else flipon = 1;
                  }
                t3--;
                }
              contra = 0;
              t3 = likelyflot + PERIOD;
              while (!contra && (t3 > PERIOD)) {
                r0 = flotlist[t3 + PERIOD];
                t1 = r0 >> 1;
                t4 = r0 << 1;
                if (ODDSYM) t4 |= t1 & 1;
                if (EVENSYM) t4 |= r0 & 1;
                surrounds[0] = r0;
                surrounds[1] = t1;
                surrounds[2] = t4;
                r0 = flotlist[t3 - PERIOD];
                t1 = r0 >> 1;
                t4 = r0 << 1;
                if (ODDSYM) t4 |= t1 & 1;
                if (EVENSYM) t4 |= r0 & 1;
                surrounds[3] = r0;
                surrounds[4] = t1;
                surrounds[5] = t4;
                r0 = flotlist[t3];
                t1 = r0 >> 1;
                t4 = r0 << 1;
                if (ODDSYM) t4 |= t1 & 1;
                if (EVENSYM) t4 |= r0 & 1;
                surrounds[6] = t1;
                surrounds[7] = t4;
                c = 4;
                while (c--) sigs[c] = 0;
                c = 8;
                while (c--) {
                  carryin = surrounds[c];
                  d = 0;
                  while (d < 4) {
                    carryout = sigs[d] & carryin;
                    sigs[d] ^= carryin;
                    carryin = carryout;
                    d++;
                    }
                  }
                contra = flotlist[t3 + XMOVE] ^ (sigs[1] & ~sigs[2] & (sigs[0] | r0));
//printf("likelyflot %d %4x %4x %4x %4x %4x %4x\n", t3, flotlist[t3 + XMOVE], r0, sigs[3], sigs[2], sigs[1], sigs[0]);
                t3--;
                }
              if (contra) likelyflot = 0;
              }
            if (symflip) likelyflot = 1;
            if (!likelyflot) {
              urows = 0;
              t3 = rownum + 1;
              while (t3--) if (shiplist[t3]) {
                if (!(reaches[shiplist[t3]] & 0x8000)) urows++;
                reaches[shiplist[t3]] |= 0x8000;
                }
              if (urows > UNIQUE) {
                printf("#P %d %d\n", xscreen, yscreen);
                if (NOSYM) xscreen += BITSWIDE + 6;
                else xscreen += 2*BITSWIDE + 6;
                if (xscreen > 300) {
                  yscreen += rownum/PERIOD + 8;
                  xscreen = 0;
                  }
                flipon = 0;
                t3 = 1;
                while (t3 < rownum) {
                  if (((allroads + shiplist[t3])->parent - allroads) >= 100000000)
                    t1 = (allroads + (allroads + shiplist[t3])->value)->value;
                  else t1 = (allroads + shiplist[t3])->value;
                  if (flipon) {
                    t8 = t1;
                    t1 = 0;
                    d = BITSWIDE;
                    while (d--) {
                      t1 <<= 1;
                      t1 |= (1 << BUFFERIGHT) & t8;
                      t8 >>= 1;
                      }
                    }
                  if (!t1) putchar('.');
                  if (NOSYM && t1) {
                    t1 >>= (BUFFERIGHT - EXRIGHT);
                    }
                  else if (t1) {
                    t2 = 1 << (BUFFERIGHT + BITSWIDE + EXLEFT - 1);
                    while (t2 > 1) {
                      if (t2 & t1) putchar('*');
                      else putchar('.');
                      t2 >>= 1;
                      }
                    if (EVENSYM) {
                      if (t2 & t1) putchar('*');
                      else putchar('.');
                      }
                    else if (ODDSYM);
                    else {
                      if (t2 & t1) putchar('*');
                      else putchar('.');
                      putchar('.');
                      }
                    }
                  while (t1) {
                    if (t1 & 1) putchar('*');
                    else putchar('.');
                    t1 >>= 1;
                    }
                  putchar('\n');
                  t4 = t3 + PERIOD;
                  while (t3 < t4) {
                    t3++;
                    if (((allroads + shiplist[t3])->parent - allroads) >= 150000000) {
                      if (flipon) flipon = 0;
                      else flipon = 1;
                      }
                    }
                  }
                fflush(stdout);
                }
              }
            }
          }
        t2++;
        }
      }
    }
  printf("#D %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", rownum, numtriesb, numtries, contriesb, contries,
         conmissqb, conmissq, contritsb, contrits, contramissesb, contramisses,
         sortop - sortroads, inrealship, inship, openship, offroad - allroads, trims, numhits);
  ancestor = onroad;
  if (TREEOUT) while (ancestor < offroad) {
    printf("#N %x %d\n", ancestor->value, onroad - ancestor->parent);
    ancestor++;
    }
  fflush(stdout);
  if ((onroad < offroad) && (MEMCOMP & 1)) {
    tancestor = onroad;
    nchild = offroad - 1;
    ancestor = tancestor;
    while (nchild->parent > allroads) {
      if (nchild->parent == ancestor) {
        nchild->parent = tancestor;
        nchild--;
        }
      else {
        ancestor = nchild->parent;
        tancestor--;
        tancestor->value = nchild->parent->value;
        tancestor->parent = nchild->parent->parent;
        nchild->parent = tancestor;
        nchild--;
        }
      }
    ancestor = allroads + 1;
    nchild = tancestor;
    while (nchild->parent == allroads) {
      ancestor->value = nchild->value;
      ancestor->parent = nchild->parent;
      ancestor++;
      nchild++;
      }
    while (nchild < offroad) {
      ancestor->value = nchild->value;
      ancestor->parent = nchild->parent - (nchild - ancestor);
      ancestor++;
      nchild++;
      }
    onroad = onroad - (nchild - ancestor);
    offroad = ancestor;
    nchild = offroad;
    }
  if ((onroad < offroad) && (MEMCOMP & 2)) {
    t0 = 1;
    while (t0 < (offroad - onroad)) {
      t1 = 0;
      while (t1 < (offroad - onroad)) {
        t5 = t1;
        t3 = t1;
        t1 += t0;
        if (t1 > (offroad - onroad)) t1 = offroad - onroad;
        t2 = t1;
        t4 = t1;
        t1 += t0;
        if (t1 > (offroad - onroad)) t1 = offroad - onroad;
        while (t5 < t1) {
          if (t4 == t1) {
            ancestor = onroad + t3;
            tancestor = offroad + t5;
            tancestor->value = ancestor->value;
            tancestor->parent = ancestor->parent;
            t3++;
            }
          else if (t3 == t2) {
            ancestor = onroad + t4;
            tancestor = offroad + t5;
            tancestor->value = ancestor->value;
            tancestor->parent = ancestor->parent;
            t4++;
            }
          else {
            ancestor = onroad + t3;
            tancestor = onroad + t4;
            c = 2*PERIOD - 1;
            while (c && (ancestor->value == tancestor->value)) {
              c--;
              ancestor = ancestor->parent;
              tancestor = tancestor->parent;
              }
            if ((ancestor->value < tancestor->value) || ((ancestor->value == tancestor->value) && (ancestor->parent < tancestor->parent))) {
              ancestor = onroad + t3;
              tancestor = offroad + t5;
              tancestor->value = ancestor->value;
              tancestor->parent = ancestor->parent;
              t3++;
              }
            else {
              ancestor = onroad + t4;
              tancestor = offroad + t5;
              tancestor->value = ancestor->value;
              tancestor->parent = ancestor->parent;
              t4++;
              }
            }
          t5++;
          }
        }
      t1 = 0;
      while (t1 < (offroad - onroad)) {
        tancestor = onroad + t1;
        ancestor = offroad + t1;
        tancestor->value = ancestor->value;
        tancestor->parent = ancestor->parent;
        t1++;
        }
      t0 <<= 1;
      }
    t2 = 0;
    t1 = 0;
    ancestor = onroad + t1;
    tancestor = offroad + t2;
    tancestor->value = ancestor->value;
    tancestor->parent = ancestor->parent;
    t2++;
    t1++;
    while (t1 < (offroad - onroad)) {
      ancestor = onroad + t1;
      tancestor = offroad + t2;
      tancestor->value = ancestor->value;
      tancestor->parent = ancestor->parent;
      ancestor = onroad + t1 - 1;
      tancestor = onroad + t1;
      c = 2*PERIOD - 1;
      while (c && (ancestor->value == tancestor->value)) {
        c--;
        ancestor = ancestor->parent;
        tancestor = tancestor->parent;
        }
      if (ancestor->value != tancestor->value) t2++;
/*
      else {
        ancestor = onroad + t1 - 1;
        printf("#P %d 0\n", rownum);
        while (ancestor != allroads) {
          t3 = ancestor->value >> BUFFERIGHT;
          if (!t3) putchar('.');
          while (t3) {
            if (t3 & 1) putchar('*');
            else putchar('.');
            t3 >>= 1;
            }
          putchar('\n');
          c = PERIOD;
          while (c--) ancestor = ancestor->parent;
          }
        ancestor = onroad + t1;
        printf("#P %d 0\n", rownum);
        while (ancestor != allroads) {
          t3 = ancestor->value >> BUFFERIGHT;
          if (!t3) putchar('.');
          while (t3) {
            if (t3 & 1) putchar('*');
            else putchar('.');
            t3 >>= 1;
            }
          putchar('\n');
          c = PERIOD;
          while (c--) ancestor = ancestor->parent;
          }
        }
*/
      t1++;
      }
    t1 = t2;
    while (t1--) {
      tancestor = onroad + t1;
      ancestor = offroad + t1;
      tancestor->value = ancestor->value;
      tancestor->parent = ancestor->parent;
      }
    trims = (offroad - onroad) - t2;
    offroad = onroad + t2;
    t0 = 1;
    while (t0 < (offroad - onroad)) {
      t1 = 0;
      while (t1 < (offroad - onroad)) {
        t5 = t1;
        t3 = t1;
        t1 += t0;
        if (t1 > (offroad - onroad)) t1 = offroad - onroad;
        t2 = t1;
        t4 = t1;
        t1 += t0;
        if (t1 > (offroad - onroad)) t1 = offroad - onroad;
        while (t5 < t1) {
          if (t4 == t1) {
            ancestor = onroad + t3;
            tancestor = offroad + t5;
            tancestor->value = ancestor->value;
            tancestor->parent = ancestor->parent;
            t3++;
            }
          else if (t3 == t2) {
            ancestor = onroad + t4;
            tancestor = offroad + t5;
            tancestor->value = ancestor->value;
            tancestor->parent = ancestor->parent;
            t4++;
            }
          else {
            ancestor = onroad + t3;
            tancestor = onroad + t4;
            if ((ancestor->parent < tancestor->parent) || ((ancestor->parent == tancestor->parent) && (ancestor->value < tancestor->value))) {
              ancestor = onroad + t3;
              tancestor = offroad + t5;
              tancestor->value = ancestor->value;
              tancestor->parent = ancestor->parent;
              t3++;
              }
            else {
              ancestor = onroad + t4;
              tancestor = offroad + t5;
              tancestor->value = ancestor->value;
              tancestor->parent = ancestor->parent;
              t4++;
              }
            }
          t5++;
          }
        }
      t1 = 0;
      while (t1 < (offroad - onroad)) {
        tancestor = onroad + t1;
        ancestor = offroad + t1;
        tancestor->value = ancestor->value;
        tancestor->parent = ancestor->parent;
        t1++;
        }
      t0 <<= 1;
      }
    nchild = offroad;
    }
  }
c = MAXROW;
while (c--) forbearcnt[c] = 0;
c = offroad - allroads;
while (c--) if (reaches[c] & 0x8000) forbearcnt[reaches[c] & 0x7fff]++;
printf("#D 1");
c = 0;
while (forbearcnt[++c] != 0) printf(" %d", forbearcnt[c]);
printf("  %d\n", c);
c = MAXROW;
while (c--) forbearcnt[c] = 0;
c = offroad - allroads;
while (c--) if (reaches[c] & 0x8000) forbearcnt[(reaches[c] >> 16) & 0x7fff]++;
printf("#D 1");
c = 0;
while (forbearcnt[++c] != 0) printf(" %d", forbearcnt[c]);
printf("  %d\n", c);
if (!(xscreen | yscreen) || TREEOUT) printf("#P %d 0\n", -(BITSWIDE + 10));
ancestor = firsthit;
if ((ancestor->parent - allroads) >= 100000000) {
  ancestor = ancestor->parent - 100000000;
  rownum--;
  }
while ((rownum - 2) % (PERIOD)) {
  ancestor = ancestor->parent;
  rownum--;
  }
if (!(xscreen | yscreen) || TREEOUT) while (ancestor != allroads) {
              t1 = ancestor->value;
              if (!t1) putchar('.');
              if (NOSYM && t1) {
                t1 >>= (BUFFERIGHT - EXRIGHT);
                }
              else if (t1) {
                t2 = 1 << (BUFFERIGHT + BITSWIDE + EXLEFT - 1);
                while (t2 > 1) {
                  if (t2 & t1) putchar('*');
                  else putchar('.');
                  t2 >>= 1;
                  }
                if (EVENSYM) {
                  if (t2 & t1) putchar('*');
                  else putchar('.');
                  }
                else if (ODDSYM);
                else {
                  if (t2 & t1) putchar('*');
                  else putchar('.');
                  putchar('.');
                  }
                }
              while (t1) {
                if (t1 & 1) putchar('*');
                else putchar('.');
                t1 >>= 1;
                }
              putchar('\n');
              c = PERIOD;
              while (c--) ancestor = ancestor->parent;
  }
exit(0);
}

