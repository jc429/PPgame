#ifndef _PSTRING_
#define _PSTRING_

#include <stdio.h>

typedef enum CharCode_T{		//maps chars to where they are on my sprites
	CH_CODE_a	=	1	,
	CH_CODE_b	=	2	,
	CH_CODE_c	=	3	,
	CH_CODE_d	=	4	,
	CH_CODE_e	=	5	,
	CH_CODE_f	=	6	,
	CH_CODE_g	=	7	,
	CH_CODE_h	=	8	,
	CH_CODE_i	=	9	,
	CH_CODE_j	=	10	,
	CH_CODE_k	=	11	,
	CH_CODE_l	=	12	,
	CH_CODE_m	=	13	,
	CH_CODE_n	=	14	,
	CH_CODE_o	=	15	,
	CH_CODE_p	=	16	,
	CH_CODE_q	=	17	,
	CH_CODE_r	=	18	,
	CH_CODE_s	=	19	,
	CH_CODE_t	=	20	,
	CH_CODE_u	=	21	,
	CH_CODE_v	=	22	,
	CH_CODE_w	=	23	,
	CH_CODE_x	=	24	,
	CH_CODE_y	=	25	,
	CH_CODE_z	=	26	,
	CH_CODE_A	=	33	,
	CH_CODE_B	=	34	,
	CH_CODE_C	=	35	,
	CH_CODE_D	=	36	,
	CH_CODE_E	=	37	,
	CH_CODE_F	=	38	,
	CH_CODE_G	=	39	,
	CH_CODE_H	=	40	,
	CH_CODE_I	=	41	,
	CH_CODE_J	=	42	,
	CH_CODE_K	=	43	,
	CH_CODE_L	=	44	,
	CH_CODE_M	=	45	,
	CH_CODE_N	=	46	,
	CH_CODE_O	=	47	,
	CH_CODE_P	=	48	,
	CH_CODE_Q	=	49	,
	CH_CODE_R	=	50	,
	CH_CODE_S	=	51	,
	CH_CODE_T	=	52	,
	CH_CODE_U	=	53	,
	CH_CODE_V	=	54	,
	CH_CODE_W	=	55	,
	CH_CODE_X	=	56	,
	CH_CODE_Y	=	57	,
	CH_CODE_Z	=	58	,
	
	CH_CODE_0	=	64	,
	CH_CODE_1	=	65	,
	CH_CODE_2	=	66	,
	CH_CODE_3	=	67	,
	CH_CODE_4	=	68	,
	CH_CODE_5	=	69	,
	CH_CODE_6	=	70	,
	CH_CODE_7	=	71	,
	CH_CODE_8	=	72	,
	CH_CODE_9	=	73	,

	CH_CODE_SPC	=	32	,
	CH_CODE_DOT	=	76	,
	CH_CODE_CMA	=	77	,
	CH_CODE_QUE	=	75	,
	CH_CODE_EXC	=	74	,
	CH_CODE_APO =	78	,
	CH_CODE_QUO	=	79	,

}CharCode;

typedef struct Letter_T{
	CharCode chr;
	int width;		//width of char in pixels (includes one blank pixel of space in each direction?)
}Letter;

char *copy_string(char *destination,const char *source);
Letter GetCharCode(char ch);

#endif