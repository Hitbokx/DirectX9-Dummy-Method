#include <Windows.h>
#include <iostream>
#include "Includes.h"

void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene{ nullptr };
extern LPDIRECT3DDEVICE9 pDevice{ nullptr };

void APIENTRY hkEndScene( LPDIRECT3DDEVICE9 o_pDevice )
{
	if ( !pDevice )
		pDevice = o_pDevice;

	DrawFilledRect( 25, 25, 100, 100, D3DCOLOR_ARGB( 255, 0, 255, 255 ) );

	DrawFilledRect(windowWidth/2-2,windowHeight / 2 - 2, )

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
		memcpy( EndSceneBytes, (char*)d3d9Device[42], 7 );

		oEndScene = (tEndScene)g_hook.TrampHook32<7>( (BYTE*)d3d9Device[42], (BYTE*)hkEndScene );
	}

	while ( !(GetAsyncKeyState( VK_END )) )
	{

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