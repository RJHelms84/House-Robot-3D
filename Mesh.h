#pragma once

#include "Engine.h"

namespace Batman
{
	//Bounding Box Struct
	struct BBOX
	{
		Vector3 min;
		Vector3 max;
	};

	//Bounding Sphere Struct
	struct BSPHERE
	{
		Vector3 center;
		float radius;
	};

	//inheriting from entity
	class Mesh : public Entity
	{
	private:
		//D3D object pointers
		LPD3DXMESH mesh;
		D3DMATERIAL9* materials;
		DWORD material_count;
		LPDIRECT3DTEXTURE9* textures;

		//mesh transform properties
		Vector3 position;
		Vector3	rotation;
		Vector3 scale;

		//transformation matrices
		Matrix matWorld;
		Matrix matTranslate;
		Matrix matRotate;
		Matrix matScale;

		//FX pointer
		Effect *p_effect;

		//previous position
		Vector3 previousPosition;

	public:
		//constructor / destructor
		Mesh(void);
		virtual ~Mesh(void);

		//load from file
		bool Load(char* filename, bool computeNormals=false);
		//file opening / location handlers
		bool findFile(std::string *filename);
		bool doesFileExist(std::string &filename);
		void splitPath(std::string& inputPath, std::string* pathOnly,
					std::string* filenameOnly);

		//update and render methods
		void Update(float deltaTime);
		void Render();
		void Render(Effect *effect,
			std::string fxTextureParam="Texture");
		void setEffect(Effect* effect) {p_effect = effect;}

		//getters and setters for world transform matrix
		Matrix getMatrix() {return matWorld;}
		void setMatrix(Matrix matrix) {matWorld=matrix;}
		void setMatrix(D3DXMATRIX matrix) {matWorld=(Matrix)matrix;}

		//getters for mesh and properties
		LPD3DXMESH getMesh() {return mesh;}
		virtual int getFaceCount();
		virtual int getVertexCount();

		//standard wrappers for primitive creation
		void createTorus(float innerRadius=0.5f, float outerRadius=1.0f,
						int sides=20, int rings=20);
		void createCylinder(float radius1, float radius2, float length,
						int slices, int stacks);
		void createSphere(double radius=1.0f, int slices=1.0f, int stacks=10);
		void createCube(double width=1.0f, double height=1.0f,
						double depth=1.0f);
		void createTeapot();

		//apply transformation matrices
		void Transform();

		//Manipulation of position, scale, rotation
		void Rotate(Vector3 rot);
		void Rotate(double x, double y, double z);
		void setPosition(Vector3 pos) {previousPosition=position; position=pos;}
		void setPosition(double x, double y, double z);
		void restorePreviousPosition() { position=previousPosition; }
		Vector3 getPosition() {return position;}
		void setRotation(Vector3 rot) {rotation=rot;}
		void setRotation(double x, double y, double z);
		Vector3 getRotation() {return rotation;}
		void setScale(Vector3 value) {scale=value;}
		void setScale(double x, double y, double z);
		void setScale(double value);
		Vector3 getScale() {return scale;}

		//collision support
		BBOX getBoundingBox();
		BSPHERE getBoundingSphere();
	};
};