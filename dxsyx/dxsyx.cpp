//
//  dxsyx.cpp
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

#include "dxsyx.h"

using namespace std;

#include <fstream>
#include <sstream>

// ======================================================================
DxSyxOsc::DxSyxOsc(const uint8_t osc_num, DxSyx &dx) {
    _osc_num = osc_num;
    syx_eg_rate_1             = dx.GetDataCS();
    syx_eg_rate_2             = dx.GetDataCS();
    syx_eg_rate_3             = dx.GetDataCS();
    syx_eg_rate_4             = dx.GetDataCS();
    syx_eg_level_1            = dx.GetDataCS();
    syx_eg_level_2            = dx.GetDataCS();
    syx_eg_level_3            = dx.GetDataCS();
    syx_eg_level_4            = dx.GetDataCS();
    syx_kbd_lev_scl_brk_pt    = dx.GetDataCS();
    syx_kbd_lev_scl_lft_depth = dx.GetDataCS();
    syx_kbd_lev_scl_rht_depth = dx.GetDataCS();
    uint8_t tmp               = dx.GetDataCS();
    syx_kbd_lev_scl_lft_curve = 0x3 & tmp;
    syx_kbd_lev_scl_rht_curve = 0x3 & (tmp >> 2);
    tmp                       = dx.GetDataCS();
    syx_osc_detune            = 0x0f & (tmp >> 3);
    syx_kbd_rate_scaling      = 0x07 & tmp;
    tmp                       = dx.GetDataCS();
    syx_key_vel_sensitivity   = 0x07 & (tmp >> 2);
    syx_amp_mod_sensitivity   = 0x03 & tmp;
    syx_operator_output_level = dx.GetDataCS();
    tmp                       = dx.GetDataCS();
    syx_osc_freq_coarse       = 0x1f & (tmp >> 1);
    syx_osc_mode              = 0x01 & tmp;
    syx_osc_freq_fine         = dx.GetDataCS();
    
}


// ======================================================================
DxSyxVoice::DxSyxVoice(DxSyx &dx) {
    for(int i = 0; i < SYX_NUM_OSC; ++i) {
        syx_oscs[i] = DxSyxOsc(6-i, dx);
    }
    syx_pitch_eg_rate_1       = dx.GetDataCS();
    syx_pitch_eg_rate_2       = dx.GetDataCS();
    syx_pitch_eg_rate_3       = dx.GetDataCS();
    syx_pitch_eg_rate_4       = dx.GetDataCS();
    syx_pitch_eg_level_1      = dx.GetDataCS();
    syx_pitch_eg_level_2      = dx.GetDataCS();
    syx_pitch_eg_level_3      = dx.GetDataCS();
    syx_pitch_eg_level_4      = dx.GetDataCS();
    syx_algorithm_num         = dx.GetDataCS();
    uint8_t tmp               = dx.GetDataCS();
    syx_feedback              = 0x07 & tmp;
    syx_oscillator_sync       = 0x01 & (tmp >> 3);
    syx_lfo_speed             = dx.GetDataCS();
    syx_lfo_delay             = dx.GetDataCS();
    syx_lfo_pitch_mod_depth   = dx.GetDataCS();
    syx_lfo_amp_mod_depth     = dx.GetDataCS();
    tmp                       = dx.GetDataCS();
    syx_pitch_mod_sensitivity = 0x07 & (tmp >> 4);
    syx_lfo_waveform          = 0x07 & (tmp >> 1);
    syx_lfo_sync              = 0x01 & tmp;
    syx_transpose             = dx.GetDataCS();
    for(int i = 0; i < 10; ++i) {
        syx_name[i] = FixChar(dx.GetDataCS());
    }
    syx_name[10] = '\0';
}

char DxSyxVoice::FixChar(char c) {
    if (c < ' ') {
        return '_';
    }
    switch (c) {
        case 92: // Yen
            return 'Y';
            break;
        case 126: // <<
            return '<';
            break;
        case 127: // >>
            return '>';
            break;
            
        default:
            return c;
            break;
    }
}

// ======================================================================
DxSyx::DxSyx(const char *filename) {
    DxSyx(string(filename, strnlen(filename,1024)));
}

