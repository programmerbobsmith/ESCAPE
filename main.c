#include <ti/getcsc.h>
#include <stdint.h>
#include <graphx.h>
#include <fileioc.h>
#include <sys/util.h>
#include <sys/rtc.h>

int main()
{
	gfx_Begin();
	gfx_ZeroScreen();
	gfx_SetTextFGColor(254);
	uint8_t guess_var = ti_Open("GUESS","r");
	struct guess
	{
		unsigned int high_scores[2];
	}data;
	unsigned int key;
	if (guess_var == 0)
	{
		guess_var = ti_Open("GUESS","r+");
		data.high_scores[0]=data.high_scores[1]=data.high_scores[2]=50;
		ti_Write(&data,sizeof(struct guess),1,guess_var);
	}
	ti_Close(guess_var);
	guess_var = ti_Open("GUESS","r+");
	ti_Read(&data,sizeof(struct guess),1,guess_var);
	ti_Rewind(guess_var);
	gfx_PrintStringXY("Escape to the OS",100,40);
	gfx_PrintStringXY("High Scores:",120,60);
	gfx_PrintStringXY("1:   ",125,70);
	gfx_PrintInt(data.high_score[0],2);
	gfx_PrintStringXY("2:   ",125,80);
	gfx_PrintInt(data.high_scores[1],2);
	gfx_PrintStringXY("3:   ",125,90);
	gfx_PrintInt(data.high_scores[2],2);
	gfx_PrintStringXY("Press Enter to Start",130,130);
	while(key = os_GetCSC(), key != sk_Enter)
	{
		if(key == sk_Clear)
		{
			ti_SetArchiveStatus(true,guess_var);
			ti_Close(guess_var);
			gfx_End();
			return 0;
		}
	}
	unsigned int key_array[] = {sk_Yequ,sk_Window,sk_Zoom,sk_Trace,sk_Graph,sk_2nd,sk_Mode,sk_Del,sk_Up,sk_Alpha,sk_GraphVar,sk_Stat,sk_Left,sk_Down,sk_Right,sk_Math,sk_Apps,sk_Prgm,sk_Vars,sk_Clear,sk_Recip,sk_Sin,sk_Cos,sk_Tan,sk_Power,sk_Square,sk_Comma,sk_LParen,sk_RParen,sk_Div,sk_Log,sk_7,sk_8,sk_9,sk_Mul,sk_Ln,sk_4,sk_5,sk_6,sk_Sub,sk_Store,sk_1,sk_2,sk_3,sk_Add,sk_0,sk_DecPnt,sk_Chs,sk_Enter};
	unsigned int previous_key, correct_key, tries;
	srand(rtc_Time());
	correct_key = randInt(0,48);
	tries = previous_key = key = 0;
	while(key != key_array[correct_key])
	{
		gfx_ZeroScreen();
		gfx_PrintStringXY("Guess the Key!",100,30);
		gfx_PrintStringXY("Number of Tries:   ",40,60);
		gfx_PrintInt(tries,3);
		gfx_PrintStringXY("Previous Key:      ",40,75);
		gfx_PrintInt(previous_key,3);
		while(key = os_GetCSC(), key==0);
		tries++;
		previous_key = key;
	}
	gfx_PrintStringXY("You have guessed the correct key!",0,100);
	gfx_PrintStringXY("Press the [Clear] to exit!",90,120);
	if(tries<data.high_scores[0])
	{
		data.high_scores[2]=data.high_scores[1];
		data.high_scores[1]=data.high_scores[0];
		data.high_scores[0]=tries;
	}else if(tries<data.high_scores[1] && tries>data.high_scores[0])
	{
		data.high_scores[2]=data.high_scores[1];
		data.high_scores[1]=tries;
	}else if(tries<data.high_scores[2] && tries>data.high_scores[1])
	{
		data.high_scores[2]=tries;
	}
	ti_Write(&data,sizeof(struct guess),1,guess_var);
	ti_SetArchiveStatus(true,guess_var);
	ti_Close(guess_var);
	while(os_GetCSC() != sk_Clear);
	gfx_End();
	return 0;
}
