#include "Includes.h"

int windowHeight{0}, windowWidth{0};

BOOL CALLBACK enumWind( HWND handle, LPARAM lp )
{
	DWORD procId;
	GetWindowThreadProcessId( handle, &procId );

	if ( GetCurrentProcessId( ) != procId )
		return TRUE;

	window = handle;

	return FALSE;
}

HWND GetProcessWindow( )
{
	window = NULL;

	EnumWindows( enumWind, NULL );

	RECT rect;

	if ( GetWindowRect( window, &rect ) )
	{
		windowWidth = rect.right - rect.left;
		windowHeight = rect.bottom - rect.top;
	}

	windowWidth += 171;
	windowHeight += 138;

	return window;
}

bool GetD3D9Device( void** pTable, size_t size )
{
	if ( !pTable )
		return false;

	IDirect3D9* pD3D{ Direct3DCreate9( D3D_SDK_VERSION ) };

	if ( !pD3D )
		return false;

	IDirect3DDevice9* pDummyDevice{ nullptr };

	D3DPRESENT_PARAMETERS d3dpp{};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow( );

	HRESULT dummyDevCreated{ pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice ) };

	if ( dummyDevCreated != S_OK )
	{
		d3dpp.Windowed = !d3dpp.Windowed;

		HRESULT dummyDevCreated{ pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice ) };

		if ( dummyDevCreated != S_OK )
		{
			pD3D->Release( );
			return false;
		}
	}

	memcpy( pTable, *(void***)pDummyDevice, size );

	pDummyDevice->Release( );
	pD3D->Release( );

	return true;
}