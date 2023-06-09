#include "hack.h"
#include "dxStuff.h"

Hack::~Hack( )
{
	this->FontF->Release( );
	this->LineL->Release( );
}

void Hack::Init( )
{
	entList = *(EntityList**)(modBase.client + offs.entList);
	localPlayer = *(PlayerEnt**)(modBase.client + offs.localPlayer);
}

void Hack::Update( )
{
	memcpy( &viewMatrix, (PBYTE*)(modBase.engine + offs.dwViewMatrix), sizeof( viewMatrix ) );
	this->checkButtons( );
}

void Hack::checkButtons( )
{
	if ( GetAsyncKeyState( button.showMenu ) & 1 )
		settings.showMenu = !settings.showMenu;

	if ( GetAsyncKeyState( button.snapLines ) & 1 )
		settings.snapLines = !settings.snapLines;

	if ( GetAsyncKeyState( button.box2D ) & 1 )
		settings.box2D = !settings.box2D;

	if ( GetAsyncKeyState( button.status2D ) & 1 )
		settings.status2D = !settings.status2D;

	if ( GetAsyncKeyState( button.statusText ) & 1 )
		settings.statusText = !settings.statusText;

	if ( GetAsyncKeyState( button.box3D ) & 1 )
		settings.box3D = !settings.box3D;

	if ( GetAsyncKeyState( button.velEsp ) & 1 )
		settings.velEsp = !settings.velEsp;

	if ( GetAsyncKeyState( button.headlineEsp ) & 1 )
		settings.headlineEsp = !settings.headlineEsp;

	if ( GetAsyncKeyState( button.rcsCrosshair ) & 1 )
		settings.rcsCrosshair = !settings.rcsCrosshair;

	if ( GetAsyncKeyState( button.showTeamates ) & 1 )
		settings.showTeamates = !settings.showTeamates;
}

bool Hack::CheckValidEnt( PlayerEnt* ent, PlayerEnt* localPlayer )
{

	if ( ent == nullptr )
		return false;

	if ( ent == localPlayer )
		return false;

	if ( ent->health <= 1 )
		return false;

	if ( ent->bDormant )
		return false;

	return true;
}

bool  Hack::WorldToScreen( Vector3 pos, vec2& screen )
{
	vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if ( clipCoords.w < 0.1f )
		return false;

	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

Vector3 Hack::GetBonePos( PlayerEnt* ent, int boneID )
{
	Vector3 bonePos{};

	auto boneArrayAccess{ ent->pBoneMatrix };

	bonePos.x = (*boneArrayAccess)[boneID].matrix[3];
	bonePos.y = (*boneArrayAccess)[boneID].matrix[7];
	bonePos.z = (*boneArrayAccess)[boneID].matrix[11];

	return bonePos;
}

Vector3 Hack::TransformVector( Vector3 src, Vector3 ang, float d )
{
	Vector3 newPos{};
	newPos.x = src.x + (cosf( TORAD( ang.y ) ) * d);
	newPos.y = src.y + (sinf( TORAD( ang.y ) ) * d);
	newPos.z = src.z + (tanf( TORAD( ang.x ) ) * d);

	return newPos;
}