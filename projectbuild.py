import os
from shutil import copytree, rmtree
from shutil import move as shutilmove

PROJECTBUILD_FOLDER = "/tmp/projectbuild"  

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


def create_project(dir: str):
    charlie2D_path = os.getcwd()
    dir = os.path.abspath(dir)

    make_projectbuild_dir(dir)
    copytree("./include", fr"{dir}/include")
    copytree("./lib", fr"{dir}/lib")
    
    # FOR SYSTEM LINUX
    make_projectbuild_dir(PROJECTBUILD_FOLDER)
    os.chdir(PROJECTBUILD_FOLDER)
    os.system(fr"cmake {charlie2D_path}; cmake --build .")
    libdir = fr"{dir}/lib/linux/charlie2D"
    os.makedirs(libdir)
    shutilmove(fr"{PROJECTBUILD_FOLDER}/libcharlie2D.a", fr"{libdir}/libcharlie2D.a")

    # FOR SYSTEM WEB
    make_projectbuild_dir(PROJECTBUILD_FOLDER)
    os.chdir(PROJECTBUILD_FOLDER)
    os.system("source ~/emsdk/emsdk_env.sh")
    os.system(fr"emcmake cmake {charlie2D_path}; emmake make")
    libdir = fr"{dir}/lib/emscripten/charlie2D"
    os.makedirs(libdir)
    shutilmove(fr"{PROJECTBUILD_FOLDER}/libcharlie2D.a", fr"{libdir}/libcharlie2D.a")

create_project("./FullBuild")
    
