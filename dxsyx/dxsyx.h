//
//  dxsyx.h
//  dxsyx
//
//  Created by Roger Allen on 1/5/15.
//  Copyright (c) 2015 Roger Allen. All rights reserved.
//

#ifndef dxsyx_dxsyx_h
#define dxsyx_dxsyx_h

#include <iostream>
#include <array>
#include <cstdint>

// This only handles 32-voice packed DX7 .syx files
const int SYX_FILE_SIZE  = 4096 + 8;
const int SYX_NUM_VOICES = 32;
const int SYX_NUM_OSC    = 6;

class DxSyx;  // declare so Osc, Voice can use

// ======================================================================
// http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104

enum class DxSyxPrintMode { Names, Full };
class DxSyxConfig
{
public:
    static DxSyxConfig& get()
    {
        static DxSyxConfig instance;
        return instance;
    }
    DxSyxPrintMode print_mode = DxSyxPrintMode::Names;
    
private:
    DxSyxConfig() {}
    DxSyxConfig(DxSyxConfig const&);    // Don't Implement
    void operator=(DxSyxConfig const&); // Don't implement
    
};

// ======================================================================
class DxSyxOsc {
protected: // << operator needs to grab these
    uint8_t _osc_num;
    uint8_t syx_eg_rate_1;
    uint8_t syx_eg_rate_2;
    uint8_t syx_eg_rate_3;
    uint8_t syx_eg_rate_4;
    uint8_t syx_eg_level_1;
    uint8_t syx_eg_level_2;
    uint8_t syx_eg_level_3;
    uint8_t syx_eg_level_4;
    uint8_t syx_kbd_lev_scl_brk_pt;
    uint8_t syx_kbd_lev_scl_lft_depth;
    uint8_t syx_kbd_lev_scl_rht_depth;
    uint8_t syx_kbd_lev_scl_lft_curve;
    uint8_t syx_kbd_lev_scl_rht_curve;
    uint8_t syx_osc_detune;
    uint8_t syx_kbd_rate_scaling;
    uint8_t syx_key_vel_sensitivity;
    uint8_t syx_amp_mod_sensitivity;
    uint8_t syx_operator_output_level;
    uint8_t syx_osc_freq_coarse;
    uint8_t syx_osc_mode;
    uint8_t syx_osc_freq_fine;
    
public:
    DxSyxOsc() {} // default does nothing
    DxSyxOsc(const uint8_t osc_num, DxSyx &dx);
    
    friend std::ostream& operator<<(std::ostream& os, const DxSyxOsc& syx);
};

// ======================================================================
class DxSyxVoice {
protected: // << operator needs to grab these
    std::array<DxSyxOsc, SYX_NUM_OSC> syx_oscs;
    uint8_t  syx_pitch_eg_rate_1;
    uint8_t  syx_pitch_eg_rate_2;
    uint8_t  syx_pitch_eg_rate_3;
    uint8_t  syx_pitch_eg_rate_4;
    uint8_t  syx_pitch_eg_level_1;
    uint8_t  syx_pitch_eg_level_2;
    uint8_t  syx_pitch_eg_level_3;
    uint8_t  syx_pitch_eg_level_4;
    uint8_t  syx_algorithm_num;
    uint8_t  syx_feedback;
    uint8_t  syx_oscillator_sync;
    uint8_t  syx_lfo_speed;
    uint8_t  syx_lfo_delay;
    uint8_t  syx_lfo_pitch_mod_depth;
    uint8_t  syx_lfo_amp_mod_depth;
    uint8_t  syx_pitch_mod_sensitivity;
    uint8_t  syx_lfo_waveform;
    uint8_t  syx_lfo_sync;
    uint8_t  syx_transpose;
    char     syx_name[11];
    
    char FixChar(char c);
    
public:
    DxSyxVoice() {} // default does nothing
    DxSyxVoice(DxSyx &dx);
    
    friend std::ostream& operator<<(std::ostream& os, const DxSyxVoice& syx);
};

// ======================================================================
class DxSyx {
    std::string _filename;
    uint8_t     _data[SYX_FILE_SIZE];
    int         _cur_checksum;
    uint32_t    _cur_data_index;
       
    std::array<DxSyxVoice, SYX_NUM_VOICES> syx_voices;
    
    void    ReadFile(const char *filename);
    void    UnpackSyx();
    void    CheckHeader();
    void    UnpackVoices();
    void    UnpackVoice(int n);
    void    CheckCurrentSum();
    uint8_t GetData();
    
public:
    DxSyx(const char *filename);
    uint8_t GetDataCS();
    
    friend std::ostream& operator<<(std::ostream& os, const DxSyx& syx);
};

#endif
