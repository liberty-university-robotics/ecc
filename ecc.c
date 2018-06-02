#include <stdio.h>
#include <math.h> //cc -lm ecc.c
int outfd=0;

typedef int bit;

typedef struct {
	bit d[4];
	bit c[8];
	bit r[4];
	int valid;
} code;

bit G[8][4] = {
{1, 1, 0, 1}, 
{1, 0, 1, 1},
{1, 0, 0, 0}, 
{0, 1, 1, 1},
{0, 1, 0, 0},
{0, 0, 1, 0},
{0, 0, 0, 1},
{1, 1, 1, 0}
};

bit H[4][8] = {
{1, 0, 1, 0, 1, 0, 1, 0},
{0, 1, 1, 0, 0, 1, 1, 0},
{0, 0, 0, 1, 1, 1, 1, 0},
{1, 1, 1, 1, 1, 1, 1, 1}
};


void enc(code *c)
{
	int i,j;
	for(i=0;i<8;i++){
		for(j=0;j<4;j++){
			c->c[i]=c->c[i]^(c->d[j]&G[i][j]);
		}
	}
}
int dec(code *c)
{
	int i,j,r;	
	for(i=0;i<4;i++){
		for(j=0;j<8;j++){
			c->r[i]^=c->c[j]^H[i][j];
		}
	}
	r=c->r[0]+c->r[1]*2+c->r[2]*4;
	c->d[r]^=1;
	c->valid=!((!!r)+c->r[3]);
	return !(c->valid);
}
int bitn(int n)
{
	return 1<<n;
}
unsigned char frombits(bit*d)
{
	return d[0]+2*d[1]+4*d[2]+8*d[3];
}
unsigned char tobyte(bit *d)
{
	return d[0]+2*d[1]+4*d[2]+8*d[3] +
	16*d[4]+32*d[5]+64*d[6]+128*d[7];
}
void tobits(bit *d, unsigned int a,int o)
{
	int i=0;
	for(i=0;i<4;i++){
		d[i]+=a&bitn(i+o);
	}
}
void split(char d, code *l, code *h)
{
	tobits(l->d,d,0);
	tobits(h->d,d,1);
}
unsigned char unsplit(code h, code l)
{
	return frombits(l.d) + frombits(h.d)*16;
}
void pack(unsigned char *d,int len, unsigned char *b)
{
	int i=0;
	code l,h;
	for(i=0;i<len;i++){
		l = (const code){0};
		h = (const code){0};
		split(d[i],&l,&h);
		enc(&h);
		enc(&l);
		b[i*2]  =tobyte(l.c);
		b[i*2+1]=tobyte(h.c);
	}
}
int unpack(unsigned char *d, int len, unsigned char *b)
{
	int i=0;
	code l,h;
	int inv=0;
	for(i=0;i<len;i++){
		l = (const code){0};
		h = (const code){0};
		tobits(&l.c,b[i*2],0);
		tobits(&l.c+4,b[i*2],4);
		tobits(&h.c,b[i*2+1],0);
		tobits(&h.c+4,b[i*2+1],4);
		inv+=dec(&h);
		inv+=dec(&l);
		d[i]=unsplit(h,l);
	}
	return inv;
}
int main()
{
	char buff[26]={0};
	char *t="hello world";
	char rt[13]={0};
	pack(t,6,buff);
//	printf("%d\n", unpack(rt,6,buff));
//	printf("%s\n",rt);
	return 0;
}
