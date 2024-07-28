import os
from shutil import copytree, rmtree
from shutil import move as shutilmove

PROJECTBUILD_FOLDER = "/tmp/projectbuild"  

LINUX = 0
WINDOWS = 1
WEB = 2

def make_projectbuild_dir(dir:str):
    try:
        os.mkdir(dir)
    except FileExistsError:
        rmtree(dir)
        os.mkdir(dir)
    except OSError:
        if exc.errno != errno.EEXIST:
            raise
        rmtree(dir)
        os.mkdir(dir)

def move_files(dir: str, platform: int):
    # STATIC
    # shutilmove(fr"{PROJECTBUILD_FOLDER}/libcharlie2D.a", fr"{libdir}/libcharlie2D.a")

    # SHARED
    if (platform == LINUX):
        libdir = fr"{dir}/lib/linux/charlie2D"
        os.makedirs(libdir)
        shutilmove(fr"{PROJECTBUILD_FOLDER}/libcharlie2D.so", fr"{libdir}/libcharlie2D.so")

    if (platform == WINDOWS):
        libdir = fr"{dir}/lib/windows/charlie2D"
        os.makedirs(libdir)
        shutilmove(fr"{PROJECTBUILD_FOLDER}/libcharlie2D.dll", fr"{dir}/bin/libcharlie2D.dll")
        shutilmove(fr"{PROJECTBUILD_FOLDER}/libcharlie2D.dll.a", fr"{libdir}/libcharlie2D.dll.a")

    if (platform == WEB):
        libdir = fr"{dir}/lib/emscripten/charlie2D"
        os.makedirs(libdir)
        shutilmove(fr"{PROJECTBUILD_FOLDER}/libcharlie2D.a", fr"{libdir}/libcharlie2D.a")

def create_project(dir: str):
    charlie2D_path = os.getcwd()
    dir = os.path.abspath(dir)

    make_projectbuild_dir(dir)
    copytree("./include", fr"{dir}/include")
    copytree("./lib", fr"{dir}/lib")
    copytree("./bin", fr"{dir}/bin")
    
    # FOR SYSTEM WINDOWS
    make_projectbuild_dir(PROJECTBUILD_FOLDER)
    os.chdir(PROJECTBUILD_FOLDER)
    os.system(fr"cmake -DCMAKE_TOOLCHAIN_FILE=~/TC-mingw.cmake {charlie2D_path}; cmake --build .")
    move_files(dir, WINDOWS)

    # FOR SYSTEM LINUX
    make_projectbuild_dir(PROJECTBUILD_FOLDER)
    os.chdir(PROJECTBUILD_FOLDER)
    os.system(fr"cmake {charlie2D_path}; cmake --build .")
    move_files(dir, LINUX)

    # FOR SYSTEM WEB
    make_projectbuild_dir(PROJECTBUILD_FOLDER)
    os.chdir(PROJECTBUILD_FOLDER)
    os.system("source ~/emsdk/emsdk_env.sh")
    os.system(fr"emcmake cmake {charlie2D_path}; emmake make")
    move_files(dir, WEB)

create_project("./FullBuild")
    
