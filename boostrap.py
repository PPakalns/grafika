#!/usr/bin/env python3

import os
import sys
import shutil
import subprocess

build_type = "Release"
if len(sys.argv) > 1:
    if not sys.argv[1] in ["Release", "Debug"]:
        raise Exception("Supported CMAKE_BUILD_TYPEs are 'Release' and 'Debug'")
    build_type = sys.argv[1]

build_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), "build")

if not os.path.isdir(build_dir):
    os.makedirs(build_dir)

os.chdir(build_dir)

subprocess.check_call(["cmake", "..", "-DCMAKE_BUILD_TYPE={}".format(build_type)])
