#   replacement of .hpp by .h header file name
#   - all headers git rename *.hpp -> *.h
#   in headers
#   - include guards: _HPP -> _H
#   - inclusions: #include "\w+.hpp" -> "\w+.h"
#   in sources
#   - inclusions: #include "\w+.hpp" -> <\w+.h>

import re
from glob import glob
from subprocess import run
from os.path import dirname, abspath, realpath, join
from os import chdir

rxEnd   = re.compile(r'[ \t]*$', re.M)
rxInc   = re.compile(r'^#include +"(\w+)\.hpp"', re.M)
rxRen   = re.compile(r'^(\w+\.h)pp')
rxGuard = re.compile(r'_HPP$', re.M)

myDir = dirname(realpath(abspath(__file__)))

def gitren(old:str, new:str):
    print(old, '->', new)
    run(['git', 'mv', old, new])

chdir(join(myDir, 'outtake'))

for file in glob('*.cpp'):
    with open(file, 'r') as fh:
        txt = fh.read()
        txt = rxInc.sub(r'#include <\1.h>', rxEnd.sub('', txt))
        print(txt)
        fh.close()
        with open(file, 'w') as fh:
            fh.write(txt)

#chdir(join(myDir, 'inc'))
for file in glob('*.hpp'):
    with open(file, 'r') as fh:
        txt = fh.read()
        txt = rxInc.sub(r'#include "\1.h"', 
            rxGuard.sub('_H', 
            rxEnd.sub('', txt)))
        print(txt)
        fh.close()
        with open(file, 'w') as fh:
            fh.write(txt)
        new = rxRen.sub(r'\1', file)
        gitren(file, new)
