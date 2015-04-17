#include <stdio.h>
#include <string.h>

//pseudocode - doesnt work (yet)

typedef struct SaveData_T{
	// NO POINTERS GO IN HERE
	int test_int;
	char test_string[50];
}SaveData;

void Create_Save(void *data){
	SaveData sd;
	//sd.test_int = data->i;
	//strcpy(sd.test_string,data->str,50);
}

void Save_Data(char *filepath, SaveData *data){
	FILE *savefile;
	savefile = fopen(filepath,"wb");
	fwrite(data,sizeof(SaveData),1,savefile);
	fclose(savefile);
}

SaveData *Save_Data(char *filepath){
	FILE *loadfile;
	SaveData *data;
	loadfile = fopen(filepath,"rb");
	fread(data,sizeof(SaveData),1,loadfile);
	fclose(loadfile);
	return data;
}