#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "ModuleCollision.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{}


bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	spritesheetN = config.child("spritesheetS").attribute("name").as_string();
	pugi::xml_node animations;
	for (animations = config.child("animations").first_child(); animations && ret; animations = animations.next_sibling("animation"))
	{
		p2SString temp(animations.attribute("name").as_string());
		if (temp == "angel_idle")
		{
			for (pugi::xml_node frame = animations.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
				Player.angel_idle.PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

			Player.angel_idle.speed = animations.attribute("speed").as_float();
			Player.angel_idle.loop = animations.attribute("loop").as_bool();
		}
		if (temp == "angel_moving")
		{
			for (pugi::xml_node frame = animations.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
				Player.angel_moving.PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

			Player.angel_moving.speed = animations.attribute("speed").as_float();
			Player.angel_moving.loop = animations.attribute("loop").as_bool();
		}
		if (temp == "angel_jumping")
		{
			for (pugi::xml_node frame = animations.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
				Player.angel_jumping.PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

			Player.angel_jumping.speed = animations.attribute("speed").as_float();
			Player.angel_jumping.loop = animations.attribute("loop").as_bool();
		}
		if (temp == "angel_falling")
		{
			for (pugi::xml_node frame = animations.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
				Player.angel_falling.PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

			Player.angel_falling.speed = animations.attribute("speed").as_float();
			Player.angel_falling.loop = animations.attribute("loop").as_bool();
		}
		if (temp == "angel_idle_M")
		{
			for (pugi::xml_node frame = animations.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
				Player.angel_idle_M.PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

			Player.angel_idle_M.speed = animations.attribute("speed").as_float();
			Player.angel_idle_M.loop = animations.attribute("loop").as_bool();
		}
		if (temp == "angel_moving_M")
		{
			for (pugi::xml_node frame = animations.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
				Player.angel_moving_M.PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

			Player.angel_moving_M.speed = animations.attribute("speed").as_float();
			Player.angel_moving_M.loop = animations.attribute("loop").as_bool();
		}
		if (temp == "angel_jumping_M")
		{
			for (pugi::xml_node frame = animations.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
				Player.angel_jumping_M.PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

			Player.angel_jumping_M.speed = animations.attribute("speed").as_float();
			Player.angel_jumping_M.loop = animations.attribute("loop").as_bool();
		}
		if (temp == "angel_falling_M")
		{
			for (pugi::xml_node frame = animations.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
				Player.angel_falling_M.PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

			Player.angel_falling_M.speed = animations.attribute("speed").as_float();
			Player.angel_falling_M.loop = animations.attribute("loop").as_bool();
		}
	}

	//Player.current_animation = &Player.angel_idle;

	Player.jumpSpeed.x = config.child("jumpSpeed").attribute("x").as_int();
	Player.jumpSpeed.y = config.child("jumpSpeed").attribute("y").as_int();

	Player.maxSpeed.x = config.child("maxSpeed").attribute("x").as_int();
	Player.maxSpeed.y = config.child("maxSpeed").attribute("y").as_int();


	Player.acceleration.x = config.child("acceleration").attribute("x").as_int();
	Player.acceleration.y = config.child("acceleration").attribute("y").as_int();

	Player.offSet.x = config.child("offPath").attribute("x").as_int();
	Player.offSet.y = config.child("offPath").attribute("y").as_int();


	Player.position.x = config.child("initPos").attribute("x").as_int();
	Player.position.y = config.child("initPos").attribute("y").as_int();

	Player.initPosition.x = config.child("initPos").attribute("x").as_int();
	Player.initPosition.y = config.child("initPos").attribute("y").as_int();

	Player.collider = App->collision->AddCollider({ config.child("position").attribute("x").as_int(), config.child("position").attribute("y").as_int(), config.child("col").attribute("w").as_int(), config.child("col").attribute("h").as_int() }, COLLIDER_PLAYER, this);

	return ret;
}

bool j1Player::Start()
{
	//Player.LoadPushbacks();
	Player.speed = { 0,0 };

	graphics = App->tex->Load(spritesheetN.GetString());
	if (graphics == nullptr)
	{
		return false;
	}

	isAlive = true;

	Player.isJumping = false;
	Player.canDash = true;

	Player.current_animation = &Player.angel_idle;


	return true;
}

bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	MirrorSprite();

	if (Player.isJumping && Player.canDJump)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)

			DoubleJump();
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		Player.xDirection = 1, SpeedUp();
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		Player.xDirection = -1, SpeedUp();
	else
		SpeedDown();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && Player.onFloor)
	{
		Player.isJumping = true;
		Player.maxSpeed.x += Player.jumpSpeed.x;
		Player.speed.x = Player.jumpSpeed.x*Player.xDirection;
		Player.speed.y = Player.jumpSpeed.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (App->scene->mapname != "map1.tmx")
		{
			p2SString map = "map1.tmx";
			App->map->mapChange(&map);
			Player.position = Player.initPosition;
			App->scene->mapname = map;
		}
		else
		{
			Player.position = Player.initPosition;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (App->scene->mapname != "map2.tmx")
		{
			p2SString map = "map2.tmx";
			App->map->mapChange(&map);
			Player.position = Player.initPosition;
			App->scene->mapname = map;
		}
		else
		{
			Player.position = Player.initPosition;
		}
	}

	Player.speed = Gravity(Player.speed);


	AnimChange();
	PlayerMov();
	Draw();
	return true;
}

bool j1Player::PostUpdate()
{
	/*if (!isAlive)
	{
		App->scenechange->ChangeMap(App->scene->currentMap, App->scene->fade_time);
	}*/

	PositionCameraOnPlayer();

	return true;
}

bool j1Player::Clean()
{
	LOG("Cleaning Player");
	App->tex->UnLoad(graphics);
	return true;
}

//Save Game
bool j1Player::Save(pugi::xml_node& data) const
{
	data.append_child("position").append_attribute("x") = Player.position.x;
	data.child("position").append_attribute("y") = Player.position.y;
	data.append_child("speed").append_attribute("x") = Player.speed.x;
	data.child("speed").append_attribute("y") = Player.speed.y;
	data.append_child("col").append_attribute("width") = Player.collider->rect.w;
	data.child("col").append_attribute("height") = Player.collider->rect.h;
	data.child("col").append_attribute("x") = Player.collider->rect.x;
	data.child("col").append_attribute("y") = Player.collider->rect.y;
	data.append_child("onFloor").append_attribute("value") = Player.onFloor;

	return true;
}

// Load Game
bool j1Player::Load(pugi::xml_node& data)
{
	Player.position.x = data.child("position").attribute("x").as_int();
	Player.position.y = data.child("position").attribute("y").as_int();
	Player.speed.x = data.child("speed").attribute("x").as_int();
	Player.speed.y = data.child("speed").attribute("y").as_int();
	Player.collider->rect.w = data.child("col").attribute("width").as_int();
	Player.collider->rect.h = data.child("col").attribute("height").as_int();
	Player.collider->rect.x = data.child("col").attribute("x").as_int();
	Player.collider->rect.y = data.child("col").attribute("y").as_int();
	Player.onFloor = data.child("onFloor").attribute("value").as_bool();

	return true;
}



void j1Player::MirrorSprite()
{
	if (Player.speed.x < 0)
		Player.mirror = true;
	else if (Player.speed.x > 0)
		Player.mirror = false;
}


void j1Player::SpeedUp()
{
	Player.speed.x += Player.acceleration.x * Player.xDirection;

	if (Player.xDirection > 0)
	{
		if (Player.speed.x > Player.maxSpeed.x)
			Player.speed.x = Player.maxSpeed.x;
	}

	else
	{
		if (Player.speed.x < Player.xDirection*Player.maxSpeed.x)
			Player.speed.x = Player.xDirection*Player.maxSpeed.x;
	}
}

void j1Player::SpeedDown()
{
	if (Player.speed.x != 0)
		Player.speed.x -= Player.acceleration.x * Player.xDirection;
}

void j1Player::ArrivesFloor()
{
	if (Player.isJumping)
	{
		Player.isJumping = false;
		Player.maxSpeed.x -= Player.jumpSpeed.x;
	}



	Player.canDJump = false;
	Player.onFloor = true;
	Player.angel_jumping.Reset();
}

void j1Player::DoubleJump()
{
	Player.canDJump = false;
	Player.isJumping = true;
	Player.maxSpeed.x += Player.jumpSpeed.x;
	Player.speed.x = Player.jumpSpeed.x*Player.xDirection;
	Player.speed.y = Player.jumpSpeed.y;
}

void j1Player::AnimChange()
{
	if (!Player.mirror)
	{
		if (Player.onFloor && Player.speed.y == 0)
		{
			if (Player.speed.x == 0)
			{
				Player.current_animation = &Player.angel_idle;
			}
			else
			{
				Player.current_animation = &Player.angel_moving;
			}
		}
		else if (Player.speed.y < 0)
		{
			Player.current_animation = &Player.angel_jumping;
		}
		else
		{
			Player.current_animation = &Player.angel_falling;
		}
	}
	else
	{
		if (Player.onFloor && Player.speed.y == 0)
		{
			if (Player.speed.x == 0)
			{
				Player.current_animation = &Player.angel_idle_M;
			}
			else
			{
				Player.current_animation = &Player.angel_moving_M;
			}
		}
		else if (Player.speed.y < 0)
		{
			Player.current_animation = &Player.angel_jumping_M;
		}
		else
		{
			Player.current_animation = &Player.angel_falling_M;
		}
	}
	
}

void j1Player::PlayerMov()
{
	Player.position += Player.speed;

	Player.collider->SetPos(Player.position.x + Player.offSet.x, Player.position.y + Player.offSet.y);
}

void j1Player::Draw()
{
	if (Player.mirror)
		App->render->Blit(graphics, Player.position.x, Player.position.y, &(Player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, -1.0);
	else
		App->render->Blit(graphics, Player.position.x, Player.position.y, &(Player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, -1.0);
}

iPoint j1Player::Gravity(iPoint vec)
{
	//vec.y += Player.acceleration.y;
	//if (vec.y > Player.maxSpeed.y)
	//{
	//	vec.y = Player.maxSpeed.y;
	//}
	vec.y = 1;
	return vec;
}

bool j1Player::PositionCameraOnPlayer()
{
	/*App->render->camera.x = Player.position.x - App->render->camera.w / 2;
	if (App->render->camera.x < 0)App->render->camera.x = 0;
	App->render->camera.y = Player.position.y - App->render->camera.h / 3;
	if (App->render->camera.y + App->win->height > App->map->data.height*App->map->data.tile_height)App->render->camera.y = App->map->data.height*App->map->data.tile_height - App->win->height;*/
	return true;
}

//void PlayerData::LoadPushbacks()
//{
//
//}

void j1Player::RestartPlayer()
{
	//Escribir valores iniciales del mundo
}

//void j1Player::BecomeGrounded()
//{
//	if (isJumping)
//	{
//		isJumping = false;
//		maxSpeed.x -= jumpForce.x;
//	}
//
//	if (Current_Animation == &falling)
//		AddSFX(5, 0);
//
//	canDash = true;
//	grounded = true;
//	jumping_up.Reset();
//}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL)
	{
		if (Player.speed.y >= 0 && c2->rect.y > c1->rect.y)
		{
			Player.position.y = c2->rect.y - c1->rect.h;
			ArrivesFloor();
		}
		else if ((c2->rect.y + c2->rect.h) > c1->rect.y && c2->rect.y < c1->rect.y)
		{
			if (Player.speed.x > 0)
				Player.position.x = c2->rect.x - 1;
			else if (Player.speed.x < 0)
				Player.position.x = c2->rect.x + c2->rect.w + 1;
		}
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WIN)
	{
		Player.position = Player.initPosition;
		App->scene->ChangeScene();
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY_SHOT)
	{
		Player.position = Player.initPosition;
	}

}