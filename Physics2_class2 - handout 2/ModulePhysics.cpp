#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"


ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

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
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
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

void ModulePhysics::CreateBox(float x, float y)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(x, y);

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	shape.SetAsBox(2.0f, 1.0f);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0;

	b->CreateFixture(&fixture);

	if (number_of_bodies < 100)
	{
		bodies[number_of_bodies].body = b;
		++number_of_bodies;
	}

}

void ModulePhysics::CreateCircle(float x, float y)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(x, y);

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(25);
	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);
	if(number_of_bodies < 100)
	{ 
	bodies[number_of_bodies].body = b;
	++number_of_bodies;
	}
}

void ModulePhysics::CreateH(float x, float y)
{
	int points[24] = {
		-38, 80,
		-44, -54,
		-16, -60,
		-16, -17,
		19, -19,
		19, -79,
		61, -77,
		57, 73,
		17, 78,
		20, 16,
		-25, 13,
		-9, 72
	};
	b2Vec2 array[12];

	int vector_counter = 0;
	int vertice_counter = 0;

	while (vertice_counter < 24)
	{
		array[vector_counter].x = PIXEL_TO_METERS(points[vertice_counter]);

		vertice_counter++;

		array[vector_counter].y = PIXEL_TO_METERS(points[vertice_counter]);

		vertice_counter++;
		vector_counter++;
	}

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(x, y);

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	shape.CreateLoop(array, 12);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	if (number_of_bodies < 100)
	{
		bodies[number_of_bodies].body = b;
		++number_of_bodies;
	}
}

void ModulePhysics::CreateRic(float x, float y)
{
	int rick_head[56] = {
		13, 36,
		41, 39,
		40, 0,
		76, 31,
		87, 4,
		94, 41,
		110, 35,
		103, 58,
		116, 66,
		109, 73,
		110, 84,
		104, 93,
		109, 100,
		102, 105,
		101, 116,
		106, 122,
		98, 126,
		86, 146,
		62, 147,
		45, 132,
		35, 136,
		38, 125,
		24, 123,
		30, 116,
		10, 103,
		29, 91,
		0, 77,
		29, 62
	};
	b2Vec2 array[28];

	int vector_counter = 0;
	int vertice_counter = 0;

	while (vertice_counter < 56)
	{
		array[vector_counter].x = PIXEL_TO_METERS(rick_head[vertice_counter]);

		vertice_counter++;

		array[vector_counter].y = PIXEL_TO_METERS(rick_head[vertice_counter]);

		vertice_counter++;
		vector_counter++;
	}

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(x, y);

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	shape.CreateLoop(array, 28);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	if (number_of_bodies < 100)
	{
		bodies[number_of_bodies].body = b;
		++number_of_bodies;
	}
}

b2Vec2 ModulePhysics::Position_for_drawing(b2Vec2 position, float radius)
{
	position.y = position.y - sin(45)*radius -4;

	position.x = position.x - cos(45)*radius - 12;

	return position;
}