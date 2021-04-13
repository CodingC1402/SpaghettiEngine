#pragma once
#include "RigidBody.h"
#include <list>
#include <vector>

typedef class Rigidbody* PRigidbody;
typedef class Physic* PPhysic;

class Physic
{
	friend class App;
public:
	void Init();
	void Update();
	static Physic* GetInstance();
protected:
	Physic() = default;
	~Physic();
	static PPhysic __instance;
private:
	std::list<PRigidbody> renderBuffer;
};