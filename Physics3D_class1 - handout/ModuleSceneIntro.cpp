#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	// TODO 2: Place the camera one unit up in Y and one unit to the right
	// experiment with different camera placements, then use LookAt()
	// to make it look at the center

	App->camera->Position.y = 1;

	App->camera->LookAt(vec3(0,0,0));
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 1: Create a Plane primitive. This uses the plane formula
	// so you have to express the normal of the plane to create 
	// a plane centered around 0,0. Make that it draw the axis for reference
	Plane boing_747 = Plane();

	boing_747.axis = true;

	boing_747.Render();
	// TODO 6: Draw a sphere of 0.5f radius around the center
	// Draw somewhere else a cube and a cylinder in wireframe

	Sphere cojón_de_mamut = Sphere(0.5f);
	cojón_de_mamut.SetPos(1, 1, 0);
	cojón_de_mamut.Render();

	Cylinder rabo_de_mamut = Cylinder(0.5f, 2);
	rabo_de_mamut.SetPos(-1, 1, 0);
	rabo_de_mamut.SetRotation(90,vec3(0,0,1));
	rabo_de_mamut.Render();


	Cube cabesa_de_mamuto = Cube();
	cabesa_de_mamuto.SetPos(3, 1, 0);
	cabesa_de_mamuto.SetRotation(90, vec3(0, 0, 1));
	cabesa_de_mamuto.Render();


	return UPDATE_CONTINUE;
}

