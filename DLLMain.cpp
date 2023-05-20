#include <Windows.h>
#include <iostream>
#include "Includes.h"

void* d3d9Device[119];
tEndScene oEndScene{ nullptr };
extern LPDIRECT3DDEVICE9 pDevice{ nullptr };
Hack* hack{};

void APIENTRY hkEndScene( LPDIRECT3DDEVICE9 o_pDevice )
{
	if ( !pDevice )
		pDevice = o_pDevice;

	EntityList* pEntList = *(EntityList**)(modBase.client + offs.entList);
	PlayerEnt* pLocalPlayer{ *(PlayerEnt**)(modBase.client + offs.localPlayer) };
	int numPlayers{ *(int*)(modBase.engine + offs.numPlayers) };

	for ( int i{ 1 }; i < numPlayers; ++i )
	{
		PlayerEnt* currEnt{ (PlayerEnt*)(pEntList->EntList[i].m_pEntity) };

		if ( !(hack->CheckValidEnt( currEnt, pLocalPlayer )) )
			continue;

		D3DCOLOR colour{};

		if ( currEnt->team == pLocalPlayer->team )
			colour = D3DCOLOR_ARGB( 255, 0, 255, 0 );
		else
			colour = D3DCOLOR_ARGB( 255, 255, 0, 0 );

		float viewMatrix[16]{};

		memcpy( &viewMatrix, (PBYTE*)(modBase.engine + offs.dwViewMatrix), sizeof( viewMatrix ) );

		Vector3 entHead3D{ hack->GetBonePos( currEnt, 0 ) };
		for ( int i{ 0 }; i < 69; ++i )
		{
			Vector3 entHead = hack->GetBonePos( currEnt, i );

			if ( entHead.x == 0 || entHead.y == 0 || entHead.z == 0 )
				break;

			if ( entHead.z > entHead3D.z )
			{
				entHead3D = entHead;
			}
		}
		entHead3D.z += 8;
		vec2 entPos2D{};
		vec2 entHead2D{};
		// snapline
		if ( hack->WorldToScreen( currEnt->playerCoordinates, entPos2D ) ) 
		{
			DrawLine( entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, colour );

			if ( hack->WorldToScreen( entHead3D, entHead2D ) ) 
			{
				DrawESPBox2D( entHead2D, entPos2D, 2, colour );
			}
		}
	}

	hack->crossHair2D.x = windowWidth / 2;
	hack->crossHair2D.y = windowHeight / 2;

	vec2 l, r, t, b;

	l = r = t = b = hack->crossHair2D;
	l.x -= hack->crossHairSize;
	r.x += hack->crossHairSize;
	b.y += hack->crossHairSize;
	t.y -= hack->crossHairSize;

	DrawLine( l, r, 2, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	DrawLine( t, b, 2, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	oEndScene( pDevice );
}

DWORD WINAPI myThreadProc( HMODULE hInstDLL )
{
	AllocConsole( ); // To allocate console for logging
	FILE* f;
	freopen_s( &f, "CONOUT$", "w", stdout );
	std::cout << "Yo! Hitbokx here.\n";

	if ( GetD3D9Device( d3d9Device, sizeof( d3d9Device ) ) )
	{
		oEndScene = (tEndScene)g_hook.TrampHook32<7>( (BYTE*)d3d9Device[42], (BYTE*)hkEndScene );
	}

	hack = new Hack( );
	hack->Init( );

	while ( !(GetAsyncKeyState( VK_END )) )
	{
		hack->Update( );
		Sleep( 1 );
	}

	g_hook.unHook<7>( (BYTE*)d3d9Device[42] );

	if ( f )
		fclose( f );
	FreeConsole( );
	FreeLibraryAndExitThread( hInstDLL, 0 );//to call DLL_PROCESS_DETACH and resource deallocation
	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch ( fdwReason )
	{
		case DLL_PROCESS_ATTACH:

			::DisableThreadLibraryCalls( hinstDLL );
			CloseHandle( CreateThread( nullptr, 0, LPTHREAD_START_ROUTINE( myThreadProc ), hinstDLL, 0, nullptr ) );
			break;

		case DLL_PROCESS_DETACH:
			//perform any necesary clean-up
			break;

	}
	return TRUE; //Succesful DLL_PROCESS_ATTACH
}