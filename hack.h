#pragma once
#include "Classes.h"
#include "Structs.h"
#include <d3d9.h>
#include <d3dx9.h>

#define ABS(x) ((x < 0) ? (-x) : (x))

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
	//ID3DXLine* LineL;

	vec2 crossHair2D;
	int crossHairSize{ 4 };

	ID3DXLine* LineL{};

	void Init( );
	void Update( );
	bool CheckValidEnt( PlayerEnt* ent, PlayerEnt* localPlayer );
	bool WorldToScreen( Vector3 pos, vec2& screen );

	Vector3 GetBonePos( PlayerEnt* ent, int bone );
};