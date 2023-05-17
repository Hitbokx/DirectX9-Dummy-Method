#include "Includes.h"

void DrawFilledRect( int x, int y, int width, int height, D3DCOLOR color )
{
	D3DRECT rect{ x, y, x + width, y + height };
	pDevice->Clear( 1, &rect, D3DCLEAR_TARGET, color, 0, 0 );
}

void DrawLine( int x1, int y1, int x2, int y2, int thickness, D3DCOLOR colour )
{
	ID3DXLine* LineL{};
	D3DXCreateLine( pDevice, &LineL );

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2( x1, y1 );
	Line[1] = D3DXVECTOR2( x2, y2 );
	LineL->SetWidth( thickness );
	LineL->Draw( Line, 2, colour );
	LineL->Release( );
}