#pragma once
#include "Classes.h"
#include "Structs.h"

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

	void Init( );
	void Update( );
	bool CheckValidEnt( PlayerEnt* ent, PlayerEnt* localPlayer );
	bool WorldToScreen( Vector3 pos, vec2& screen, float viewMatrix[16] );
};