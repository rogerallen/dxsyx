//
//  mk2syx.cpp
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

#include <regex>
#include <cstring> // for memset, memcpy
#include "dxsyx.h"
#include "mk2syx.h"

namespace {
    static uint8_t GetConfigValue(std::vector<std::string>::const_iterator& it) {
        it++;
        return atoi(it->c_str());
    }

    static bool AssignConfigValue(uint8_t& target, uint8_t max, std::vector<std::string>::const_iterator& it, uint8_t shift=0) {
        auto key = *it;
        auto val = GetConfigValue(it);
        if (val <= max) {
            target |= val << shift;
            return true;
        }
        std::cerr << "WARNING: mk2 config value out of range: " << key << "=" << int(val) << " (max:" << int(max) << ")" << std::endl;
        return false;
    }

    static bool AssignSignedConfigValue(uint8_t& target, std::vector<std::string>::const_iterator& it) {
        auto key = *it++;
        int8_t val = atoi(it->c_str());
        if (val <= 50 && val >= -50) {
            val += 50;
            target = val;
            return true;
        }
        std::cerr << "WARNING: mk2 config value out of range: " << key << "=" << int(val) << " (min:-50 max:50)" << std::endl;
        return false;
    }
}

// ======================================================================
// NOTE: unlike DxSyxOsc and DxSyxVoice, this structure's layout is the same as the packed sysex data format
DxSyxMk2AdditionalVoiceParameters::DxSyxMk2AdditionalVoiceParameters(const DxSyxVoice &voice, const std::string &configLine) {
    memset(this,0,sizeof(DxSyxMk2AdditionalVoiceParameters));

    bc_pitch_bias = 50;
    at_pitch_bias = 50;

    // NOTE: mk2ScaleOscAmpMod==true will change DX7 amp mod values 1,2,3 to DX7 mk2 amp mod values 2,4,6
    int scale = DxSyxConfig::get().mk2ScaleOscAmpMod ? 1 : 0;

    osc_amp_mod_1_2 |= voice.syx_oscs[5].syx_amp_mod_sensitivity << (3 + scale);
    osc_amp_mod_1_2 |= voice.syx_oscs[4].syx_amp_mod_sensitivity << scale;
    osc_amp_mod_3_4 |= voice.syx_oscs[3].syx_amp_mod_sensitivity << (3 + scale);
    osc_amp_mod_3_4 |= voice.syx_oscs[2].syx_amp_mod_sensitivity << scale;
    osc_amp_mod_5_6 |= voice.syx_oscs[1].syx_amp_mod_sensitivity << (3 + scale);
    osc_amp_mod_5_6 |= voice.syx_oscs[0].syx_amp_mod_sensitivity << scale;

   // std::string configLine("osc1_scale=0 osc2_scale=1 osc3_scale=0 osc4_scale=1 osc5_scale=0 osc6_scale=1 osc1_amp_mod=0 osc2_amp_mod=0 osc3_amp_mod=0 osc4_amp_mod=0 osc5_amp_mod=0 osc6_amp_mod=0 rnd_pitch=1 peg_vel_sw=1 lfo_key_trig=1 peg_range=2 pb_range=7 key_mode=3 pb_mode=1 pb_step=9 port_step=5 port_mode=1 port_time=70 mw_pmod=88 mw_amod=87 mw_eg_bias=86 fc1_pmod=85 fc1_amod=84 fc1_eg_bias=83 fc1_volume=82 bc_pmod=81 bc_amod=80 bc_eg_bias=79 bc_pitch_bias=78 at_pmod=77 at_amod=76 at_eg_bias=75 at_pitch_bias=74 peg_rate_scale=7 fc2_pmod=73 fc2_amod=72 fc2_eg_bias=71 fc2_volume=70 midi_ctrl_pmod=69 midi_ctrl_amod=68 midi_ctrl_eg_bias=67 midi_ctrl_volume=66 fc1_as_cs1=1 unison_detune=4");
    std::regex re("(=|,| )");
    std::sregex_token_iterator first{configLine.begin(),configLine.end(),re,-1};
    std::vector<std::string> configs = {first,std::sregex_token_iterator()};
    DecodeConfigs(configs);
}

DxSyxMk2AdditionalVoiceParameters::DxSyxMk2AdditionalVoiceParameters(const uint8_t* data) {
    memcpy(this,data,sizeof(DxSyxMk2AdditionalVoiceParameters));
}

void DxSyxMk2AdditionalVoiceParameters::DecodeConfigs(const std::vector<std::string>& configs) {
    for (auto it=configs.begin(); DecodeConfig(it,configs.end()); ++it);
}

