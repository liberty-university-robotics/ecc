#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//"constants"
#define k 4
#define n 8

	//Matrix constants are zero when you run off the ends.
int ai[] = {0,1,1,1,1,0,1,0,1,1,0,0,1,0,0,1};
int Ai(int i,int j) {return (i<k&&j<n-k)?ai[i+(k*j)]:0;}//k x n-k
int A(int i,int j) {return i<k&&j<n-k?(i==j?0:1):0;}//k x n-k
int G(int i,int j) {return j<k?(j==i?1:0):((2+A(j-k,i))%2);}//k x n
int H(int i,int j) {return j<n-k?Ai(i,j):i==(j-k)?1:0;}//n-k x n

//vector stuff
//ANSI C allows structures to be returned by value but not arrays. 
typedef struct { int v[n]; int b;} word; //by using this type we can use the stack for memory management.
int *b(word *v, int i){if(i<0||i>n){printf("index error\n");exit(1);}return &(v->v[i]);}
void dup(word a,word b) {memcpy(b.v,a.v,sizeof(int)*n);}
int  weq(word a,word c){int i;for(i=0;i<n;i++)if(*b(&a,i)!=*b(&c,i)) return 0;return 1;}
	//left multiply op with v
word wap(word v, int(*op)(int,int)){int i,j;word r={0}; for(i=0;i<n;i++)for(j=0;j<n;j++) *b(&r,j)=((*b(&v,i)*op(i,j))+*b(&r,j))%2;return r;}
	//right multiply op with v'
word warp(word v, int(*op)(int,int)){int i,j;word r={0}; for(i=0;i<n;i++)for(j=0;j<n;j++) *b(&r,i)=((*b(&v,j)*op(i,j))+*b(&r,i))%2;return r;}

void pop(int(*op)(int,int)){int i,j;for(i=0;i<n;i++)for(j=0;j<n;j++)printf("%d,%c",op(i,j),j==n-1?'\n':' ');}
void pv(word v){int i;for(i=0;i<n;i++) printf("%d ",*b(&v,i));}
int w2i(word v){int i,r;for(i=r=0;i<n;i++)r=r+((*b(&v,i))*(1<<i));return r;}
int mw2i(word v){int i,r;for(i=r=0;i<k;i++)r=r+((*b(&v,i))*(1<<i));return r;}
int wu2i(word v){ int i,r;for(i=r=0;i<n;i++)r+=(*b(&v,i));return r;}
word i2w(int r){int i; word v={0};for(i=0;i<n;i++)*b(&v,i)=((r&(1<<i))?1:0);;return v;}

//ecc stuff
word enc(word a){return wap(a,G);}
word syn(word a){return warp(a,H);}
word dec(word a){int f=wu2i(syn(a));a.b=f<n-k?0:1;*b(&a,(f)%(n+1))=(*b(&a,(f)%(n+1))+1)%2;return a;}

//fun
void tpmat(void) {
	printf("A=\n");
	pop(A);
	printf("Ai=\n");
	pop(Ai);
	printf("G=\n");
	pop(G);
	printf("H=\n");
	pop(H);
}
void ted(void){
	int i;
	printf("S should be zero unless there's an error\n");
	for(i=0;i<(1<<k);i++){
		word v=i2w(i);
		printf("%d\t",i);
		printf("w:"); pv(v); printf(" e:");pv(enc(v));printf(" s:");pv(syn(enc(v)));printf(" ed:");pv(dec(enc(v)));
		printf("\n");
	}
}
void tes(void){
	int i;
	for(i=0;i<n;i++){
		printf("%d\t",i);
		printf("s:");pv(syn(i2w(1<<i)));
		printf("\n");
	}
}
int main(int argc, char **argv) {
//	tpmat();
	ted();
	tes();
	return 0;
}

