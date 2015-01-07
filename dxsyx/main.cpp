//
//  main.cpp
//  dxsyx
//
//  Created by Roger Allen on 1/5/15.
//  Copyright (c) 2015 Roger Allen. All rights reserved.
//

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
}

int main(int argc, const char * argv[])
{
    try {
        for(int i = 1; i < argc; ++i) {
            if (argv[i][0] == '-') {
                if (argv[i][1] == 'v') {
                    DxSyxConfig::get().print_mode = DxSyxPrintMode::Full;
                } else if (argv[i][1] == 'h') {
                    PrintUsage();
                    return 0;
                } else {
                    PrintUsage();
                    return 1;
                }
            } else {
                DxSyx d = DxSyx(argv[i]);
                cout << d;
            }
        }
        return 0;
    } catch (const runtime_error& error) {
        cout << error.what() << endl;
        return 1;
    }
}
