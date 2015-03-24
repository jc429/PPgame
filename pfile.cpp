#include "pfile.h"


void What(){
	char *yamlpath = "cfg/test.yaml";

	FILE *fh = fopen(yamlpath, "r");
  yaml_parser_t parser;
  yaml_event_t  event;   /* New variable */

  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_file(&parser, fh);

  /* START new code */
  do {
    if (!yaml_parser_parse(&parser, &event)) {		///WHY DOES THIS BREAK
       printf("Parser error %d\n", parser.error);
       exit(EXIT_FAILURE);
    }

    switch(event.type)
    { 
    case YAML_NO_EVENT: puts("No event!"); break;
    /* Stream start/end */
    case YAML_STREAM_START_EVENT: puts("STREAM START"); break;
    case YAML_STREAM_END_EVENT:   puts("STREAM END");   break;
    /* Block delimeters */
    case YAML_DOCUMENT_START_EVENT: puts("<b>Start Document</b>"); break;
    case YAML_DOCUMENT_END_EVENT:   puts("<b>End Document</b>");   break;
    case YAML_SEQUENCE_START_EVENT: puts("<b>Start Sequence</b>"); break;
    case YAML_SEQUENCE_END_EVENT:   puts("<b>End Sequence</b>");   break;
    case YAML_MAPPING_START_EVENT:  puts("<b>Start Mapping</b>");  break;
    case YAML_MAPPING_END_EVENT:    puts("<b>End Mapping</b>");    break;
    /* Data */
    case YAML_ALIAS_EVENT:  printf("Got alias (anchor %s)\n", event.data.alias.anchor); break;
    case YAML_SCALAR_EVENT: printf("Got scalar (value %s)\n", event.data.scalar.value); break;
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  /* END new code */

  /* Cleanup */
  yaml_parser_delete(&parser);
  fclose(fh);

}

void TestFile(char *path){
	FILE *file;
	file = fopen(path, "r");
	yaml_parser_t parser;
	int result = yaml_parser_initialize(&parser);

	if(file==NULL) return;
	if(result != 1) return;

	yaml_parser_set_input_file(&parser, file);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	yaml_token_t  token;

	do {
		yaml_parser_scan(&parser, &token);
		switch(token.type){
			// Stream start/end 
			case YAML_STREAM_START_TOKEN: puts("STREAM START"); break;
			case YAML_STREAM_END_TOKEN:   puts("STREAM END");   break;
			// Token types (read before actual token) 
			case YAML_KEY_TOKEN:   printf("(Key token)   "); break;
			case YAML_VALUE_TOKEN: printf("(Value token) "); break;
			// Block delimeters 
			case YAML_BLOCK_SEQUENCE_START_TOKEN: puts("<b>Start Block (Sequence)</b>"); break;
			case YAML_BLOCK_ENTRY_TOKEN:          puts("<b>Start Block (Entry)</b>");    break;
			case YAML_BLOCK_END_TOKEN:            puts("<b>End block</b>");              break;
			// Data 
			case YAML_BLOCK_MAPPING_START_TOKEN:  puts("[Block mapping]");            break;
			case YAML_SCALAR_TOKEN:  printf("scalar %s \n", token.data.scalar.value); break;
			// Others 
			default:
			  printf("Got token of type %d\n", token.type);
		}
    if(token.type != YAML_STREAM_END_TOKEN)
      yaml_token_delete(&token);
  } while(token.type != YAML_STREAM_END_TOKEN);
  yaml_token_delete(&token);
//////////////////////////////////////////////////////////////////////////////////////////////////////

	yaml_parser_delete(&parser);
	fclose(file);
}