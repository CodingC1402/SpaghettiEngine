#pragma once

typedef class GameTimer* PGameTimer;
typedef class InputSystem* PInputSystem;
typedef class SceneManager* PSceneManager;

class Game
{
	friend class App;
	friend class Physic;
	friend class SceneManager;
public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	// Call the app to quit the game loop then call quit
	static void CallQuit();
protected:
	static void Init();
	static void Update();

	// Clean up resource
	static void Quit();

	// Will be called by physic
	static void FixUpdate();
	static void ResetGameTimer() noexcept;

	Game() = default;
	~Game();
private:
	inline static PGameTimer timer = nullptr;
	inline static PInputSystem input = nullptr;
	inline static PSceneManager sceneManager = nullptr;
};