/* 
 * File:   zlib_test.cpp
 * Author: alex
 *
 * Created on February 21, 2015, 12:58 AM
 */

#include "zlib.h"


int main() {
    // example to test linking only
    z_stream c_stream;
    c_stream.opaque = (voidpf) 0;
    (void) c_stream;
            
    return 0;
}

