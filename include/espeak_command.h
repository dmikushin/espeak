#ifndef ESPEAK_COMMAND_H
#define ESPEAK_COMMAND_H

#ifndef PLATFORM_WINDOWS
#include <unistd.h>
#endif
#include "speak_lib.h"

enum t_espeak_type
  {
    ET_TEXT,
    ET_MARK,
    ET_KEY,
    ET_CHAR,
    ET_PARAMETER,
    ET_PUNCTUATION_LIST,
    ET_VOICE_NAME,
    ET_VOICE_SPEC,
    ET_TERMINATED_MSG
  };

typedef struct 
{
  uint32_t unique_identifier;
  void* text;
  size_t size;
  uint32_t position;
  espeak_POSITION_TYPE position_type;
  uint32_t end_position;
  uint32_t flags;
  void* user_data;
} t_espeak_text;

typedef struct 
{
  uint32_t unique_identifier;
  void* text;
  size_t size;
  const char* index_mark;
  uint32_t end_position;
  uint32_t flags;
  void* user_data;
} t_espeak_mark;

typedef struct 
{
	uint32_t unique_identifier;
	void* user_data;
	wchar_t character; 
} t_espeak_character;

typedef struct 
{
	uint32_t unique_identifier;
	void* user_data;
	const char* key_name; 
} t_espeak_key;


typedef struct 
{
  uint32_t unique_identifier;
  void* user_data;
} t_espeak_terminated_msg;


typedef struct 
{
  espeak_PARAMETER parameter; 
  int32_t value; 
  int32_t relative;
} t_espeak_parameter;

enum t_command_state 
{
  CS_UNDEFINED, // The command has just been created
  CS_PENDING, // stored in the fifo
  CS_PROCESSED // processed
};

typedef struct 
{
  enum t_espeak_type type;
  enum t_command_state state; 

  union command
  {
    t_espeak_text my_text; 
    t_espeak_mark my_mark; 
    t_espeak_key  my_key; 
    t_espeak_character my_char; 
    t_espeak_parameter my_param;
    const wchar_t* my_punctuation_list;
    const char *my_voice_name;
    espeak_VOICE my_voice_spec;
    t_espeak_terminated_msg my_terminated_msg;
  } u;
} t_espeak_command;


t_espeak_command* create_espeak_text(const void *text, size_t size, uint32_t position, espeak_POSITION_TYPE position_type, uint32_t end_position, uint32_t flags, void* user_data);

t_espeak_command* create_espeak_mark(const void *text, size_t size, const char *index_mark, uint32_t end_position, uint32_t flags, void* user_data);

t_espeak_command* create_espeak_terminated_msg(uint32_t unique_identifier, void* user_data);

t_espeak_command* create_espeak_key(const char *key_name, void *user_data);

t_espeak_command* create_espeak_char(wchar_t character, void *user_data);

t_espeak_command* create_espeak_parameter(espeak_PARAMETER parameter, int32_t value, int32_t relative);

t_espeak_command* create_espeak_punctuation_list(const wchar_t *punctlist);

t_espeak_command* create_espeak_voice_name(const char *name);

t_espeak_command* create_espeak_voice_spec(espeak_VOICE *voice_spec);

void process_espeak_command( t_espeak_command* the_command);

int32_t delete_espeak_command( t_espeak_command* the_command);

void display_espeak_command(t_espeak_command* the_command);


espeak_ERROR sync_espeak_Synth(uint32_t unique_identifier, const void *text, size_t size, 
		      uint32_t position, espeak_POSITION_TYPE position_type, 
		      uint32_t end_position, uint32_t flags, void* user_data);
espeak_ERROR sync_espeak_Synth_Mark(uint32_t unique_identifier, const void *text, size_t size, 
			   const char *index_mark, uint32_t end_position, 
			   uint32_t flags, void* user_data);
void sync_espeak_Key(const char *key);
void sync_espeak_Char(wchar_t character);
void sync_espeak_SetPunctuationList(const wchar_t *punctlist);
void sync_espeak_SetParameter(espeak_PARAMETER parameter, int32_t value, int32_t relative);
int32_t sync_espeak_SetVoiceByName(const char *name);
int32_t sync_espeak_SetVoiceByProperties(espeak_VOICE *voice_selector);
espeak_ERROR SetVoiceByName(const char *name);
espeak_ERROR SetVoiceByProperties(espeak_VOICE *voice_selector);
void SetParameter(int32_t parameter, int32_t value, int32_t relative);

int32_t sync_espeak_terminated_msg(uint32_t unique_identifier, void* user_data);

//>
#endif
