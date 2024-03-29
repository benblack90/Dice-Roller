#include "../NCLCoreClasses/KeyboardMouseController.h"

#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif
#include "PhysicsSystem.h"

#include "StateGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class DiceRoller {
		public:

			enum AvailableDice
			{
				d4,
				d6,
				d8,
				d20,
				MAX
			};

			DiceRoller();
			~DiceRoller();

			virtual void UpdateGame(float dt);

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();
			void SelectObject();
			void InitWorld();

			void InitDiceTray();

			void UpdateActiveDice();
			void ResetDicePositions();
			void RollDice();
			void DisplayDiceResults();


			GameObject* AddFloorToWorld(const Vector3& position, const Vector3& dimensions);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, Texture* tex, float inverseMass = 10.0f);
			GameObject* AddD4(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddD6(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);
			GameObject* AddD8(const Vector3& position, float height, float inverseMass = 10.0f);
			GameObject* AddD20(const Vector3& position, float height, float inverseMass = 10.0f);

#ifdef USEVULKAN
			GameTechVulkanRenderer* renderer;
#else
			GameTechRenderer* renderer;
#endif
			PhysicsSystem* physics;
			GameWorld* world;

			KeyboardMouseController controller;

			float		forceMagnitude;

			Mesh* capsuleMesh = nullptr;
			Mesh* cubeMesh = nullptr;
			Mesh* sphereMesh = nullptr;

			Texture* basicTex = nullptr;
			Texture* d4Tex = nullptr;
			Texture* d6Tex = nullptr;
			Texture* d8Tex = nullptr;
			Texture* d10Tex = nullptr;
			Texture* d12Tex = nullptr;
			Texture* d20Tex = nullptr;
			Texture* woodTex = nullptr;
			Shader* basicShader = nullptr;

			//dice meshes
			Mesh* d4Mesh = nullptr;
			Mesh* d6Mesh = nullptr;
			Mesh* d8Mesh = nullptr;
			Mesh* d10Mesh = nullptr;
			Mesh* d12Mesh = nullptr;
			Mesh* d20Mesh = nullptr;

			Vector3 d4Start = {-5,5,-4 };
			Vector3 d6Start = { -5,5,-2 };
			Vector3 d8Start = { -5,5,0 };
			Vector3 d20Start = { -5,5,2 };

			bool diceActive;
			float sceneTime;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset = Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			GameObject* objClosest = nullptr;
			GameObject* selectedDice[MAX];
			GameObject* rollingDice[MAX];
		};
	}
}

