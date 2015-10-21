#include "pfile.h"
#include "dialogue.h"

#include "player.h"


//Keeping all the rapidjson stuff together in case I decide to replace it


#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>

//using namespace rapidjson;

rapidjson::Document ParseFile(char* path);
Message *ParseDialogueCFG(Message *msg, const rapidjson::Value& dialogue_parse);
Sprite *ParseSprite(const rapidjson::Value& sprite_parse);
Animation *ParseAnimation(const rapidjson::Value& anim_parse);
CharData *ParseCharData(const rapidjson::Value& cd_parse);

void What();

void What(){ //sample rapidjson stuff
	//const char json[] = "{	\"hello\": \"world\", \"t\": true , \"f\": false, \"n\": null, \"i\": 123, \"pi\": 3.1416,\"a\": [ 1,2,3,4]}";
	
	FILE* pFile = fopen("testfiles/test.json", "rb");
	char buffer[65536];
	rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
	rapidjson::Document document;
	document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);


    assert(document.IsObject());
	
	/*


	assert(document["t"].IsBool());         // JSON true/false are bool. Can also uses more specific function IsTrue().
	printf("t = %s\n", document["t"].GetBool() ? "true" : "false");

	assert(document["f"].IsBool()); printf("f = %s\n", document["f"].GetBool() ? "true" : "false");

	printf("n = %s\n", document["n"].IsNull() ? "null" : "?");

	assert(document["i"].IsNumber());       // Number is a JSON type, but C++ needs more specific type.
	assert(document["i"].IsInt());          // In this case, IsUint()/IsInt64()/IsUInt64() also return true.
	printf("i = %d\n", document["i"].GetInt());     // Alternative (int)document["i"]

	assert(document["pi"].IsNumber());
	assert(document["pi"].IsDouble());
	printf("pi = %g\n", document["pi"].GetDouble());

	const Value& a = document["a"]; // Using a reference for consecutive access is handy and faster.
	assert(a.IsArray());
	for (SizeType i = 0; i < a.Size(); i++) // rapidjson uses SizeType instead of size_t.
        printf("a[%d] = %d\n", i, a[i].GetInt());*/
}

inline rapidjson::Document ParseFile(char* path){
	//sadly doesnt work?
	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
	rapidjson::Document document;
	document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
	assert(document.IsObject());
	fclose(pFile);
	return document;
}


Message *OpenDialogue(char *path){ // "testfiles/test.json"
	Message *msg = NULL;
	/////
	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
	rapidjson::Document dialogue;
	dialogue.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
	assert(dialogue.IsObject());
	fclose(pFile);


	msg = ParseDialogueCFG(msg,dialogue); 

	return msg;
}

NPC **LoadEntitiesCFG(char *path){
	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
	rapidjson::Document doc;
	doc.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
	assert(doc.IsObject());
	fclose(pFile);

	NPC *npclist[16];	//bad - replace with dynamic list 
	for(int i = 0; i < 16; i++)
		npclist[i] = NULL;

	Message *msg;
	int i = 0;
	for(rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr){
		int posx, posy;
		char name [64];

		if(strcmp(itr->name.GetString(),"info")==0)
			continue;
		else if(strcmp(itr->name.GetString(),"sprites")==0){
			const rapidjson::Value& spr_parse = doc[itr->name];
			for(rapidjson::Value::ConstMemberIterator itr2 = spr_parse.MemberBegin(); itr2 != spr_parse.MemberEnd(); ++itr2){
				ParseSprite(spr_parse[itr2->name]);
			}
		}else{

			const rapidjson::Value& npc_parse = doc[itr->name];
			copy_string(name,npc_parse["name"].GetString());
		
			posx = npc_parse["pos_x"].GetInt();
			posy = npc_parse["pos_y"].GetInt();
			npclist[i] = new NPC(posx,posy,name);

			if(npc_parse.HasMember("sprite")){
				Logger::Log("yo");
				char sprpath[40];
				copy_string(sprpath,npc_parse["sprite"].GetString());
				Sprite *spr = GetSprite(sprpath);
			//	printf("%i %i %i \n",spr->w,spr->h,spr->framesperline);
				npclist[i]->SetEntAnims(spr);
			}

			/* NPC dialogue can be stored within the NPC's structure or loaded from a separate json file*/
			if(npc_parse.HasMember("dialogue_file")){
				assert(npc_parse["dialogue_file"].IsString());
				copy_string(npclist[i]->dialoguepath,npc_parse["dialogue_file"].GetString());
				msg = OpenDialogue(npclist[i]->dialoguepath);
				GiveNPCMessage(npclist[i],msg);
			}
			else if(npc_parse.HasMember("dialogue")){
				const rapidjson::Value& dialogue_parse = npc_parse["dialogue"];
				msg = ParseDialogueCFG(msg, dialogue_parse);
				GiveNPCMessage(npclist[i],msg);
		
			}

			i++;
		}
	}

	return npclist;
}

