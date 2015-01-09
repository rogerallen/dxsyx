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
#include "dxsyx.h"

using namespace std;

void PrintUsage()
{
    cout << "dxsyx - a DX7 SYSEX file toolkit" << endl;
    cout << endl;
    cout << "usage:" << endl;
    cout << "  dxsyx [options] file1.syx file2.syx ..." << endl;
    cout << "options:" << endl;
    cout << "  -h = help (this message)" << endl;
    cout << "  -v = print full data as YAML" << endl;
    cout << "  -s = config.txt output.syx = select specific voices, output to new syx file" << endl;
}

bool parse_arg(int &i, const char **argv)
{
    if (argv[i][1] == 'v') {
        DxSyxConfig::get().print_mode = DxSyxOutputMode::Full;
    } else if (argv[i][1] == 's') {
        DxSyxConfig::get().config_filename = argv[++i];
        DxSyxConfig::get().output_filename = argv[++i];
        DxSyxConfig::get().print_mode = DxSyxOutputMode::Syx;
    } else if (argv[i][1] == 'h') {
        PrintUsage();
        return false;
    } else {
        cout << "ERROR: unknown option -" << argv[i][1] << endl;
        PrintUsage();
        return false;
    }
    return true;
}

int main(int argc, const char * argv[])
{
    try {
        DxSyxDB db;
        for(int i = 1; i < argc; ++i) {
            if (argv[i][0] == '-') {
                if(!parse_arg(i,argv)) {
                    return 1;
                }
            } else {
                DxSyx d = DxSyx(argv[i]);
                db.add(d);
                cout << d;
            }
        }
        if (DxSyxConfig::get().print_mode == DxSyxOutputMode::Syx) {
            db.dump();
        }
        return 0;
    } catch (const runtime_error& error) {
        cout << error.what() << endl;
        return 1;
    }
}
