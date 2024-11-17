#include "Game.h"

int main() {
	// Program entry point.
	Game game; // Creating our game object.
	while (!game.GetWindow()->IsDone()) {
		// Game loop.
		game.Update();
		game.Render();
	}
}