bool DxSyxMk2AdditionalVoiceParameters::DecodeConfig(std::vector<std::string>::const_iterator& it, const std::vector<std::string>::const_iterator& end) {
    if (it != end) {
        // there must be a better way
        if (!it->compare("osc1_scale")) {
            return AssignConfigValue(osc_scaling_modes,1,it);
        }
        else if (!it->compare("osc2_scale")) {
            return AssignConfigValue(osc_scaling_modes,1,it,1);
        }
        else if (!it->compare("osc3_scale")) {
            return AssignConfigValue(osc_scaling_modes,1,it,2);
        }
        else if (!it->compare("osc4_scale")) {
            return AssignConfigValue(osc_scaling_modes,1,it,3);
        }
        else if (!it->compare("osc5_scale")) {
            return AssignConfigValue(osc_scaling_modes,1,it,4);
        }
        else if (!it->compare("osc6_scale")) {
            return AssignConfigValue(osc_scaling_modes,1,it,5);
        }
        // amp mod comes from the base dx7 osc data but can be overridden manually
        else if (!it->compare("osc1_amp_mod")) {
            return AssignConfigValue(osc_amp_mod_1_2,7,it,3);
        }
        else if (!it->compare("osc2_amp_mod")) {
            return AssignConfigValue(osc_amp_mod_1_2,7,it);
        }
        else if (!it->compare("osc3_amp_mod")) {
            return AssignConfigValue(osc_amp_mod_3_4,7,it,3);
        }
        else if (!it->compare("osc4_amp_mod")) {
            return AssignConfigValue(osc_amp_mod_3_4,7,it);
        }
        else if (!it->compare("osc5_amp_mod")) {
            return AssignConfigValue(osc_amp_mod_5_6,7,it,3);
        }
        else if (!it->compare("osc6_amp_mod")) {
            return AssignConfigValue(osc_amp_mod_5_6,7,it);
        }
        else if (!it->compare("rnd_pitch")) {
            return AssignConfigValue(rndpitch_pegvelsw_lfokeytrig_pegrange,7,it,4);
        }
        else if (!it->compare("peg_vel_sw")) {
            return AssignConfigValue(rndpitch_pegvelsw_lfokeytrig_pegrange,1,it,3);
        }
        else if (!it->compare("lfo_key_trig")) {
            return AssignConfigValue(rndpitch_pegvelsw_lfokeytrig_pegrange,1,it,2);
        }
        else if (!it->compare("peg_range")) {
            return AssignConfigValue(rndpitch_pegvelsw_lfokeytrig_pegrange,3,it);
        }
        else if (!it->compare("pb_range")) {
            return AssignConfigValue(pbrange_keymode,12,it,2);
        }
        else if (!it->compare("key_mode")) {
            return AssignConfigValue(pbrange_keymode,3,it);
        }
        else if (!it->compare("pb_mode")) {
            return AssignConfigValue(pbmode_pbstep,3,it,4);
        }
        else if (!it->compare("pb_step")) {
            return AssignConfigValue(pbmode_pbstep,12,it);
        }
        else if (!it->compare("port_mode")) {
            return AssignConfigValue(portstep_portmode,12,it);
        }
        else if (!it->compare("port_step")) {
            return AssignConfigValue(portstep_portmode,12,it,1);
        }
        else if (!it->compare("port_time")) {
            return AssignConfigValue(port_time,99,it);
        }
        else if (!it->compare("mw_pmod")) {
            return AssignConfigValue(mw_pmod,99,it);
        }
        else if (!it->compare("mw_amod")) {
            return AssignConfigValue(mw_amod,99,it);
        }
        else if (!it->compare("mw_eg_bias")) {
            return AssignConfigValue(mw_eg_bias,99,it);
        }
        else if (!it->compare("fc1_pmod")) {
            return AssignConfigValue(fc1_pmod,99,it);
        }
        else if (!it->compare("fc1_amod")) {
            return AssignConfigValue(fc1_amod,99,it);
        }
        else if (!it->compare("fc1_eg_bias")) {
            return AssignConfigValue(fc1_eg_bias,99,it);
        }
        else if (!it->compare("fc1_volume")) {
            return AssignConfigValue(fc1_volume,99,it);
        }
        else if (!it->compare("bc_pmod")) {
            return AssignConfigValue(bc_pmod,99,it);
        }
        else if (!it->compare("bc_amod")) {
            return AssignConfigValue(bc_amod,99,it);
        }
        else if (!it->compare("bc_eg_bias")) {
            return AssignConfigValue(bc_eg_bias,99,it);
        }
        else if (!it->compare("bc_pitch_bias")) {
            return AssignSignedConfigValue(bc_pitch_bias,it);
        }
        else if (!it->compare("at_pmod")) {
            return AssignConfigValue(at_pmod,99,it);
        }
        else if (!it->compare("at_amod")) {
            return AssignConfigValue(at_amod,99,it);
        }
        else if (!it->compare("at_eg_bias")) {
            return AssignConfigValue(at_eg_bias,99,it);
        }
        else if (!it->compare("at_pitch_bias")) {
            return AssignSignedConfigValue(at_pitch_bias,it);
        }
        else if (!it->compare("peg_rate_scale")) {
            return AssignConfigValue(peg_rate_scale,7,it);
        }
        else if (!it->compare("fc2_pmod")) {
            return AssignConfigValue(fc2_pmod,99,it);
        }
        else if (!it->compare("fc2_amod")) {
            return AssignConfigValue(fc2_amod,99,it);
        }
        else if (!it->compare("fc2_eg_bias")) {
            return AssignConfigValue(fc2_eg_bias,99,it);
        }
        else if (!it->compare("fc2_volume")) {
            return AssignConfigValue(fc2_volume,99,it);
        }
        else if (!it->compare("midi_ctrl_pmod")) {
            return AssignConfigValue(midi_ctrl_pmod,99,it);
        }
        else if (!it->compare("midi_ctrl_amod")) {
            return AssignConfigValue(midi_ctrl_amod,99,it);
        }
        else if (!it->compare("midi_ctrl_eg_bias")) {
            return AssignConfigValue(midi_ctrl_eg_bias,99,it);
        }
        else if (!it->compare("midi_ctrl_volume")) {
            return AssignConfigValue(midi_ctrl_volume,99,it);
        }
        else if (!it->compare("fc1_as_cs1")) {
            return AssignConfigValue(fc1cs1_unidetune,1,it,3);
        }
        else if (!it->compare("unison_detune")) {
            return AssignConfigValue(fc1cs1_unidetune,7,it);
        }
        std::cerr << "WARNING: unknown mk2 config value: " << *it << std::endl;
    }
    return false;
}

