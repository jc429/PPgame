#include "pstring.h"

#include <stdio.h>
#include <string.h>

char *copy_string(char *dest,const char *src){
	return strcpy(dest,src);
}

Letter GetCharCode(char ch){
	Letter l;
	switch(ch){
	case 'a':	l.chr = CH_CODE_a;		l.width = 6;	break;
	case 'b':	l.chr = CH_CODE_b;		l.width = 6;	break;
	case 'c':	l.chr = CH_CODE_c;		l.width = 6;	break;
	case 'd':	l.chr = CH_CODE_d;		l.width = 6;	break;
	case 'e':	l.chr = CH_CODE_e;		l.width = 6;	break;
	case 'f':	l.chr = CH_CODE_f;		l.width = 5;	break;
	case 'g':	l.chr = CH_CODE_g;		l.width = 6;	break;
	case 'h':	l.chr = CH_CODE_h;		l.width = 6;	break;
	case 'i':	l.chr = CH_CODE_i;		l.width = 3;	break;
	case 'j':	l.chr = CH_CODE_j;		l.width = 5;	break;
	case 'k':	l.chr = CH_CODE_k;		l.width = 6;	break;
	case 'l':	l.chr = CH_CODE_l;		l.width = 3;	break;
	case 'm':	l.chr = CH_CODE_m;		l.width = 7;	break;
	case 'n':	l.chr = CH_CODE_n;		l.width = 5;	break;
	case 'o':	l.chr = CH_CODE_o;		l.width = 6;	break;
	case 'p':	l.chr = CH_CODE_p;		l.width = 6;	break;
	case 'q':	l.chr = CH_CODE_q;		l.width = 6;	break;
	case 'r':	l.chr = CH_CODE_r;		l.width = 5;	break;
	case 's':	l.chr = CH_CODE_s;		l.width = 6;	break;
	case 't':	l.chr = CH_CODE_t;		l.width = 5;	break;
	case 'u':	l.chr = CH_CODE_u;		l.width = 6;	break;
	case 'v':	l.chr = CH_CODE_v;		l.width = 7;	break;
	case 'w':	l.chr = CH_CODE_w;		l.width = 7;	break;
	case 'x':	l.chr = CH_CODE_x;		l.width = 7;	break;
	case 'y':	l.chr = CH_CODE_y;		l.width = 6;	break;
	case 'z':	l.chr = CH_CODE_z;		l.width = 5;	break;

	case 'A':	l.chr = CH_CODE_A;		l.width = 7;	break;
	case 'B':	l.chr = CH_CODE_B;		l.width = 6;	break;
	case 'C':	l.chr = CH_CODE_C;		l.width = 7;	break;
	case 'D':	l.chr = CH_CODE_D;		l.width = 7;	break;
	case 'E':	l.chr = CH_CODE_E;		l.width = 6;	break;
	case 'F':	l.chr = CH_CODE_F;		l.width = 6;	break;
	case 'G':	l.chr = CH_CODE_G;		l.width = 7;	break;
	case 'H':	l.chr = CH_CODE_H;		l.width = 6;	break;
	case 'I':	l.chr = CH_CODE_I;		l.width = 5;	break;
	case 'J':	l.chr = CH_CODE_J;		l.width = 6;	break;
	case 'K':	l.chr = CH_CODE_K;		l.width = 7;	break;
	case 'L':	l.chr = CH_CODE_L;		l.width = 6;	break;
	case 'M':	l.chr = CH_CODE_M;		l.width = 7;	break;
	case 'N':	l.chr = CH_CODE_N;		l.width = 7;	break;
	case 'O':	l.chr = CH_CODE_O;		l.width = 7;	break;
	case 'P':	l.chr = CH_CODE_P;		l.width = 6;	break;
	case 'Q':	l.chr = CH_CODE_Q;		l.width = 7;	break;
	case 'R':	l.chr = CH_CODE_R;		l.width = 6;	break;
	case 'S':	l.chr = CH_CODE_S;		l.width = 6;	break;
	case 'T':	l.chr = CH_CODE_T;		l.width = 7;	break;
	case 'U':	l.chr = CH_CODE_U;		l.width = 7;	break;
	case 'V':	l.chr = CH_CODE_V;		l.width = 7;	break;
	case 'W':	l.chr = CH_CODE_W;		l.width = 9;	break;
	case 'X':	l.chr = CH_CODE_X;		l.width = 7;	break;
	case 'Y':	l.chr = CH_CODE_Y;		l.width = 7;	break;
	case 'Z':	l.chr = CH_CODE_Z;		l.width = 7;	break;

	case '0':	l.chr = CH_CODE_0;		l.width = 6;	break;
	case '1':	l.chr = CH_CODE_1;		l.width = 4;	break;
	case '2':	l.chr = CH_CODE_2;		l.width = 6;	break;
	case '3':	l.chr = CH_CODE_3;		l.width = 6;	break;
	case '4':	l.chr = CH_CODE_4;		l.width = 6;	break;
	case '5':	l.chr = CH_CODE_5;		l.width = 6;	break;
	case '6':	l.chr = CH_CODE_6;		l.width = 6;	break;
	case '7':	l.chr = CH_CODE_7;		l.width = 6;	break;
	case '8':	l.chr = CH_CODE_8;		l.width = 6;	break;
	case '9':	l.chr = CH_CODE_9;		l.width = 6;	break;

	case ' ':	l.chr = CH_CODE_SPC;	l.width = 4;	break;
	case '.':	l.chr = CH_CODE_DOT;	l.width = 3;	break;
	case ',':	l.chr = CH_CODE_CMA;	l.width = 3;	break;
	case '!':	l.chr = CH_CODE_EXC;	l.width = 3;	break;
	case '?':	l.chr = CH_CODE_QUE;	l.width = 6;	break;
	case '\'':	l.chr = CH_CODE_APO;	l.width = 1;	break;
	case '"':	l.chr = CH_CODE_QUO;	l.width = 5;	break;

	default: 
		l.chr = (CharCode)0;	l.width = 8;	break;
	}

	return l;
}


