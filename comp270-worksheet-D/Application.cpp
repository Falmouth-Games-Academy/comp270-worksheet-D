#include "stdafx.h"
#include "Application.h"
// Constructor -- initialise application-specific data here
Application::Application()
{
}

Application::~Application()
{
}

// Run the application
// Return true if the application finishes successfully, or false if an error occurs
bool Application::run()
{
	// Initialisation
	if (!initSDL())
		return false;

	setupScene();

	// Main loop
	m_quit = false;
	while (!m_quit)
	{
		// Process events
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			processEvent(ev);
		}

		// Process benchmark
		if (benchmark)
			processBenchmark();

		// Render current state
		update();
		render();
		SDL_RenderPresent(m_renderer);
	}

	// Shutdown
	shutdownSDL();
	return true;
}

// Initialise the required parts of the SDL library
// Return true if initialisation is successful, or false if initialisation fails
bool Application::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_window = SDL_CreateWindow("COMP270", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, c_windowWidth, c_windowHeight, SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

// Shutdown the SDL library
void Application::shutdownSDL()
{
	if (m_renderer)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	SDL_Quit();
}

// Induce custom benchmarking events
void Application::processBenchmark()
{
	m_player.rotate(-0.1f);
	m_player.applyThrust(0.1f);
	shoot(0.5f);
}

// Process a single event
void Application::processEvent(const SDL_Event &ev)
{
	switch (ev.type)
	{
	case SDL_QUIT:
		m_quit = true;
		break;

	case SDL_KEYDOWN:
	{
		if (ev.key.keysym.sym == SDLK_ESCAPE)
			m_quit = true;
		else
		{
			const Uint8 *state = SDL_GetKeyboardState(NULL);
			
			// Player movement
			if (state[SDL_SCANCODE_LEFT])
				m_player.rotate(-0.1f);

			if (state[SDL_SCANCODE_RIGHT])
				m_player.rotate(0.1f);

			if (state[SDL_SCANCODE_UP])
				m_player.applyThrust(0.5f);

			// Trigger
			if (ev.key.keysym.sym == SDLK_SPACE)
				shoot(0.5f);
		}
		break;
	}
	default:
		break;
	}
}

// Set up the initial objects in the scene
void Application::setupScene()
{
	m_player.setPosition(c_windowWidth / 2, c_windowHeight / 2);

	// Create some initial asteroids, anywhere on the screen and heading in any direction
	for (unsigned i = 0; i < c_numInitialAsteriods; ++i)
	{
		Point2D pos(rand() % c_windowWidth, rand() % c_windowHeight);
		Vector2D vel(float(rand() % 100) / 50.0f - 1.0f, float(rand() % 100) / 50.0f - 1.0f);
		vel.normalise();
		vel *= (rand() % 5) / 8 + 0.2f;

		spawnAsteroid(pos, vel, c_asteroidMaxScale);
	}
}

void Application::update()
{
	m_player.update();

	// Move the bullets to their new positions
	for (auto& bullet : arr_bullets)
	{
		bullet->update();
	}

	// Spawn asteroids from the corners, heading inwards
	if ((rand() % 100 < c_spawnRate || (benchmark && rand() % 100 < b_spawnRate)) && asteroidCount < MAXASTEROIDS)
	{
		bool left = rand() % 100 > 50;
		bool top = rand() % 100 > 50;
		
		Point2D pos(left ? 0 : c_windowWidth, top ? 0 : c_windowHeight);
		Vector2D vel((rand() % 100) * c_windowWidth / 100 - pos.x,
			(rand() % 100) * c_windowHeight / 100 - pos.y);
		vel.normalise();
		vel *= (rand() % 5) / 8 + 0.2f;

		spawnAsteroid(pos, vel, c_asteroidMaxScale);
	}

	// Update asteroids and check for collisions with bullets
	for (auto& asteroid : arr_asteroids)
	{
		if (asteroid->isAlive())
		{
			// Move the asteroid to its new position
			asteroid->update();

			// See if any of the (live) bullets are inside this asteroid
			for (auto& bullet : arr_bullets)
			{
				if (bullet != NULL)
				{
					if ((bullet->getPosition() - asteroid->getPosition()).magnitudeSquared() < COLLISIONRANGE * COLLISIONRANGE)
					{
						if (bullet->isAlive() && asteroid->pointIsInside(bullet->getPosition()))
						{
							// Shatter the asteroid by killing it and spawning some new ones
							// roughly where it is
							unsigned numFragments = rand() % (c_maxFragments - 2) + 2;
							for (unsigned i = 0; i < numFragments; ++i)
							{
								// Only add new asteroids if we are below the cap
								if (asteroidCount < MAXASTEROIDS)
								{
									Vector2D vel(float(rand() % 100) / 50.0f - 1.0f, float(rand() % 100) / 50.0f - 1.0f);
									vel.normalise();
									vel *= (rand() % 5) / 8 + 0.2f;

									Point2D pos = asteroid->getPosition() + Vector2D(float(rand() % 10) / 5.0f - 1.0f, float(rand() % 10) / 5.0f - 1.0f) * asteroid->getScale();
									spawnAsteroid(pos, vel, asteroid->getScale() / float(numFragments-1));
								}
							}

							asteroid->kill();
							bullet->kill();

							break;
						}
					}

					// If bullet is offscreen, destroy it - I am aware of the existence of isOffScreen(), but I believe this implementation is equally suitable
					if (bullet->getPosition().x < 0 || bullet->getPosition().y < 0 || bullet->getPosition().x > c_windowWidth || bullet->getPosition().y > c_windowHeight)
						bullet->kill();
				}
			}

			// If asteroid is offscreen, destroy it
			if (asteroid->getPosition().x < 0 || asteroid->getPosition().y < 0 || asteroid->getPosition().x > c_windowWidth || asteroid->getPosition().y > c_windowHeight)
				asteroid->kill();
		}
	}

	// Perform garbage collection on bullets/asteroids marked as 'dead'
	cleaup();
}

// Render the scene
void Application::render()
{
	// Clear screen
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);
	
	SDL_SetRenderDrawColor(m_renderer, 255, 100, 100, 255);
	m_player.draw(m_renderer);

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, 255);
	for (int i = 0; i < MAXBULLETS; i++)
	{
		if (arr_bullets[i] != NULL)
		{
			arr_bullets[i]->draw(m_renderer);
		}
	}

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		if (arr_asteroids[i] != NULL)
		{
			arr_asteroids[i]->draw(m_renderer);
		}
	}
}

