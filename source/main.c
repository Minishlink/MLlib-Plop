/*			-==========================-
			|		    Plop!   	   |
			-==========================-

main.c

Author : Minishlink
Created : June 2009 (The game "Plop!" was created on 30/08/09 though, it was kind of a test file)
Latest modified : 01/09/09 (Day/Month/Year)

Twitter : http://twitter.com/Minishlink
Website : http://minishlink.dev-fr.org and http://louislagrange.free.fr
MLlib Website : http://louislagrange.free.fr/MLlib 							*/

#include <MLlib.h>

#include "cursor_png.h"
#include "font_png.h"
#include "fontBlack_png.h"
#include "plop_png.h"
#include "background_png.h"
#include "bg_plop_png.h"
#include "bg_instructions_png.h"
#include "bg_credits_png.h"
#include "bg_pause_png.h"
#include "bg_start_png.h"
#include "button_a_png.h"
#include "button_a_highlighted_png.h"
#include "button_b_png.h"
#include "button_b_highlighted_png.h"
#include "bg_niveau_0_png.h"
#include "bg_niveau_1_png.h"
#include "bg_won_png.h"
#include "bg_lost_png.h"
#include "first_png.h"
#include "second_png.h"
#include "third_png.h"
#include "fourth_png.h"
#include "fifth_png.h"
#include "sixth_png.h"
#include "youpi_raw.h"
#include "yihaa_mp3.h"
#include "happy_days_mod.h"
#include "haplev_mod.h"

#define MAX_PLOP 		2048
#define MENU_MAX_PLOP	5
#define MENU_VELOCITY	40, 90
#define GAME_VELOCITY	30, 68
#define WIDTH 			640
#define HEIGHT 			480
#define SIZE_PLOP		64
#define RADIUS_PLOP		26

ML_Image plopData, cursorData, fontData, fontBlackData, backgroundData, bg_plopData, bg_instructionsData,
bg_creditsData, bg_pauseData, bg_startData, button_aData, button_a_highlightedData, button_bData, button_b_highlightedData,
bg_niveau_0Data, bg_niveau_1Data, bg_wonData, bg_lostData,
firstData, secondData, thirdData, fourthData, fifthData, sixthData;

ML_Sprite plop[MAX_PLOP], cursor, font, fontBlack, background, bg_plop, bg_instructions, 
bg_credits, bg_pause, bg_start, button_a, button_a_highlighted, button_b, button_b_highlighted,
bg_niveau_0, bg_niveau_1, bg_won, bg_lost,
first, second, third, fourth, fifth, sixth;

static MODPlay mod;

int i;

void menu();
void game();
void initGame(int max_plop_niveau);
void betweenPlopBounce(s16 *x1, s16 *y1, s8 *dx1, s8 *dy1, s16 *x2, s16 *y2, s8 *dx2, s8 *dy2);

