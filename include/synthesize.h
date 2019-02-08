/***************************************************************************
 *   Copyright (C) 2005 to 2014 by Jonathan Duddington                     *
 *   email: jonsd@users.sourceforge.net                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write see:                           *
 *               <http://www.gnu.org/licenses/>.                           *
 ***************************************************************************/

#define espeakINITIALIZE_PHONEME_IPA   0x0002   // move this to speak_lib.h, after eSpeak version 1.46.02


#define N_PHONEME_LIST  1000    // enough for source[N_TR_SOURCE] full of text, else it will truncate

#define MAX_HARMONIC  400           // 400 * 50Hz = 20 kHz, more than enough
#define N_SEQ_FRAMES   25           // max frames in a spectrum sequence (real max is ablut 8)
#define STEPSIZE  64                // 2.9mS at 22 kHz sample rate

// flags set for frames within a spectrum sequence
#define FRFLAG_KLATT           0x01   // this frame includes extra data for Klatt synthesizer
#define FRFLAG_VOWEL_CENTRE    0x02   // centre pointof vowel
#define FRFLAG_LEN_MOD         0x04   // reduce effect of length adjustment
#define FRFLAG_BREAK_LF        0x08   // but keep f3 upwards
#define FRFLAG_BREAK           0x10   // don't merge with next frame
#define FRFLAG_BREAK_2         0x18   // FRFLAG_BREAK_LF or FRFLAG_BREAK
#define FRFLAG_FORMANT_RATE    0x20   // Flag5 allow increased rate of change of formant freq
#define FRFLAG_MODULATE        0x40   // Flag6 modulate amplitude of some cycles to give trill
#define FRFLAG_DEFER_WAV       0x80   // Flag7 defer mixing WAV until the next frame
#define FRFLAG_LEN_MOD2      0x4000   // reduce effect of length adjustment, used for the start of a vowel
#define FRFLAG_COPIED        0x8000   // This frame has been copied into temporary rw memory

#define SFLAG_SEQCONTINUE      0x01   // a liquid or nasal after a vowel, but not followed by a vowel
#define SFLAG_EMBEDDED         0x02   // there are embedded commands before this phoneme
#define SFLAG_SYLLABLE         0x04   // vowel or syllabic consonant
#define SFLAG_LENGTHEN         0x08   // lengthen symbol : included after this phoneme
#define SFLAG_DICTIONARY       0x10   // the pronunciation of this word was listed in the xx_list dictionary
#define SFLAG_SWITCHED_LANG    0x20   // this word uses phonemes from a different language
#define SFLAG_PROMOTE_STRESS   0x40   // this unstressed word can be promoted to stressed

#define SFLAG_PREV_PAUSE     0x1000   // consider previous phoneme as pause
#define SFLAG_NEXT_PAUSE     0x2000   // consider next phoneme as pause

// embedded command numbers
#define EMBED_P     1   // pitch
#define EMBED_S     2   // speed (used in setlengths)
#define EMBED_A     3   // amplitude/volume
#define EMBED_R     4   // pitch range/expression
#define EMBED_H     5   // echo/reverberation
#define EMBED_T     6   // different tone for announcing punctuation (not used)
#define EMBED_I     7   // sound icon
#define EMBED_S2    8   // speed (used in synthesize)
#define EMBED_Y     9   // say-as commands
#define EMBED_M    10   // mark name
#define EMBED_U    11   // audio uri
#define EMBED_B    12   // break
#define EMBED_F    13   // emphasis
#define EMBED_C    14   // capital letter indication

#define N_EMBEDDED_VALUES    15
extern int32_t embedded_value[N_EMBEDDED_VALUES];
extern int32_t embedded_default[N_EMBEDDED_VALUES];


#define N_PEAKS   9
#define N_PEAKS2  9     // plus Notch and Fill (not yet implemented)
#define N_MARKERS 8

#define N_KLATTP   10   // this affects the phoneme data file format
#define N_KLATTP2  14   // used in vowel files, with extra parameters for future extensions

#define KLATT_AV      0
#define KLATT_FNZ     1    // nasal zero freq
#define KLATT_Tilt    2
#define KLATT_Aspr    3
#define KLATT_Skew    4

#define KLATT_Kopen   5
#define KLATT_AVp     6
#define KLATT_Fric    7
#define KLATT_FricBP  8
#define KLATT_Turb    9



