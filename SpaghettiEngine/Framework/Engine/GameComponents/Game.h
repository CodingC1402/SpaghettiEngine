#pragma once

typedef class GameTimer* PGameTimer;
typedef class InputSystem* PInputSystem;
typedef class SceneManager* PSceneManager;

class Game
{
	friend class App;
	friend class Physic;
public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
protected:
	static void Init();
	static void Update();
	static void End();

	// Will be called by physic
	static void FixUpdate();

	Game() = default;
	~Game();
private:
	inline static PGameTimer timer = nullptr;
	inline static PInputSystem input = nullptr;
	inline static PSceneManager sceneManager = nullptr;
};