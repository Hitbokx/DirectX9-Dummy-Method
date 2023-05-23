#include <Windows.h>
#include <iostream>
#include "Includes.h"
#include <sstream>
#include <string.h>

void* d3d9Device[119];
tEndScene oEndScene{ nullptr };
extern LPDIRECT3DDEVICE9 pDevice{ nullptr };
Hack* hack{};

void APIENTRY hkEndScene( LPDIRECT3DDEVICE9 o_pDevice )
{
	if ( !pDevice )
		pDevice = o_pDevice;

	DrawText( "1ST COPIED ESP BY HITBOKX!", windowWidth / 2, windowHeight - 20, D3DCOLOR_ARGB( 255, 0, 200, 200 ) );

	int menuOffX = windowWidth / 2;
	int menuOffY = 50;

	D3DCOLOR enabled = D3DCOLOR_ARGB( 255, 0, 255, 0 );
	D3DCOLOR disabled = D3DCOLOR_ARGB( 255, 255, 0, 0 );

	if ( !hack->settings.showMenu )
	{
		DrawText( "Show Menu (INS)", menuOffX, menuOffY, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else
	{
		DrawText( "Show Teammates (F1)", menuOffX, menuOffY + 0 * 12, hack->settings.showTeamates ? enabled : disabled );
		DrawText( "Snaplines (F2)", menuOffX, menuOffY + 1 * 12, hack->settings.snapLines ? enabled : disabled );
		DrawText( "2D Box (F3)", menuOffX, menuOffY + 2 * 12, hack->settings.box2D ? enabled : disabled );
		DrawText( "2D StatusBars (F4)", menuOffX, menuOffY + 3 * 12, hack->settings.status2D ? enabled : disabled );
		DrawText( "Status Text (F5)", menuOffX, menuOffY + 4 * 12, hack->settings.statusText ? enabled : disabled );
		DrawText( "3D Box (F6)", menuOffX, menuOffY + 5 * 12, hack->settings.box3D ? enabled : disabled );
		DrawText( "Velocity ESP(F7)", menuOffX, menuOffY + 6 * 12, hack->settings.velEsp ? enabled : disabled );
		DrawText( "HeadLine ESP (F8)", menuOffX, menuOffY + 7 * 12, hack->settings.headlineEsp ? enabled : disabled );
		DrawText( "Recoil Crosshair (F9)", menuOffX, menuOffY + 8 * 12, hack->settings.rcsCrosshair ? enabled : disabled );

		DrawText( "Hide Menu (INS)", menuOffX, menuOffY + 9 * 12, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}

	EntityList* pEntList = *(EntityList**)(modBase.client + offs.entList);
	PlayerEnt* pLocalPlayer{ *(PlayerEnt**)(modBase.client + offs.localPlayer) };
	int numPlayers{ *(int*)(modBase.engine + offs.numPlayers) };

	for ( int i{ 1 }; i < numPlayers; ++i )
	{
		PlayerEnt* currEnt{ (PlayerEnt*)(pEntList->EntList[i].m_pEntity) };

		if ( !(hack->CheckValidEnt( currEnt, pLocalPlayer )) )
			continue;

		D3DCOLOR espColour, snapLineColour, velColour, headLineColour;

		if ( currEnt->team == pLocalPlayer->team )
		{
			espColour = hack->colour.team.esp;
			snapLineColour = hack->colour.team.snapline;
			velColour = hack->colour.team.velocity;
			headLineColour = hack->colour.team.headline;
		}
		else
		{
			espColour = hack->colour.enemy.esp;
			snapLineColour = hack->colour.enemy.snapline;
			velColour = hack->colour.enemy.velocity;
			headLineColour = hack->colour.enemy.headline;
		}

		if ( !hack->settings.showTeamates && (currEnt->team == pLocalPlayer->team) )
			continue;

		Vector3 entHead3D{ hack->GetBonePos( currEnt, 0 ) };

		int j{ 0 };
		for ( int i{ 0 }; i < 69; ++i )
		{
			Vector3 entHead = hack->GetBonePos( currEnt, i );

			if ( entHead.x == 0 || entHead.y == 0 || entHead.z == 0 )
				break;

			if ( entHead.z > entHead3D.z )
			{
				entHead3D = entHead;
				j = i;
			}
		}

		if ( j == 27 )
			entHead3D = hack->GetBonePos( currEnt, 6 );

		entHead3D.z += 8;
		vec2 entPos2D{};
		vec2 entHead2D{};

		if ( hack->WorldToScreen( currEnt->playerCoordinates, entPos2D ) ) 
		{
			if ( hack->settings.velEsp )
			{
				Vector3 velOff = currEnt->playerCoordinates + (currEnt->velocity * 0.25);

				vec2 velOff2D;

				if ( hack->WorldToScreen( velOff, velOff2D ) )
				{
					DrawLine( entPos2D, velOff2D, 2, velColour );
					DrawFilledRect( velOff2D.x - 2, velOff2D.y - 2, 4, 4, espColour );
				}
			}

			if ( hack->settings.snapLines )
			{
				// snapline
				DrawLine( entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, snapLineColour );
			}

			if ( hack->WorldToScreen( entHead3D, entHead2D ) ) 
			{
				if ( hack->settings.box2D )
				{
					DrawESPBox2D( entHead2D, entPos2D, 2, espColour );
				}

				if ( hack->settings.box3D )
				{
					//Vector3* viewAngles{ (Vector3*)(modBase.engine + offs.ViewAnglesW) };

					DrawESPBox3D( entHead3D, currEnt->playerCoordinates, currEnt->viewAngles.y, 25, 2, espColour );
				}

				if ( hack->settings.status2D )
				{
					int height = ABS( entPos2D.y - entHead2D.y );
					int dx = (entPos2D.x - entHead2D.x);

					float healthPerc = currEnt->health / 100.f;

					vec2 botHealth, topHealth;
					int healthHeight = height * healthPerc;

					botHealth.y = entPos2D.y;
					botHealth.x = entPos2D.x - (height / 4) - 5;

					topHealth.y = entHead2D.y + height - healthHeight;
					topHealth.x = entPos2D.x - (height / 4) - 5 - (dx * healthPerc);

					DrawLine( botHealth, topHealth, 2, hack->colour.health );
				}

				if ( hack->settings.headlineEsp )
				{
					Vector3  head3D = entHead3D;
					head3D.z -= 8;

					Vector3 entAngles;
					entAngles.x = currEnt->viewAngles.x;
					entAngles.y = currEnt->viewAngles.z;
					entAngles.z = 0;

					Vector3 endPoint = hack->TransformVector( head3D, entAngles, 60 );

					vec2 endPoint2D, head2D;

					hack->WorldToScreen( head3D, head2D );

					if ( hack->WorldToScreen( endPoint, endPoint2D ) )
					{
						DrawLine( head2D, endPoint2D, 2, headLineColour );
					}
				}

				if ( hack->settings.statusText )
				{
					std::stringstream s1;
					s1 << currEnt->health;

					std::string t1 = "Health(%): " + s1.str( );

					char* healthMsg = (char*)t1.c_str( );
					DrawText( healthMsg, entPos2D.x, entPos2D.y+4, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				}
			}
		}
	}

	if ( hack->settings.rcsCrosshair )
	{
		hack->crossHair2D.x = windowWidth / 2;
		hack->crossHair2D.y = windowHeight / 2;

		vec2 l, r, t, b;

		l = r = t = b = hack->crossHair2D;
		l.x -= hack->crossHairSize;
		r.x += hack->crossHairSize;
		b.y += hack->crossHairSize;
		t.y -= hack->crossHairSize;

		DrawLine( l, r, 2, hack->colour.crosshair );
		DrawLine( t, b, 2, hack->colour.crosshair );
	}

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
	}

	g_hook.unHook<7>( (BYTE*)d3d9Device[42] );

	// So while ejecting, we don't crash the game
	Sleep( 2000 );

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