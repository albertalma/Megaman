#include "cBicho.h"
#include "cScene.h"
#include "Globals.h"
#include <stdio.h>
#include <Windows.h>
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")
using namespace std;

cBicho::cBicho(void)
{
	seq=0;
	delay=0;
	live = 0;
	shooting = false;
	ostion = false;
	jumping = false;
	shotProgress = 0;
	isAlive = true;
	deadTime = 0;
	isBoss = false;
	engine = createIrrKlangDevice();
	isPlayer = false;
}

cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
}

void cBicho::Reset(int tileX, int tileY)
{
	SetTile(tileX, tileY);
	seq = 0;
	delay = 0;
	live = 0;
	shooting = false;
	ostion = false;
	jumping = false;
	shotProgress = 0;
	isAlive = true;
	deadTime = 0;
	isBoss = false;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cBicho::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
void cBicho::GetTile(int *tx,int *ty)
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width,int *height)
{
	*width = w;
	*height = h;
}

bool cBicho::IsAlive() {
	return isAlive;
}

void cBicho::SetAlive(bool aliveState)
{
	isAlive = aliveState;
}

void cBicho::SetDeadTime(int time) {
	deadTime = time;
}

int cBicho::GetDeadTime() {
	return deadTime;
}

void cBicho::ResetFrame() {
	seq = 0;
	delay = 0;
}
bool cBicho::Collides(cRect *rc)
{
	int centerX = rc->left;
	centerX += rc->right;
	centerX /= 2;
	int centerY = rc->top;
	centerY += rc->bottom;
	centerY /= 2;
	/*if ((x<centerX) && (x + w>centerX) && (y<centerY) && (y + h>centerY)) {
		char s[256];
		sprintf(s, "Player x: %d x+w: %d y: %d y+h %d \n", x, x + w, y, y + h);
		OutputDebugStringA(s);
		sprintf(s, "Enemy left: %d right: %d bottom %d top: %d \n", rc->left, rc->right, rc->bottom, rc->top);
		OutputDebugStringA(s);
		sprintf(s, "CenterX %d \n", rc->left, rc->right, centerX);
		OutputDebugStringA(s);
		sprintf(s, "Center y %d \n", centerY);
		OutputDebugStringA(s);
	}*/
	return ((x<centerX) && (x+w>centerX) && (y<centerY) && (y+h>centerY));
}
bool cBicho::CollidesMapWall(int *map,bool right, int level)
{
	int tile_x, tile_y;
	int j;
	int width_tiles, height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if (right)	tile_x += width_tiles;

	int width = SCENE_WIDTH;
	if (level == 3) width = SCENE_WIDTH_VS;
	for (j = 0; j<height_tiles; j++)
	{
		if (map[tile_x + ((tile_y + j)*width)] != 0)	return true;
	}

	return false;
}

