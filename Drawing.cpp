#include "hack.h"
#include "dxStuff.h"

extern Hack* hack;

void DrawFilledRect( int x, int y, int width, int height, D3DCOLOR color )
{
	D3DRECT rect{ x, y, x + width, y + height };
	pDevice->Clear( 1, &rect, D3DCLEAR_TARGET, color, 0, 0 );
}

void DrawLine( int x1, int y1, int x2, int y2, int thickness, D3DCOLOR colour )
{
	if ( !hack->LineL )
		D3DXCreateLine( pDevice, &hack->LineL );

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2( x1, y1 );
	Line[1] = D3DXVECTOR2( x2, y2 );
	hack->LineL->SetWidth( thickness );
	hack->LineL->Draw( Line, 2, colour );
}

void DrawLine( vec2 src, vec2 dst, int thickness, D3DCOLOR colour )
{
	DrawLine( src.x, src.y, dst.x, dst.y, thickness, colour );
}

void DrawESPBox2D( vec2 top, vec2 bot, int thickness, D3DCOLOR color )
{
	int height = std::abs( top.y - bot.y );
	vec2 tl, tr;  
	tl.x = top.x - height / 4;
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;

	vec2 bl, br;
	bl.x = bot.x - height / 4;
	br.x = bot.x + height / 4;
	bl.y = br.y = bot.y;

	DrawLine( tl, tr, thickness, color );
	DrawLine( bl, br, thickness, color );
	DrawLine( tl, bl, thickness, color );
	DrawLine( tr, br, thickness, color );
}

void DrawESPBox3D( Vector3 top, Vector3 bot, float a, int width, int thickness, D3DCOLOR colour )
{
	int height3D = ABS(top.z - bot.z);
	Vector3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.z = b2.z = b3.z = b4.z = bot.z;

	b1.x = bot.x + (cos( TORAD( a + 45 ) ) * width);
	b1.y = bot.y + (sin( TORAD( a + 45 ) ) * width);

	b2.x = bot.x + (cos( TORAD( a + 135 ) ) * width);
	b2.y = bot.y + (sin( TORAD( a + 135 ) ) * width);

	b3.x = bot.x + (cos( TORAD( a + 225 ) ) * width);
	b3.y = bot.y + (sin( TORAD( a + 225 ) ) * width);

	b4.x = bot.x + (cos( TORAD( a + 315 ) ) * width);
	b4.y = bot.y + (sin( TORAD( a + 315 ) ) * width);

	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + height3D;

	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + height3D;

	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + height3D;

	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + height3D;

	vec2 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;

	if ( w2s( b1, b1_2 ) && w2s( b2, b2_2 ) && w2s( b3, b3_2 ) && w2s( b4, b4_2 ) && w2s( t1, t1_2 ) && w2s( t2, t2_2 ) && w2s( t3, t3_2 ) && w2s( t4, t4_2 ) )
	{
		DrawLine( t1_2, b1_2, thickness, colour );
		DrawLine( t2_2, b2_2, thickness, colour );
		DrawLine( t3_2, b3_2, thickness, colour );
		DrawLine( t4_2, b4_2, thickness, colour );

		DrawLine( t1_2, t2_2, thickness, colour );
		DrawLine( t2_2, t3_2, thickness, colour );
		DrawLine( t3_2, t4_2, thickness, colour );
		DrawLine( t4_2, t1_2, thickness, colour );

		DrawLine( b1_2, b2_2, thickness, colour );
		DrawLine( b2_2, b3_2, thickness, colour );
		DrawLine( b3_2, b4_2, thickness, colour );
		DrawLine( b4_2, b1_2, thickness, colour );
	}
}

void DrawText( const char* text, float x, float y, D3DCOLOR colour )
{
	RECT rect;
	if ( !hack->FontF )
		D3DXCreateFont( pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Aerial", &hack->FontF );

	SetRect( &rect, x + 1, y + 1, x + 1, y + 1 );
	hack->FontF->DrawTextA( NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB( 255, 0, 0, 0 ) );

	SetRect( &rect, x, y, x, y );
	hack->FontF->DrawTextA( NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, colour );
}