#pragma once

typedef class GameTimer* PGameTimer;
typedef class InputSystem* PInputSystem;
typedef class SceneManager* PSceneManager;
typedef class Physic* PPhysic;

class Game
{
	friend class App;
public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	
	void Init();
	void Update() const;
protected:
	Game() = default;
	~Game();

	static Game* GetInstance();
private:
	PGameTimer timer = nullptr;
	PInputSystem input = nullptr;
	PSceneManager sceneManager = nullptr;
	PPhysic physic = nullptr;

	static Game* __instance;
};