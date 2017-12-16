//basic code and assets are from video how to make asteroids in 3 minutes
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
#include <cstring>
#include <time.h>
#include <list>

using namespace sf;
using namespace std;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;


Texture t1, t2, t3, t4, t5, t6, t7;
int score = 0;
int level = 1;
int screen = 1;
int life = 3;



Font font;
Text Life;
Text Score;
Text Menu;
Text GameOver;
Text Level;

SoundBuffer buff1;
SoundBuffer buff2;
SoundBuffer buff3;
SoundBuffer buff4;
SoundBuffer buff5;
Sound boom;
Sound fire;
Sound crash;
Sound complete;
Sound gameover;
Music music;

class Animation
{
public:
	float Frame, speed;
	Sprite sprite;
	std::vector<IntRect> frames;

	Animation() {}

	Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
		Frame = 0;
		speed = Speed;

		for (int i = 0; i<count; i++)
			frames.push_back(IntRect(x + i*w, y, w, h));

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}


	void update()
	{
		Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n>0) sprite.setTextureRect(frames[int(Frame)]);
	}

	bool isEnd()
	{
		return Frame + speed >= frames.size();
	}

};


Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);



class Entity
{
public:
	float x, y, dx, dy, R, angle;
	bool life;
	bool justCollide = false;
	float speed = 1;
	std::string name;
	Animation anim;

	Entity()
	{
		life = 1;
	}

	void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1)
	{
		anim = a;
		x = X; y = Y;
		angle = Angle;
		R = radius;
	}

	virtual void update() {};

	void draw(RenderWindow &app)
	{
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);

		CircleShape circle(R);
		circle.setFillColor(Color(255, 0, 0, 170));
		circle.setPosition(x, y);
		circle.setOrigin(R, R);
	}

	virtual ~Entity() {};
};

list<Entity*> entities;


class asteroid : public Entity
{
public:
	asteroid()
	{
		dx = rand() % 8 - 4;
		dy = rand() % 8 - 4;
		name = "asteroid";
	}

	void  update()
	{
		x += dx * speed;
		y += dy * speed;

		if (x>W) x = 0;  if (x<0) x = W;
		if (y>H) y = 0;  if (y<0) y = H;
	}

};



class bullet : public Entity
{
public:
	bullet()
	{
		name = "bullet";
	}

	void  update()
	{
		dx = cos(angle*DEGTORAD) * 6;
		dy = sin(angle*DEGTORAD) * 6;
		x += dx;
		y += dy;

		if (x>W || x<0 || y>H || y<0) life = 0;
	}

};


class player : public Entity
{
public:
	bool thrust;

	player()
	{
		name = "player";
	}

	void update()
	{
		if (thrust)
		{
			dx += cos(angle*DEGTORAD)*0.2;
			dy += sin(angle*DEGTORAD)*0.2;
		}
		else
		{
			dx *= 0.99;
			dy *= 0.99;
		}

		int maxSpeed = 15;
		float speed = sqrt(dx*dx + dy*dy);
		if (speed>maxSpeed)
		{
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}

		x += dx;
		y += dy;

		if (x>W) x = 0; if (x<0) x = W;
		if (y>H) y = 0; if (y<0) y = H;
	}

};

player *p;


bool isCollide(Entity *a, Entity *b)
{
	return (b->x - a->x)*(b->x - a->x) +
		(b->y - a->y)*(b->y - a->y)<
		(a->R + b->R)*(a->R + b->R);
}


void generateAsteroid(int num)
{
	int asteroidCount = 0;

	for (int i = 0; asteroidCount < num; i++)
	{
		bool collide = false;
		asteroid *a = new asteroid();
		a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
		a->speed = level;

		for (auto b : entities)
			if (isCollide(a, b))
			{
				collide = true;
			}
		if (!collide)
		{
			entities.push_back(a);
			asteroidCount++;
		}
	}
}


void Initial()
{
	generateAsteroid(3 * level);
	p = new player();
	p->settings(sPlayer, W / 2, H / 2, 0, 20);
	p->dx = 0;
	p->dy = 0;
	entities.push_back(p);

	Score.setFont(font);
	Score.setCharacterSize(50);
	Score.setPosition(100 , 100);
	Score.setFillColor(sf::Color::Green);
	Score.setString("Score: 0");

	Life.setFont(font);
	Life.setCharacterSize(50);
	Life.setPosition(100, 150);
	Life.setFillColor(sf::Color::Green);
	Life.setString("Life:3");

	Level.setFont(font);
	Level.setCharacterSize(50);
	Level.setPosition(1000, 100);
	Level.setFillColor(sf::Color::Green);
	Level.setString("Level:1");

}


