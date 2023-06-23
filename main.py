import os
import sys

import vars
import VPGen


if __name__ == '__main__':
    if not os.path.exists(vars.lib_path) or '-n' in sys.argv: VPGen.buildLibrary()
    obstacles = VPGen.loadAndGenerate()
    VPGen.API.plot(obstacles)
