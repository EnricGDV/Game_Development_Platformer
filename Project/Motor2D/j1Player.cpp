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

	//folder.create(config.child("folder").child_value());
	//texture_path = config.child("sprite_sheet").attribute("source").as_string();

	Player.jumpSpeed.x = config.child("jumpSpeed").attribute("x").as_int();
	Player.jumpSpeed.y = config.child("jumpSpeed").attribute("y").as_int();

	Player.maxSpeed.x = config.child("maxSpeed").attribute("x").as_int();
	Player.maxSpeed.y = config.child("maxSpeed").attribute("y").as_int();


	Player.acceleration.x = config.child("acceleration").attribute("x").as_int();
	Player.acceleration.y = config.child("acceleration").attribute("y").as_int();

	Player.offPath.x = config.child("offPath").attribute("x").as_int();
	Player.offPath.y = config.child("offPath").attribute("y").as_int();


	Player.collider = App->collision->AddCollider({ config.child("initPos").attribute("x").as_int(), config.child("initPos").attribute("y").as_int(), config.child("col").attribute("w").as_int(), config.child("col").attribute("h").as_int() }, COLLIDER_PLAYER, this);

	return ret;
}

bool j1Player::Start()
{
	Player.LoadPushbacks();

	Player.speed = { 0,0 };


	isAlive = true;

	Player.isJumping = false;
	Player.canDash = true;

	Player.current_animation = &Player.idle;


	//Player.PlayerTexture = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));

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
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)

			DoubleJump();
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Player.xDirection = 1, SpeedUp();
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Player.xDirection = -1, SpeedUp();
	else
		SpeedDown();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Player.onFloor)
	{
		Player.isJumping = true;
		Player.maxSpeed.x += Player.jumpSpeed.x;
		Player.speed.x = Player.jumpSpeed.x*Player.xDirection;
		Player.speed.y = Player.jumpSpeed.y;
	}

	Player.speed = Gravity(Player.speed);


	AnimChange();
	PlayerMov();
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
	App->tex->UnLoad(Player.PlayerTexture);
	return true;
}

// Load Game
bool j1Player::Load(pugi::xml_node& data)
{
	Player.position.x = data.child("position").attribute("x").as_int();
	Player.position.y = data.child("position").attribute("y").as_int();
	Player.speed.x = data.child("speed").attribute("x").as_int();
	Player.speed.y = data.child("speed").attribute("y").as_int();
	Player.collider->rect.w = data.child("collider").attribute("width").as_int();
	Player.collider->rect.h = data.child("collider").attribute("height").as_int();
	Player.collider->rect.x = data.child("collider").attribute("x").as_int();
	Player.collider->rect.y = data.child("collider").attribute("y").as_int();
	Player.onFloor = data.child("onFloor").attribute("value").as_bool();

	return true;
}


//Save Game
bool j1Player::Save(pugi::xml_node& data) const
{
	data.append_child("position").append_attribute("x") = Player.position.x;
	data.child("position").append_attribute("y") = Player.position.y;
	data.append_child("speed").append_attribute("x") = Player.speed.x;
	data.child("speed").append_attribute("y") = Player.speed.y;
	data.append_child("collider").append_attribute("width") = Player.collider->rect.w;
	data.child("collider").append_attribute("height") = Player.collider->rect.h;
	data.child("collider").append_attribute("x") = Player.collider->rect.x;
	data.child("collider").append_attribute("y") = Player.collider->rect.y;
	data.append_child("onFloor").append_attribute("value") = Player.onFloor;

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
	Player.jumping.Reset();
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
	if (Player.onFloor && Player.speed.y == 0)
	{
		if (Player.speed.x == 0)
		{
			Player.current_animation = &Player.idle;
		}
		else
		{
			Player.current_animation = &Player.moving;
		}
	}
	else if (Player.speed.y < 0)
	{
		Player.current_animation = &Player.jumping;
	}
	else
	{
		Player.current_animation = &Player.falling;
	}
}

void j1Player::PlayerMov()
{
	Player.position += Player.speed;

	Player.collider->SetPos(Player.position.x + Player.offPath.x, Player.position.y + Player.offPath.y);
}

void j1Player::Draw()
{
	if (Player.mirror)
		App->render->Blit(Player.PlayerTexture, Player.position.x, Player.position.y, &(Player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, -1.0);
	else
		App->render->Blit(Player.PlayerTexture, Player.position.x, Player.position.y, &(Player.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, -1.0);
}

iPoint j1Player::Gravity(iPoint vec)
{
	vec.y += Player.acceleration.y;
	if (vec.y > Player.maxSpeed.y)
	{
		vec.y = Player.maxSpeed.y;
	}
	return vec;
}

bool j1Player::PositionCameraOnPlayer()
{
	App->render->camera.x = Player.position.x - App->render->camera.w / 3;
	if (App->render->camera.x < 0)App->render->camera.x = 0;
	App->render->camera.y = Player.position.y - App->render->camera.h / 2;
	//if (App->render->camera.y + App->win->height > App->map->data.height*App->map->data.tile_height)App->render->camera.y = App->map->data.height*App->map->data.tile_height - App->win->height;
	return true;
}

void PlayerData::LoadPushbacks()
{

}

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

		}
}