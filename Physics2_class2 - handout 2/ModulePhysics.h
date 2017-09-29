#pragma once
#include "Module.h"
#include "Globals.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class b2World;
class b2Body;


#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif
// TODO 6: Create a small class that keeps a pointer to tghe b2Body
// and has a method to request the position
// then write the implementation in the .cpp
// Then make your circle creation function to return a pointer to that class
class BodyPos
{
public:
	b2Body* body;

	b2Vec2 GetPos()
	{
		b2Vec2 position = body->GetPosition();
		position.x = METERS_TO_PIXELS(position.x);
		position.y = METERS_TO_PIXELS(position.y);
		return position;
	}
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();


	
	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains
	void CreateCircle(float, float);

	void CreateBox(float, float);

	void CreateH(float, float);

	void CreateRic(float, float);

	b2Vec2 Position_for_drawing(b2Vec2 position, float radius);

	BodyPos bodies[1000];

	int number_of_bodies = 0;
private:


	bool debug;
	b2World* world;
};