typedef struct {  // 64 bytes
	short frflags;
	short ffreq[7];
	unsigned char length;
	unsigned char rms;
	unsigned char fheight[8];
	unsigned char fwidth[6];          // width/4  f0-5
	unsigned char fright[3];          // width/4  f0-2
	unsigned char bw[4];        // Klatt bandwidth BNZ /2, f1,f2,f3
	unsigned char klattp[5];    // AV, FNZ, Tilt, Aspr, Skew
	unsigned char klattp2[5];   // continuation of klattp[],  Avp, Fric, FricBP, Turb
	unsigned char klatt_ap[7];  // Klatt parallel amplitude
	unsigned char klatt_bp[7];  // Klatt parallel bandwidth  /2
	unsigned char spare;        // pad to multiple of 4 bytes
} frame_t;   //  with extra Klatt parameters for parallel resonators


typedef struct {  // 44 bytes
	short frflags;
	short ffreq[7];
	unsigned char length;
	unsigned char rms;
	unsigned char fheight[8];
	unsigned char fwidth[6];          // width/4  f0-5
	unsigned char fright[3];          // width/4  f0-2
	unsigned char bw[4];        // Klatt bandwidth BNZ /2, f1,f2,f3
	unsigned char klattp[5];    // AV, FNZ, Tilt, Aspr, Skew
} frame_t2;   //  without the extra Klatt parameters



// formant data used by wavegen
typedef struct {
	int32_t freq;     // Hz<<16
	int32_t height;   // height<<15
	int32_t left;     // Hz<<16
	int32_t right;    // Hz<<16
	DOUBLEX freq1; // floating pointversions of the above
	DOUBLEX height1;
	DOUBLEX left1;
	DOUBLEX right1;
	DOUBLEX freq_inc;    // increment by this every 64 samples
	DOUBLEX height_inc;
	DOUBLEX left_inc;
	DOUBLEX right_inc;
}  wavegen_peaks_t;

typedef struct {
unsigned char *pitch_env;
int32_t pitch;          // pitch Hz*256
int32_t pitch_ix;       // index into pitch envelope (*256)
int32_t pitch_inc;      // increment to pitch_ix
int32_t pitch_base;     // Hz*256 low, before modified by envelope
int32_t pitch_range;    // Hz*256 range of envelope

unsigned char *mix_wavefile;  // wave file to be added to synthesis
int32_t n_mix_wavefile;       // length in bytes
int32_t mix_wave_scale;         // 0=2 byte samples
int32_t mix_wave_amp;
int32_t mix_wavefile_ix;
int32_t mix_wavefile_max;    // length of available WAV data (in bytes)
int32_t mix_wavefile_offset;

int32_t amplitude;
int32_t amplitude_v;
int32_t amplitude_fmt;   // percentage amplitude adjustment for formant synthesis
} WGEN_DATA;


typedef struct {
	double a;
	double b;
	double c;
	double x1;
	double x2;
}  RESONATOR;


typedef struct {
   short length_total;  // not used
   unsigned char  n_frames;
   unsigned char  sqflags;
   frame_t2  frame[N_SEQ_FRAMES];     // max. frames in a spectrum sequence
} SPECT_SEQ;   // sequence of espeak formant frames

typedef struct {
   short length_total;  // not used
   unsigned char  n_frames;
   unsigned char  sqflags;
   frame_t  frame[N_SEQ_FRAMES];     // max. frames in a spectrum sequence
} SPECT_SEQK;   // sequence of klatt formants frames


typedef struct {
	short length;
	short frflags;
	frame_t *frame;
} frameref_t;

// a clause translated into phoneme codes (first stage)
typedef struct {
	unsigned short synthflags;  // NOTE Put shorts on 32bit boundaries, because of RISC OS compiler bug?
	unsigned char phcode;
	unsigned char stresslevel;
	unsigned short sourceix;  // ix into the original source text string, only set at the start of a word
	unsigned char wordstress;  // the highest level stress in this word
	unsigned char tone_ph;    // tone phoneme to use with this vowel
} PHONEME_LIST2;


typedef struct {
// The first section is a copy of PHONEME_LIST2
	unsigned short synthflags;
	unsigned char phcode;
	unsigned char stresslevel;
	unsigned short sourceix;  // ix into the original source text string, only set at the start of a word
	unsigned char wordstress;  // the highest level stress in this word
	unsigned char tone_ph;    // tone phoneme to use with this vowel

	PHONEME_TAB *ph;
	uint32_t length;  // length_mod
	unsigned char env;    // pitch envelope number
	unsigned char type;
	unsigned char prepause;
	unsigned char postpause;
	unsigned char amp;
	unsigned char newword;   // bit 0=start of word, bit 1=end of clause, bit 2=start of sentence
	unsigned char pitch1;
	unsigned char pitch2;
#ifdef _ESPEAKEDIT
	unsigned char std_length;
	uint32_t phontab_addr;
	int32_t sound_param;
#endif
} PHONEME_LIST;


