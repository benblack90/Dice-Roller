#pragma once
#include "CollisionVolume.h"

namespace NCL {
	class D20Volume : CollisionVolume
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
			NINE,
			TEN,
			ELEVEN,
			TWELVE,
			THIRTEEN,
			FOURTEEN,
			FIFTEEN,
			SIXTEEN,
			SEVENTEEN,
			EIGHTEEN,
			NINETEEN,
			TWENTY,
			MAX
		};
		/*This construction is very particular to this specific model and texture of dice*/
		D20Volume(float edgeLength = 1.0f)
		{
			type = VolumeType::D20_Dice;
			this->edgeLength = edgeLength;

			//the vertices in a regular icosahedron are at the cyclic coordinates of {0, +/-1, +/-golden ratio} for an edge length of 2
			//the values are halved for our standard edge-length of 1
			localVerts[0] = { 0.0f,0.5f * edgeLength, halfGoldRatio * edgeLength };
			localVerts[1] = { 0.0f,-0.5f * edgeLength, halfGoldRatio * edgeLength };
			localVerts[2] = { 0.0f,0.5f * edgeLength, -halfGoldRatio * edgeLength };
			localVerts[3] = { 0.0f,-0.5f * edgeLength, -halfGoldRatio * edgeLength };
			localVerts[4] = { halfGoldRatio * edgeLength,0.0f, 0.5f * edgeLength };
			localVerts[5] = { -halfGoldRatio * edgeLength,0.0f, 0.5f * edgeLength };
			localVerts[6] = { halfGoldRatio * edgeLength,0.0f, -0.5f * edgeLength };
			localVerts[7] = { -halfGoldRatio * edgeLength,0.0f, -0.5f * edgeLength };
			localVerts[8] = { 0.5f * edgeLength,halfGoldRatio * edgeLength,0.0f  };
			localVerts[9] = { -0.5f * edgeLength,halfGoldRatio * edgeLength,0.0f };
			localVerts[10] = { 0.5f * edgeLength,-halfGoldRatio * edgeLength,0.0f };
			localVerts[11] = { -0.5f * edgeLength,-halfGoldRatio * edgeLength,0.0f };

			SetFaceNormals();
		}
		~D20Volume() {}

		float GetEdgeLength() const { return edgeLength; }

		void SetFaceNormals()
		{
			faceNormals[ONE] = Vector3::Cross(localVerts[1] - localVerts[0], localVerts[4] - localVerts[0]);
			faceNormals[TWO] = Vector3::Cross(localVerts[6] - localVerts[2], localVerts[3] - localVerts[2]);
			faceNormals[THREE] = Vector3::Cross(localVerts[9] - localVerts[5], localVerts[9] - localVerts[0]);
			faceNormals[FOUR] = Vector3::Cross(localVerts[11] - localVerts[10], localVerts[3] - localVerts[10]);
			faceNormals[FIVE] = Vector3::Cross(localVerts[6] - localVerts[10], localVerts[4] - localVerts[10]);
			faceNormals[SIX] = Vector3::Cross(localVerts[7] - localVerts[5], localVerts[11] - localVerts[5]);
			faceNormals[SEVEN] = Vector3::Cross(localVerts[8] - localVerts[0], localVerts[8] - localVerts[4]);
			faceNormals[EIGHT] = Vector3::Cross(localVerts[2] - localVerts[9], localVerts[7] - localVerts[9]);
			faceNormals[NINE] = Vector3::Cross(localVerts[11] - localVerts[5], localVerts[1] - localVerts[5]);
			faceNormals[TEN] = Vector3::Cross(localVerts[8] - localVerts[2], localVerts[8] - localVerts[9]);
			faceNormals[ELEVEN] = Vector3::Cross(localVerts[11] - localVerts[1], localVerts[10] - localVerts[1]);
			faceNormals[TWELVE] = Vector3::Cross(localVerts[6] - localVerts[8], localVerts[2] - localVerts[8]);
			faceNormals[THIRTEEN] = Vector3::Cross(localVerts[1] - localVerts[4], localVerts[10] - localVerts[4]);
			faceNormals[FOURTEEN] = Vector3::Cross(localVerts[3] - localVerts[7], localVerts[11] - localVerts[7]);
			faceNormals[FIFTEEN] = Vector3::Cross(localVerts[4] - localVerts[8], localVerts[6] - localVerts[8]);
			faceNormals[SIXTEEN] = Vector3::Cross(localVerts[7] - localVerts[9], localVerts[5] - localVerts[9]);
			faceNormals[SEVENTEEN] = Vector3::Cross(localVerts[8] - localVerts[9], localVerts[8] - localVerts[0]);
			faceNormals[EIGHTEEN] = Vector3::Cross(localVerts[10] - localVerts[6], localVerts[3] - localVerts[6]);
			faceNormals[NINETEEN] = Vector3::Cross(localVerts[5] - localVerts[0], localVerts[1] - localVerts[0]);
			faceNormals[TWENTY] = Vector3::Cross(localVerts[3] - localVerts[2], localVerts[7] - localVerts[2]);			
		}

		//NB: while the d20 is a regular icosahedron, this support function is particular to this mesh's orientation. 
		Maths::Vector3 Support(const Maths::Vector3& dir, const NCL::CSC8503::Transform& tr) const override
		{
			Vector3 localDir = (tr.GetOrientation().Conjugate() * dir).Normalised();
			float dotMax = FLT_MIN;
			int index = -1;
			for (int i = 0; i < 12; i++)
			{
				float currentDot = Vector3::Dot(localDir, localVerts[i].Normalised());
				if (currentDot > dotMax)
				{
					dotMax = currentDot;
					index = i;
				}
			}
			return tr.GetOrientation() * localVerts[index] + tr.GetPosition();

		}

		short GetFaceResult(const NCL::CSC8503::Transform& tr) const
		{
			Vector3 upLocalised = (tr.GetOrientation().Conjugate() * Vector3(0,1,0)).Normalised();
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
		Vector3 localVerts[12];
		Vector3 faceNormals[20];
		float edgeLength;
		//the golden ratio is used here to work out where the vertices are. It's halved, because the whole golden ratio works for an edge length of 2, not 1
		float halfGoldRatio = (1.0f + sqrt(5.0f)) / 2.0f / 2.0f;		
	};
}