DxSyx::DxSyx(const string &filename) {
    //cerr << "reading " << filename << endl;
    _filename       = filename;
    _cur_checksum   = 0;
    _cur_data_index = 0;
    ReadFile(_filename);
    UnpackSyx();
}

void DxSyx::ReadFile(const string &filename)
{
    ifstream fl(filename, ifstream::in|ifstream::binary);
    if(!fl.good()) {
        throw runtime_error(string("ERROR: problem opening file."));
    }
    fl.seekg( 0, ios::end );
    size_t len = fl.tellg();
    if(len != SYX_FILE_SIZE) {
        throw runtime_error(string("ERROR: filesize not equal to 4096+8"));
    }
    fl.seekg(0, ios::beg);
    fl.read((char *)_data, SYX_FILE_SIZE);
    fl.close();
}

void DxSyx::UnpackSyx()
{
    CheckHeader();
    UnpackVoices();
    CheckCurrentSum();
}

void DxSyx::CheckHeader()
{
    
    uint8_t syx_status = GetData();
    uint8_t syx_id = GetData();
    uint8_t syx_sub_status_channel_num = GetData();
    (void)syx_sub_status_channel_num; // fix unused warning
    uint8_t syx_format_number = GetData();
    uint8_t syx_byte_count_ms = GetData();
    uint8_t syx_byte_count_ls = GetData();
    
    if(!((syx_status        == 0xf0) &&
         (syx_id            == 0x43) &&
         (syx_format_number == 0x09) &&
         (syx_byte_count_ms == 0x20) &&
         (syx_byte_count_ls == 0x00))) {
        throw runtime_error(string("ERROR: header mismatch from expected."));
    }
}

void DxSyx::UnpackVoices() {
    for(int i = 0; i < SYX_NUM_VOICES; ++i) {
        UnpackVoice(i);
    }
}

void DxSyx::UnpackVoice(int n) {
    syx_voices[n] = DxSyxVoice(*this);
}

void DxSyx::CheckCurrentSum() {
    uint8_t cur_masked_sum = 0x7f & _cur_checksum;
    uint8_t syx_expected_sum = GetData();
    uint8_t syx_status = GetData();
    if((cur_masked_sum != syx_expected_sum)) {
        throw runtime_error(string("ERROR: bad checksum."));
    }
    if(syx_status != 0xf7) {
        throw runtime_error(string("ERROR: bad end status."));
    }
    if(_cur_data_index != SYX_FILE_SIZE) {
        throw runtime_error(string("ERROR: EOF position."));
    }
}

uint8_t DxSyx::GetData()
{
    return _data[_cur_data_index++];
}

uint8_t DxSyx::GetDataCS()
{
    int8_t d = (int8_t)_data[_cur_data_index++];
    _cur_checksum -= d;
    return d;
}

// ======================================================================
DxSyxDB::DxSyxDB() {
    auto lines = ReadConfigFile();
    for (auto line : lines) {
        string syx_filename = GetConfigLineFilename(line);
        if (FilenameIndex(syx_filename) < 0) {
            AddSyx(DxSyx(syx_filename));
        }
    }
}

vector<string> DxSyxDB::ReadConfigFile() {
    vector<string> lines;
    string line;
    ifstream fl(DxSyxConfig::get().config_filename);
    if(!fl.good()) {
        throw runtime_error(string("ERROR: problem opening config file."));
    }
    while (getline (fl,line)) {
        lines.push_back(line);
    }
    fl.close();
    if(lines.size() != 32) {
        throw runtime_error(string("ERROR: expecting only 32 lines in config file."));
    }
    return lines;
}

void DxSyxDB::WriteSyxFile(const uint8_t *data) {
    ofstream fl(DxSyxConfig::get().output_filename, ios::out | ios::trunc | ios::binary);
    if (!fl.is_open()) {
        throw runtime_error(string("ERROR: problem opening syx output file."));
    }
    fl.write((const char *)data, SYX_FILE_SIZE);
    fl.close();
}

