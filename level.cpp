#include "level.h"

extern Tile* World[WORLD_W][WORLD_H];

int LoadCFG(Level *l, char* path){
	char buf[255];
	FILE *fileptr = NULL;
	fileptr = fopen(path,"r");
	if(!fileptr){
		fprintf(stderr,"couldn't find file: ",path);
		return -1;
	}

	int progress = 0;

	{
		fscanf(fileptr,"%s",buf);
		if(buf[0] == '#')
			fgets(buf,255,fileptr);
		else{
		//	fprintf(stdout,"\n");
		//	fprintf(stdout,buf);
			if(strcmp(buf,"level_def:")==0){
//				fprintf(stdout,"A LEVEL DEFINITION! \n");
				if(LoadLevelDef(l,fileptr,buf)==0)
					progress++;
				else
					fprintf(stdout,"Level Load failed\n");
			}
			if(strcmp(buf,"sprite_def:")==0){
//				fprintf(stdout,"A SPRITE DEFINITION! \n");
				if(LoadSpriteDef(l,fileptr,buf)==0){
					fscanf(fileptr,"%s",buf);
					progress++;
				}else
					fprintf(stdout,"Sprite Load failed\n");
			}
			if(strcmp(buf,"tile_def:")==0){
//				fprintf(stdout,"A TILE DEFINITION! \n");
				if(LoadTileDef(l,fileptr,buf)==0){
					fscanf(fileptr,"%s",buf);
					progress++;
				}else
					fprintf(stdout,"Tile Load failed\n");
			}
			if(strcmp(buf,"map_def:")==0){
//				fprintf(stdout,"A MAP DEFINITION! \n");
				if(LoadMapDef(l,fileptr,buf)==0)
					progress++;
				else
					fprintf(stdout,"Map Load failed\n");
			}
			
		}
		
	}

	fclose(fileptr);
	return(progress!=4);
}

int LoadLevelDef(Level *l, FILE *fileptr, char *buf){
	bool loaded = false;
	do{
		fscanf(fileptr,"%s",buf);
		if(buf[0] == '#'){
			fgets(buf,255,fileptr);
			continue;
		}
		if(strcmp(buf,"width:")==0){
			fscanf(fileptr,"%i",&l->width);
		}
		else if(strcmp(buf,"height:")==0){
			fscanf(fileptr,"%i",&l->height);
		}
		else if(strcmp(buf,"numsprites:")==0){
			fscanf(fileptr,"%i",&l->numsprites);
		}
		else if(strcmp(buf,"numtiles:")==0){
			fscanf(fileptr,"%i",&l->numtiles);
		}
		else
			loaded = true;
	}while(!loaded);
	return 0;
}

int LoadSpriteDef(Level *l,FILE *fileptr,char *buf){
	bool loaded = false;
	int spriteno = 0;
	char *path = NULL; 
	int swidth,sheight,sframes;

	do{
		fscanf(fileptr,"%s",buf);
		if(buf[0] == '#'){
			fgets(buf,255,fileptr);
			continue;
		}
		if(l->sprites != NULL){
			if(l->sprites[spriteno]!=NULL){
				
				fscanf(fileptr,"%i",&spriteno);
				fscanf(fileptr,"%s",buf);
				path = buf;
			//	fscanf(fileptr,"%s",path);
				fscanf(fileptr,"%i",&swidth);
				fscanf(fileptr,"%i",&sheight);
				fscanf(fileptr,"%i",&sframes);

				l->sprites[spriteno] = LoadSprite(path,swidth,sheight,sframes);
			}
			else
				loaded = true;
		}else
			return -1;
	}while((spriteno+1)<l->numsprites);
	return 0;
}

int LoadTileDef(Level *l,FILE *fileptr,char *buf){
	int tileno = 0;
	bool tilefree;
	int lowers;
	int uppers;
	for(int i = 0; i < MAX_TILES;i++){
		l->tiles[i] = NULL;
	}
	do{
		fscanf(fileptr,"%s",buf);
		if(buf[0] == '#'){
			fgets(buf,255,fileptr);
			continue;
		}
		if(l->tiles != NULL){
			fscanf(fileptr,"%i",&tileno);
			if(l->tiles[tileno]==NULL){
				fscanf(fileptr,"%i",&tilefree);
				fscanf(fileptr,"%i",&lowers);
				fscanf(fileptr,"%i",&uppers);

				l->tiles[tileno] = LoadTile(tilefree,l->sprites[lowers],l->sprites[uppers]);
				if(uppers<0)
					l->tiles[tileno]->upperspr = NULL;
				if(lowers<0)
					l->tiles[tileno]->lowerspr = NULL;
				
			}
		}else
			return -1;
	}while((tileno+1)<l->numtiles);
	return 0;
}

int LoadMapDef(Level *l,FILE *fileptr,char *buf){
	int tileno = 0;
	int tilecount = 0;
	int tiles = l->width*l->height;
	while(tilecount < tiles){
		fscanf(fileptr,"%i",&tileno);
		int x = tilecount % l->width;
		int y = tilecount / l->width;
		Vec2i *loc = new Vec2i;
		loc->x = x*TILE_W;
		loc->y = y*TILE_H;
		World[x][y] = (Tile*)malloc(sizeof(Tile));
		memcpy(World[x][y], l->tiles[tileno], sizeof(Tile));
		World[x][y]->position = *loc;
//		fprintf(stdout,"Tile %i, %i    %i   \n ",x,y,tileno);
//		fprintf(stdout,World[x][y]->lowerspr->filename);
//		fprintf(stdout,"   Position: %i, %i \n",World[x][y]->position.x,World[x][y]->position.y);
		tilecount++;
	}
/*	int temp = WORLD_W - l->width;
	while(temp > 0){			//fills in null tiles to the right of the level
		for(int i = 0; i < l->height; i++){
			World[temp+l->width][i] = NULL;
		}
		temp--;
	}
	temp = WORLD_H - l->height;
	while(temp > 0){
		for(int i = 0; i < WORLD_W; i++){
			World[i][temp+l->height] = NULL;
		}
		temp--;
	}
	*/
	return 0;
}

int SaveCFG(Level *l, char* path){
	char buf[255];
	int cur_line = 0;
	FILE *fileptr = NULL;
	fileptr = fopen(path,"w");
	if(!fileptr){
		fprintf(stderr,"couldn't find character file: ",path);
		return -1;
	}
//		fprintf(fileptr,"tile_width: %d\n",f->t_width);
//		fprintf(fileptr,"tile_height: %d\n",f->t_height);
//		fprintf(fileptr,"tiles_per_row: %d\n",f->t_per_row);
		
	

	fclose(fileptr);
	return 0;
}

Tile* LoadTile(bool fr, Sprite *lower, Sprite *upper){
	Tile *t = new Tile;
	t->lowerframe = 0;
	t->upperframe = 0;
	t->free = fr;
	t->lowerspr = lower;
	t->upperspr = upper;
	t->debugFill = upper;
	return t;
}