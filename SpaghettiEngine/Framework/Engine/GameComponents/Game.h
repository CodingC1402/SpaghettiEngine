#pragma once

typedef class GameTimer* PGameTimer;
typedef class InputSystem* PInputSystem;
typedef class SceneManager* PSceneManager;

class Game
{
	friend class App;
public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	
	void Init();
	void Update() const;
	void FixUpdate() const;
	void End();
protected:
	Game() = default;
	~Game();

	static Game* GetInstance();
private:
	PGameTimer timer = nullptr;
	PInputSystem input = nullptr;
	PSceneManager sceneManager = nullptr;

	static Game* __instance;
};