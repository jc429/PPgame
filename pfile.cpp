#include "pfile.h"
#include "dialogue.h"
#include "combat_ent.h"
#include "player.h"


//Keeping all the rapidjson stuff together in case I decide to replace it

#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>

using namespace rapidjson;

Document ParseFile(char* path);


Message *ParseDialogueCFG(Message *msg, const rapidjson::Value& dialogue_parse);


void What(){
	//const char json[] = "{	\"hello\": \"world\", \"t\": true , \"f\": false, \"n\": null, \"i\": 123, \"pi\": 3.1416,\"a\": [ 1,2,3,4]}";
	
	FILE* pFile = fopen("testfiles/test.json", "rb");
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));
	Document document;
	document.ParseStream<0, UTF8<>, FileReadStream>(is);


    assert(document.IsObject());

	
	
	
	
	/*


	assert(document["t"].IsBool());         // JSON true/false are bool. Can also uses more specific function IsTrue().
	printf("t = %s\n", document["t"].GetBool() ? "true" : "false");

	assert(document["f"].IsBool());
	printf("f = %s\n", document["f"].GetBool() ? "true" : "false");

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
	FileReadStream is(pFile, buffer, sizeof(buffer));
	Document document;
	document.ParseStream<0, UTF8<>, FileReadStream>(is);
	assert(document.IsObject());
	fclose(pFile);
	return document;
}


Message *OpenDialogue(char *path){ // "testfiles/test.json"
	Message *msg = NULL;
	/////
	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));
	Document doc;
	doc.ParseStream<0, UTF8<>, FileReadStream>(is);
	assert(doc.IsObject());
	fclose(pFile);
	
	//////
	char text[255];
	char op1[255];
	char op2[255];
	//////
	assert(doc["testspeech"].IsObject());
	const Value& dialogue = doc["testdialogue"];
	if(dialogue.HasMember("text")){
		msg = NewMessage();
		strcpy(text,dialogue["text"].GetString());
		CreateMessage(msg,text);
		if(dialogue.HasMember("prompt")){
			const Value& prompt = dialogue["prompt"];
			if(prompt.HasMember("option1")){
				const Value& option1 = prompt["option1"];
				msg->next[0] = NewMessage();
				strcpy(op1,option1["text"].GetString());
				CreateMessage(msg->next[0],op1);
			}
			if(prompt.HasMember("option2")){
				const Value& option2 = prompt["option2"];
				msg->next[1] = NewMessage();
				strcpy(op2,option2["text"].GetString());
				CreateMessage(msg->next[1],op2);
			}
			if(prompt.HasMember("option3")){
			}
			msg->hasPrompt = true;
			msg->prompt = LoadMenuYesNo();
		} 
		else if(dialogue.HasMember("next")){
			const Value& next = dialogue["next"];
			msg->next[0] = NewMessage();
			if(next.HasMember("text")){
				strcpy(op1,next["text"].GetString());
			}
			CreateMessage(msg->next[0],op1);
		}
		
	}


	return msg;
}

NPC **LoadEntitiesCFG(char *path){
	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));
	Document doc;
	doc.ParseStream<0, UTF8<>, FileReadStream>(is);
	assert(doc.IsObject());
	fclose(pFile);

	NPC *npclist[16];
	for(int i = 0; i < 16; i++)
		npclist[i] = NULL;

	assert(doc["chunk1_npcs"].IsObject());
	const Value& npcs = doc["chunk1_npcs"];
	int i = 0;
	for(rapidjson::Value::ConstMemberIterator itr = npcs.MemberBegin(); itr != npcs.MemberEnd(); ++itr){
		int posx, posy;
		char name [64];
		const rapidjson::Value& npc_parse = npcs[itr->name];
		strcpy(name,npc_parse["name"].GetString());
		
		posx = npc_parse["pos_x"].GetInt();
		posy = npc_parse["pos_y"].GetInt();
		npclist[i] = new NPC(posx,posy,name);

		/* NPC dialogue can be stored within the NPC's structure or loaded from a separate json file*/
		if(npc_parse.HasMember("dialogue_file")){
			assert(npc_parse["dialogue_file"].IsString());
			strcpy(npclist[i]->dialoguepath,npc_parse["dialogue_file"].GetString());
			Message *msg = OpenDialogue(npclist[i]->dialoguepath);
			GiveNPCMessage(npclist[i],msg);
		}
		else if(npc_parse.HasMember("dialogue")){
			const rapidjson::Value& dialogue_parse = npc_parse["dialogue"];
			Message *msg;
			msg = ParseDialogueCFG(msg, dialogue_parse);
			GiveNPCMessage(npclist[i],msg);
		
		}

		i++;
	}

	return npclist;
}


CombatEnt *LoadCombatEntCFG(char *path){
	CombatEnt *ent = LoadCombatEnt();

	FILE* pFile = fopen(path, "rb");
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));
	Document doc;
	doc.ParseStream<0, UTF8<>, FileReadStream>(is);
	assert(doc.IsObject());
	fclose(pFile);

	assert(doc["test-enemy-egg"].IsObject());
	const Value& parsed_ent = doc["test-enemy-egg"];
	
	assert(parsed_ent["name"].IsString());
	ent->name = new char[32];
	strcpy(ent->name,parsed_ent["name"].GetString());

	return ent;
}

Message *ParseDialogueCFG(Message *msg, const rapidjson::Value& dialogue_parse){
	msg = NewMessage();
	assert(dialogue_parse["text"].IsString());
	strcpy(msg->text,dialogue_parse["text"].GetString());
	if(dialogue_parse.HasMember("next")){
		const rapidjson::Value& next = dialogue_parse["next"];
		assert(next.IsObject());
		msg->next[0] = ParseDialogueCFG(msg->next[0],next);
	}
	else{
		if(dialogue_parse.HasMember("prompt")){
			char options[6][16];
			const Value& prompt = dialogue_parse["prompt"];
			assert(prompt.IsObject());
			if(prompt.HasMember("option1")){
				const Value& option1 = prompt["option1"];
				assert(option1.IsObject());
				if(option1.HasMember("answer")){
					strcpy(options[0],option1["answer"].GetString());
				}
				msg->next[0] = ParseDialogueCFG(msg->next[0],option1);
			}
			if(prompt.HasMember("option2")){
				const Value& option2 = prompt["option2"];
				assert(option2.IsObject());
				if(option2.HasMember("answer")){
					strcpy(options[1],option2["answer"].GetString());
				}
				msg->next[1] = ParseDialogueCFG(msg->next[1],option2);
			}
			if(prompt.HasMember("option3")){
				const Value& option3 = prompt["option3"];
				assert(option3.IsObject());
				msg->next[2] = ParseDialogueCFG(msg->next[2],option3);
			}
			if(prompt.HasMember("option4")){
				const Value& option4 = prompt["option4"];
				assert(option4.IsObject());
				msg->next[3] = ParseDialogueCFG(msg->next[3],option4);
			}
			if(prompt.HasMember("option5")){
				const Value& option5 = prompt["option5"];
				assert(option5.IsObject());
				msg->next[4] = ParseDialogueCFG(msg->next[4],option5);
			}
			if(prompt.HasMember("option6")){
				const Value& option6 = prompt["option6"];
				assert(option6.IsObject());
				msg->next[5] = ParseDialogueCFG(msg->next[5],option6);
			}
			msg->hasPrompt = true;
			msg->prompt = LoadCustomMenu(2,options[0],options[1]);
		}
	}
	return msg;
}
