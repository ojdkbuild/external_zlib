/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   zlib_test.cpp
 * Author: alex
 *
 * Created on February 21, 2015, 12:58 AM
 */

#include <iostream>
#include <cstdio>
#include <cstring> // for strlen

#include "zlib.h"

#include "staticlib/config/assert.hpp"

// https://gist.github.com/arq5x/5315739
// adapted from: http://stackoverflow.com/questions/7540259/deflate-and-inflate-zlib-h-in-c
void test_deflate() {
    // original string len = 36
    char a[50] = "Hello Hello Hello Hello Hello Hello!";

    // placeholder for the compressed (deflated) version of "a"
    char b[50];

    // placeholder for the UNcompressed (inflated) version of "b"
    char c[50];

    printf("Uncompressed string is: %s\n", a);


    printf("\n----------\n\n");

    // STEP 1.
    // deflate a into b. (that is, compress a into b)
    // zlib struct
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "b" as the compressed output
    defstream.avail_in = (uInt) strlen(a) + 1; // size of input, string + terminator
    defstream.next_in = (Bytef *) a; // input char array
    defstream.avail_out = (uInt)sizeof (b); // size of output
    defstream.next_out = (Bytef *) b; // output char array
    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);
    // This is one way of getting the size of the output
    printf("Compressed string is: %s\n", b);

    printf("\n----------\n\n");

    // STEP 2.
    // inflate b into c
    // zlib struct
    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    // setup "b" as the input and "c" as the compressed output
    infstream.avail_in = (uInt) ((char*) defstream.next_out - b); // size of input
    infstream.next_in = (Bytef *) b; // input char array
    infstream.avail_out = (uInt)sizeof (c); // size of output
    infstream.next_out = (Bytef *) c; // output char array
    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);
    printf("Uncompressed string is: %s\n", c);

    // make sure uncompressed is exactly equal to original.
    slassert(strcmp(a, c) == 0);
}

int main() {
    try {
        test_deflate();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