int main(int argc, char **argv)
{
	ML_Init();
	ML_InitRand();
	ML_InitMP3();
	ML_InitMOD(&mod);
	
	ML_SplashScreen();	
	
	// Loading and initializing the simple font
	ML_LoadSpriteFromBuffer(&fontData, &font, font_png, 10, 200);
	ML_InitTile(&font, 8, 8);
	ML_SetSpriteScale(&font, 2, 2);
	ML_LoadSpriteFromBuffer(&fontBlackData, &fontBlack, fontBlack_png, 10, 200);
	ML_InitTile(&fontBlack, 8, 8);
	ML_SetSpriteScale(&fontBlack, 2, 2);
	
	// Loading the cursor made by drmr
	ML_LoadSpriteFromBuffer(&cursorData, &cursor, cursor_png, 0, 0);
	
	// Loading the Plop! image
	ML_LoadSpriteFromBuffer(&bg_plopData, &bg_plop, bg_plop_png, 214, 20);
	
	// Loading the Instructions image
	ML_LoadSpriteFromBuffer(&bg_instructionsData, &bg_instructions, bg_instructions_png, 0, -10);
	
	// Loading the Credits image
	ML_LoadSpriteFromBuffer(&bg_creditsData, &bg_credits, bg_credits_png, 0, 0);
	
	// Loading the Pause image
	ML_LoadSpriteFromBuffer(&bg_pauseData, &bg_pause, bg_pause_png, 0, 0);
	
	// Loading the Start image
	ML_LoadSpriteFromBuffer(&bg_startData, &bg_start, bg_start_png, 218, 336);
	
	// Loading the Button A image
	ML_LoadSpriteFromBuffer(&button_aData, &button_a, button_a_png, 288, 340);
	button_a.alpha = 230;
	
	// Loading the Button A Highlighted image
	ML_LoadSpriteFromBuffer(&button_a_highlightedData, &button_a_highlighted, button_a_highlighted_png, 288, 340);
	
	// Loading the Button B image
	ML_LoadSpriteFromBuffer(&button_bData, &button_b, button_b_png, 368, 340);
	button_b.alpha = 230;
	
	// Loading the Button B Highlighted image
	ML_LoadSpriteFromBuffer(&button_b_highlightedData, &button_b_highlighted, button_b_highlighted_png, 368, 340);
	
	// Loading the "I wish you a merry christmas guys" background
	ML_LoadSpriteFromBuffer(&backgroundData, &background, background_png, 192, 112);
	
	// Loading Floors images
	ML_LoadSpriteFromBuffer(&bg_niveau_0Data, &bg_niveau_0, bg_niveau_0_png, 0, 0);
	ML_LoadSpriteFromBuffer(&bg_niveau_1Data, &bg_niveau_1, bg_niveau_1_png, 0, 0);
	ML_LoadSpriteFromBuffer(&firstData, &first, first_png, 263, 94);
	ML_LoadSpriteFromBuffer(&secondData, &second, second_png, 260, 94);
	ML_LoadSpriteFromBuffer(&thirdData, &third, third_png, 262, 94);
	ML_LoadSpriteFromBuffer(&fourthData, &fourth, fourth_png, 257, 94);
	ML_LoadSpriteFromBuffer(&fifthData, &fifth, fifth_png, 262, 94);
	ML_LoadSpriteFromBuffer(&sixthData, &sixth, sixth_png, 262, 94);
	
	// Loading Won image
	ML_LoadSpriteFromBuffer(&bg_wonData, &bg_won, bg_won_png, 0, 0);
	
	// Loading Lost image
	ML_LoadSpriteFromBuffer(&bg_lostData, &bg_lost, bg_lost_png, 0, 0);
	
	// Loading and initializing these awesome plops
	ML_LoadSpriteFromBuffer(&plopData, &plop[0], plop_png, 0, 0);
	
	plop[0].visible = 0;
	for(i = 0; i < MAX_PLOP; i++)
		plop[i] = plop[0];
	i = 0;
	
	menu();
	
	return 0;
}

