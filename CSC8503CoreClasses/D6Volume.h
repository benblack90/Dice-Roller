#pragma once
#include "OBBVolume.h"

namespace NCL 
{

	class D6Volume : OBBVolume
	{
	public:
		enum DiceNumber
		{
			ONE,
			TWO,
			THREE,
			FOUR,
			FIVE,
			SIX,
			MAX
		};

		D6Volume(const Maths::Vector3& halfDims)
			:OBBVolume(halfDims)
		{
			SetFaceNormals();
		}

		~D6Volume() {};

		Vector3 GetHalfDimensions() { return OBBVolume::GetHalfDimensions(); }

		void SetFaceNormals()
		{
			faceNormals[ONE] = Vector3::Cross(Vector3(-halfSizes.x, halfSizes.y, halfSizes.z) - Vector3(halfSizes.x, halfSizes.y, halfSizes.z),
				Vector3(halfSizes.x, -halfSizes.y, halfSizes.z) - Vector3(halfSizes.x, halfSizes.y, halfSizes.z));
			faceNormals[TWO] = Vector3::Cross(Vector3(halfSizes.x, halfSizes.y, -halfSizes.z) - Vector3(halfSizes.x, halfSizes.y, halfSizes.z),
				Vector3(-halfSizes.x, halfSizes.y, halfSizes.z) - Vector3(halfSizes.x, halfSizes.y, halfSizes.z));
			faceNormals[THREE] = Vector3::Cross(Vector3(-halfSizes.x, halfSizes.y, -halfSizes.z) - Vector3(-halfSizes.x, halfSizes.y, halfSizes.z),
				Vector3(-halfSizes.x, -halfSizes.y, halfSizes.z) - Vector3(-halfSizes.x, halfSizes.y, halfSizes.z));
			faceNormals[FOUR] = -faceNormals[THREE];
			faceNormals[FIVE] = -faceNormals[TWO];
			faceNormals[SIX] = -faceNormals[ONE];
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
		Vector3 faceNormals[6];
	};
}