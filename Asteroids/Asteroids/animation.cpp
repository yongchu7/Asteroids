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
using namespace sf;
using namespace std;

Animation::Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
{
	Frame = 0;
	speed = Speed;

	for (int i = 0; i<count; i++)
		frames.push_back(IntRect(x + i*w, y, w, h));

	sprite.setTexture(t);
	sprite.setOrigin(w / 2, h / 2);
	sprite.setTextureRect(frames[0]);
};

void Animation::update()
{
	Frame += speed;
	int n = frames.size();
	if (Frame >= n) Frame -= n;
	if (n>0) sprite.setTextureRect(frames[int(Frame)]);
}

bool Animation::isEnd()
{
	return Frame + speed >= frames.size();
}