#include "unit.h"
#include "s3ePointer.h"


Unit::Unit(float hp, float cost, float attack, float speed, 
		float munch_speed, float range, float sight,
		float spread_speed, float spread_radius, Player* owner,
		Game* game, CIwVec2 position)
		: hp(hp), cost(cost), attack(attack), speed(speed),
		  munch_speed(munch_speed), range(range), sight(sight),
		  spread_speed(spread_speed), spread_radius(spread_radius),
		  owner(owner), game(game), position(position)
{
}
/*
Unit::Unit(float _r, float _theta) : r(r), theta(theta) {}
*/


void Unit::renderSprite(int frameNumber, float angle, float scaleFactor) {
		
	int left = position.x;
	int top = position.y;	
	
	CIwSVec3* vertices = (CIwSVec3*)malloc(sizeof(CIwSVec3)*4);
	CIwSVec2* UVs = (CIwSVec2*)malloc(sizeof(CIwSVec2)*4);
	
	//set up model space vertices
	
	int vertexDist = scaleFactor*spriteSize/2;
	
	vertices[0] = CIwSVec3(-1*vertexDist, -1*vertexDist, -1);
	vertices[2] = CIwSVec3(vertexDist, -1*vertexDist, -1);
	vertices[3] = CIwSVec3(vertexDist, vertexDist, -1);
	vertices[1] = CIwSVec3(-1*vertexDist, vertexDist, -1);
	
	CIwMat modelTransform = CIwMat::g_Identity;
	modelTransform.SetRotZ(angle);
	modelTransform.SetTrans(CIwVec3(left, -1*top, 1));
	IwGxSetModelMatrix(&modelTransform, false);
	
	//set up UV offset for the given frame number
	//TODO This was figured out purely by trial and error, and only works
	//for sheets with 64x64 sprites. Need to figure out how Airplay interprets
	//UV coordinates - makes no sense to me right now.
	UVs[0] = CIwSVec2(frameNumber*682, 0);
	UVs[2] = CIwSVec2((frameNumber+1)*682, 0);
	UVs[3] = CIwSVec2((frameNumber+1)*682, 4096);
	UVs[1] = CIwSVec2(frameNumber*682, 4096);

	//render the image to screen
	IwGxSetUVStream(UVs);
	IwGxSetColStream(NULL);
	IwGxSetVertStreamModelSpace(vertices, 4);
	IwGxDrawPrims(IW_GX_QUAD_STRIP, NULL, 4);
	IwGxFlush();
	
	free(vertices);
	free(UVs);
}


int Unit::getId(){ return uid; }
void Unit::setId(int uid){ this->uid = uid; }



void Unit::setRTheta(float rad, float ang){ 
	r = rad;
	theta = ang;
	position.x = (r/sin(theta))-1;
	position.y = position.x*sin(theta);
}

Player& Unit::getOwner(){
	return *owner;
}

Game* Unit::getGame(){
	return game;
}

Unit* Unit::getAttacking(){return Attacking;}
void Unit::setAttacking(Unit* unit){Attacking = unit;}

Unit* Unit::getPursuing(){return Pursuing;}
void Unit::setPursuing(Unit* unit){Pursuing=unit;}

bool Unit::attacking(){if(Attacking!=NULL){return true;}else{return false;}}
bool Unit::pursuing(){if(Pursuing!=NULL){return true;}else{return false;}}



void Unit::setOwner(Player& p){
	owner = &p;
}

float Unit::getHp(){
	return hp;
}
float Unit::getRange(){return range;}

void Unit::setHp(float f){
	hp = f;
}

void Unit::decrementHp(float f){
	hp -= f;
}

void Unit::setPosition(int x, int y){
	position.x = x;
	position.y = y;
	r = sqrt(x^2 + y^2);
	theta = asin(y/x);
	
}

void Unit::setPosition(const CIwVec2& newPosition){
	position = newPosition;
	float x = newPosition.x;
	float y = newPosition.y;
	r = sqrt(x*x + y*y);
	theta = asin(y/x);
}

CIwSVec2 Unit::getPosition(){return position;}

float Unit::getR(){ return r; }
float Unit::getTheta(){ return theta; }

void Unit::increaseX(float x){}
void Unit::increaseY(float y){}
float Unit::getX(){return 0.0f;}
float Unit::getY(){return 0.0f;}

float Unit::getSpeed(){return speed;}

void Unit::Attack(){};
void Unit::RecieveDamage(){};

