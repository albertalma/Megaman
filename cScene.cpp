#include "cScene.h"
#include "Globals.h"
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

int size = 1 / 16;
int lev;

cScene::cScene(void)
{
	lev = 10;
	level = 1;
	engine = createIrrKlangDevice();
	first = true;
	pintant = false;
}

cScene::~cScene(void)
{
}

int cScene::GetCurrentLevel()
{
	return level;
}

int cScene::NextLevel()
{
	return level++;
}

int cScene::ResetLevel()
{
	level = 1;
	return level;
}

int cScene::Level3()
{
	level = 3;
	return level;
}

bool cScene::LoadLevel(int levelLoad, int sublevel)
{

	bool res;
	FILE *fd;
	char file[16];
	int i, j, px, py;
	int tile;
	float coordx_tile, coordy_tile;
	res = true;
	level = levelLoad;
	if (levelLoad<10) sprintf(file, "%s%d%d%s", (char *)FILENAME, level, sublevel, (char *)FILENAME_EXT);
	else		 sprintf(file, "%s%d%s", (char *)FILENAME, level, (char *)FILENAME_EXT);
	fd = fopen(file, "r");
	if (fd == NULL) return false;

	id_DL = glGenLists(1);
	glNewList(id_DL, GL_COMPILE);
	glBegin(GL_QUADS);
	char s[256];
	int sceneH, sceneW;
	if (level == 3) {
		sceneH = SCENE_HEIGHT_VS;
		sceneW = SCENE_WIDTH_VS;
	}
	else {
		sceneH = SCENE_HEIGHT;
		sceneW = SCENE_WIDTH;
	}
	for (j = sceneH - 1; j >= 0; j--)
	{
		px = SCENE_Xo;
		py = SCENE_Yo + (j*TILE_SIZE);

		for (i = 0; i<sceneW; i++)
		{
			fscanf(fd, "%d", &tile);
			if (tile != 0) {
				tile -= 1;
				map[(j*sceneW) + i] = tile;
				if (level == 1) {
					if (tile == 29) collisionMap[(j*sceneW) + i] = 2;
					else if (tile < 60) collisionMap[(j*sceneW) + i] = 0;
					else collisionMap[(j*sceneW) + i] = 1;
				}
				else if (level == 2) {
					if (tile == 11 || tile == 21 || tile == 16 || tile == 26 || tile == 18 || tile == 19 || tile == 28 || tile == 29 || tile == 39 || tile == 65) 
						collisionMap2[(j*sceneW) + i] = 0;
					else collisionMap2[(j*sceneW) + i] = 1;
				}
				else if (level == 3) {
					if (tile == 11 || tile == 21 || tile == 16 || tile == 26 || tile == 18 || tile == 19 || tile == 28 || tile == 29 || tile == 39 || tile == 65)
						collisionMap3[(j*sceneW) + i] = 0;
					else collisionMap3[(j*sceneW) + i] = 1;
				}

				coordx_tile = (map[(j*sceneW) + i]) % 10;
				coordy_tile = (map[(j*sceneW) + i]) / 10;

				float size_g = 1.0f / 10.0f;

				float size_x = coordx_tile * size_g;
				float size_y = coordy_tile * size_g;


				//BLOCK_SIZE = 24, FILE_SIZE = 64
				// 24 / 64 = 0.375
				glTexCoord2f(size_x, size_y + size_g);	glVertex2i(px, py);
				glTexCoord2f(size_x + size_g, size_y + size_g);	glVertex2i(px + TILE_SIZE, py);
				glTexCoord2f(size_x + size_g, size_y);	glVertex2i(px + TILE_SIZE, py + TILE_SIZE);
				glTexCoord2f(size_x, size_y);	glVertex2i(px, py + TILE_SIZE);
			}
			else {
			}
			px += TILE_SIZE;
		}
		fscanf(fd, "%d", &tile); //pass enter
	}

	glEnd();
	glEndList();

	fclose(fd);

	return res;
}
void cScene::Draw(int tex_id)
{
	pintant = true;
	if (lev > 29) lev = 10;
	if (lev%10 == 0) LoadLevel(level,lev/10);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
	++lev;
}
int* cScene::GetMap()
{
	return map;
}

int* cScene::GetCollisionMap()
{
	if (level == 1) return collisionMap;
	else if (level == 2) return collisionMap2;
	else return collisionMap3;
}