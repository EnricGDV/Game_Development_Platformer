#pragma once

#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"

struct SDL_Texture;

struct PlayerData
{
	Animation*			current_animation;
	Animation			angel_idle;
	Animation			angel_moving;
	Animation			angel_jumping;
	Animation			angel_falling;
	bool				canDJump;
	bool				isDemon;
	bool				canDash;
	bool				mirror;
	bool				isJumping;
	bool				onFloor;
	int					xDirection;
	iPoint				jumpSpeed;
	iPoint				acceleration;
	iPoint				maxSpeed;
	iPoint				speed;
	iPoint				position;
	iPoint				offSet;
	Collider*			collider;
	//SDL_Texture*		PlayerTexture;
	


	//void LoadPushbacks();
};

class j1Player : public j1Module
{
public:

	j1Player();

	//Destructor
	virtual ~j1Player();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void Draw();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;



	bool Clean();

	void DoubleJump();

	void MirrorSprite();

	void AnimChange();

	iPoint Gravity(iPoint vec);

	void SpeedUp();

	void SpeedDown();

	void RestartPlayer();

	bool PositionCameraOnPlayer();

	void ArrivesFloor();

	void PlayerMov();

	void OnCollision(Collider* c1, Collider* c2);

private:

public:

	bool				isAlive;
	PlayerData			Player;

private:

	p2SString			spritesheetN;
	fPoint				temp;
	SDL_Texture*		graphics = nullptr;
};





#endif // !__j1PLAYER_H__


