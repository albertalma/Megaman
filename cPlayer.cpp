
#include "cPlayer.h"
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

cPlayer::cPlayer() {
	engine = createIrrKlangDevice();
}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	float size = 1.0f / 14.0f;


	switch(GetState())
	{
		//1
		case STATE_LOOKLEFT:	
			xo = 13.0f * size;	yo = 1.0f*size;
			break;
		//4
		case STATE_LOOKRIGHT:	
			xo = 13.0f * size; yo = 0.0f*size;
			break;
		//1..3
		case STATE_WALKLEFT:	
			xo = (11.0f * size) - (GetFrame()* size);	yo = 1.0f*size;
			NextFrame(11);
			break;
		//4..6
		case STATE_WALKRIGHT:	
			xo = (size*0.0f)+(GetFrame()* size); yo = 0.0f;
			NextFrame(11);
			break;
		case STATE_JUMP_UP_RIGHT:
			xo = (size * 11.0f); yo = 0;
			NextFrame(7);
			break;
		case STATE_JUMP_UP_LEFT:
			xo = (size * 11.0f); yo = size;
			NextFrame(7);
			break;
		case STATE_FALLING_RIGHT:
			xo = (size * 12.0f); yo = 0;
			NextFrame(7);
			break;
		case STATE_FALLING_LEFT:
			xo = (size * 12.0f); yo = size;
			NextFrame(7);
			break;
		case STATE_HITED:
			xo = (size * 7.0f) + (GetFrame()* size); yo = 5.0f*size;
			NextFrame(11);
			break;

		case STATE_JUMP_HIT_LEFT:
			xo = (size * 6.0f) - (GetFrame()* size); yo = size*5.0f;
			NextFrame(3);
			break;
		case STATE_JUMP_HIT_RIGHT:
			xo = (size * 7.0f) + (GetFrame()* size); yo = size*5.0f;
			NextFrame(3);
			break;


	}
	xf = xo + size;
	yf = yo + size;

	DrawRect(tex_id,xo,yf,xf,yo);
}
void cPlayer::DrawShot(int tex_id, bool isRight)
{
	float xo, yo, xf, yf;
	xo = 0.0f + (GetFrame()*0.25f); 
	NextFrame(4);
	xf = xo + 0.25f;
	yo = 1.0f;
	yf = 0.0f;
	DrawShotRect(tex_id, xo, yo, xf, yf);
}

void cPlayer::DrawLiveBar(int tex_id)
{
	float xo, yo, xf, yf;
	xo = 0.0f ;
	xf = 1.0f;
	yo = (live * 0.2f) + 0.2f;
	yf = (live * 0.2f);
	DrawLiveBarRect(tex_id, xo, yo, xf, yf);
}

void cPlayer::DrawLiveBar2(int tex_id, int num)
{
	float xo, yo, xf, yf;
	xo = 0.0f;
	xf = 1.0f;
	yo = (live * 0.2f) + 0.2f;
	yf = (live * 0.2f);
	DrawLiveBarRect2(tex_id, xo, yo, xf, yf, num);
}
