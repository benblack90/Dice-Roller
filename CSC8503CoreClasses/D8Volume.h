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

			localVerts[0] = { 0,height * 0.95f,0 };
			localVerts[1] = { 0,-height * 0.95f,0 };
			localVerts[2] = { height,0,0 };
			localVerts[3] = { -height,0,0 };
			localVerts[4] = { 0,0,height };
			localVerts[5] = { 0,0,-height };
			SetFaceNormals();
		}
		~D8Volume() {}

		void SetFaceNormals()
		{
			faceNormals[ONE] = Vector3::Cross(localVerts[1] - localVerts[4], localVerts[2] - localVerts[4]);
			faceNormals[TWO] = Vector3::Cross(localVerts[3] - localVerts[0], localVerts[4] - localVerts[0]);
			faceNormals[THREE] = Vector3::Cross(localVerts[4] - localVerts[1], localVerts[3] - localVerts[1]);
			faceNormals[FOUR] = Vector3::Cross(localVerts[4] - localVerts[0], localVerts[2] - localVerts[0]);
			faceNormals[FIVE] = Vector3::Cross(localVerts[5] - localVerts[3], localVerts[1] - localVerts[3]);
			faceNormals[SIX] = Vector3::Cross(localVerts[2] - localVerts[0], localVerts[5] - localVerts[0]);
			faceNormals[SEVEN] = Vector3::Cross(localVerts[5] - localVerts[1], localVerts[2] - localVerts[1]);
			faceNormals[EIGHT] = Vector3::Cross(localVerts[5] - localVerts[0], localVerts[3] - localVerts[0]);
		}

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

		short GetFaceResult(const NCL::CSC8503::Transform& tr) const
		{
			Vector3 upLocalised = (tr.GetOrientation().Conjugate() * Vector3(0, 1, 0)).Normalised();
			float dotMax = FLT_MIN;
			short index = -1;
			for (short i = 0; i < MAX; i++)
			{
				float currentDot = Vector3::Dot(upLocalised, faceNormals[i]);
				if (currentDot > dotMax)
				{
					dotMax = currentDot;
					index = i;
				}
			}
			return index + 1;
		}
		

	protected:
		float height;
		Vector3 localVerts[6];
		Vector3 faceNormals[8];

	};
}