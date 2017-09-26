#pragma once
#include "Module.h"
#include "Globals.h"

#include "Box2D/Box2D/Box2D.h"
#pragma comment (lib, "Box2D/libx86/Debug/Box2D.lib")

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();
	float converter(float, bool); // 0 es de m->p y 1 es de p->m

private:

	bool debug;
	b2World* world;
};