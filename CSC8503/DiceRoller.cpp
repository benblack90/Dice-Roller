#include "DiceRoller.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "TextureLoader.h"

#include "PositionConstraint.h"
#include "OrientationConstraint.h"
#include "StateGameObject.h"
#include "D4Volume.h"
#include "D8Volume.h"
#include "D20Volume.h"




using namespace NCL;
using namespace CSC8503;

DiceRoller::DiceRoller() : controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	world = new GameWorld();
#ifdef USEVULKAN
	renderer = new GameTechVulkanRenderer(*world);
	renderer->Init();
	renderer->InitStructures();
#else 
	renderer = new GameTechRenderer(*world);
#endif

	physics = new PhysicsSystem(*world);

	forceMagnitude = 10.0f;
	physics->UseGravity(true);

	world->GetMainCamera().SetController(controller);

	controller.MapAxis(0, "Sidestep");
	controller.MapAxis(1, "UpDown");
	controller.MapAxis(2, "Forward");

	controller.MapAxis(3, "XLook");
	controller.MapAxis(4, "YLook");

	InitialiseAssets();
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void DiceRoller::InitialiseAssets() {
	cubeMesh = renderer->LoadMesh("cube.msh");
	sphereMesh = renderer->LoadMesh("sphere.msh");
	capsuleMesh = renderer->LoadMesh("capsule.msh");
	d4Mesh = renderer->LoadMesh("d4.msh");
	d6Mesh = renderer->LoadMesh("d6.msh");
	d8Mesh = renderer->LoadMesh("d8.msh");
	d10Mesh = renderer->LoadMesh("d10.msh");
	d12Mesh = renderer->LoadMesh("d12.msh");
	d20Mesh = renderer->LoadMesh("d20.msh");

	basicTex = renderer->LoadTexture("checkerboard.png");
	d4Tex = renderer->LoadTexture("d4_Albedo.png");
	d6Tex = renderer->LoadTexture("d6_Albedo.png");
	d8Tex = renderer->LoadTexture("d8_Albedo.png");
	d10Tex = renderer->LoadTexture("d10_Albedo.png");
	d12Tex = renderer->LoadTexture("d12_Albedo.png");
	d20Tex = renderer->LoadTexture("d20_Albedo.png");
	woodTex = renderer->LoadTexture("wood_Albedo.png");
	basicShader = renderer->LoadShader("scene.vert", "scene.frag");

	InitCamera();
	InitWorld();
}

DiceRoller::~DiceRoller() {
	delete cubeMesh;
	delete sphereMesh;
	delete d4Mesh;
	delete d6Mesh;
	delete d8Mesh;
	delete d10Mesh;
	delete d12Mesh;
	delete d20Mesh;

	delete basicTex;
	delete d4Tex;
	delete d6Tex;
	delete d8Tex;
	delete d10Tex;
	delete d12Tex;
	delete d20Tex;
	delete woodTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void DiceRoller::UpdateGame(float dt) {
	world->GetMainCamera().UpdateCamera(dt);
	UpdateKeys();
	SelectObject();

	world->UpdateWorld(dt);
	renderer->Update(dt);
	physics->Update(dt);

	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void DiceRoller::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
	}
}


void DiceRoller::InitCamera() {
	world->GetMainCamera().SetNearPlane(0.1f);
	world->GetMainCamera().SetFarPlane(500.0f);
	world->GetMainCamera().SetPitch(-40.0f);
	world->GetMainCamera().SetYaw(315.0f);
	world->GetMainCamera().SetPosition(Vector3(-15, 20, 15));
	lockedObject = nullptr;
}

void DiceRoller::InitWorld() {
	world->ClearAndErase();
	physics->Clear();

	InitDiceTray();

	GameObject* dice = AddD4({ -14,0,-2 }, 1, 10);
	dice->GetPhysicsObject()->useGravity = false;
	dice->SetCollisionLayer(staticObj);
	dice->SetName("selD4");
	dice = AddD6({ -14,0,-4 }, { 0.5,0.5,0.5 }, 10);
	dice->GetPhysicsObject()->useGravity = false;
	dice->SetName("selD6");
	dice->SetCollisionLayer(staticObj);
	dice = AddD8({ -14,0,-6 }, 1, 10);
	dice->GetPhysicsObject()->useGravity = false;
	dice->SetName("selD8");
	dice->SetCollisionLayer(staticObj);
	dice = AddD20({ -14,0,-8 }, 1, 10);
	dice->GetPhysicsObject()->useGravity = false;
	dice->SetCollisionLayer(staticObj);
	dice->SetName("selD20");
}

GameObject* DiceRoller::AddD4(const Vector3& position, float height, float inverseMass)
{
	GameObject* d4 = new GameObject();
	D4Volume* volume = new D4Volume(height);

	Vector3 d4Size = Vector3(height, height, height);
	d4->SetBoundingVolume((CollisionVolume*)volume);

	d4->GetTransform()
		.SetScale(d4Size)
		.SetPosition(position);

	d4->SetRenderObject(new RenderObject(&d4->GetTransform(), d4Mesh, d4Tex, basicShader));
	d4->SetPhysicsObject(new PhysicsObject(&d4->GetTransform(), d4->GetBoundingVolume()));
	d4->GetPhysicsObject()->SetInverseMass(inverseMass);
	d4->GetPhysicsObject()->InitSphereInertia();
	world->AddGameObject(d4);
	return d4;
}

