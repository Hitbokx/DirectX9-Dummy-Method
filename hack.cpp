#include "Includes.h"

void Hack::Init( )
{
	entList = (EntityList*)(modBase.client + offs.entList);
	localPlayer = (PlayerEnt*)(modBase.client + offs.localPlayer);
}

void Hack::Update( )
{
	memcpy( &viewMatrix, (PBYTE*)(modBase.engine + offs.dwViewMatrix), sizeof( viewMatrix ) );
}

bool Hack::CheckValidEnt( PlayerEnt* ent )
{
	if ( ent == nullptr )
		return false;

	if ( ent == localPlayer )
		return false;

	if ( ent->health <= 0 )
		return false;

	if ( ent->bDormant )
		return false;

	return true;
}

bool  Hack::WorldToScreen( vec3 pos, vec2& screen )
{
	vec4 clipCoords{};

	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if ( clipCoords.w < 0.1f )
		return false;

	vec3 NDC{};
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return true;
}