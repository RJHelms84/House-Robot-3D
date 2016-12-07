#pragma once

#include "Engine.h"

namespace Batman
{
	class Effect
	{
	private:
		//pointer to D3D effect
		LPD3DXEFFECT p_effect;

	public:
		//constructor / destructor
		Effect();
		~Effect();
		//return effect object
		LPD3DXEFFECT getObject() { return p_effect; }

		//load FX file
		bool Load(std::string filename);

		//set FX technique
		//(shader files can have multiple techniques)
		void setTechnique(std::string technique);

		//set matrices for rendering (view, proj, world)
		void setViewMatrix(D3DXMATRIX matrix, std::string fxViewParam="View");
		void setProjectionMatrix(D3DXMATRIX matrix, std::string fxProjParam="Projection");
		void setWorldMatrix(D3DXMATRIX matrix, std::string fxWorldParam="World");

		//set individual variables in fx files
		void setParam(std::string name, D3DXMATRIX matrix);
		void setParam(std::string name, LPDIRECT3DTEXTURE9 texture);
		void setParam(std::string name, LPDIRECT3DCUBETEXTURE9 cubeTexture);
		void setParam(std::string name, D3DXVECTOR4 vector);
		void setParam(std::string name, D3DXVECTOR3 vector);
		void setParam(std::string name, Vector3 vector);
		void setParam(std::string name, float f);

		//start rendering / end rendering
		bool Begin();
		void End();
	};
};