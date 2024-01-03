#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "Timer.h"
#include "Threats.h"
#include "Explosion.h"
#include "Text.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include "stdafx.h"


BaseObject g_background;
TTF_Font* g_font_text = NULL;

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); 

	g_window = SDL_CreateWindow("Fire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // Tao window
	if (g_window == NULL)
	{
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL)
		{
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}

		if (TTF_Init() == -1)
		{
			success = false;
		}


		g_font_text = TTF_OpenFont("font//dlxfont.ttf", 15);
		if (g_font_text == NULL)
		{
			return false;
		}
	}
	return success;
}

void close()
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

/*void loadMenu()
{
	menu = IMG_LoadTexture(renderer, "img/menubgr.png");
	start = IMG_LoadTexture(renderer, "img/start.png");
	start_rect = { SCREEN_WIDTH / 2 - 125, 250, 250, 80 };
}*/

bool LoadBackground()
{
	bool ret = g_background.LoadImg("img//background1.png", g_screen);
	if (ret == false)
		return false;

	return true;
}


std::vector<Threats*> MakeThreatList()
{
	std::vector<Threats*> list_threats;

	Threats* dynamic_threats = new Threats[20];
	for (int i = 0; i < 20; i++)
	{
		Threats* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_left2.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(Threats::MOVE_IN_SPACE_TH);
			p_threat->set_xpos(500 + i * 500);
			p_threat->set_ypos(200);
			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			p_threat->SetAnimationPos(pos1, pos2);
			p_threat->set_input_left(1);
			list_threats.push_back(p_threat);
		}
	}



	Threats* threats_objs = new Threats[20];

	for (int i = 0; i < 20; i++)
	{
		Threats* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_level.png", g_screen);
			p_threat->set_clips();
			p_threat->set_xpos(700 + i*1500);
			p_threat->set_ypos(250);
			p_threat->set_type_move(Threats::STATIC_TH);
			p_threat->set_input_left(0);

			Bullet* p_bullet = new Bullet();
			p_threat->InitBullet(p_bullet, g_screen);

			list_threats.push_back(p_threat);
		}
	}

	return list_threats;
}

