#include "GameManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "iostream"

using namespace std;

int GameManager::initialize()
{
	cout << "Starting Engine...\n\n";

	if (Window.initialize())
	{
		cout << "WindowManager failed to start\n";
	}
	cout << "WindowManager started\n";
	if (Resource.Initialize())
	{
		cout << "ResourceManager failed to start\n";
	}
	cout << "ResourceManager started\n";
	if (World.Initialize())
	{
		cout << "WorldManger failed to start\n";
	}
	cout << "WorldManager started\n";

	cout << "GameManager Started\n\n";

	cout << "Loading game with scene " << World.getBaseScene() << "\n\n";
	World.loadScene(World.getBaseScene());

	return 0;
}

void GameManager::run()
{
	cout << "Starting Game...\n\n";
	
	World.Start();

	while (!glfwWindowShouldClose(Window.getWindow()))
	{
		m_deltaTime = getTime() - m_timeOnLastFrame;
		Window.clear();

		glfwPollEvents();

		World.Update();
		World.LateUpdate();

		Window.Swap();
		m_timeOnLastFrame = getTime();
	}
}

void GameManager::terminate()
{
	cout << "Stopping Engine...\n\n";

	Window.terminate();
	cout << "Shut Down Window Manager\n";
	Input.terminate();
	cout << "Shut Down Input Manager\n";
	Resource.Terminate(); 
	cout << "Shut Down Resource Manager\n";
	World.Terminate();
	cout << "Shut Down World Manager\n";

	cout << "Shut Down Game Manager\n";
}

GameManager& GameManager::getInstance()
{
	static GameManager single;
	return single;
}

float GameManager::getTime()
{
	return static_cast<float>(glfwGetTime());
}

float GameManager::getDeltaTime() const
{
	return m_deltaTime;
}

GameManager::GameManager() = default;

void GameManager::quit()
{
	glfwSetWindowShouldClose(Window.getWindow(), GLFW_TRUE);
}