void Application::shoot(float speed)
{
	for (int i = 0; i < MAXBULLETS; i++)
	{
		// Replace a null pointer with a new bullet
		if (arr_bullets[i] == NULL)
		{
			bulletCount++;
			arr_bullets[i] = new Bullet(m_player.getGunPosition(), m_player.getDirection() * (m_player.getSpeed() + speed));
			return;
		}
	}
}

void Application::spawnAsteroid(Point2D pos, Vector2D vel, float maxScale)
{
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		// Replace a null pointer witha  new asteroid
		if (arr_asteroids[i] == NULL)
		{
			asteroidCount++;
			arr_asteroids[i] = new Asteroid(pos, vel, float(rand() % 10) * maxScale / 15.0f + maxScale * 0.25f,
				float(rand() % 10) * c_asteroidMaxRotationSpeed / 5.0f - c_asteroidMaxRotationSpeed);;
			return;
		}
	}
}

// Iterate through all asteroids and bullets and delete those that have been marked as 'dead'
void Application::cleaup()
{
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		if (arr_asteroids[i] != nullptr)
		{
			if (!arr_asteroids[i]->isAlive())
			{
				delete arr_asteroids[i];
				arr_asteroids[i] = NULL;
				asteroidCount--;
			}
		}
	}

	for (int i = 0; i < MAXBULLETS; i++)
	{
		if (arr_bullets[i] != nullptr)
		{
			if (!arr_bullets[i]->isAlive())
			{
				delete arr_bullets[i];
				arr_bullets[i] = NULL;
				bulletCount--;
			}
		}
	}
}

bool Application::isOffscreen(const Drifter * drifter) const
{
	Point2D pos = drifter->getPosition();
	return (pos.x < 0.0f || pos.x > c_windowWidth || pos.y < 0.0f || pos.y > c_windowHeight);
}

// Application entry point
int main(int, char**)
{
	Application application;
	if (application.run())
		return 0;
	else
		return 1;
}