std::vector<uint8_t> DxSyxMk2AdditionalVoiceParameters::GetData() const {
    std::vector<uint8_t> data;
    data.resize(sizeof(DxSyxMk2AdditionalVoiceParameters));
    memcpy(&data.front(),this,sizeof(DxSyxMk2AdditionalVoiceParameters));
    return data;
}

std::string DxSyxMk2AdditionalVoiceParameters::DumpOscScalingMode(uint8_t oscIdx) const {
    return ((osc_scaling_modes & (1 << oscIdx)) == 0) ? "0 (normal)" : "1 (fractional)";
}

uint8_t DxSyxMk2AdditionalVoiceParameters::GetOscAmpMod(uint8_t oscIdx) const {
    uint8_t ampMod = osc_amp_mod_1_2;
    switch (oscIdx) {
        case 2: case 3:
            ampMod = osc_amp_mod_3_4; break;
        case 4: case 5:
            ampMod = osc_amp_mod_5_6; break;
    }
    uint8_t shift = oscIdx & 0x1 ? 0 : 3;
    return (ampMod >> shift) & 0x7;
}

std::string DxSyxMk2AdditionalVoiceParameters::DumpPEGRange() const {
    switch (rndpitch_pegvelsw_lfokeytrig_pegrange & 0x3) {
        case 0x3:
            return "3 (1/2oct)";
        case 0x2:
            return "2 (1oct)";
        case 0x1:
            return "1 (4oct)";
    }
    return "0 (8oct)";
}

std::string DxSyxMk2AdditionalVoiceParameters::DumpKeyMode() const {
    std::string mode = pbrange_keymode & 0x2 ? " (unison " : " (";
    mode += pbrange_keymode & 0x1 ? "mono)" : "poly)";
    return mode;
}

std::string DxSyxMk2AdditionalVoiceParameters::DumpPitchBendMode() const {
    switch (pbmode_pbstep & 0x30) {
        case 0x30:
            return "3 (key on)";
        case 0x20:
            return "2 (high)";
        case 0x10:
            return "1 (low)";
    }
    return "0 (normal)";
}