GameObject* DiceRoller::AddD6(const Vector3& position, Vector3 dimensions, float inverseMass)
{
	GameObject* d6 = new GameObject();
	OBBVolume* volume = new OBBVolume(dimensions);
	d6->SetBoundingVolume((CollisionVolume*)volume);

	d6->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);


	d6->SetRenderObject(new RenderObject(&d6->GetTransform(), d6Mesh, d6Tex, basicShader));
	d6->SetPhysicsObject(new PhysicsObject(&d6->GetTransform(), d6->GetBoundingVolume()));

	d6->GetPhysicsObject()->SetInverseMass(inverseMass);
	d6->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(d6);

	return d6;
}

GameObject* DiceRoller::AddD8(const Vector3& position, float height, float inverseMass)
{
	GameObject* d8 = new GameObject();
	D8Volume* volume = new D8Volume(height);

	Vector3 d8Size = Vector3(height, height, height);
	d8->SetBoundingVolume((CollisionVolume*)volume);

	d8->GetTransform()
		.SetScale(d8Size)
		.SetPosition(position)
		.SetOrientation(Quaternion::AxisAngleToQuaterion({ 0.7,0.7,0.7 }, 20));

	d8->SetRenderObject(new RenderObject(&d8->GetTransform(), d8Mesh, d8Tex, basicShader));
	d8->SetPhysicsObject(new PhysicsObject(&d8->GetTransform(), d8->GetBoundingVolume()));
	d8->GetPhysicsObject()->SetInverseMass(inverseMass);
	d8->GetPhysicsObject()->InitD8Inertia();
	world->AddGameObject(d8);
	return d8;
}

GameObject* DiceRoller::AddD10(const Vector3& position, float height, float inverseMass)
{
	return nullptr;
}

GameObject* DiceRoller::AddD12(const Vector3& position, float height, float inverseMass)
{
	return nullptr;
}

GameObject* DiceRoller::AddD20(const Vector3& position, float height, float inverseMass)
{
	GameObject* d20 = new GameObject();
	D20Volume* volume = new D20Volume(height);

	Vector3 d20Size = Vector3(height, height, height);
	d20->SetBoundingVolume((CollisionVolume*)volume);

	d20->GetTransform()
		.SetScale(d20Size)
		.SetPosition(position);

	d20->SetRenderObject(new RenderObject(&d20->GetTransform(), d20Mesh, d20Tex, basicShader));
	d20->SetPhysicsObject(new PhysicsObject(&d20->GetTransform(), d20->GetBoundingVolume()));
	d20->GetPhysicsObject()->SetInverseMass(inverseMass);
	d20->GetPhysicsObject()->InitSphereInertia();
	world->AddGameObject(d20);
	return d20;
}

GameObject* DiceRoller::AddCubeToWorld(const Vector3& position, Vector3 dimensions, Texture* tex, float inverseMass) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	if (tex == nullptr) tex = basicTex;
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, tex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* DiceRoller::AddFloorToWorld(const Vector3& position, const Vector3& dimensions) {
	GameObject* floor = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(dimensions * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, woodTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));
	floor->SetCollisionLayer(staticObj);

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}


/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple'
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* DiceRoller::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

void DiceRoller::InitDiceTray()
{
	Vector3 dimensions = { 10,2,10 };
	GameObject* floor = AddFloorToWorld({ 0,0,0 }, dimensions);
	AddCubeToWorld({ 0,dimensions.y,dimensions.z - 1.0f }, { dimensions.x,3,1 }, woodTex, 0);
	AddCubeToWorld({ 0,dimensions.y,-dimensions.z + 1.0f }, { dimensions.x,3,1 }, woodTex, 0);
	AddCubeToWorld({ dimensions.x - 1.0f,dimensions.y,0 }, { 1,3,dimensions.z }, woodTex, 0);
	AddCubeToWorld({ -dimensions.x + 1.0f,dimensions.y,0 }, { 1,3,dimensions.z }, woodTex, 0);
}

void DiceRoller::SelectObject()
{
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Left))
	{
		Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true))
		{
			GameObject* closest = (GameObject*)closestCollision.node;
			AvailableDice dice = MAX;
			if (closest->GetName() == "selD4")
				dice = d4;
			else if (closest->GetName() == "selD6")
				dice = d6;
			else if (closest->GetName() == "selD8")
				dice = d8;
			else if (closest->GetName() == "selD20")
				dice = d20;
			else
				return;


			//deselect if already present
			if (selectedDice[dice])
			{
				selectedDice[dice]->GetRenderObject()->SetColour({ 1,1,1,1 });
				selectedDice[dice] = nullptr;
			}
			else
			{
				selectedDice[dice] = closest;
				selectedDice[dice]->GetRenderObject()->SetColour({ 0,1,0,1 });
			}
		}
	}
}