#define pd_FMT    0
#define pd_WAV    1
#define pd_VWLSTART 2
#define pd_VWLEND 3
#define pd_ADDWAV 4

#define N_PHONEME_DATA_PARAM 16
#define pd_INSERTPHONEME   i_INSERT_PHONEME
#define pd_APPENDPHONEME   i_APPEND_PHONEME
#define pd_CHANGEPHONEME   i_CHANGE_PHONEME
#define pd_CHANGE_NEXTPHONEME  i_REPLACE_NEXT_PHONEME
#define pd_LENGTHMOD       i_SET_LENGTH

#define pd_FORNEXTPH     0x2
#define pd_DONTLENGTHEN  0x4
#define pd_REDUCELENGTHCHANGE 0x8
typedef struct {
	int32_t pd_control;
	int32_t pd_param[N_PHONEME_DATA_PARAM];  // set from group 0 instructions
	int32_t sound_addr[5];
	int32_t sound_param[5];
	int32_t vowel_transition[4];
	int32_t pitch_env;
	int32_t amp_env;
	char ipa_string[18];
} PHONEME_DATA;


typedef struct {
	int32_t fmt_control;
	int32_t use_vowelin;
	int32_t fmt_addr;
	int32_t fmt_length;
	int32_t fmt_amp;
	int32_t fmt2_addr;
	int32_t fmt2_lenadj;
	int32_t wav_addr;
	int32_t wav_amp;
	int32_t transition0;
	int32_t transition1;
	int32_t std_length;
} FMT_PARAMS;

typedef struct {
    PHONEME_LIST prev_vowel;
} WORD_PH_DATA;

// instructions

#define i_RETURN        0x0001
#define i_CONTINUE      0x0002
#define i_NOT           0x0003

// Group 0 instrcutions with 8 bit operand.  These values go into bits 8-15 of the instruction
#define i_CHANGE_PHONEME 0x01
#define i_REPLACE_NEXT_PHONEME 0x02
#define i_INSERT_PHONEME 0x03
#define i_APPEND_PHONEME 0x04
#define i_APPEND_IFNEXTVOWEL 0x05
#define i_VOICING_SWITCH 0x06
#define i_PAUSE_BEFORE   0x07
#define i_PAUSE_AFTER    0x08
#define i_LENGTH_MOD     0x09
#define i_SET_LENGTH     0x0a
#define i_LONG_LENGTH    0x0b
#define i_CHANGE_PHONEME2 0x0c  // not yet used
#define i_IPA_NAME       0x0d

#define i_CHANGE_IF      0x10  // 0x10 to 0x14

#define i_ADD_LENGTH     0x0c


// conditions and jumps
#define i_CONDITION  0x2000
#define i_OR         0x1000  // added to i_CONDITION

#define i_JUMP       0x6000
#define i_JUMP_FALSE 0x6800
#define i_SWITCH_NEXTVOWEL 0x6a00
#define i_SWITCH_PREVVOWEL 0x6c00
#define MAX_JUMP     255  // max jump distance

// multi-word instructions
#define i_CALLPH     0x9100
#define i_PITCHENV   0x9200
#define i_AMPENV     0x9300
#define i_VOWELIN    0xa100
#define i_VOWELOUT   0xa200
#define i_FMT        0xb000
#define i_WAV        0xc000
#define i_VWLSTART   0xd000
#define i_VWLENDING  0xe000
#define i_WAVADD     0xf000

// conditions
#define i_isDiminished 0x80
#define i_isUnstressed 0x81
#define i_isNotStressed 0x82
#define i_isStressed   0x83
#define i_isMaxStress  0x84

#define i_isBreak      0x85
#define i_isWordStart  0x86
#define i_notWordStart 0x87
#define i_isWordEnd    0x88
#define i_isAfterStress 0x89
#define i_isNotVowel   0x8a
#define i_isFinalVowel 0x8b
#define i_isVoiced     0x8c
#define i_isFirstVowel 0x8d
#define i_isSecondVowel 0x8e
#define i_isSeqFlag1   0x8f
#define i_IsTranslationGiven 0x90


// place of articulation
#define i_isVel      0x28

