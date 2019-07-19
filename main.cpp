#include <iostream>
#include  <iomanip>
#include <cmath>
using namespace std;

#define        DBG                1
#define        DRAM_SIZE        (32)
#define        CACHE_SIZE        (8)
//#define        pa               (5)                //physical address
#define        blocksize        (4)                  // not constant

//int M[DRAM_SIZE];

enum cacheResType {MISS=0, HIT=1};
struct cache
{
    int v;
    int tag;
    int index;
    int offset;
};
cache C[CACHE_SIZE];
/* The following implements a random number generator */
unsigned int m_w = 0xABCCAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05786902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
//    cout << m_w << "   " <<( m_z << 16) << endl;
    
    return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGenA()
{
    static unsigned int addr=0;
    cout << addr << endl;
    return (addr++)%(DRAM_SIZE/4);
}

unsigned int memGenB()
{
    static unsigned int addr=0;
    return  rand_()%(64*1024);
}

unsigned int memGenC()
{
    static unsigned int a1=0, a0=0;
    a0++;
    if(a0 == 512) { a1++; a0=0; }
    if(a1 == 128) a1=0;
    //cout << a1 << "            " << a0 << endl;
    return(a1 + a0*128);
}

unsigned int memGenD()
{
    static unsigned int addr=0;
    return (addr++)%(1024*4);
}

unsigned int memGenE()
{
    static unsigned int addr=0;
    return (addr++)%(1024*16);
}

unsigned int memGenF()
{
    static unsigned int addr=0;
    return (addr+=64)%(128*1024); // increase by 64 :4-64 offset is small index is large
}
unsigned int menGenT ()
{
    int A[12] = { };

    static unsigned int x=0;    // addr =A[x++];
    return (A[x++]);
}
unsigned int menGenT2 ()
{
    int A[8] = {22, 26, 22, 26, 16, 3, 16, 18};
    
    static unsigned int x=0;    // addr =A[x++];
    return (A[x++]);
}

int binrep(int ci)
{
    int num = ci;
    int dec_value = 0;
    
    // Initializing base value to 1, i.e 2^0
    int base = 1;
    
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        
        dec_value += last_digit * base;
        
        base = base * 2;
    }
    
    return dec_value;
}
// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr, int offset, int tag, int cacheindex, int blockindex)
{
    // This functon accepts the memory address for the memory transaction and
    // returns whether it caused a cache miss or a cache hit
    // The current implementation assumes there is no cache; so, every transaction is a miss
    int byteperblock = pow(2, offset);
    offset = addr % byteperblock;
    int numoflines = CACHE_SIZE/blocksize;
    cacheindex = (addr /byteperblock) %  numoflines;
    tag = (addr/ ( numoflines* byteperblock));
    int val = (cacheindex);
//    cout << val << endl;
   // int tag2 = C[val].tag;
    //    int tag2 = (check/ ( numoflines* byteperblock));
    if ((C[val].v ==1)&&(C[val].tag == tag))
    {
        return HIT;
    }
    else
    {
        int bindex = binrep(addr>> offset);
        //        int check2 = M[bindex];
     //   C[val].index=bindex;
        C[val].tag = addr/ ( numoflines* byteperblock);
        C[val].v =1;
        C[val].offset = addr % byteperblock;
        return MISS;
    }
}

char *msg[2] = {"Miss","Hit"};
#define        NO_OF_Iterations    1000000     // CHange to 1,000,000
int main()
{
   // int A[12] = { 0, 4, 16, 132, 232, 160, 1024, 30, 140, 3100, 180, 2180};

    int nummiss=0;
    for (int i = 0; i < CACHE_SIZE; i++)
        C[i].v = 0;
    int offset = log2(blocksize);
    int numblock = ceil (DRAM_SIZE/blocksize);
    int cacheaddress = log2(CACHE_SIZE);
    int tag;
    int blockindex = log2(numblock);
    int cacheindex = cacheaddress - offset;
    float hit = 0;
    cacheResType r;
    unsigned int addr;
    cout << "Direct Mapped Cache Simulator\n";
    //    for (int j =0; j< DRAM_SIZE; j++)
    //    {
    //         addr = memGenD();
    //        M[j] = addr;
    //    }
    for(int inst=0;inst<8;inst++)
    {
        addr = menGenT2();
       // cout << addr << endl;
        //        tag = addr >> cacheaddress;
        r = cacheSimDM(addr, offset, tag, cacheindex, blockindex);
        if(r == HIT)
            hit++;
        else
            nummiss ++;
//       cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[r] <<")\n";
    }
//    cout << "memGenA" << blocksize << endl;
//    cout << "Hit ratio = " << (100*hit/NO_OF_Iterations)/100<< endl;
        cout << "Hit ratio = " << (100*hit/8)/100<< endl;
//    cout << "HIT    " << hit << endl;
    cout << "Miss   " << nummiss << endl;
}