void menu()
{
	ML_PlayMODFromBuffer(&mod, happy_days_mod);
	
	bool exit = 0, instructions = 1;
	int j = 0, z = 0;
	
	for(i = 0; i < MAX_PLOP; i++)
	{
		if(i <= MENU_MAX_PLOP)
		{
			plop[i].visible = true;
			if(!plop[i].x || !plop[i].y)
			{
				plop[i].x = ML_Rand(0, WIDTH-SIZE_PLOP);
				plop[i].y = ML_Rand(0, HEIGHT-SIZE_PLOP);
				plop[i].dx = ML_Rand(-4, 4);
   				plop[i].dy = ML_Rand(-4, 4);
			}
		}
		else plop[i].visible = false;
		
   		plop[i].dy = 0;
   	}
	
	i = MENU_MAX_PLOP;
	
	while(1)
	{
		// I wanna quit now
		if(Wiimote[0].Newpress.Home) { exit = true; }
		
		// Let's create some plops !
		if(Wiimote[0].Newpress.A && i < MAX_PLOP)
		{
			plop[i].x = cursor.x - plop[i].tileWidth/2;
			plop[i].y = cursor.y - plop[i].tileHeight/2;
			plop[i].visible = 1;
			i++;
		}
		
		// Too much plops for me, reset so I can try again please
		if(Wiimote[0].Released.Two)
		{
			i = 0;
			
			for(i = 0; i < MAX_PLOP; i++)
			{
				plop[i].visible = 0;
				plop[i].dx = ML_Rand(-4, 4);
		   		plop[i].dy = ML_Rand(-4, 4);
			}
			
			i = 0;
		}
		
		// C'mon, let's make'em move and appear
		for(j = 0; j < i; j++)
		{	
			if(plop[j].visible)
			{
				// I like to move it !
				// Bounce in the left and right side
				if(plop[j].x + plop[j].dx > WIDTH-SIZE_PLOP || plop[j].x + plop[j].dx < 0)					plop[j].dx = -plop[j].dx * 0.5;  // reduce the bounce
				
				// Bounce in the up and down side
				if(plop[j].y + plop[j].dy < 0)
					plop[j].dy = -plop[j].dy;
	
				if(plop[j].y + plop[j].dy > HEIGHT-SIZE_PLOP)
					plop[j].dy = -plop[j].dy * 0.4; // reduce the bounce
	
				if((abs(plop[j].dy) < 0.2) && (plop[j].y > HEIGHT-SIZE_PLOP-2))
				{
					ASND_SetVoice(ASND_GetFirstUnusedVoice(), VOICE_MONO_16BIT, 22050 + ML_Rand(0, 22050), 0, (char *)youpi_raw, youpi_raw_size, 110, 110, NULL);
					plop[j].dy = ML_Rand(30, 68); // a bounce if the velocity is too low
				}  	
				
				if((abs(plop[j].dx) < 0.2) && ((plop[j].x > WIDTH-2) || (plop[j].x < -SIZE_PLOP)))
				{
					ASND_SetVoice(ASND_GetFirstUnusedVoice(), VOICE_MONO_16BIT, 22050 + ML_Rand(0, 22050), 0, (char *)youpi_raw, youpi_raw_size, 110, 110, NULL);
					plop[j].dx = ML_Rand(30, 68); // a bounce if the velocity is too low
				} 
	
				for(z = 0; z < i; z++)
				{
					if(z == i)
						continue;

					if(abs(plop[j].x+plop[j].dx-plop[z].x) < 2 * RADIUS_PLOP && abs(plop[j].y+plop[j].dy-plop[z].y) < 2 * RADIUS_PLOP) 
					{
						betweenPlopBounce(&plop[j].x, &plop[j].y, &plop[j].dx, &plop[j].dy, &plop[z].x, &plop[z].y, &plop[z].dx, &plop[z].dy);
					}
					
					// Wiimote haz teh control
					if(Wiimote[0].GForce.X < -2) plop[z].dx = -ML_Rand(MENU_VELOCITY);
					else if(Wiimote[0].GForce.X > 2) plop[z].dx = ML_Rand(MENU_VELOCITY);
				
					if(Wiimote[0].GForce.Y < -2) plop[z].dy = -ML_Rand(MENU_VELOCITY);
					else if(Wiimote[0].GForce.Y > 2) plop[z].dy = ML_Rand(MENU_VELOCITY);
				}
			
				// Move now !
				plop[j].x += plop[j].dx;
				plop[j].y += plop[j].dy;
			
				// Gravity rocks
				plop[j].dy += 1;
			
				// Don't fall, little plop ! :'(
				if(plop[j].y > HEIGHT-SIZE_PLOP) plop[j].y = HEIGHT-(SIZE_PLOP+6);
				if(plop[j].x > WIDTH-SIZE_PLOP) plop[j].x = WIDTH-(SIZE_PLOP+6);
				else if(plop[j].x < 0) plop[j].x = 0;
				
				// Rotate guys !
				ML_RotateSprite(&plop[j], Wiimote[0].IR.Angle, 0);
			
				// Let me be on the screen, please !							
				ML_DrawSprite(&plop[j]);
			}
		}
		
		// Plop!
		ML_DrawSprite(&bg_plop);
		
		// Instructions or credits ?
		if(instructions) 
		{
			// Who did this game ?
			if(Wiimote[0].Released.One) instructions = false;
			
			// Play the game !
			if(Wiimote[0].Held.A && Wiimote[0].Held.B) 
			{
				ML_PlayMP3FromBuffer(yihaa_mp3, yihaa_mp3_size);
				game();
			}
			
			ML_DrawSprite(&bg_start);
			
			if(Wiimote[0].Held.A)
				ML_DrawSprite(&button_a_highlighted);
			else ML_DrawSprite(&button_a);
			
			if(Wiimote[0].Held.B)
				ML_DrawSprite(&button_b_highlighted);
			else ML_DrawSprite(&button_b);
			
			ML_DrawSprite(&bg_instructions);
		}
		else
		{
			if(Wiimote[0].Released.Minus || Wiimote[0].Released.B) instructions = true;
			
			ML_DrawSprite(&bg_credits);
		} 
		
		if(Wiimote[0].Held.Plus && Wiimote[0].Held.Minus) ML_Screenshot("screenshot");
		
		ML_DrawSpriteText(&font, 20, 20, "%d", ML_GetFPS());
		
		// Cursor rocks \o/
		ML_Cursor(&cursor, 0);
		
		// Beautiful effect for quitting the app
		if(exit)
		{
			if(ML_FadeOut())
			{ 
				ML_DeleteImage(&plopData);
				ML_DeleteImage(&fontData);
				ML_DeleteImage(&cursorData);
				ML_DeleteImage(&backgroundData);
				ML_DeleteImage(&bg_plopData);
				ML_DeleteImage(&bg_instructionsData);
				ML_DeleteImage(&bg_creditsData);
				ML_DeleteImage(&bg_pauseData);
				ML_DeleteImage(&bg_startData);
				ML_DeleteImage(&button_aData);
				ML_DeleteImage(&button_a_highlightedData);
				ML_DeleteImage(&button_bData);
				ML_DeleteImage(&button_b_highlightedData);
				ML_Exit();
			}
		}
		
		// Magic function
		ML_Refresh();
	}
}