bool cBicho::CollidesMapFloor(int *map, bool nextStep, int level)
{
	int tile_x, tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	if (nextStep && IsLookingRight()) {
		tile_x += 2;
	}
	else if (nextStep) {
		tile_x -= 2;
	}

	width_tiles = w / TILE_SIZE;
	if ((x % TILE_SIZE) != 0) width_tiles++;
	on_base = false;
	i = 0;
	int width = SCENE_WIDTH;
	if (level == 3) width = SCENE_WIDTH_VS;
	while ((i<width_tiles) && !on_base)
	{
		if ((y % TILE_SIZE) == 0)
		{
			if (map[(tile_x + i) + ((tile_y - 1) * width)] != 0) {
				on_base = true;
			}
		}
		else
		{
			if (map[(tile_x + i) + (tile_y * width)] != 0)
			{
				y = (tile_y + 1) * TILE_SIZE;
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}

bool cBicho::CollidesWater(int *map)
{
	int tile_x, tile_y;
	int width_tiles;
	bool on_base;
	int i;


	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	if ((x % TILE_SIZE) != 0) width_tiles++;

	on_base = false;
	i = 0;
	while ((i<width_tiles) && !on_base)
	{
		if ((y % TILE_SIZE) == 0)
		{
			if (map[(tile_x + i) + ((tile_y - 1) * SCENE_WIDTH)] == 2) {
				on_base = true;
			}
		}
		else
		{
			if (map[(tile_x + i) + (tile_y * SCENE_WIDTH)] == 2)
			{
				y = (tile_y + 1) * TILE_SIZE;
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}

void cBicho::GetArea(cRect *rc)
{
	rc->left   = x;
	rc->right  = x+w;
	rc->bottom = y;
	rc->top    = y+h;
}

void cBicho::GetShotArea(cRect *rc)
{
	rc->left = xShot-SHOT_OFFSET_X;
	rc->right = xShot + wShot -SHOT_OFFSET_X;
	rc->bottom = yShot - SHOT_OFFSET_Y;
	rc->top = yShot + hShot - SHOT_OFFSET_Y;
}



void cBicho::DrawRect(int tex_id, float xo, float yo, float xf, float yf)
{
	int screen_x, screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y + h);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::DrawShotRect(int tex_id, float xo, float yo, float xf, float yf)
{
	int screen_x, screen_y;

	screen_x = xShot;
	screen_y = yShot;


	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + wShot, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + wShot, screen_y + hShot);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + hShot);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::DrawLiveBarRect(int tex_id, float xo, float yo, float xf, float yf)
{
	int screen_x, screen_y;

	screen_x = 30;
	screen_y = 230;

	if (x < 180){ 
		screen_x = 50;
	}
	else if (x >= 180 && x <= (185 * 16))
	{
		screen_x = x - 134;
	}
	else{
		screen_x = (180 * 16) - 50;
	}

	if (isBoss){
		screen_x = (180 * 17)+50;
	}
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	char s[256];
		glTexCoord2f(xo, yo);	glVertex2i (screen_x, screen_y);
		glTexCoord2f(xf, yo);	glVertex2i(screen_x + 80, screen_y);
		glTexCoord2f(xf, yf);	glVertex2i(screen_x + 80.0f, screen_y + 16.0f);
		glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + 16.0f);
		glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::DrawLiveBarRect2(int tex_id, float xo, float yo, float xf, float yf, int num)
{
	int screen_x, screen_y;

	screen_x = 30;
	screen_y = 230;

	if (num == 1){
		screen_x = 70;
	}
	else 
	{
		screen_x = 310;
	}
	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	char s[256];
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + 80, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + 80.0f, screen_y + 16.0f);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + 16.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::MoveLeft(int *map, bool boss, int level)
{
	int xaux;
	int step = STEP_LENGTH;
	if (ostion) step = 1;
	if (boss) step = 5;
	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		xaux = x;
		x -= step;
		if (CollidesMapWall(map, false, level))
		{
			x = xaux;
			state = STATE_LOOKLEFT;
		}
		if (state != STATE_WALKLEFT)
		{
			if (ostion) state = STATE_JUMP_HIT_LEFT;
			else if (jumping) state = STATE_JUMP_UP_LEFT;
			else state = STATE_WALKLEFT;
			ResetFrame();
		}
	}
	//Advance, no problem
	else
	{
		x -= step;
		if(state != STATE_WALKLEFT)
		{
			if (ostion) state = STATE_JUMP_HIT_LEFT;
			else if (jumping) state = STATE_JUMP_UP_LEFT;
			else state = STATE_WALKLEFT;
			ResetFrame();
		}
	}
}

void cBicho::MoveRight(int *map, bool boss, int level)
{
	int xaux;
	int step = STEP_LENGTH;
	if (ostion) step = 1;
	if (boss) step = 5;
	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		xaux = x;
		x += step;

		if(CollidesMapWall(map,true,level))
		{
			x = xaux;
			state = STATE_LOOKRIGHT;
		}
	}
	//Advance, no problem
	else
	{
		x += step;

		if(state != STATE_WALKRIGHT)
		{
			//SI SALTA K SALTI
			if (ostion) state = STATE_JUMP_HIT_RIGHT;
			else if (jumping) state = STATE_JUMP_UP_RIGHT;
			else state = STATE_WALKRIGHT;
			ResetFrame();
		}
	}
}

void cBicho::Stop()
{
	//SI ESTA SALTANT NO ESTA STOP, NO ES MOU PERO SALTA
	if (!jumping ){
		switch (state)
		{
			case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;
			case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
			case STATE_FALLING_LEFT:state = STATE_LOOKLEFT;		break;
			case STATE_FALLING_RIGHT:state = STATE_LOOKRIGHT;	break;
			case STATE_JUMP_UP_LEFT:state = STATE_LOOKLEFT;		break;
			case STATE_JUMP_UP_RIGHT:state = STATE_LOOKRIGHT;	break;
			case STATE_JUMP_HIT_LEFT:state = STATE_LOOKLEFT;	break;
			case STATE_JUMP_HIT_RIGHT:state = STATE_LOOKRIGHT;	break;
		}
	}
}

bool cBicho::IsLookingRight()
{
	if (state >= STATE_LOOKLEFT && state <= STATE_SHOOTLEFT)
		return false;
	else return true;
}

void cBicho::Jump(int *map, int level)
{
	if(!jumping)
	{
		if(CollidesMapFloor(map, false,level))
		{
			jumping = true;
			jump_alfa = 0;
			jump_y = y;
			//SALTA A ALGUN CANTO
			if (!IsLookingRight()) {
				state = STATE_JUMP_UP_LEFT;
			}
			else {
				state = STATE_JUMP_UP_RIGHT;
			}
		}
	}
}

void cBicho::Ostion(int *map)
{
	if (!ostion){
		live += 1;
		if (live >= 4){
			isAlive = false;
		}
			ostion = true;
			jumping = true;
			jump_alfa = 0;
			jump_y = y;
			if (!IsLookingRight())
				state = STATE_JUMP_HIT_LEFT;
			else state = STATE_JUMP_HIT_RIGHT;

	}
}

bool cBicho::IsOstioning(){
	return ostion;
}

/*bool cBicho::IsHited(cEnemy Enemies[], int size){
	bool mandao = false;
	for (int i = 0; i < size; ++i) {
		int xRival, yRival;
		Enemies[i].GetPosition(&xRival, &yRival);

		
		if (x == xRival || x+1 == xRival || x-1 == xRival) {
			if (((y + 35 / 2) >= (yRival - 35 / 2)) && ((y + 35 / 2) <= (yRival + 35 / 2))) {
				return true;
			}
		}	

	}
	return false;
}*/

void cBicho::Hited()
{
	OutputDebugString("Player hited \n");
	if (state >= STATE_LOOKLEFT && state <= STATE_SHOOTLEFT) {
	}
	else {
		x -= STEP_LENGTH*3;
		state = STATE_HITED;
	}
		
}

void cBicho::Shot(int *map, bool isRight)
{
	if (!shooting) {
		if (isPlayer)engine->play2D("megamanshoot.wav");
		if (isRight) isRightShot = true;
		else isRightShot = false;
		shooting = true;
		xShot = x + SHOT_OFFSET_X;
		yShot = y + SHOT_OFFSET_Y;
	}
}

bool cBicho::IsShooting() {
	return shooting;
}

bool cBicho::IsJumping() {
	return jumping;
}

bool cBicho::IsShootingRight() {
	return isRightShot;
}

void cBicho::SetShotDimensions(int width, int height){
	wShot = width;
	hShot = height;
}

void cBicho::SetLiveBar(int width, int height){
	wLive = width;
	hLive = height;
}

void cBicho::GetShotPosition(int *xResult, int *yResult) {
	*xResult = xShot;
	*yResult = yShot;
}

void cBicho::SetShotPosition(int xResult, int yResult) {
	xShot = xResult;
	yShot = yResult;
}

void cBicho::SetShotProgress(int prog) {
	shotProgress = prog;
}

bool cBicho::ShotCollidesWall(int *map, bool right, int level)
{
	int tile_x, tile_y;
	int j;
	int width_tiles, height_tiles;

	tile_x = xShot / TILE_SIZE;
	tile_y = yShot / TILE_SIZE;
	width_tiles = wShot / TILE_SIZE;
	height_tiles = hShot / TILE_SIZE;

	int width = SCENE_WIDTH;
	if (level == 3) width = SCENE_WIDTH_VS;
	for (j = 0; j<height_tiles; j++)
	{
		if (map[tile_x + ((tile_y + j)*width)] != 0)	return true;
	}

	return false;
}

void cBicho::EraseShot() {
	shooting = false;
	shotProgress = 0;
	xShot = 0;
	yShot = 0;
}

void cBicho::ShotLogic(int type, int *map)
{
	int step = SHOT_STEP;
	int distShot = DIST_SHOT;
	switch (type) {
	case PLAYER:
		step = SHOT_STEP;
		distShot = DIST_SHOT;
		break;
	case ENEMY2:
		step = SHOT_ENEMY_STEP;
		distShot = DIST_SHOT_ENEMY;
		break;
	case BURSTMAN:
		step = SHOT_BOSS_STEP;
		distShot = DIST_SHOT_BOSS;
		break;
	}
	shotProgress += step;

	if (shotProgress >= distShot) {
		EraseShot();
	}
	else {
		if (isRightShot) xShot += step;
		else xShot -= step;
	}
}

void cBicho::JumpLogic(int *map, bool boss, int level) {
	float alfa;
	int step = JUMP_STEP;
	int height = JUMP_HEIGHT;
	if (ostion){
		step = 6;
		height = 40;
	}
	if (boss) height = 100;
	jump_alfa += step;//JUMP_STEP;

	if (jump_alfa == 180)
	{
		ostion = false;
		jumping = false;
		y = jump_y;
	}
	else
	{
		if (ostion) MoveLeft(map, boss,level);
		alfa = ((float)jump_alfa) * 0.017453f;
		y = jump_y + (int)(((float)height) * sin(alfa));
		if (jump_alfa > 90)
		{
			//Over floor?
			jumping = !CollidesMapFloor(map, false,level);
			//ESTA CAIENT DESPRES DE SALTAR
			
			if (!IsLookingRight()){
				if (ostion) state = STATE_JUMP_HIT_LEFT;
				else state = STATE_FALLING_LEFT;
			}
			else if (ostion) state = STATE_JUMP_HIT_RIGHT;
			else state = STATE_FALLING_RIGHT;
		}
	}
}

void cBicho::FallingLogic(int *map)
{
	y -= (2 * STEP_LENGTH);
	//ESTA CAIENT D'ALGUN LLOC SOL SENSE SALTAR
	if (state >= STATE_LOOKLEFT && state <= STATE_SHOOTLEFT)
		state = STATE_FALLING_LEFT;
	else state = STATE_FALLING_RIGHT;
}
void cBicho::Logic(int *map, int level)
{
	if (CollidesWater(map) && level != 3) {
		live = 3;
		Ostion(map);
	}
	else {
			if (shooting) ShotLogic(PLAYER,map);
			if (jumping) JumpLogic(map, false,level);
			else if (!CollidesMapFloor(map, false,level)) FallingLogic(map);
	}
}

void cBicho::NextFrame(int max)
{
	delay++;
	if(delay == FRAME_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}
int cBicho::GetFrame()
{
	return seq;
}
int cBicho::GetState()
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}
