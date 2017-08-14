//
//  dxsyx.h
//  dxsyx
//
// Created by Roger Allen on 1/5/15.
// Copyright (c) 2015 Roger Allen. All rights reserved.
//
// Dxsyx is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Dxsyx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Dxsyx.  If not, see <http://www.gnu.org/licenses/>.

#ifndef dxsyx_dxsyx_h
#define dxsyx_dxsyx_h

#include <array>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <vector>

// This only handles 32-voice packed DX7 .syx files
const int SYX_FILE_SIZE  = 4096 + 8;
const int SYX_VOICE_SIZE = 4096/32;
const int SYX_NUM_VOICES = 32;
const int SYX_NUM_OSC    = 6;

class DxSyx;  // declare so Osc, Voice can use

// ======================================================================
// http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104

enum class DxSyxOutputMode { Breed, Full, Names, NamesCrc, Syx };
class DxSyxConfig
{
public:
    static DxSyxConfig& get()
    {
        static DxSyxConfig instance;
        return instance;
    }
    std::string config_filename;
    std::string output_filename;
    DxSyxOutputMode print_mode = DxSyxOutputMode::Names;
    bool ignoreChecksum = false;

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

    void    Initialize(const std::string &filename);
    void    ReadFile(const std::string &filename);
    void    UnpackSyx();
    void    CheckHeader();
    void    UnpackVoices();
    void    UnpackVoice(int n);
    void    CheckCurrentSum();
    uint8_t GetData();

public:
    DxSyx(const char *filename);
    DxSyx(const std::string &filename);
    uint8_t              GetDataCS();
    std::vector<uint8_t> GetVoiceData(int n);
    std::string          GetFilename();

    friend std::ostream& operator<<(std::ostream& os, DxSyx& syx);
};

// ======================================================================
class DxSyxDB {
    std::vector<DxSyx>       _syxs;
    std::vector<std::string> _config_file_lines;

    void                     ReadConfigFile();
    void                     WriteSyxFile(const uint8_t *data);
    std::tuple<int, int>     DecodeConfigLine(const std::string &line);
    std::string              GetConfigLineFilename(const std::string &filename);
    std::vector<uint8_t>     GetVoiceData(const int voice_num, const int syx_num);
    int                      FilenameIndex(const std::string filename);
    std::tuple<int, int>     GetRandomTuple(int max);
    std::vector<uint8_t>     BreedVoiceData(int a, int b);
    void                     AddSyx(const DxSyx &s) {
        _syxs.push_back(s);
    };

public:
    DxSyxDB();
    void DumpSyx();
    void BreedSyx();

    friend std::ostream& operator<<(std::ostream& os, const DxSyx& syx);
};

#endif
