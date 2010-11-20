#ifndef _UNIT_H
#define _UNIT_H

#include "IwGx.h"
#include "IwGeom.h"
#include "game.h"
#include "player.h"

class Unit {
    protected:
		/* Preliminary stats. Subject to change. */
	    float hp;
		float cost;
		float attack;
		float speed;
		float munch_speed;
		float range;
		float sight;
		float spread_speed;
		float spread_radius;

		float r;
		float theta;

		Player *owner;
		Game* game;
		CIwSVec2 position;
		int uid;
	
		//info for sprite animation
		int spriteSize;
		int numFrames;
		int curFrame;
	
		Unit* Attacking;
		Unit* Pursuing;
	
		/**
		Utility method that subclasses will use to render their sprites. Assumes that 
		current material has already been set to the sprite image.
		 
		@param frameNumber which frame of the sprite sheet to display (indexed from 0)
		@param angle angle to rotate the sprite by
		@param scaleFactor factor to scale the sprite by
		*/
		void renderSprite(int frameNumber, float angle, float scaleFactor);

        
    public:
	
		Unit(float hp, float cost, float attack, float speed, 
				float munch_speed, float range, float sight,
				float spread_speed, float spread_radius, Player* owner,
				Game* game, CIwVec2 position);

		virtual ~Unit() {};

		void setPosition(int32 x, int32 y);
		void setPosition(const CIwVec2& position);
		
		CIwSVec2 getPosition();
    
		float getSpeed(); 
		float getRange();
		int getId();
		void setId(int uuid);
        
		Player& getOwner();
		void setOwner(Player& p);
		
		Game* getGame();
	
		Unit* getAttacking();
		void setAttacking(Unit* unit);
		
		Unit* getPursuing();
		void setPursuing(Unit* unit);
	
		bool attacking();
		bool pursuing();
        
		float getHp();
		void setHp(float f);
		void decrementHp(float f);
		
		float getR();
		float getTheta();
	
	
		//To deal with simultaneous altering of R Theta and X,Y Pos setR and setTheta must be combined
		//Please Refractor accordingly.
	
		void setRTheta(float x, float y);
	
		/* IGNORE THE FAllOWING.  I already updated set position to change theta and r
		 and vice versa so use which ever coordinate system works best for you cause
		 their are times where either works.
		 
		   Units are going to keep track of their location in terms of r, theta, but
		   the following methods preserve distance, so calling increaseX(5.0f) might
		   change theta by more or less than 5. Additionally, it will wrap around
		   as appropriate.
		 */
		void increaseX(float x);
		void increaseY(float y);
		float getX();
		float getY();
	
	
		virtual char* getTextureName() = 0;
		virtual bool update() = 0;
		virtual void display() = 0;
		
		void Attack();
		void RecieveDammage(); 
	
};

#endif
