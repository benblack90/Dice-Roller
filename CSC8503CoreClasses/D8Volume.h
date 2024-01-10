#pragma once
#include "CollisionVolume.h"

namespace NCL {
	class D8Volume : CollisionVolume
	{
	public:
		//for use with FACES for d8s
		enum DiceNumber
		{
			ONE,
			TWO,
			THREE,
			FOUR,
			FIVE,
			SIX,
			SEVEN,
			EIGHT,
			MAX
		};
		/*This construction is very particular to this specific model and texture of dice*/
		D8Volume(float height = 1.0f)
		{
			type = VolumeType::D8_Dice;
			this->height = height;

			localVerts[0] = { 0,1,0 };
			localVerts[1] = { 0,-1,0 };
			localVerts[2] = { 1,0,0 };
			localVerts[3] = { -1,0,0 };
			localVerts[4] = { 0,0,1 };
			localVerts[5] = { 0,0,-1 };
		}
		~D8Volume() {}

		float GetHeight() const { return height; }

		/*NOTE: while the d8 shape is an octohedron, this support point is specific to this mesh and its orientation, and not general to all octohedra*/
		Maths::Vector3 Support(const Maths::Vector3& dir, const NCL::CSC8503::Transform& tr) const override
		{
			Vector3 localDir = (tr.GetOrientation().Conjugate() * dir).Normalised();
			float dotMax = FLT_MIN;
			int index = -1;
			for (int i = 0; i < 6; i++)
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
		Vector3 localVerts[6];
	};
}