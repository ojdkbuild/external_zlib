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
 * File:   minizip_test.cpp
 * Author: alex
 *
 * Created on April 8, 2015, 8:30 PM
 */

#include <iostream>
#include <array>
#include <cassert>

#include "minizip/zip.h"
#include "minizip/unzip.h"

namespace { // anonymous

void test_zip() {
    auto zf = zipOpen("test.zip", APPEND_STATUS_CREATE);
    (void) zf;
    assert(zf);
    {
        auto res_open1 = zipOpenNewFileInZip(zf, "file1", nullptr, nullptr,
                0, nullptr, 0, nullptr, Z_DEFLATED, 6);
        (void) res_open1;
        assert(!res_open1);
        auto res_write1 = zipWriteInFileInZip(zf, "foo", 3);
        (void) res_write1;
        assert(!res_write1);
        auto res_close1 = zipCloseFileInZip(zf);
        (void) res_close1;
        assert(!res_close1);
    }
    {
        auto res_open2 = zipOpenNewFileInZip(zf, "file2", nullptr, nullptr,
                0, nullptr, 0, nullptr, Z_DEFLATED, 6);
        (void) res_open2;
        assert(!res_open2);
        auto res_write2 = zipWriteInFileInZip(zf, "bar", 3);
        (void) res_write2;
        assert(!res_write2);
        auto res_close2 = zipCloseFileInZip(zf);
        (void) res_close2;
        assert(!res_close2);
    }
    auto res_close = zipClose(zf, nullptr);
    (void) res_close;    
    assert(!res_close);    
}

void test_unzip() {
    auto zf = unzOpen("test.zip");
    (void) zf;
    assert(zf);
    auto find_res = unzLocateFile(zf, "file2", 1);
    (void) !find_res;
    assert(!find_res);
    auto open_res = unzOpenCurrentFile(zf);
    (void) open_res;
    assert(!open_res);
    
    std::array<char, 3> buf{{}};
    auto len = static_cast<unsigned int>(buf.size());
    auto read_res = unzReadCurrentFile(zf, buf.data(), len);
    (void) read_res;
    assert(3 == read_res);
    assert("bar" == std::string(buf.data(), buf.size()));
    
    auto res_close_current = unzCloseCurrentFile(zf);
    (void) res_close_current;
    assert(!res_close_current);
    auto res_close = unzClose(zf);
    (void) res_close;
    assert(!res_close);
}

} // namespace

int main() {
    test_zip();
    test_unzip();
    
    return 0;
}

