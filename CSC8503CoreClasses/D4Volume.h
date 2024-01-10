#pragma once
#include "CollisionVolume.h"

namespace NCL {
	class D4Volume : CollisionVolume
	{
	public:
		enum DiceNumber
		{
			ONE,
			TWO,
			THREE,
			FOUR,
			MAX
		};
		/*This construction is very particular to this specific model and texture of dice, where the one faces +y, and the 3 +z, and 2 +x*/
		D4Volume(float height = 1.0f) 
		{
			type = VolumeType::D4_Dice;
			this->height = height;
			localVerts[ONE] = { 0.0f,height * ratioOfHeightAboveOrigin,0.0f };
			localVerts[THREE] = Matrix4::Rotation(270-(60*(1-ratioOfHeightAboveOrigin)), {1,0,0}) * localVerts[ONE];
			Vector3 rotatedZ = Matrix4::Rotation(120, { 0,1,0 }) * Vector3(1, 0, 0);
			localVerts[FOUR] = Matrix4::Rotation(270 - (60 * (1-ratioOfHeightAboveOrigin)), rotatedZ) * localVerts[ONE];
			rotatedZ = Matrix4::Rotation(120, { 0,1,0 }) * rotatedZ;
			localVerts[TWO] = Matrix4::Rotation(270 - (60 * (1 - ratioOfHeightAboveOrigin)), rotatedZ) * localVerts[ONE];
		}
		~D4Volume() {}

		float GetHeight() const {return height;}
		float GetRatioOfHeightAboveOrigin() { return ratioOfHeightAboveOrigin; }

		/*NOTE: while the d4 shape is a tetrahedron, this support point is specific to this mesh and its orientation, and not general to all tetrahedra*/
		Maths::Vector3 Support(const Maths::Vector3& dir, const NCL::CSC8503::Transform& tr) const override
		{
			Vector3 localDir = (tr.GetOrientation().Conjugate() * dir).Normalised();
			float dotMax = FLT_MIN;
			int index = -1;
			for (int i = 0; i < 4; i++)
			{
				if (Vector3::Dot(localDir, localVerts[i].Normalised()) > dotMax)
				{
					dotMax = Vector3::Dot(localDir, localVerts[i].Normalised());
					index = i;
				}
			}
			return tr.GetOrientation() * localVerts[index] + tr.GetPosition();

		}

		
	protected:
		float height;
		float ratioOfHeightAboveOrigin = 0.7;
		Vector3 localVerts[MAX];
	};
}