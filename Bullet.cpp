#include "Bullet.h"
#include "stdafx.h"

Bullet::Bullet()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	bullet_type_ = SNOWBALL;
}

Bullet::~Bullet()
{

}

bool Bullet::LoadImgBullet(SDL_Renderer* des)
{
	bool ret = false;
	if (bullet_type_ == LASER_BULLET)
	{
		ret = LoadImg("img//laser_bullet.png", des);
	}
	else
	{
		ret = LoadImg("img//player_bullet.png", des);
	}

	return ret;
}

void Bullet::HandleMove(const int& x_border, const int& y_border, bool is_player /*true*/)
{
	if (bullet_dir_ == DIR_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x > x_border)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP)
	{
		rect_.y -= y_val_;
		if (rect_.y < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0)
		{
			is_move_ = false;
		}

		rect_.y == y_val_;
		if (rect_.y < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x > x_border)
		{
			is_move_ = false;
		}

		rect_.y -= y_val_;
		if (rect_.y < 0)
		{
			is_move_ = false;
		}
	}
}