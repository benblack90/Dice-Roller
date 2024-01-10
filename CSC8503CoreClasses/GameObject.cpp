#include "GameObject.h"
#include "CollisionDetection.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "NetworkObject.h"
#include "D4Volume.h"
#include "D8Volume.h"
#include "D20Volume.h"

using namespace NCL::CSC8503;

GameObject::GameObject(const std::string& objectName, CollisionLayer layer, bool useGravity)	{
	name			= objectName;
	worldID			= -1;
	isActive		= true;
	boundingVolume	= nullptr;
	physicsObject	= nullptr;
	renderObject	= nullptr;
	networkObject	= nullptr;
	this->layer = layer;
	this->useGravity = useGravity;
}

GameObject::~GameObject()	{
 	delete boundingVolume;
	delete physicsObject;
	delete renderObject;
	delete networkObject;
}

bool GameObject::GetBroadphaseAABB(Vector3&outSize) const {
	if (!boundingVolume) {
		return false;
	}
	outSize = broadphaseAABB;
	return true;
}

void GameObject::UpdateBroadphaseAABB() {
	if (!boundingVolume) {
		return;
	}
	if (boundingVolume->type == VolumeType::AABB) {
		broadphaseAABB = ((AABBVolume&)*boundingVolume).GetHalfDimensions();
	}
	else if (boundingVolume->type == VolumeType::Sphere) {
		float r = ((SphereVolume&)*boundingVolume).GetRadius();
		broadphaseAABB = Vector3(r, r, r);
	}
	else if (boundingVolume->type == VolumeType::OBB) {
		Matrix3 mat = Matrix3(transform.GetOrientation());
		mat = mat.Absolute();
		Vector3 halfSizes = ((OBBVolume&)*boundingVolume).GetHalfDimensions();
		broadphaseAABB = mat * halfSizes;
	}
	else if (boundingVolume->type == VolumeType::D4_Dice)
	{
		Matrix3 mat = Matrix3(transform.GetOrientation());
		mat = mat.Absolute();
		float sizes = ((D4Volume&)*boundingVolume).GetHeight() * ((D4Volume&)*boundingVolume).GetRatioOfHeightAboveOrigin();
		broadphaseAABB = mat * Vector3(sizes,sizes,sizes);
	}
	else if (boundingVolume->type == VolumeType::D8_Dice)
	{
		Matrix3 mat = Matrix3(transform.GetOrientation());
		mat = mat.Absolute();
		float sizes = ((D8Volume&)*boundingVolume).GetHeight();
		broadphaseAABB = mat * Vector3(sizes, sizes, sizes);
	}
	else if (boundingVolume->type == VolumeType::D20_Dice)
	{
		Matrix3 mat = Matrix3(transform.GetOrientation());
		mat = mat.Absolute();
		float sizes = ((D20Volume&)*boundingVolume).GetEdgeLength() * 2;
		broadphaseAABB = mat * Vector3(sizes, sizes, sizes);
	}
}