int main()
{
	music.openFromFile("BGM.ogg");
	music.play();
	srand(time(0));
	RenderWindow app(VideoMode(W, H), "Asteroids!");
	app.setFramerateLimit(60);

	font.loadFromFile("arial.ttf");

	t1.loadFromFile("spaceship.png");
	t2.loadFromFile("background.jpg");
	t3.loadFromFile("type_C.png");
	t4.loadFromFile("rock.png");
	t5.loadFromFile("fire_blue.png");
	t6.loadFromFile("rock_small.png");
	t7.loadFromFile("type_B.png");

	buff1.loadFromFile("ball_lost.wav");
	boom.setBuffer(buff1);
	buff2.loadFromFile("fire.wav");
	fire.setBuffer(buff2);
	buff3.loadFromFile("ball_lost.wav");
	crash.setBuffer(buff3);
	buff4.loadFromFile("level_complete.wav");
	complete.setBuffer(buff4);
	buff5.loadFromFile("gameover.wav");
	gameover.setBuffer(buff5);
	

	t1.setSmooth(true);
	t2.setSmooth(true);

	Sprite background(t2);


	Menu.setFont(font);
	Menu.setCharacterSize(50);
	Menu.setPosition(100,100);
	Menu.setFillColor(sf::Color::Green);
	Menu.setString("Asteroids\n\n1. Press Space to Start game! \n\n2. Press Esc to Exit!");



	while (app.isOpen())
	{

		Event event;
		while (app.pollEvent(event))
		{
			
			
			if (event.type == Event::Closed || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape) && screen == 1))
				app.close();
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape) && screen == 2)
			{
				screen = 1;
				for (auto i = entities.begin(); i != entities.end();)
				{
					Entity *e = *i;
					i = entities.erase(i);
					delete e;
				}

				life = 3;
				score = 0;
				level = 1;
			}

			if (event.type = Event::KeyPressed)
				if (event.key.code == Keyboard::Space && screen == 1)
				{
					screen = 2;
					level = 1;
					for (auto i = entities.begin(); i != entities.end();)
					{
						Entity *e = *i;
						i = entities.erase(i);
						delete e;
					}

					Initial();
					life = 3;
					score = 0;
				}
			if (event.key.code == Keyboard::Space && screen == 2)
			{
				bullet *b = new bullet();
				b->settings(sBullet, p->x, p->y, p->angle, 10);
				entities.push_back(b);
				fire.play();
			}
			if (event.key.code == Keyboard::Space && screen == 3)
			{
				screen = 1;
			}
		}

		app.draw(background);
		if (screen == 1)
		{
			app.draw(Menu);
		}

		else if (screen == 2)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 2;
			if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 2;
			if (Keyboard::isKeyPressed(Keyboard::Up)) p->thrust = true;
			else p->thrust = false;



			for (auto a : entities)
				for (auto b : entities)
				{
					if (a->name == "asteroid" && b->name == "bullet" && a->life && b->life)
						if (isCollide(a, b))
						{
							boom.play();
							score++;
							Score.setString("Score:" + to_string(score));

							a->life = false;
							b->life = false;

							Entity *e = new Entity();
							e->settings(sExplosion, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);

							if (a->R == 15)
							{
								Entity *e = new asteroid();
								e->settings(sRock_small, a->x + 11, a->y, 0, 10);
								e->speed = level;
								Level.setString("Level:" + to_string(level));
								float ta = e->dx;
								float tb = e->dy;
								entities.push_back(e);
								e = new asteroid();
								e->settings(sRock_small, a->x - 11, a->y, 0, 10);
								e->speed = level;
								e->dx = -ta;
								e->dy = -tb;
								entities.push_back(e);
							}
							if (a->R == 25)
							{
								Entity *e = new asteroid();
								e->settings(sRock, a->x + 16, a->y, 0, 15);
								e->speed = level;
								Level.setString("Level:" + to_string(level));
								float ta = e->dx;
								float tb = e->dy;
								entities.push_back(e);
								e = new asteroid();
								e->settings(sRock, a->x - 16, a->y, 0, 15);
								e->speed = level;
								e->dx = -ta;
								e->dy = -tb;
								entities.push_back(e);
							}
						}

					if (a->name == "player" && b->name == "asteroid")
						if (isCollide(a, b))
						{
							
							crash.play();
							music.play();
							life--;
							Life.setString("Life:" + to_string(life));
							b->life = false;

							Entity *e = new Entity();
							e->settings(sExplosion_ship, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);

							p->settings(sPlayer, W / 2, H / 2, 0, 20);
							p->dx = 0; p->dy = 0;
						}

					if (a->name == "asteroid" && b->name == "asteroid" && !(a->x == b->x && a->y == b->y) && a->life && b->life)
						if (isCollide(a, b))
						{
							if (!a->justCollide && !b->justCollide)
							{
								a->dx = -a->dx;
								a->dy = -a->dy;
								b->dx = -b->dx;
								b->dy = -b->dy;
								a->justCollide = true;
								b->justCollide = true;
							}
						}
				}
			for (auto a : entities)
				for (auto b : entities) {
					a->justCollide = false;
					b->justCollide = false;
				}

			if (p->thrust)
			{
				p->anim = sPlayer_go;
			}
			else   p->anim = sPlayer;


			for (auto e : entities)
				if (e->name == "explosion")
					if (e->anim.isEnd()) e->life = 0;


			bool clearAll = true;
			for (auto e : entities)
				if (e->name == "asteroid"&&e->life)
					clearAll = false;
			if (clearAll)
			{
				level++;
				complete.play();
				generateAsteroid(3 * level);
			}


			for (auto i = entities.begin(); i != entities.end();)
			{
				Entity *e = *i;

				e->update();
				e->anim.update();

				if (e->life == false) { i = entities.erase(i); delete e; }
				else i++;
			}


			for (auto i : entities)
				i->draw(app);

			app.draw(Score);
			app.draw(Life);
			app.draw(Level);
			if (life <= 0)
			{
				for (auto i = entities.begin(); i != entities.end();)
				{
					Entity *e = *i;
					i = entities.erase(i);
					delete e;
				}
				screen = 3;
			}
		}

		else if (screen == 3)
		{
			gameover.play();
			app.draw(Score);
			app.draw(Life);
			app.draw(Level);
			GameOver.setFont(font);
			GameOver.setCharacterSize(50);
			GameOver.setPosition(W / 5, H / 3);
			GameOver.setFillColor(sf::Color::Green);
			GameOver.setString("Game Over!\n\nPress Space to restart");
			app.draw(GameOver);
			

		}


		app.display();
	}

	return 0;
}