/*
CombatEnt *LoadCombatEntCFG(char *path){
	CombatEnt *ent;
	int id;

	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));
	Document doc;
	doc.ParseStream<0, UTF8<>, FileReadStream>(is);
	assert(doc.IsObject());
	fclose(pFile);


	assert(doc["test-boss-egg"].IsObject());
	const Value& parsed_ent = doc["test-boss-egg"];

	assert(parsed_ent["id"].IsInt());
	ent = LoadCombatEnt(parsed_ent["id"].GetInt());
	
	assert(parsed_ent["name"].IsString());
	copy_string(ent->chardata->name,parsed_ent["name"].GetString());

	

	return ent;
}

vector<CombatEnt*> LoadEnemyDataCFG(char *path){

	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));
	Document doc;
	doc.ParseStream<0, UTF8<>, FileReadStream>(is);
	assert(doc.IsObject());
	fclose(pFile);

	vector<CombatEnt*> enemies;

	CharData *cd;
	int i = 0;
	for(rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr){
		CombatEnt *cent;
		Sprite *spr;
		Animation *anim;
		if(strcmp(itr->name.GetString(),"info")==0)
			continue;

		const rapidjson::Value& enemy_parse = doc[itr->name];

		cd = ParseCharData(enemy_parse);
		cent = LoadCombatEnt(cd->id);

		if(enemy_parse.HasMember("sprite")){
			spr = ParseSprite(enemy_parse["sprite"]);
		}

		if(enemy_parse.HasMember("animations")){
			const rapidjson::Value& anims = enemy_parse["animations"];
			int i = 0;
			for(rapidjson::Value::ConstMemberIterator itr = anims.MemberBegin(); itr != anims.MemberEnd(); ++itr){
				anim = ParseAnimation(itr->value);
				AddCombatEntAnim(cent,anim,i);
				++i;
			}
		}

		enemies.push_back(cent);
		i++;
	}

	return enemies;
}*/

CharData *ParseCharData(const rapidjson::Value& cd_parse){
	CharData *cd;
	assert(cd_parse["id"].IsInt());
	cd = LoadCharData(ID_ENEMY + cd_parse["id"].GetInt());

	assert(cd_parse["name"].IsString());
	copy_string(cd->name,cd_parse["name"].GetString());

	
	return cd;
}

