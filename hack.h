#pragma once
#include "Classes.h"
#include "Structs.h"
#include <d3d9.h>
#include <d3dx9.h>

#define ABS(x) ((x < 0) ? (-x) : (x))
#define TORAD(x) ((x)*0.01745329252) 
#define w2s(x, y) hack->WorldToScreen(x, y)

struct vec2
{
	float x{};
	float y{};
};

struct vec4
{
	float x{};
	float y{};
	float z{};
	float w{};
};

class Hack
{
public:
	PlayerEnt* localPlayer{};
	EntityList* entList{};
	float viewMatrix[16];

	ID3DXLine* LineL{};
	ID3DXFont* FontF{};

	vec2 crossHair2D;
	int crossHairSize{ 4 };

	~Hack( );



	void Init( );
	void Update( );

	void checkButtons( );

	bool CheckValidEnt( PlayerEnt* ent, PlayerEnt* localPlayer );
	bool WorldToScreen( Vector3 pos, vec2& screen );

	Vector3 GetBonePos( PlayerEnt* ent, int bone );

	Vector3 TransformVector( Vector3 src, Vector3 ang, float d );

	struct Settings
	{
		bool showMenu{ false };
		bool showTeamates{ true };
		bool snapLines{ false };
		bool box2D{ false };
		bool status2D{ false };
		bool statusText{ false };
		bool box3D{ false };
		bool velEsp{ false };
		bool headlineEsp{ false };
		bool rcsCrosshair{ false };
	}settings;

	struct Buttons
	{
		DWORD showMenu{ VK_INSERT };
		DWORD showTeamates{ VK_F1 };
		DWORD snapLines{ VK_F2 };
		DWORD box2D{ VK_F3 };
		DWORD status2D{ VK_F4 };
		DWORD statusText{ VK_F5 };
		DWORD box3D{ VK_F6 };
		DWORD velEsp{ VK_F7 };
		DWORD headlineEsp{ VK_F8 };
		DWORD rcsCrosshair{ VK_F9 };
	}button;

	struct Colours
	{
		D3DCOLOR health = D3DCOLOR_ARGB( 255, 46, 139, 87 );
		D3DCOLOR crosshair = D3DCOLOR_ARGB( 255, 255, 255, 255 );

		struct Team
		{
			D3DCOLOR esp{ D3DCOLOR_ARGB( 255, 0, 255, 0 ) };
			D3DCOLOR snapline{ D3DCOLOR_ARGB( 255, 0, 255, 0 ) };
			D3DCOLOR velocity{ D3DCOLOR_ARGB( 255, 0, 0, 255 ) };
			D3DCOLOR headline{ D3DCOLOR_ARGB( 255, 0, 0, 255 ) };
		}team;

		struct Enemy
		{
			D3DCOLOR esp{ D3DCOLOR_ARGB( 255, 255, 0, 0 ) };
			D3DCOLOR snapline{ D3DCOLOR_ARGB( 255, 255, 0, 0 ) };
			D3DCOLOR velocity{ D3DCOLOR_ARGB( 255, 0, 255, 255 ) };
			D3DCOLOR headline{ D3DCOLOR_ARGB( 255, 0, 255, 255 ) };
		}enemy;


	}colour;
};