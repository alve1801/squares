#define OLC_PGE_APPLICATION
#include <stdio.h>
#include "olc.h"
#define height 50
#define width 80

char*font;
bool terrain[height*width];

char getindex(int x,int y){
	// 1 2
	// 8 4
	return
	terrain[(x  )*width+y  ]*0x1 |
	terrain[(x  )*width+y+1]*0x2 |
	terrain[(x+1)*width+y+1]*0x4 |
	terrain[(x+1)*width+y  ]*0x8;
}

class Game : public olc::PixelGameEngine{
	public:Game(){sAppName="Game";}public:bool
	OnUserCreate()override{return(1);}

	olc::Pixel sga(short color){
	int r=(color&0x0f00)<<4;
	int g=(color&0x00f0)<<8;
	int b=(color&0x000f)<<12;
	r=r|r>>4|r>>8|r>>12;
	g=g|g>>4|g>>8|g>>12;
	b=b|b>>4|b>>8|b>>12;
	return olc::Pixel(r,g,b);}

	// XXX
	void pc(char c,int sx,int sy,short fgc,short bgc){
		olc::Pixel fg=sga(fgc);
		olc::Pixel bg=sga(bgc);
		for(int x=0;x<8;x++)
			for(int y=0;y<8;y++)
				Draw(
					sy*8+y,
					sx*8+x,
					((char)font[(unsigned char)c*8+x]&(char)(128>>y))?fg:bg
				);
		return;
	}

	void render(int mx,int my){
		for(int x=0;x<height-1;x++)
		for(int y=0;y<width-1;y++)
		pc(getindex(x,y),x,y,0x0fff,0x0000);

		for(int x=-2;x<2;x++)
		for(int y=-2;y<2;y++)
		Draw(mx*8+x,my*8+y,sga(0x0f00));
	}

	bool OnUserUpdate(float fElapsedTime)override{
		int x=(GetMouseX()+4)/8,y=(GetMouseY()+4)/8;
		if(GetMouse(0).bHeld)terrain[y*width+x]=1;
		if(GetMouse(1).bHeld)terrain[y*width+x]=0;
		if(GetKey(olc::ESCAPE).bPressed)exit(0);
		render(x,y);
		return true;
	}
};

int main(){
	// import font
	font=(char*)malloc(1<<7);
	FILE*f=fopen("bitmap","r");
	for(int i=0;i<128;i++)
		*(font+i)=getc(f);
	fclose(f);

	// XXX initialize terrain
	//for(int i=0;i<height*width;i++)terrain[i]=((char*)main)[i/8]&(1<<i);
	f=fopen("i","r");
	for(int i=0;i<13;i++)getc(f); // XXX
	for(int i=0;i<height*width;i++)terrain[i]=getc(f)>127;

	// olcpge main loop
	Game game;if(game.Construct(width*8, height*8, 2, 2))game.Start();return(0);
}

/*
some structure to keep track of terrain
	either finite or procgen'd
	make it finite for now

also we can interact w/ it
	w/ mouse no less

*/
