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
			faceNormals[SEVENTEEN] = Vector3::Cross(localVerts[8] - localVerts[9], localVerts[8] - localVerts[0]);
			faceNormals[TEN] = Vector3::Cross(localVerts[8] - localVerts[2], localVerts[8] - localVerts[9]);
			faceNormals[SEVEN] = Vector3::Cross(localVerts[8] - localVerts[0], localVerts[8] - localVerts[4]);
			faceNormals[THREE] = Vector3::Cross(localVerts[9] - localVerts[5], localVerts[9] - localVerts[0]);
			faceNormals[SIXTEEN] = Vector3::Cross(localVerts[5] - localVerts[0], localVerts[1] - localVerts[0]);
			faceNormals[ONE] = Vector3::Cross(localVerts[1] - localVerts[0], localVerts[4] - localVerts[0]);
		}

		//NB: while the d20 is a regular icosahedron, this support function is particular to this mesh's orientation. 
		Maths::Vector3 Support(const Maths::Vector3& dir, const NCL::CSC8503::Transform& tr) const override
		{
			Vector3 localDir = (tr.GetOrientation().Conjugate() * dir).Normalised();
			float dotMax = FLT_MIN;
			int index = -1;
			for (int i = 0; i < 12; i++)
			{
				if (Vector3::Dot(localDir, localVerts[i].Normalised()) > dotMax)
				{
					dotMax = Vector3::Dot(localDir, localVerts[i].Normalised());
					index = i;
				}
			}
			return tr.GetOrientation() * localVerts[index] + tr.GetPosition();

		}

		Vector3 localVerts[12];
		Vector3 faceNormals[20];


	protected:
		float edgeLength;
		//the golden ratio is used here to work out where the vertices are. It's halved, because the whole golden ratio works for an edge length of 2, not 1
		float halfGoldRatio = (1.0f + sqrt(5.0f)) / 2.0f / 2.0f;		
	};
}