tuple<int, int> DxSyxDB::DecodeConfigLine(const string &line) {
    stringstream ss(line.substr(11));  // start after name+','
    string voice_str, file_str;
    getline(ss, voice_str, ',');
    getline(ss, file_str, ',');
    int voice_num, file_num = FilenameIndex(file_str);
    istringstream(voice_str) >> voice_num;
    if (file_num >= 0) {
        return make_tuple(voice_num, file_num);
    }
    throw runtime_error(string("ERROR: did not find file from config file."));
}

string DxSyxDB::GetConfigLineFilename(const string &line) {
    stringstream ss(line.substr(11));  // start after name+','
    string voice_str, file_str;
    getline(ss, voice_str, ',');
    getline(ss, file_str, ',');
    return file_str;
}

int DxSyxDB::FilenameIndex(const string filename) {
    int file_num = 0;
    for (auto syx : _syxs) {
        if (filename == syx.GetFilename()) {
            return file_num;
        }
        ++file_num;
    };
    return -1; // not found
}

vector<uint8_t> DxSyx::GetVoiceData(int n) {
    vector<uint8_t> d;
    for(int i = 0; i < 4096/32; ++i) {
        d.push_back(_data[6+(n*4096/32)+i]);
    }
    return d;
}

string DxSyx::GetFilename() {
    return _filename;
}

vector<uint8_t> DxSyxDB::GetVoiceData(const int voice_num, const int syx_num) {
    return _syxs[syx_num].GetVoiceData(voice_num);
}


void DxSyxDB::DumpSyx() {
    auto lines = ReadConfigFile();
    
    // create syx file data buffer
    uint8_t data[SYX_FILE_SIZE];
    data[0] = 0xf0;
    data[1] = 0x43;
    data[2] = 0x00;
    data[3] = 0x09;
    data[4] = 0x20;
    data[5] = 0x00;
    data[SYX_FILE_SIZE-1] = 0xf7;
    // insert voices
    auto i = 6, checksum = 0;
    for (auto line : lines) {
        auto voice_syx_nums = DecodeConfigLine(line);
        for (auto d : GetVoiceData(get<0>(voice_syx_nums), get<1>(voice_syx_nums))) {
            data[i++] = d;
            checksum -= d;
        }
    }
    if(i != SYX_FILE_SIZE-2) {
        throw runtime_error(string("ERROR: did not write enough data to syx file."));
    }
    data[SYX_FILE_SIZE-2] = 0x7f & checksum;
    
    // write out data
    WriteSyxFile(data);
}

// ======================================================================
// Gathered all the print routines together

static void PrintOscLine(std::ostream& os, int n, const char *ds, int d)
{
    os << "    op" << n << ds << d << endl;
}

