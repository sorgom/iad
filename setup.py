#!python3
import getopt, sys, re
from os.path import isabs, dirname, abspath, realpath, isdir, exists, join
from os import makedirs, environ, chdir, getcwd, remove
from shutil import copyfile
from glob import glob
from subprocess import run, DEVNULL

verbose = False
preview = False
targetDir = None
debug = False

#   project folder with all symbolik linking expanded
projDir = dirname(realpath(abspath(__file__)))
#   project folder as begin of string regex
rxProjDir = re.compile(r'^' + re.escape(projDir) + r'\b')

#   sub folders to copy
targetSubDirs = ['bin', 'db', 'htm', 'bbl', 'sh', 'tmp']

def help():
    print(f"""
usage: {dirname(__file__)} [options]
options:
-t  <installation target folder>
    default $HOME/iad_runtime    
-d  compile with debug output
-p  preview      
-v  verbose  
-h  this help
""")
    sys.exit(0)

def echo(*args):
    if verbose or preview: print(*args)

#   less ugly substitute of assert
def check(cond:bool, *args):
    if not cond:
        print(*args)
        sys.exit(1)

def checkDir(dir:str):
    check(isabs(dir), dir, 'is not an absolute path')
    parDir = dirname(dir)
    check(exists(parDir), f'parent directory {parDir} does not exist')
    if not isdir(dir):
        check(not exists(dir), dir, 'exists and is not a folder')
        echo('creating', dir) 
        if not preview: makedirs(dir)

def goDir(dir:str):
    check(isdir(dir))
    chdir(dir)

def goProjDir(subdir:str):
    goDir(join(projDir, subdir))

def copyDir(src:str, dest:str):
    echo(src, '->', dest)
    check(isdir(src), 'missing folder', src)
    checkDir(dest)
    curdir = getcwd()
    chdir(src)
    for item in glob('*'):
        echo(item)
        trg = join(dest, item)
        if (isdir(item)) : copyDir(item, trg)
        else: copyfile(item, trg) 
    chdir(curdir)    

def rm(file:str):
    if exists(file): remove(file)

def placeTemplate(file:str):
    dir, trg = file.split('.', 2)[1:3]
    tFile = join(targetDir, dir, trg)
    echo('->', tFile)
    with open(file, 'r') as fh:
        txt = fh.read()
        fh.close()
        txt = re.sub(r'<TARGET_DIR>', targetDir, txt)
        echo(txt)
        with open(tFile, 'w') as fh:
            fh.write(txt)

def runc(com:str):
    a = com.split()
    if verbose or preview: run(a)
    else: run(a, stdout=DEVNULL)

#   command line & target folder
try:
    opts, args = getopt.getopt(sys.argv[1:], 't:dpvh')
except getopt.GetoptError as err:
    print(err)
    sys.exit(2)
for o, v in opts:
    if   o == '-v': verbose = True
    elif o == '-h': help()
    elif o == '-t': targetDir = v
    elif o == '-p': preview = True
    elif o == '-d': debug = True

#   default target: $HOME/iad_runtime
if targetDir is None:
    targetDir = join(environ.get('HOME'), 'iad_runtime')

targetDir = realpath(abspath(targetDir))

#   make sure target folder is not part of project folder
check(not rxProjDir.match(targetDir), targetDir, 'points to project folder', projDir)

checkDir(targetDir)

#   build binary
goProjDir('srv')
rm('CMakeCache.txt')
if debug: 
    runc('cmake -DDEVEL=1 .')
else: 
    runc('cmake .')
runc('make')

#   create databases
goProjDir('db')
run('./mkdbs.sh')

#   copy folders to target
for dir in targetSubDirs:
    copyDir(join(projDir, dir), join(targetDir, dir))

#   expand templates
goProjDir('templates')
for file in glob('template.*'): placeTemplate(file)

