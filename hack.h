#pragma once

struct vec2
{
	float x{};
	float y{};
};

struct vec3
{
	float x{};
	float y{};
	float z{};
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

	void Init( );
	void Update( );
	bool CheckValidEnt( PlayerEnt* ent );
	bool WorldToScreen( vec3 pos, vec2& screen );
}