// phflags
#define i_isSibilant   0x45    // bit 5 in phflags
#define i_isPalatal    0x49    // bit 9 in phflags
#define i_isLong       0x55    // bit 21 in phflags
#define i_isRhotic     0x57    // bit 23 in phflags
#define i_isFlag1      0x5c
#define i_isFlag2      0x5d
#define i_isFlag3      0x5e

#define i_StressLevel  0x800



typedef struct {
	int32_t name;
	int32_t length;
	char *data;
	char *filename;
} SOUND_ICON;

typedef struct {
	int32_t name;
	uint32_t  next_phoneme;
	int32_t mbr_name;
	int32_t mbr_name2;
	int32_t percent;         // percentage length of first component
	int32_t control;
} MBROLA_TAB;

typedef struct {
	int32_t pause_factor;
	int32_t clause_pause_factor;
	uint32_t min_pause;
	int32_t wav_factor;
	int32_t lenmod_factor;
	int32_t lenmod2_factor;
	int32_t min_sample_len;
	int32_t loud_consonants;
	int32_t fast_settings[8];
} SPEED_FACTORS;


typedef struct {
	char name[12];
	unsigned char flags[4];
	signed char head_extend[8];

	unsigned char prehead_start;
	unsigned char prehead_end;
	unsigned char stressed_env;
	unsigned char stressed_drop;
	unsigned char secondary_drop;
	unsigned char unstressed_shape;

	unsigned char onset;
	unsigned char head_start;
	unsigned char head_end;
	unsigned char head_last;

	unsigned char head_max_steps;
	unsigned char n_head_extend;

	signed char unstr_start[3];    // for: onset, head, last
	signed char unstr_end[3];

	unsigned char nucleus0_env;     // pitch envelope, tonic syllable is at end, no tail
	unsigned char nucleus0_max;
	unsigned char nucleus0_min;

	unsigned char nucleus1_env;     //     when followed by a tail
	unsigned char nucleus1_max;
	unsigned char nucleus1_min;
	unsigned char tail_start;
	unsigned char tail_end;

	unsigned char split_nucleus_env;
	unsigned char split_nucleus_max;
	unsigned char split_nucleus_min;
	unsigned char split_tail_start;
	unsigned char split_tail_end;
	unsigned char split_tune;

	unsigned char spare[8];
	int32_t spare2;       // the struct length should be a multiple of 4 bytes
} TUNE;

extern int32_t n_tunes;
extern TUNE *tunes;

// phoneme table
extern PHONEME_TAB *phoneme_tab[N_PHONEME_TAB];

// list of phonemes in a clause
extern int32_t n_phoneme_list;
extern PHONEME_LIST phoneme_list[N_PHONEME_LIST+1];
extern uint32_t embedded_list[];

extern unsigned char env_fall[128];
extern unsigned char env_rise[128];
extern unsigned char env_frise[128];

#define MAX_PITCH_VALUE  101
extern unsigned char pitch_adjust_tab[MAX_PITCH_VALUE+1];

// queue of commands for wavegen
#define WCMD_KLATT	1
#define WCMD_KLATT2	2
#define WCMD_SPECT	3
#define WCMD_SPECT2	4
#define WCMD_PAUSE	5
#define WCMD_WAVE    6
#define WCMD_WAVE2   7
#define WCMD_AMPLITUDE 8
#define WCMD_PITCH	9
#define WCMD_MARKER	10
#define WCMD_VOICE   11
#define WCMD_EMBEDDED 12
#define WCMD_MBROLA_DATA 13
#define WCMD_FMT_AMPLITUDE 14
#define WCMD_SONIC_SPEED 15



#define N_WCMDQ   170
#define MIN_WCMDQ  25   // need this many free entries before adding new phoneme

extern long64 wcmdq[N_WCMDQ][4];
extern int32_t wcmdq_head;
extern int32_t wcmdq_tail;

// from Wavegen file
int32_t WcmdqFree();
void WcmdqStop();
int32_t WcmdqUsed();
void WcmdqInc();
int32_t WavegenOpenSound();
int32_t WavegenCloseSound();
int32_t WavegenInitSound();
void WavegenInit(int32_t rate, int32_t wavemult_fact);
float polint(float xa[],float ya[],int32_t n,float x);
int32_t WavegenFill(int32_t fill_zeros);
void MarkerEvent(int32_t type, uint32_t char_position, int32_t value, int32_t value2, unsigned char *out_ptr);


extern unsigned char *wavefile_data;
extern int32_t samplerate;
extern int32_t samplerate_native;