void game()
{
	ML_PlayMODFromBuffer(&mod, haplev_mod);
	
	bool pause = 0;
	int j = 0, z = 0, time = 0, santaOut = 0, timeNiveau = 0, timeTotal = 0, santaOutTotal = 0;
	u16 go = 0, niveau_max_plop = 2;
	u8 niveau = 0;
	
	initGame(niveau_max_plop);
	
	while(1)
	{
		// Pause the game, I have to go to the toilets, it's urgent !!!
		if(Wiimote[0].Newpress.Home) { pause = true; }
		
		// Merry christmas guys !
		ML_DrawSprite(&background);
		
		// C'mon, let's make'em move and appear
		for(j = 0; j < i; j++)
		{	
			if(plop[j].visible)
			{
				if(!pause)
				{
					// I like to move it !
					// Bounce in the left and right side
					if(plop[j].x + plop[j].dx > WIDTH-SIZE_PLOP || plop[j].x + plop[j].dx < 0)						plop[j].dx = -plop[j].dx * 0.5;  // reduce the bounce
				
					// Bounce in the up and down side
					if(plop[j].y + plop[j].dy < 0)
						plop[j].dy = -plop[j].dy;
	
					if(plop[j].y + plop[j].dy > HEIGHT-SIZE_PLOP)
						plop[j].dy = -plop[j].dy * 0.4; // reduce the bounce
	
					if((abs(plop[j].dy) < 0.2) && (plop[j].y > HEIGHT-SIZE_PLOP-2))
					{
						ASND_SetVoice(ASND_GetFirstUnusedVoice(), VOICE_MONO_16BIT, 22050 + ML_Rand(0, 22050), 0, (char *)youpi_raw, youpi_raw_size, 110, 110, NULL);
						plop[j].dy = ML_Rand(GAME_VELOCITY); // a bounce if the velocity is too low
					}  	
				
					if((abs(plop[j].dx) < 0.2) && ((plop[j].x > WIDTH-2) || (plop[j].x < -SIZE_PLOP)))
					{
						ASND_SetVoice(ASND_GetFirstUnusedVoice(), VOICE_MONO_16BIT, 22050 + ML_Rand(0, 22050), 0, (char *)youpi_raw, youpi_raw_size, 110, 110, NULL);
						plop[j].dx = ML_Rand(GAME_VELOCITY); // a bounce if the velocity is too low
					} 
	
					for(z = 0; z < i; z++)
					{
						if(z == i)
							continue;

						if(abs(plop[j].x+plop[j].dx-plop[z].x) < 2 * RADIUS_PLOP && abs(plop[j].y+plop[j].dy-plop[z].y) < 2 * RADIUS_PLOP) 
						{
							betweenPlopBounce(&plop[j].x, &plop[j].y, &plop[j].dx, &plop[j].dy, &plop[z].x, &plop[z].y, &plop[z].dx, &plop[z].dy);
						}
					
						// Wiimote haz teh control
						if(Wiimote[0].GForce.X < -2) plop[z].dx = Wiimote[0].GForce.X*5;
						else if(Wiimote[0].GForce.X > 2) plop[z].dx = Wiimote[0].GForce.X*5;
					
						if(Wiimote[0].GForce.Y < -2) plop[z].dy = Wiimote[0].GForce.Y*5;
						else if(Wiimote[0].GForce.Y > 2) plop[z].dy = Wiimote[0].GForce.Y*5;
					}
			
					// Move now !
					plop[j].x += plop[j].dx;
					plop[j].y += plop[j].dy;
			
					// Gravity rocks
					plop[j].dy += 1;
			
					// Don't fall, little plop ! :'(
					if(plop[j].y > HEIGHT-SIZE_PLOP) plop[j].y = HEIGHT-(SIZE_PLOP+6);
				
					// Rotate guys !
					ML_RotateSprite(&plop[j], Wiimote[0].IR.Angle, 0);
					
					// Are you out, small santa' ?
					if(plop[j].x < -SIZE_PLOP || plop[j].x > WIDTH || plop[j].y > HEIGHT || plop[j].y < -SIZE_PLOP) santaOut++;
				}
			
				// Let me be on the screen, please !						
				ML_DrawSprite(&plop[j]);
			}
		}
		
		// Time will be up !
		if(go >= 10 && !pause)
		{
			if(!time) 
			{
				ML_DrawSprite(&bg_lost);
				ML_DrawSpriteText(&fontBlack, 237, 447-16, "%d", santaOutTotal);
				ML_DrawSpriteText(&fontBlack, 479, 447-16, "%d", timeTotal);
				
				if(Wiimote[0].Released.Plus) menu();
			}
			else time--;
		}
		else if(go < 10 && go >= 1) go++;
		else if(go == 0 && !pause)
		{
			if(Wiimote[0].Released.Plus)
			{
				go = 1;
			
				switch(niveau)
				{
					case 0:
						time = 300;
						niveau_max_plop = 10;
						break;
					case 1:
						time = 300;
						niveau_max_plop = 20;
						break;
					case 2:
						time = 350;
						niveau_max_plop = 40;
						break;
					case 3:
						time = 400;
						niveau_max_plop = 80;
						break;
					case 4:
						time = 400;
						niveau_max_plop = 100;
						break;
					case 5:
						time = 400;
						niveau_max_plop = 200;
						break;
					case 6:
						time = 400;
						niveau_max_plop = 400;
						break;
					case 7:
						menu();
						break;
					default:;
				}
				
				timeNiveau = time;
				initGame(niveau_max_plop);
			}

			switch(niveau)
			{
				case 0:  
					ML_DrawSprite(&bg_niveau_0);
					break;
				case 1:
					ML_DrawSprite(&bg_niveau_1);
					ML_DrawSprite(&first);
					ML_DrawSpriteText(&fontBlack, 269, 225-16, "%d", timeNiveau-time);
					ML_DrawSpriteText(&fontBlack, 333, 252-16, "%d", timeTotal);
					ML_DrawSpriteText(&fontBlack, 328, 277-16, "%d", niveau_max_plop);
					ML_DrawSpriteText(&fontBlack, 393, 305-16, "%d", santaOutTotal);
					break;
				case 2:
					ML_DrawSprite(&bg_niveau_1);
					ML_DrawSprite(&second);
					ML_DrawSpriteText(&fontBlack, 269, 225-16, "%d", timeNiveau-time);
					ML_DrawSpriteText(&fontBlack, 333, 252-16, "%d", timeTotal);
					ML_DrawSpriteText(&fontBlack, 328, 277-16, "%d", niveau_max_plop);
					ML_DrawSpriteText(&fontBlack, 393, 305-16, "%d", santaOutTotal);
					break;
				case 3:
					ML_DrawSprite(&bg_niveau_1);
					ML_DrawSprite(&third);
					ML_DrawSpriteText(&fontBlack, 269, 225-16, "%d", timeNiveau-time);
					ML_DrawSpriteText(&fontBlack, 333, 252-16, "%d", timeTotal);
					ML_DrawSpriteText(&fontBlack, 328, 277-16, "%d", niveau_max_plop);
					ML_DrawSpriteText(&fontBlack, 393, 305-16, "%d", santaOutTotal);
					break;
				case 4:
					ML_DrawSprite(&bg_niveau_1);
					ML_DrawSprite(&fourth);
					ML_DrawSpriteText(&fontBlack, 269, 225-16, "%d", timeNiveau-time);
					ML_DrawSpriteText(&fontBlack, 333, 252-16, "%d", timeTotal);
					ML_DrawSpriteText(&fontBlack, 328, 277-16, "%d", niveau_max_plop);
					ML_DrawSpriteText(&fontBlack, 393, 305-16, "%d", santaOutTotal);
					break;
				case 5:
					ML_DrawSprite(&bg_niveau_1);
					ML_DrawSprite(&fifth);
					ML_DrawSpriteText(&fontBlack, 269, 225-16, "%d", timeNiveau-time);
					ML_DrawSpriteText(&fontBlack, 333, 252-16, "%d", timeTotal);
					ML_DrawSpriteText(&fontBlack, 328, 277-16, "%d", niveau_max_plop);
					ML_DrawSpriteText(&fontBlack, 393, 305-16, "%d", santaOutTotal);
					break;
				case 6:
					ML_DrawSprite(&bg_niveau_1);
					ML_DrawSprite(&sixth);
					ML_DrawSpriteText(&fontBlack, 269, 225-16, "%d", timeNiveau-time);
					ML_DrawSpriteText(&fontBlack, 333, 252-16, "%d", timeTotal);
					ML_DrawSpriteText(&fontBlack, 328, 277-16, "%d", niveau_max_plop);
					ML_DrawSpriteText(&fontBlack, 393, 305-16, "%d", santaOutTotal);
					break;
				case 7:
					ML_DrawSprite(&bg_won); 
					ML_DrawSpriteText(&fontBlack, 237, 447-16, "%d", santaOutTotal);
					ML_DrawSpriteText(&fontBlack, 479, 447-16, "%d", timeTotal);
					break;
			}				
		}
		
		// Time
		ML_DrawSpriteText(&font, 40, 20, "Time : %d", time);	
		
		// If all are out, there is a winner !
		if(santaOut == niveau_max_plop && go == 10 && time)
		{
			go = 0;
			niveau++;
			timeTotal += timeNiveau-time;
			santaOutTotal += santaOut;
		}
		else santaOut = 0;

		// Return to menu plz
		if(pause)
		{
			ML_DrawSprite(&bg_pause);
			if(Wiimote[0].Newpress.Plus) menu();
			else if(Wiimote[0].Newpress.Minus) pause = false;
		}
		
		ML_DrawSpriteText(&font, 20, 20, "%d", ML_GetFPS());
		
		// Cursor rocks \o/
		ML_Cursor(&cursor, 0);
		
		// Magic function
		ML_Refresh();
	}
}

