#include "muncher.h"

Muncher::Muncher(Player* owner, Game* game, CIwFVec2 position)
		 : Unit(100.0f, 50.0f, 10.0f, 15.0f, 10.0f, 5.0f, 10.0f, 0.0f, 0.0f, owner, game, position)
{
	spriteSize = 256;
	numFrames = 12;
	curFrame = 0;
	circleOffset = 0.0;
}

bool Muncher::update() {

	game->getAI()->updateAI(this);
	return true;
}

void Muncher::display(float worldRot) {
	renderSprite(curFrame++, 90, 0.15, worldRot);
}

char* Muncher::getTextureName() {
	return "muncher_sprite_sheet";
}