extern int32_t wavefile_ix;
extern int32_t wavefile_amp;
extern int32_t wavefile_ix2;
extern int32_t wavefile_amp2;
extern int32_t vowel_transition[4];
extern int32_t vowel_transition0, vowel_transition1;

#define N_ECHO_BUF 5500   // max of 250mS at 22050 Hz
extern int32_t echo_head;
extern int32_t echo_tail;
extern int32_t echo_amp;
extern short echo_buf[N_ECHO_BUF];

extern int32_t mbrola_delay;
extern char mbrola_name[20];

// from synthdata file
uint32_t LookupSound(PHONEME_TAB *ph1, PHONEME_TAB *ph2, int32_t which, int32_t *match_level, int32_t control);
frameref_t *LookupSpect(PHONEME_TAB *this_ph, int32_t which, FMT_PARAMS *fmt_params,  int32_t *n_frames, PHONEME_LIST *plist);

unsigned char *LookupEnvelope(int32_t ix);
int32_t LoadPhData(int32_t *srate);

void SynthesizeInit(void);
int32_t Generate(PHONEME_LIST *phoneme_list, int32_t *n_ph, int32_t resume);
void MakeWave2(PHONEME_LIST *p, int32_t n_ph);
int32_t SynthOnTimer(void);
int32_t SpeakNextClause(FILE *f_text, const void *text_in, int32_t control);
int32_t SynthStatus(void);
void SetSpeed(int32_t control);
void SetEmbedded(int32_t control, int32_t value);
void SelectPhonemeTable(int32_t number);
int32_t SelectPhonemeTableName(const char *name);

void Write4Bytes(FILE *f, int32_t value);
int32_t Read4Bytes(FILE *f);
int32_t Reverse4Bytes(int32_t word);
int32_t CompileDictionary(const char *dsource, const char *dict_name, FILE *log, char *err_name,int32_t flags);


#define ENV_LEN  128    // length of pitch envelopes
#define    PITCHfall   0  // standard pitch envelopes
#define    PITCHrise   2
#define N_ENVELOPE_DATA   20
extern unsigned char *envelope_data[N_ENVELOPE_DATA];

extern int32_t formant_rate[];         // max rate of change of each formant
extern SPEED_FACTORS speed;

extern long count_samples;
extern int32_t outbuf_size;
extern unsigned char *out_ptr;
extern unsigned char *out_start;
extern unsigned char *out_end;
extern int32_t event_list_ix;
extern espeak_EVENT *event_list;
extern t_espeak_callback* synth_callback;
extern int32_t option_log_frames;
extern const char *version_string;
extern const int32_t version_phdata;
extern double sonicSpeed;

#define N_SOUNDICON_TAB  80   // total entries in soundicon_tab
#define N_SOUNDICON_SLOTS 4    // number of slots reserved for dynamic loading of audio files
extern int32_t n_soundicon_tab;
extern SOUND_ICON soundicon_tab[N_SOUNDICON_TAB];

espeak_ERROR SetVoiceByName(const char *name);
espeak_ERROR SetVoiceByProperties(espeak_VOICE *voice_selector);
espeak_ERROR LoadMbrolaTable(const char *mbrola_voice, const char *phtrans, int32_t srate);
void SetParameter(int32_t parameter, int32_t value, int32_t relative);
int32_t MbrolaTranslate(PHONEME_LIST *plist, int32_t n_phonemes, int32_t resume, FILE *f_mbrola);
int32_t MbrolaGenerate(PHONEME_LIST *phoneme_list, int32_t *n_ph, int32_t resume);
int32_t MbrolaFill(int32_t length, int32_t resume, int32_t amplitude);
void MbrolaReset(void);
void DoEmbedded(int32_t *embix, int32_t sourceix);
void DoMarker(int32_t type, int32_t char_posn, int32_t length, int32_t value);
void DoPhonemeMarker(int32_t type, int32_t char_posn, int32_t length, char *name);
int32_t DoSample3(PHONEME_DATA *phdata, int32_t length_mod, int32_t amp);
int32_t DoSpect2(PHONEME_TAB *this_ph, int32_t which, FMT_PARAMS *fmt_params,  PHONEME_LIST *plist, int32_t modulation);
int32_t PauseLength(int32_t pause, int32_t control);
int32_t LookupPhonemeTable(const char *name);
unsigned char *GetEnvelope(int32_t index);
int32_t NumInstnWords(USHORT *prog);

void InitBreath(void);

void KlattInit();
void KlattReset(int32_t control);
int32_t Wavegen_Klatt2(int32_t length, int32_t modulation, int32_t resume, frame_t *fr1, frame_t *fr2);

