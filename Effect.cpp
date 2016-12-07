#include "Engine.h"

namespace Batman
{
	//constructor - nulls effect pointer
	Effect::Effect()
	{
		p_effect=NULL;
	}

	//destructor - release effect
	Effect::~Effect()
	{
		if(p_effect) p_effect->Release();
	}

	bool Effect::Load(std::string filename)
	{
		//create buffer for errors
		ID3DXBuffer *errors=0;
		//D3DXCreateFromFile - loads FX file to device
		//order: D3D device, src filename, not used, not used,
		//flags, effect pool (memory), effect pointer, errors buffer
		
		HRESULT hr = D3DXCreateEffectFromFile(g_engine->getDevice(), filename.c_str(),
							0, 0, D3DXSHADER_DEBUG, 0, &p_effect, &errors);

		//if there are errors, display them in a message box
		if(hr!=D3D_OK)
		{
			//MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
			return false;
		}
		return true;
	}

	void Effect::setTechnique(std::string technique)
	{
		//set FX technique using effect pointer
		p_effect->SetTechnique(technique.c_str());
	}

	void Effect::setViewMatrix(D3DXMATRIX matrix, std::string fxViewParam)
	{
		//set view matrix in FX file
		p_effect->SetMatrix(fxViewParam.c_str(), &matrix);
	}

	void Effect::setProjectionMatrix(D3DXMATRIX matrix, std::string fxProjParam)
	{
		//set projection matrix in fx file
		p_effect->SetMatrix(fxProjParam.c_str(), &matrix);
	}

	void Effect::setWorldMatrix(D3DXMATRIX matrix, std::string fxWorldParam)
	{
		//set projection matrix in fx file
		p_effect->SetMatrix(fxWorldParam.c_str(), &matrix);
	}

	bool Effect::Begin()
	{
		//get effect pointer or return
		if(!p_effect) return false;
		UINT passes;	//passes
		//begin (save passes if needed and begin passes)
		p_effect->Begin(&passes, 0);
		if(passes==0) return false;
		p_effect->BeginPass(0);
		return true;
	}

	void Effect::End()
	{
		//release memory
		if(!p_effect) return;
		p_effect->EndPass();
		p_effect->End();
	}

	void Effect::setParam(std::string name, D3DXMATRIX matrix)
	{
		//use set matrix on FX file
		p_effect->SetMatrix(name.c_str(), &matrix);
	}

	void Effect::setParam(std::string name, LPDIRECT3DTEXTURE9 texture)
	{
		//use set texture on fx file
		p_effect->SetTexture(name.c_str(), texture);
	}

	void Effect::setParam(std::string name, LPDIRECT3DCUBETEXTURE9 cubeTexture)
	{
		//use set texture on fx file
		p_effect->SetTexture(name.c_str(), cubeTexture);
	}

	void Effect::setParam(std::string name, D3DXVECTOR4 vector)
	{
		//use set vector on fx file
		p_effect->SetVector(name.c_str(), &vector);
	}

	void Effect::setParam(std::string name, D3DXVECTOR3 vector)
	{
		//set a fourth value before setting vector
		D3DXVECTOR4 v;
		v.x = (float)vector.x;
		v.y = (float)vector.y;
		v.z = (float)vector.z;
		v.w = 0;
		p_effect->SetVector(name.c_str(),&v);
	}

	void Effect::setParam(std::string name, float f)
	{
		//float
		p_effect->SetFloat(name.c_str(), f);
	}
};