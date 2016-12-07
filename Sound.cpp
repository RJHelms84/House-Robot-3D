#include "Engine.h"

namespace Batman {
	
	using std::map;

	map< CWaveSoundRead*, int > cSound::m_waveMap;

	cSound::cSound(char* filename)
	{
		m_pWaveSoundRead = NULL;
		m_pBuffer = NULL;

		// Create a new wave file class
		m_pWaveSoundRead = new CWaveSoundRead();
		m_waveMap[ m_pWaveSoundRead]=1;

		// Load the wave file
		if( FAILED( m_pWaveSoundRead->Open( filename)))
		{
			debug << "couldn't open sound file! " << filename << "\n";
		}

		Init();
		Fill();
	}

	cSound::cSound(cSound& in)
	{
		m_pWaveSoundRead = in.m_pWaveSoundRead;
		m_waveMap[ m_pWaveSoundRead ]++;
		Init();
		Fill();
	}

	cSound& cSound::operator=( const cSound &in )
	{
		//Destroy the old object

		int count = --m_waveMap[ m_pWaveSoundRead ];
		if(!count)
		{
			delete m_pWaveSoundRead;
		}
		m_pBuffer->Release();

		//Clone the incoming one

		m_pWaveSoundRead = in.m_pWaveSoundRead;
		m_waveMap[ m_pWaveSoundRead ]++;
		Init();
		Fill();
		return *this;

	}

	cSound::~cSound()
	{
		int count = m_waveMap[ m_pWaveSoundRead ];
		if( count == 1 )
		{
			delete m_pWaveSoundRead;
		}
		else
		{
			m_waveMap[ m_pWaveSoundRead ] = count - 1;
		}

		m_pBuffer->Release();
	}

	void cSound::Init()
	{
		/**
		* Set up the DirectSound surface. The size of the sound file
		* and the format of the data can be retrieved from the wave
		* sound object. Besides that, we only set the STATIC flag,
		* so that the driver isn't restricted in setting up the
		* buffer.
		*/
		DSBUFFERDESC dsbd;
		dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
		dsbd.dwBufferBytes = m_pWaveSoundRead->m_ckIn.cksize;
		dsbd.lpwfxFormat = m_pWaveSoundRead->m_pwfx;
		dsbd.guid3DAlgorithm = GUID_NULL;
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwReserved = 0;


		HRESULT hr;

		//Temporary pointer to old DirectSound interface
		LPDIRECTSOUNDBUFFER pTempBuffer = 0;

		//Crteate the sound buffer
		hr = Sound()->GetDSound()->CreateSoundBuffer( &dsbd, &pTempBuffer, NULL );
		if(FAILED(hr))
		{
			debug << "CreateSoundBuffer failed \n";
		}

		//Upgrade the sound buffer to version 8
		pTempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pBuffer);
		if(FAILED(hr))
		{
			debug << "SoundBuffer query to 8 failed \n";
		}

		//Release the temporary old buffer
		pTempBuffer->Release();

		/**
		*Remember how big the buffer is
		*/

		m_bufferSize = dsbd.dwBufferBytes;
	}

	void cSound::Restore()
	{
		HRESULT hr;
		if( NULL == m_pBuffer )
		{
			return;
		}

		DWORD dwStatus;
		if(FAILED(hr = m_pBuffer->GetStatus(&dwStatus)))
		{
			debug << "SoundBuffer query to 8 failed! \n";
		}

		if(dwStatus & DSBSTATUS_BUFFERLOST)
		{
			/**
			* Chances are, we got here because the app /just/
			* started, and DirectSound hasn't given us any
			* control yet. Just spin until we can restore
			* the buffer
			*/
			do
			{
				hr = m_pBuffer->Restore();
				if( hr == DSERR_BUFFERLOST )
				Sleep( 10 );
			}
			while( hr = m_pBuffer->Restore() );
			
			/**
			* The buffer was restored. Fill 'er up.
			*/
			Fill();
		}
	}

	void cSound::Fill()
	{
		HRESULT hr;
		unsigned char* pbWavData; //Pointer to actual wav data
		unsigned int cbWavSize; //size of data
		void* pbData = NULL;
		void* pbData2 = NULL;
		unsigned long dwLength; 
		unsigned long dwLength2;

		/**
		*How big the wav file is
		*/
		unsigned int nWaveFileSize = m_pWaveSoundRead->m_ckIn.cksize;

		/**
		* Allocate enough data to hold the wav file data
		*/
		pbWavData = new unsigned char[ nWaveFileSize ];
		if( NULL == pbWavData )
		{
			delete [] pbWavData;
			debug << "Out of memory! \n";
		}

		hr = m_pWaveSoundRead->Read(
								nWaveFileSize,
								pbWavData,
								&cbWavSize );
		if( FAILED( hr ) )
		{
			delete [] pbWavData;
			debug << "m_pWaveSoundRead->Read failed\n";
		}

		/**
		* Reset the file to the beginning
		*/
		m_pWaveSoundRead->Reset();

		/**
		* Lock the buffer so we can copy the data over
		*/
		hr = m_pBuffer->Lock(
						0, m_bufferSize, &pbData, &dwLength,
						&pbData2, &dwLength2, 0L );
		if( FAILED( hr ) )
		{
			delete [] pbWavData;
			debug << "m_pBuffer->Lock failed\n";
		}

		/**
		* Copy said data over, unlocking afterwards
		*/
		memcpy( pbData, pbWavData, m_bufferSize );
		m_pBuffer->Unlock( pbData, m_bufferSize, NULL, 0 );

		/**
		* We're done with the wav data memory.
		*/
		delete [] pbWavData;
	}

	bool cSound::IsPlaying()
	{
		DWORD dwStatus = 0;
		m_pBuffer->GetStatus( &dwStatus );

		if( dwStatus & DSBSTATUS_PLAYING )
			return true;
		else
			return false;
	}

	void cSound::Play( bool bLoop )
		{
			HRESULT hr;
			if( NULL == m_pBuffer )
				return;

			// Restore the buffers if they are lost
			Restore();

			// Play buffer
			DWORD dwLooped = bLoop ? DSBPLAY_LOOPING : 0L;
		if( FAILED( hr = m_pBuffer->Play( 0, 0, dwLooped)))
		{
			debug << "m_pBuffer->Play failed\n";
		}
	}

	void cSound::setPosition(int pos) {
		m_pBuffer->SetCurrentPosition(pos);
	}

		void cSound::Stop( )
		{
			HRESULT hr;
			if( NULL == m_pBuffer )
				return;

			// Restore the buffers if they are lost
			Restore();
			if( FAILED( hr = m_pBuffer->Stop()))
		{
			debug << "m_pBuffer->Stop failed\n";
		}
	}
}