std::ostream& operator<<(std::ostream& os, const DxSyxMk2AdditionalVoiceParameters& mk2params) {
    os << "      mk2_additional_voice_parameters:" << std::endl;
    for (int i=0; i<SYX_NUM_OSC; ++i) {
        os << "        osc_scaling_mode_" << i+1 << ": " << mk2params.DumpOscScalingMode(i) << std::endl;
        os << "        osc_amp_mod_" << i+1 << ": " << int(mk2params.GetOscAmpMod(i)) << std::endl;
    }
    os << "        rnd_pitch: " << int(mk2params.rndpitch_pegvelsw_lfokeytrig_pegrange >> 4) << std::endl;
    os << "        peg_by_velocity: " << int((mk2params.rndpitch_pegvelsw_lfokeytrig_pegrange >> 3) & 0x1) << std::endl;
    os << "        lfo_key_trigger: " << int((mk2params.rndpitch_pegvelsw_lfokeytrig_pegrange >> 2) & 0x1) << std::endl;
    os << "        peg_range: " << mk2params.DumpPEGRange() << std::endl;
    os << "        pb_range: " << int(mk2params.pbrange_keymode >> 2) << std::endl;
    os << "        pb_mode: " << int(mk2params.pbrange_keymode & 0x3) << mk2params.DumpKeyMode() << std::endl;
    os << "        pb_mode: " << mk2params.DumpPitchBendMode() << std::endl;
    os << "        pb_step: " << int(mk2params.pbmode_pbstep & 0xf) << std::endl;
    os << "        port_step: " << int(mk2params.portstep_portmode >> 1) << std::endl;
    os << "        port_mode: " << (mk2params.portstep_portmode & 0x1 ? "1 (fingered/fulltime)" : "0 (retain/follow)") << std::endl;
    os << "        port_time: " << int(mk2params.port_time) << std::endl;
    os << "        mw_pitch_mod: " << int(mk2params.mw_pmod) << std::endl;
    os << "        mw_amp_mod: " << int(mk2params.mw_amod) << std::endl;
    os << "        mw_eg_bias: " << int(mk2params.mw_eg_bias) << std::endl;
    os << "        fc1_pitch_mod: " << int(mk2params.fc1_pmod) << std::endl;
    os << "        fc1_amp_mod: " << int(mk2params.fc1_amod) << std::endl;
    os << "        fc1_eg_bias: " << int(mk2params.fc1_eg_bias) << std::endl;
    os << "        fc1_volume: " << int(mk2params.fc1_volume) << std::endl;
    os << "        bc_pitch_mod: " << int(mk2params.bc_pmod) << std::endl;
    os << "        bc_amp_mod: " << int(mk2params.bc_amod) << std::endl;
    os << "        bc_eg_bias: " << int(mk2params.bc_eg_bias) << std::endl;
    os << "        bc_pitch_bias: " << int(mk2params.bc_pitch_bias) - 50 << std::endl;
    os << "        at_pitch_mod: " << int(mk2params.at_pmod) << std::endl;
    os << "        at_amp_mod: " << int(mk2params.at_amod) << std::endl;
    os << "        at_eg_bias: " << int(mk2params.at_eg_bias) << std::endl;
    os << "        at_pitch_bias: " << int(mk2params.at_pitch_bias) - 50 << std::endl;
    os << "        peg_rate_scale: " << int(mk2params.peg_rate_scale) << std::endl;
    os << "        fc2_pitch_mod: " << int(mk2params.fc2_pmod) << std::endl;
    os << "        fc2_amp_mod: " << int(mk2params.fc2_amod) << std::endl;
    os << "        fc2_eg_bias: " << int(mk2params.fc2_eg_bias) << std::endl;
    os << "        fc2_volume: " << int(mk2params.fc2_volume) << std::endl;
    os << "        midi_ctrl_pitch_mod: " << int(mk2params.midi_ctrl_pmod) << std::endl;
    os << "        midi_ctrl_amp_mod: " << int(mk2params.midi_ctrl_amod) << std::endl;
    os << "        midi_ctrl_eg_bias: " << int(mk2params.midi_ctrl_eg_bias) << std::endl;
    os << "        midi_ctrl_volume: " << int(mk2params.midi_ctrl_volume) << std::endl;
    os << "        fc1_as_cs1: " << int(mk2params.fc1cs1_unidetune >> 3) << std::endl;
    os << "        unison_detune: " << int(mk2params.fc1cs1_unidetune & 0x7) << std::endl;

    return os;
}


// ======================================================================
void DxSyx::UnpackMk2AdditionalParameters() {
    if (!_mk2data.empty()
        && _mk2data.size()==SYX_MK2_ADDITIONAL_FILE_SIZE
        && _mk2data[0]==0xf0 && _mk2data[1]==0x43 && _mk2data[2]==0x00 && _mk2data[3]==0x06
        && _mk2data[4]==0x08 && _mk2data[5]==0x60) {
        for (int i=0; i<SYX_NUM_VOICES; ++i) {
            mk2params.push_back(DxSyxMk2AdditionalVoiceParameters(&_mk2data[i*35+6]));
        }
    }
}
