#pragma once
#include "hack.h"

void DrawFilledRect( int x, int y, int width, int height, D3DCOLOR colour );

void DrawLine( int x1, int y1, int x2, int y2, int thickness, D3DCOLOR colour );

void DrawLine( vec2 src, vec2 dst, int thickness, D3DCOLOR colour );

void DrawESPBox2D( vec2 top, vec2 bot, int thickness, D3DCOLOR colour );