#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <time.h>
#include <list>
#include "animation.h"
#include "entity.h"
using namespace sf;
using namespace std;

Entity::Entity()
{
	life = 1;
}

void Entity::settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1)
{
	anim = a;
	x = X; y = Y;
	angle = Angle;
	R = radius;
}

virtual void Entity::update() {};

void Entity::draw(RenderWindow &app)
{
	anim.sprite.setPosition(x, y);
	anim.sprite.setRotation(angle + 90);
	app.draw(anim.sprite);

	CircleShape circle(R);
	circle.setFillColor(Color(255, 0, 0, 170));
	circle.setPosition(x, y);
	circle.setOrigin(R, R);
	//app.draw(circle);
}

virtual ~Entity() {};