void initGame(int max_plop_niveau)
{
	for(i = 0; i < MAX_PLOP; i++)
	{
		if(i <= max_plop_niveau) 
		{
			plop[i].visible = true;
			plop[i].x = ML_Rand(0, WIDTH-SIZE_PLOP);
			plop[i].y = ML_Rand(0, HEIGHT-SIZE_PLOP);
		}
		else plop[i].visible = false;
		
		plop[i].dx = 0;
		plop[i].dy = 0;
	}

	i = max_plop_niveau;
}

void betweenPlopBounce(s16 *x1, s16 *y1, s8 *dx1, s8 *dy1, s16 *x2, s16 *y2, s8 *dx2, s8 *dy2)
{	
	// Distance
	double o2o1x = *x1 - *x2,
	o2o1y = *y1 - *y2;
	double d = sqrt(o2o1x*o2o1x+o2o1y*o2o1y);
	if (d == 0) return; // Same center ?
	
	// Coordinate system (o2,x,y)
	double Vx = *dx1 - *dx2,
	Vy = *dy1 - *dy2;
	double V = sqrt(Vx*Vx + Vy*Vy);
	if (V == 0) return; // Same velocity ?
	
	// Next coordinate system V (o2,i,j)
	double ix = Vx/V,
	iy = Vy/V,
	jx = -iy,
	jy = ix;
	
	// Height
	double H = o2o1x*jx + o2o1y*jy;
	
	// Angle
	double th = acos(H/d),
	c = cos(th),
	s = sin(th);
	
	// Velocity after the bounce in (o2,i,j)
	double v1i=V*c*c, v1j=V*c*s, v2i=V*s*s, v2j=-v1j;
	
	// In the original coordinate system (O,x,y)
	*dx1 = (v1i*ix + v1j*jx + *dx2);
	*dy1 = (v1i*iy + v1j*jy + *dy2);
	*dx2 += (v2i*ix + v2j*jx);
	*dy2 += (v2i*iy + v2j*jy);
}

