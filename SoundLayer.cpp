#include "Engine.h"

namespace Batman {

	cSoundLayer* cSoundLayer::m_pGlobalSLayer = NULL;

	cSoundLayer::cSoundLayer( HWND hWnd )
	{
		m_pDSound = NULL;
		m_pPrimary = NULL;

		if( m_pGlobalSLayer )
		{
			debug << "cSoundLayer already initialized!\n";
		}
		m_pGlobalSLayer = this;

		HRESULT             hr;
		LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

		// Create IDirectSound using the primary sound device
		hr = DirectSoundCreate8( NULL, &m_pDSound, NULL );
		if( FAILED( hr ) )
		{
			debug << "DirectSoundCreate failed!\n";
		}

		// Set coop level to DSSCL_PRIORITY
		hr = m_pDSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );
		if( FAILED( hr ) )
		{
			debug << "SetCooperativeLevel (DS) failed!\n";
		}

		// Get the primary buffer 
		DSBUFFERDESC dsbd;
		dsbd.dwBufferBytes = 0;
		dsbd.lpwfxFormat   = NULL;
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
		dsbd.dwReserved = 0;
		dsbd.guid3DAlgorithm = GUID_NULL;
       
		hr = m_pDSound->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL );
		if( FAILED( hr ) )
		{
			debug << "CreateSoundBuffer (DS) failed!\n";
		}

		// Set primary buffer format to 22kHz and 16-bit output.
		WAVEFORMATEX wfx;
		ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
		wfx.wFormatTag      = WAVE_FORMAT_PCM; 
		wfx.nChannels       = 2; 
		wfx.nSamplesPerSec  = 22050; 
		wfx.wBitsPerSample  = 16; 
		wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

		if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
		{
		   debug << "SetFormat (DS) failed!\n";
		}

		pDSBPrimary->Release();
	}

	cSoundLayer::~cSoundLayer()
	{
		m_pPrimary->Release();
		m_pDSound->Release();
		m_pGlobalSLayer = NULL;
	}

}