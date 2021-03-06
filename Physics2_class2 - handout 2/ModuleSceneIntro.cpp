#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

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
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
	// On space bar press, create a circle on mouse position
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->physics->CreateCircle(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
		
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		// TODO 1: When pressing 2, create a box on the mouse position
		App->physics->CreateBox(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
	}

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!
		App->physics->CreateH(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!

		App->physics->CreateRic(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	}
	// TODO 7: Draw all the circles using "circle" texture

	int counter = 0;
	while (counter < App->physics->number_of_bodies)
	{ 
		if (App->physics->bodies[counter].shape == _circle)
		{ 
			b2Vec2 real_position = App->physics->Position_for_drawing(App->physics->bodies[counter].GetPos(), 25);
			App->renderer->Blit(circle, real_position.x, real_position.y,nullptr,0, App->physics->bodies[counter].GetRotation());
			counter++;
		}
		else if(App->physics->bodies[counter].shape == _polygon)
		{
			b2Vec2 real_position = App->physics->Position_for_drawing(App->physics->bodies[counter].GetPos(), 25);
			App->renderer->Blit(box, real_position.x-25, real_position.y, nullptr, 0, App->physics->bodies[counter].GetRotation());
			counter++;
		}
		else
		{
			b2Vec2 real_position = App->physics->Position_for_drawing(App->physics->bodies[counter].GetPos(), 25);
			App->renderer->Blit(rick, real_position.x +25, real_position.y+25);
			counter++;
		}
	}
	return UPDATE_CONTINUE;
}
