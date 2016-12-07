#pragma once

#include "Engine.h"

namespace Batman
{

	class cSound;

	class cSoundLayer
	{

		private:
			LPDIRECTSOUND8 m_pDSound;
			LPDIRECTSOUNDBUFFER8 m_pPrimary; //primary mixer
			static cSoundLayer* m_pGlobalSLayer;
			cSoundLayer(HWND hWnd);

		public:
			virtual ~cSoundLayer();
			static cSoundLayer* GetSound()
			{
				return m_pGlobalSLayer;
			}
			LPDIRECTSOUND8 GetDSound()
			{
				return m_pDSound;
			}
			static void Create(HWND hWnd)
			{
				new cSoundLayer(hWnd);
			}
	};

	inline cSoundLayer* Sound()
	{
		return cSoundLayer::GetSound();
	}

}