Message *ParseDialogueCFG(Message *msg, const rapidjson::Value& dialogue_parse){
	msg = NewMessage();
	assert(dialogue_parse["text"].IsString());
	int len = dialogue_parse["text"].GetStringLength();
	if (len > MAX_PARAGRAPH_SIZE) //if the text we have is too big, abort mission
		return NULL;
	copy_string(msg->text,dialogue_parse["text"].GetString());
	if(dialogue_parse.HasMember("next")){
		const rapidjson::Value& next = dialogue_parse["next"];
		assert(next.IsObject());
		msg->next[0] = ParseDialogueCFG(msg->next[0],next);
	}
	else{
		if(dialogue_parse.HasMember("prompt")){
			char options[6][16];
			int optcount = 0;
			const rapidjson::Value& prompt = dialogue_parse["prompt"];
			assert(prompt.IsObject());
			if(prompt.HasMember("option1")){
				const rapidjson::Value& option1 = prompt["option1"];
				assert(option1.IsObject());
				if(option1.HasMember("answer")){
					copy_string(options[0],option1["answer"].GetString());
				}
				msg->next[0] = ParseDialogueCFG(msg->next[0],option1);
				optcount++;
			}
			if(prompt.HasMember("option2")){
				const rapidjson::Value& option2 = prompt["option2"];
				assert(option2.IsObject());
				if(option2.HasMember("answer")){
					copy_string(options[1],option2["answer"].GetString());
				}
				msg->next[1] = ParseDialogueCFG(msg->next[1],option2);
				optcount++;
			}
			if(prompt.HasMember("option3")){
				const rapidjson::Value& option3 = prompt["option3"];
				assert(option3.IsObject());
				if(option3.HasMember("answer")){
					copy_string(options[2],option3["answer"].GetString());
				}
				msg->next[2] = ParseDialogueCFG(msg->next[2],option3);
				optcount++;
			}
			if(prompt.HasMember("option4")){
				const rapidjson::Value& option4 = prompt["option4"];
				assert(option4.IsObject());
				if(option4.HasMember("answer")){
					copy_string(options[3],option4["answer"].GetString());
				}
				msg->next[3] = ParseDialogueCFG(msg->next[3],option4);
				optcount++;
			}
			if(prompt.HasMember("option5")){
				const rapidjson::Value& option5 = prompt["option5"];
				assert(option5.IsObject());
				if(option5.HasMember("answer")){
					copy_string(options[4],option5["answer"].GetString());
				}
				msg->next[4] = ParseDialogueCFG(msg->next[4],option5);
				optcount++;
			}
			if(prompt.HasMember("option6")){
				const rapidjson::Value& option6 = prompt["option6"];
				assert(option6.IsObject());
				if(option6.HasMember("answer")){
					copy_string(options[5],option6["answer"].GetString());
				}
				msg->next[5] = ParseDialogueCFG(msg->next[5],option6);
				optcount++;
			}
			msg->hasPrompt = true;
			msg->prompt = LoadCustomMenu(optcount,options);
			SetAnswers(msg,optcount);
			SetAnchor(msg->prompt,ANCHOR_TEXTBOX_RIGHT, 4);
		}
	}
	return msg;
}
/*
void LoadItemCFG(char *path){
	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));
	Document doc;
	doc.ParseStream<0, UTF8<>, FileReadStream>(is);
	assert(doc.IsObject());
	fclose(pFile);
	
	assert(doc["items"].IsObject());
	const Value& itemList = doc["items"];
	int i = 0;
	for(rapidjson::Value::ConstMemberIterator itr = itemList.MemberBegin(); itr != itemList.MemberEnd(); ++itr){
		int id = 0;
		char name[64];
		char desc[256];
		int icon = 0;
		int type = 0;
		Uint8 attributes = 0;
		int value = 0;

		const rapidjson::Value& item_parse = itemList[itr->name];
		assert(item_parse.IsObject());

		if(!item_parse.HasMember("itemID"))
			continue;
		else
			id = item_parse["itemID"].GetInt();

		if(item_parse.HasMember("name")){
			copy_string(name,item_parse["name"].GetString());
		}

		if(item_parse.HasMember("description")){
			copy_string(desc,item_parse["description"].GetString());
		}

		if(item_parse.HasMember("icon")){
			icon = item_parse["icon"].GetInt();
		}

		if(item_parse.HasMember("attributes")){
			const Value& attr_parse = item_parse["attributes"];
			assert(attr_parse.IsArray());
			for (SizeType i = 0; i < attr_parse.Size(); i++){
				if(strcmp(attr_parse[i].GetString(),"use")==0)
					attributes |= ITEM_USABLE;
				else if(strcmp(attr_parse[i].GetString(),"consume")==0)
					attributes |= ITEM_CONSUMABLE;
				else if(strcmp(attr_parse[i].GetString(),"equip")==0)
					attributes |= ITEM_EQUIPPABLE;
				else if(strcmp(attr_parse[i].GetString(),"drop")==0)
					attributes |= ITEM_DROPPABLE;
				else if(strcmp(attr_parse[i].GetString(),"sell")==0)
					attributes |= ITEM_SELLABLE;
				else if(strcmp(attr_parse[i].GetString(),"stack")==0)
					attributes |= ITEM_STACKABLE;
			}
		}

		if(item_parse.HasMember("sell_value")){
			value = item_parse["sell_value"].GetInt();
		}

	//	LoadItemData(id,name,desc,icon,type,attributes,value);
	}

}
*/

Sprite *ParseSprite(const rapidjson::Value& sprite_parse){
	char path[40];
	int tw,th,fpl,offx,offy;
	if(sprite_parse.HasMember("path")){
		copy_string(path,sprite_parse["path"].GetString());
	}
	if(sprite_parse.HasMember("tile_w")){
		tw = sprite_parse["tile_w"].GetInt();
	}
	if(sprite_parse.HasMember("tile_h")){
		th = sprite_parse["tile_h"].GetInt();
	}
	if(sprite_parse.HasMember("frames_per_line")){
		fpl = sprite_parse["frames_per_line"].GetInt();
	}
	if(sprite_parse.HasMember("offset_x")){
		offx = sprite_parse["offset_x"].GetInt();
	}
	if(sprite_parse.HasMember("offset_y")){
		offy = sprite_parse["offset_y"].GetInt();
	}
	return LoadSprite(path,tw,th,fpl,offx,offy);
}

Animation *ParseAnimation(const rapidjson::Value& anim_parse){
	Sprite *spr;
	char path[40];
	int startf,seed,length,delay;
	bool play,loop;

	if(anim_parse.HasMember("sprite")){
		copy_string(path,anim_parse["sprite"].GetString());	
		spr = GetSprite(path);
	}else
		spr = NULL;

	if(anim_parse.HasMember("start_frame")){
		startf = anim_parse["start_frame"].GetInt();
	}
	if(anim_parse.HasMember("seed")){
		seed = anim_parse["seed"].GetInt();
	}
	if(anim_parse.HasMember("length")){
		length = anim_parse["length"].GetInt();
	}
	if(anim_parse.HasMember("play")){
		play = anim_parse["play"].GetBool();
	}else
		play = 1;

	if(anim_parse.HasMember("loop")){
		loop = anim_parse["loop"].GetBool();
	}else
		loop = 1;

	if(anim_parse.HasMember("delay")){
		delay = anim_parse["delay"].GetInt();
	}else
		delay = 1;

	return LoadAnimation(spr,startf,seed,length,play,loop,delay);


}