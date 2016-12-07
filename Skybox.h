#pragma once
#include "Engine.h"

namespace Batman
{
	class Skybox
	{
	private:
		//storage of textures and mesh buffer
		Texture* textures[6];
		ID3DXMesh *mesh;
	public:
		//constructor / destructor
		Skybox(void);
		virtual ~Skybox(void);
		//create based on common filename suffix
		bool Create(std::string sharedFilename);
		//render
		void Render();
	};

};