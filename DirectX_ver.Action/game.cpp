
#include "game.h"
#include "block.h"
#include "item.h"
#include "player.h"

bool g_bPause = false;

void InitGame(void)
{
	InitPlayer();

	InitBlock();

	InitItem();

	SetItem(ITEMTYPE_STAR, D3DXVECTOR3(500.0f, 700.0f, 0.0f), D3DXCOLOR_NULL, OR_GRAVITY_GRAVITY);
}

void UninitGame(void)
{
	UninitPlayer();

	UninitBlock();

	UninitItem();
}

void UpdateGame(void)
{
	if (g_bPause == false)
	{
		UpdatePlayer();

		UpdateBlock();

		UpdateItem();
	}
}

void DrawGame(void)
{
	DrawPlayer();

	DrawBlock();

	DrawItem();
}