std::ostream& operator<<(std::ostream& os, const DxSyxOsc& syx)
{
    if (DxSyxConfig::get().print_mode == DxSyxOutputMode::Full) {
        PrintOscLine(os, syx._osc_num, "_eg_rate_1: ", syx.syx_eg_rate_1);
        PrintOscLine(os, syx._osc_num, "_eg_rate_2: ", syx.syx_eg_rate_2);
        PrintOscLine(os, syx._osc_num, "_eg_rate_3: ", syx.syx_eg_rate_3);
        PrintOscLine(os, syx._osc_num, "_eg_rate_4: ", syx.syx_eg_rate_4);
        PrintOscLine(os, syx._osc_num, "_eg_level_1: ", syx.syx_eg_level_1);
        PrintOscLine(os, syx._osc_num, "_eg_level_2: ", syx.syx_eg_level_2);
        PrintOscLine(os, syx._osc_num, "_eg_level_3: ", syx.syx_eg_level_3);
        PrintOscLine(os, syx._osc_num, "_eg_level_4: ", syx.syx_eg_level_4);
        PrintOscLine(os, syx._osc_num, "_kbd_lev_scl_brk_pt: ", syx.syx_kbd_lev_scl_brk_pt);
        PrintOscLine(os, syx._osc_num, "_kbd_lev_scl_lft_depth: ", syx.syx_kbd_lev_scl_lft_depth);
        PrintOscLine(os, syx._osc_num, "_kbd_lev_scl_rht_depth: ", syx.syx_kbd_lev_scl_rht_depth);
        PrintOscLine(os, syx._osc_num, "_kbd_lev_scl_lft_curve: ", syx.syx_kbd_lev_scl_lft_curve);
        PrintOscLine(os, syx._osc_num, "_kbd_lev_scl_rht_curve: ", syx.syx_kbd_lev_scl_rht_curve);
        PrintOscLine(os, syx._osc_num, "_osc_detune: ", syx.syx_osc_detune);
        PrintOscLine(os, syx._osc_num, "_kbd_rate_scaling: ", syx.syx_kbd_rate_scaling);
        PrintOscLine(os, syx._osc_num, "_key_vel_sensitivity: ", syx.syx_key_vel_sensitivity);
        PrintOscLine(os, syx._osc_num, "_amp_mod_sensitivity: ", syx.syx_amp_mod_sensitivity);
        PrintOscLine(os, syx._osc_num, "_operator_output_level: ", syx.syx_operator_output_level);
        PrintOscLine(os, syx._osc_num, "_osc_freq_coarse: ", syx.syx_osc_freq_coarse);
        PrintOscLine(os, syx._osc_num, "_osc_mode: ", syx.syx_osc_mode);
        PrintOscLine(os, syx._osc_num, "_osc_freq_fine: ", syx.syx_osc_freq_fine);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const DxSyxVoice& syx) {
    if (DxSyxConfig::get().print_mode == DxSyxOutputMode::Names) {
        os << syx.syx_name;
    } else if (DxSyxConfig::get().print_mode == DxSyxOutputMode::Full) {
            os << "  voice_name: " << syx.syx_name << endl;
        for(int i = 0; i < SYX_NUM_OSC; ++i) {
            os << syx.syx_oscs[i];
        }
        os << "    pitch_eg_rate_1: " << int(syx.syx_pitch_eg_rate_1) << endl;
        os << "    pitch_eg_rate_2: " << int(syx.syx_pitch_eg_rate_2) << endl;
        os << "    pitch_eg_rate_3: " << int(syx.syx_pitch_eg_rate_3) << endl;
        os << "    pitch_eg_rate_4: " << int(syx.syx_pitch_eg_rate_4) << endl;
        os << "    pitch_eg_level_1: " << int(syx.syx_pitch_eg_level_1) << endl;
        os << "    pitch_eg_level_2: " << int(syx.syx_pitch_eg_level_2) << endl;
        os << "    pitch_eg_level_3: " << int(syx.syx_pitch_eg_level_3) << endl;
        os << "    pitch_eg_level_4: " << int(syx.syx_pitch_eg_level_4) << endl;
        os << "    algorithm_num: " << int(syx.syx_algorithm_num) << endl;
        os << "    feedback: " << int(syx.syx_feedback) << endl;
        os << "    oscillator_sync: " << int(syx.syx_oscillator_sync) << endl;
        os << "    lfo_speed: " << int(syx.syx_lfo_speed) << endl;
        os << "    lfo_delay: " << int(syx.syx_lfo_delay) << endl;
        os << "    lfo_pitch_mod_depth: " << int(syx.syx_lfo_pitch_mod_depth) << endl;
        os << "    lfo_amp_mod_depth: " << int(syx.syx_lfo_amp_mod_depth) << endl;
        os << "    pitch_mod_sensitivity: " << int(syx.syx_pitch_mod_sensitivity) << endl;
        os << "    lfo_waveform: " << int(syx.syx_lfo_waveform) << endl;
        os << "    lfo_sync: " << int(syx.syx_lfo_sync) << endl;
        os << "    transpose: " << int(syx.syx_transpose) << endl;
    }
    
    return os;
}

ostream& operator<<(ostream& os, const DxSyx& syx)
{
    if (DxSyxConfig::get().print_mode == DxSyxOutputMode::Names) {
        for(int i = 0; i < SYX_NUM_VOICES; ++i) {
            os << syx.syx_voices[i] << "," << i << "," << syx._filename << endl;
        }
    } else if (DxSyxConfig::get().print_mode == DxSyxOutputMode::Full) {
        os << "--- " << endl;
        os << "filename: " << syx._filename << endl;
        for(int i = 0; i < SYX_NUM_VOICES; ++i) {
            os << syx.syx_voices[i];
        }
    }
    return os;
}
