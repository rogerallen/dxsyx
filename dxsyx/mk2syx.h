//
//  mk2syx.hpp
//  dxsyx
//
//  Created by Christian E. on 8/14/17.
//  Copyright © 2017 Roger Allen. All rights reserved.
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

#ifndef mk2syx_hpp
#define mk2syx_hpp

#include <cstdint>
#include <string>
#include <vector>

class DxSyxVoice;

const int SYX_MK2_ADDITIONAL_FILE_SIZE  = 1120 + 8;

// ======================================================================
// NOTE: unlike DxSyxOsc and DxSyxVoice, this structure's layout is the same as the packed sysex data format
class DxSyxMk2AdditionalVoiceParameters {
protected:
    uint8_t  osc_scaling_modes;
    uint8_t  osc_amp_mod_5_6;
    uint8_t  osc_amp_mod_3_4;
    uint8_t  osc_amp_mod_1_2;
    uint8_t  rndpitch_pegvelsw_lfokeytrig_pegrange;
    uint8_t  pbrange_keymode;
    uint8_t  pbmode_pbstep;
    uint8_t  portstep_portmode;
    uint8_t  port_time;
    uint8_t  mw_pmod;
    uint8_t  mw_amod;
    uint8_t  mw_eg_bias;
    uint8_t  fc1_pmod;
    uint8_t  fc1_amod;
    uint8_t  fc1_eg_bias;
    uint8_t  fc1_volume;
    uint8_t  bc_pmod;
    uint8_t  bc_amod;
    uint8_t  bc_eg_bias;
    uint8_t  bc_pitch_bias;
    uint8_t  at_pmod;
    uint8_t  at_amod;
    uint8_t  at_eg_bias;
    uint8_t  at_pitch_bias;
    uint8_t  peg_rate_scale;
    uint8_t  reserved;
    uint8_t  fc2_pmod;
    uint8_t  fc2_amod;
    uint8_t  fc2_eg_bias;
    uint8_t  fc2_volume;
    uint8_t  midi_ctrl_pmod;
    uint8_t  midi_ctrl_amod;
    uint8_t  midi_ctrl_eg_bias;
    uint8_t  midi_ctrl_volume;
    uint8_t  fc1cs1_unidetune;
    
    void DecodeConfigs(const std::vector<std::string>& configs);
    bool DecodeConfig(std::vector<std::string>::const_iterator& it, const std::vector<std::string>::const_iterator& end);
    
public:
    DxSyxMk2AdditionalVoiceParameters(const DxSyxVoice &voice, const std::string &configLine);
    DxSyxMk2AdditionalVoiceParameters(const uint8_t* data);
    
    std::vector<uint8_t> GetData() const;
    
    std::string DumpOscScalingMode(uint8_t oscIdx) const;
    uint8_t GetOscAmpMod(uint8_t oscIdx) const;
    std::string DumpPEGRange() const;
    std::string DumpKeyMode() const;
    std::string DumpPitchBendMode() const;
    
    friend std::ostream& operator<<(std::ostream& os, const DxSyxMk2AdditionalVoiceParameters& syx);
};


#endif /* mk2syx_hpp */
