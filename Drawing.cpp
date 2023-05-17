#include "Includes.h"

void DrawFilledRect( int x, int y, int width, int height, D3DCOLOR color )
{
	D3DRECT rect{ x, y, x + width, y + height };
	pDevice->Clear( 1, &rect, D3DCLEAR_TARGET, color, 0, 0 );
}