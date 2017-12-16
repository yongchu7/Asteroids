#pragma once
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
using namespace sf;
using namespace std;


class Animation
{
public:
	float Frame, speed;
	Sprite sprite;
	std::vector<IntRect> frames;

	Animation(Texture &t, int x, int y, int w, int h, int count, float Speed);
	void update();
	bool isEnd();
};