int main(int argc, char* argv[])
{
	Timer fps_timer;

	if (InitData() == false)
		return -1;

	if (LoadBackground() == false)
		return -1;

	GameMap game_map;
	game_map.LoadMap("map_good/map02.txt");
	game_map.LoadTiles(g_screen);


	MainObject p_player;
	p_player.LoadImg("img//player1_right.png", g_screen);
	p_player.set_clips();

	PlayerPower player_power;
	player_power.Init(g_screen);

	PlayerEnergy player_energy;
	player_energy.Init(g_screen);
	player_energy.SetPos(SCREEN_WIDTH * 0.5 - 300, 5);


	std::vector<Threats*> threats_list = MakeThreatList();


	Explosion exp_threat;
	bool tRet = exp_threat.LoadImg("img//expl.png", g_screen);
	if (!tRet) return -1;
	exp_threat.set_clip();

	Explosion exp_main;
	bool mRet = exp_main.LoadImg("img//expl.png", g_screen);
	exp_main.set_clip();
	if (!mRet) return -1;

	int num_die = 0;

	TextObject time_game;
	time_game.setColor(TextObject::WHITE_TEXT);

	TextObject mark_game;
	mark_game.setColor(TextObject::WHITE_TEXT);
	UINT mark_value = 0;

	TextObject energy_game;
	energy_game.setColor(TextObject::WHITE_TEXT);

	bool is_quit = false;
	while (!is_quit)
	{
		fps_timer.start();
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}

			p_player.HandleInputAction(g_event, g_screen);
		}

		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);
		Map map_data = game_map.getMap();

		p_player.HandleBullet(g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data);
		p_player.Show(g_screen);

		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);


		GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 50);
		ColorData color_data(0, 80, 150);
		Geometric::RenderRectange(rectangle_size, color_data, g_screen);
		GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 48);
		ColorData color_data2(255, 255, 255);

		Geometric::RenderOutline(outLineSize, color_data2, g_screen);

		player_power.Show(g_screen);
		player_energy.Show(g_screen);


		for (int i = 0; i < threats_list.size(); i++)
		{
			Threats* p_threat = threats_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_screen);
				p_threat->DoPlayer(map_data);
				p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_screen);


				SDL_Rect rect_player = p_player.GetRectFrame();
				bool bCol1 = false;
				std::vector<Bullet*>tBullet_list = p_threat->get_bullet_list();
				for (int jj = 0; jj < tBullet_list.size(); ++jj)
				{
					Bullet* pt_bullet = tBullet_list.at(jj);
					if (pt_bullet)
					{
						bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
						if (bCol1)
						{
							p_threat->RemoveBullet(jj);
							break;
						}
					}
				}

				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
				if (bCol1 || bCol2)
				{
					int width_exp_frame = exp_main.get_frame_height();
					int heiht_exp_height = exp_main.get_frame_width();
					for (int ex = 0; ex < 4; ex++)
					{
						int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
						int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - heiht_exp_height * 0.5;

						exp_main.set_frame(ex);
						exp_main.SetRect(x_pos, y_pos);
						exp_main.Show(g_screen);
						SDL_RenderPresent(g_screen);
					}

					num_die++;
					if (num_die <= 2)
					{
						p_player.SetRect(0, 0);
						p_player.set_comeback_time(60);
						SDL_Delay(1000);
						player_power.Decrease();
						player_power.Render(g_screen);
						continue;
					}
					else
					{
						if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
						{
							p_threat->Free();
							close();
							SDL_Quit();
							return 0;
						}
					}
				}

			}
		}

		std::vector<Bullet*> bullet_arr = p_player.get_bullet_list();
		int frame_exp_width = exp_threat.get_frame_width();
		int frame_exp_height = exp_threat.get_frame_height();

		for (int r = 0; r < bullet_arr.size(); ++r)
		{
			Bullet* p_bullet = bullet_arr.at(r);
			if (p_bullet != NULL)
			{
				for (int t = 0; t < threats_list.size(); ++t)
				{
					Threats* obj_threat = threats_list.at(t);
					if (obj_threat != NULL)
					{
						SDL_Rect tRect;

						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();
						tRect.h = obj_threat->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();

						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

						if (bCol)
						{
							mark_value++;
							for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
							{
								int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
								int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.2;

								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(g_screen);
								SDL_RenderPresent(g_screen);
							}



							p_player.RemoveBullet(r);
							obj_threat->Free();
							threats_list.erase(threats_list.begin() + t);
						}
					}
				}
			}
		}

		std::string str_time = "Time: ";
		Uint32 time_val = SDL_GetTicks() / 1000;
		Uint32 val_time = 300 - time_val;
		if (val_time <= 0)
		{
			if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			{
				is_quit = 0;
				break;
			}
		}
		else
		{
			std::string str_val = std::to_string(val_time);
			str_time += str_val;

			time_game.SetText(str_time);
			time_game.loadFromRenderedText(g_font_text, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 20);
		}

		std::string val_str_mark = std::to_string(mark_value);
		std::string strMark("Mark: ");
		strMark += val_str_mark;

		mark_game.SetText(strMark);
		mark_game.loadFromRenderedText(g_font_text, g_screen);
		mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 20);

		int energy_count = p_player.GetEnergyCount();
		std::string energy_str = std::to_string(energy_count);

		energy_game.SetText(energy_str);
		energy_game.loadFromRenderedText(g_font_text, g_screen);
		energy_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 20);

		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;

		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0)
				SDL_Delay(delay_time);
		}
	}

	for (int i = 0; i < threats_list.size(); i++)
	{
		Threats* p_threat = threats_list.at(i);
		if (p_threat)
		{
			p_threat->Free();
			p_threat = NULL;
		}
	}

	threats_list.clear();

	close();
	return 0;
}