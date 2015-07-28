#include "level.h"

/*everything in this file is terrible and needs to be redone*/

extern Tile* World[WORLD_W][WORLD_H];
SDL_Rect world;					//Rect representing a 2D game world

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
				//	fscanf(fileptr,"%s",buf);
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

	while((spriteno)<l->numsprites){
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
				spriteno+=1;
			}
			else
				loaded = true;
		}else
			return -1;
	}
	return 0;
}

int LoadTileDef(Level *l,FILE *fileptr,char *buf){
	int tileno = 0;
	int tilect = 0;
	bool tilefree;
	int height;
	int sheet;
	int lowerf;
	int upperf;

	while(fscanf(fileptr,"%s",buf)){
		if(buf[0] == '#'){
			fgets(buf,255,fileptr);
			continue;
		}
		if(strcmp(buf,"map_def:")==0)
			return 0;
		if(strncmp(buf,"tile",4)==0){
			fscanf(fileptr,"%s",buf);
			if(strcmp(buf,"tileno:")==0){
				fscanf(fileptr,"%i",&tileno);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"height:")==0){
				fscanf(fileptr,"%i",&height);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"free:")==0){
				fscanf(fileptr,"%i",&tilefree);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"sprite:")==0){
				fscanf(fileptr,"%i",&sheet);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"lowerf:")==0){
				fscanf(fileptr,"%i",&lowerf);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"upperf:")==0){
				fscanf(fileptr,"%i",&upperf);
				fscanf(fileptr,"%s",buf);
			}
			l->tiles[tilect] = LoadTile(tilefree,height,l->sprites[sheet],lowerf,upperf);
			tilect++;
			if(tilect >= l->numtiles)
				return 0;
		}
		/*
			if(strcmp(buf,"tile_width:")==0){ 
				fscanf(fileptr,"%i",&f->t_width);
				fscanf(fileptr,"%s",buf);}*/
	}

	return 0;
	/*
	int tileno = 0;
	int tilect = 0;
	bool tilefree;
	int height;
	int sheet;
	int lowerf;
	int upperf;
	for(int i = 0; i < MAX_TILES;i++){
		l->tiles[i] = NULL;
	}
	while((tilect)<l->numtiles){
		tilect++;
		fscanf(fileptr,"%s",buf);
		if(buf[0] == '#'){
			fgets(buf,255,fileptr);
			continue;
		}
		if(l->tiles != NULL){
			fscanf(fileptr,"%i",&tileno);
			if(l->tiles[tilect]==NULL){
				fscanf(fileptr,"%i",&height);
				fscanf(fileptr,"%i",&tilefree);
				fscanf(fileptr,"%i",&sheet);
				fscanf(fileptr,"%i",&lowerf);
				fscanf(fileptr,"%i",&upperf);

				l->tiles[tilect] = LoadTile(tilefree,height,l->sprites[sheet],lowerf,upperf);
				if(lowerf<0)
					l->tiles[tilect]->lowerspr = NULL;
				
			}

		}else
			return -1;
	}
	return 0;*/
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
		World[x][y] = new Tile();
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
//	char buf[255];
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

Tile* LoadTile(bool fr, int ht, Sprite *sheet, int baseframe, int floorframe, int wallframe){
	//FIX ME
	Tile *t = new Tile;
	TileStructure *ts = new TileStructure;
	ts->baseframe = 0;
	ts->height = ht;
	ts->spritesheet = sheet;
	ts->baseframe = baseframe; 
	ts->floorframe = floorframe;
	ts->wallframe = wallframe;
	t->free = fr;
	t->structure = ts;
	t->contents = NULL;
	return t;
}
