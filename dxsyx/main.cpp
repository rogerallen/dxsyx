//
//  main.cpp
//  dxsyx
//
// Created by Roger Allen on 1/5/15.
// Copyright (c) 2015 Roger Allen.
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


#include <iostream>
#include <string>
#include "dxsyx.h"

using namespace std;

void PrintUsage()
{
    cout << "dxsyx - a DX7 SYSEX file toolkit" << endl;
    cout << endl;
    cout << "usage:" << endl;
    cout << "  dxsyx [options] [file1.syx file2.syx ...]" << endl;
    cout << endl;
    cout << "options:" << endl;
    cout << "  (none)                   : print out voices" << endl;
    cout << "  --                       : read filenames from stdin" << endl;
    cout << "  -b config.txt output.syx : select specific voices, breed 32 random voices" << endl;
    cout << "  -c                       : print out voices with a data CRC" << endl;
    cout << "  -h                       : help (this message)" << endl;
    cout << "  -i                       : ignore checksum errors" << endl;
    cout << "  -s config.txt output.syx : select specific voices, output to new syx file" << endl;
    cout << "  -y                       : print full data as YAML" << endl;
}

bool parse_arg(int &i, const char **argv, bool &read_from_stdin)
{
    if (argv[i][1] == 'y') {
        DxSyxConfig::get().print_mode = DxSyxOutputMode::Full;
    } else if (argv[i][1] == 'c') {
        DxSyxConfig::get().print_mode = DxSyxOutputMode::NamesCrc;
    } else if (argv[i][1] == 's') {
        DxSyxConfig::get().config_filename = argv[++i];
        DxSyxConfig::get().output_filename = argv[++i];
        DxSyxConfig::get().print_mode = DxSyxOutputMode::Syx;
    } else if (argv[i][1] == 'b') {
        DxSyxConfig::get().config_filename = argv[++i];
        DxSyxConfig::get().output_filename = argv[++i];
        DxSyxConfig::get().print_mode = DxSyxOutputMode::Breed;
    } else if (argv[i][1] == '-') {
        read_from_stdin = true;
    } else if (argv[i][1] == 'h') {
        PrintUsage();
        return false;
    } else if (argv[i][1] == 'i') {
        DxSyxConfig::get().ignoreChecksum = true;
    } else {
        cout << "ERROR: unknown option -" << argv[i][1] << endl;
        PrintUsage();
        return false;
    }
    return true;
}

template <typename T>
void process_syx_file(T filename)
{
    try {
        DxSyx d = DxSyx(filename);
        cout << d;
    } catch (const runtime_error& error) {
        cout << "ERROR with " << filename << " " << error.what() << endl;
    }
}

int main(int argc, const char * argv[])
{
    try {
        bool read_from_stdin = false;
        for(int i = 1; i < argc; ++i) {
            if (argv[i][0] == '-') {
                if(!parse_arg(i,argv,read_from_stdin)) {
                    return 1;
                }
            } else {
                process_syx_file(argv[i]);
            }
        }
        if (read_from_stdin) {
            string line;
            while (getline(cin, line)) {
                process_syx_file(line);
            }
        }
        if (DxSyxConfig::get().print_mode == DxSyxOutputMode::Syx) {
            DxSyxDB db;
            db.DumpSyx();
        } else if (DxSyxConfig::get().print_mode == DxSyxOutputMode::Breed) {
            DxSyxDB db;
            db.BreedSyx();
        }
        return 0;
    } catch (const runtime_error& error) {
        cout << "ERROR" << error.what() << endl;
        return 1;
    }
}
