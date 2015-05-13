#include "world.h"
#include "pfile.h"

#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>

/* 
Tile - A single Tile in the game world;
Chunk - a fixed size 2d array of Tiles (not Tile pointers) - does not have to be the same size every time, but an individual chunk's size is not dynamic.
Area - a number of chunks "stitched" together, forming whatever location in the game world. All chunks of an area are loaded at once (?)
World - The main world, handles loadind/unloading of areas. Probably loads the current Area and its neighbors 
*/
Chunk ChunkList[MAX_CHUNKS];
int numChunks;

void InitChunkList(){
	for(int chunk = 0; chunk < MAX_CHUNKS; chunk++){
		ChunkList[chunk].path = "";
		for(int i = 0; i < MAX_CHUNK_SIZE_X; i++){
			for(int j = 0; j < MAX_CHUNK_SIZE_Y; j++){
				ClearTile(&ChunkList[chunk].tiles[i][j]);
			}
		}
	}
}

Chunk *LoadChunk(char* chunkpath){
	int i;
	if(numChunks >= MAX_CHUNKS){
		printf("TOO MANY CHUNKS LOADED");
	/*	for(i = 0; i <= MAX_CHUNKS; i++)
			if(ChunkList[i].used <= 0)
				FreeChunk(&ChunkList[i]);*/
	}
	for(i = 0; i < MAX_CHUNKS; i++)
		if(strcmp(ChunkList[i].path,chunkpath) == 0){
			ChunkList[i].used++;
			return &ChunkList[i];
		}
	for(i = 0; i <= MAX_CHUNKS; i++)
		if(ChunkList[i].used == 0) break;
	
	LoadChunkCFG(&ChunkList[i],chunkpath);

//	printf("Size: %i,%i\n",ChunkList[i].size.x,ChunkList[i].size.y);
	ChunkList[i].used++;
	numChunks++;
	return &ChunkList[i];
}

void LoadChunkCFG(Chunk *ch,char *chunkpath){
	FILE* pFile = fopen(chunkpath, "rb");
	char buffer[65536];
	rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
	rapidjson::Document document;
	document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
	assert(document.IsObject());
	fclose(pFile);

	assert(document["test_chunk"].IsObject());
	const rapidjson::Value& chunk = document["test_chunk"];
	assert(chunk["size_x"].IsInt());
	ch->size.x = chunk["size_x"].GetInt();
	assert(chunk["size_y"].IsInt());
	ch->size.y = chunk["size_y"].GetInt();
	if(chunk.HasMember("sprite_def")){
		const rapidjson::Value& sprites = chunk["sprite_def"];
		for(int i = 0; i < (int)sprites.MemberCount(); i++){
			const rapidjson::Value& sprite = sprites["spr_0"];
			char sprpath[255];
			int tw;
			int th;
			int fpl;
			copy_string(sprpath,sprite["path"].GetString());
			tw = sprite["tile_width"].GetInt();
			th = sprite["tile_height"].GetInt();
			fpl = sprite["frames_per_line"].GetInt();
			ch->spritelist[i] = LoadSprite(sprpath,tw,th,fpl);
			ch->numSprites++;
		}
	}else
		printf("No sprite definition!");
	if(chunk.HasMember("tile_def")){
		const rapidjson::Value& tiles = chunk["tile_def"];
		for(rapidjson::Value::ConstMemberIterator itr = tiles.MemberBegin(); itr != tiles.MemberEnd(); ++itr){
			const rapidjson::Value& curTile = tiles[itr->name];
			TileStructure *t_struct = ch->tilelist[ch->numTiles];
			if(t_struct != NULL) delete t_struct;
			t_struct = new TileStructure;
			t_struct->id = curTile["tile_id"].GetInt();
			t_struct->height = curTile["height"].GetInt();
			t_struct->solid = (curTile["solid"].GetInt() != 0);
			t_struct->spritesheet = ch->spritelist[curTile["sprite"].GetInt()];
			t_struct->baseframe = curTile["base_frame"].GetInt();
			t_struct->floorframe = curTile["floor_frame"].GetInt();
			t_struct->wallframe = curTile["wall_frame"].GetInt();
			ch->tilelist[ch->numTiles] = t_struct;
			ch->numTiles++;
	//		printf("Tile LOaded: %i \n", t_struct->id);
		}
	}else
		printf("No tile definition!");
	if(chunk.HasMember("map_def")){
		assert(chunk["map_def"].IsArray());
		const rapidjson::Value& map = chunk["map_def"];
		for (int row = 0; row <ch->size.y; row++){
			const rapidjson::Value& currentRow = map[rapidjson::SizeType(row)];
			for(int col = 0; col < ch->size.x; col++){
				ch->tiles[col][row].structure = ch->tilelist[currentRow[rapidjson::SizeType(col)].GetInt()];
				SetVec2i(ch->tiles[col][row].position,col*TILE_W,row*TILE_H);
				ch->tiles[col][row].free = true;
			//	printf("%i, ",currentRow[rapidjson::SizeType(col)].GetInt());
			}
		//	printf("\n");
		}
	}else
		printf("No map definition!");
}

void FreeChunk(Chunk *ch){
	ch->used--;
	if(ch->used > 0)
		return;
	ch->path = "";
	for(int i = 0; i < ch->size.x; i++){
		for(int j = 0; j < ch->size.y; j++){
			ClearTile(&ch->tiles[i][j]);
		}
	}
	SetVec2i(ch->size,0,0);
	numChunks--;
}

bool CheckTileAvailable(Tile *t){
	if(t==NULL)
		return false;
	if(t->contents == NULL)
		return(t->free && !t->structure->solid);
	else
		return((t->free) && (!t->structure->solid) && (t->contents->passable));
}

bool CheckTileHeights(Tile *t1, Tile *t2){
	if((t1 == NULL)||(t2 == NULL))
		return false;
	return(abs(t1->structure->height - t2->structure->height) <= 1);
}

bool CheckContentsTalk(Tile *t){
	return((t!=NULL)&&(t->contents!=NULL)&&(t->contents->talks));
}

void ClearTile(Tile *t){
	t->free = false;
	/*t->height = 0;
	t->baseframe = 0;
	t->floorframe = 0;
	t->wallframe = 0;
	t->spritesheet = NULL;*/
	t->contents = NULL;
	t->structure = NULL;
}