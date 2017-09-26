#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it
	b2Vec2 gravity(0.0f, 9.8f);

	world = new b2World(gravity);

	// TODO 4: Create a a big static circle as "ground"

	//BIG FUCKING CIRCLE
	b2BodyDef ground_whato;
	ground_whato.type = b2_staticBody;
	ground_whato.position.Set(5.0f, 4.5f);

	b2CircleShape shape;
	shape.m_radius = 1.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	
	b2Body *ground = world->CreateBody(&ground_whato);
	ground->CreateFixture(&fixture);

	//BIG FUCKING BOX
	b2BodyDef ground_whato2;
	ground_whato2.type = b2_staticBody;
	ground_whato2.position.Set(5.0f, 6.25f);

	b2PolygonShape shape2;
	shape2.SetAsBox(4.0f, 0.25f);

	
	b2FixtureDef fixture2;
	fixture2.shape = &shape2;

	b2Body *ground2 = world->CreateBody(&ground_whato2);
	ground2->CreateFixture(&fixture2);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)

	world->Step(1.0f / 60.0f, 8, 3);
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		b2BodyDef ground_whato;
		ground_whato.type = b2_dynamicBody;
		ground_whato.position.Set(converter(App->input->GetMouseX(), 1), converter(App->input->GetMouseY(), 1));

		b2CircleShape shape;
		shape.m_radius = converter(rand()%100+1, 1);

		b2FixtureDef fixture;
		fixture.shape = &shape;

		b2Body *ground = world->CreateBody(&ground_whato);
		ground->CreateFixture(&fixture);
	}
	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(converter(pos.x,0), converter(pos.y,0), converter(shape->m_radius, 0), 255, 255, 255);
				}
				break;
				case b2Shape::e_polygon:
				{
				
					b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					SDL_Rect filler;
					filler.x = converter(pos.x- shape->m_vertices->x, 0); filler.y = converter(pos.y - shape->m_vertices->y, 0); filler.w = converter (shape->m_vertices->x*2, 0); filler.h = converter(shape->m_vertices->y*2,0);
					App->renderer->DrawQuad(filler, 255, 255, 255);
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;
	return true;
}

float ModulePhysics::converter(float number, bool type)
{
	if (!type)
	{
		return number * 100;
	}
	else
	{
		return number/100;
	}
}