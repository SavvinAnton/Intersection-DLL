import sys
import json
import os
import platform

import API
import vars


system = platform.system()
if system == 'Windows':
    def buildLibrary():
        os.system(vars.lib_build_cmd_wsl)

    def loadAndGenerate():
        if os.system(vars.runner_cmd_wsl) == 0:
            with open(vars.output_path, 'r') as file:
                obstacles = json.load(file)
            obstacles = tuple(API.Obstacle(API.Point(*obstacle[0]), obstacle[1]) for obstacle in obstacles)
            return obstacles
        raise RuntimeError

elif system == 'Linux':
    def buildLibrary() -> None:
        os.system(vars.lib_build_cmd)

    def generate(geometry_data: dict) -> None:
        lib = API.loadLibrary()
        lib.updateSrand()
        obstacles: tuple = API.generate(geometry_data, lib)
        lib.clear()
        return obstacles

    def loadDataFromFile() -> dict:
        with open(vars.input_path, 'r') as file:
            meta_data: dict = json.load(file)
        return meta_data

    def loadAndGenerate() -> None:
        return generate(loadDataFromFile())


if __name__ == '__main__':
    obstacles = loadAndGenerate()
    obstacles = tuple(((obstacle.point.x, obstacle.point.y, obstacle.point.z), obstacle.r) for obstacle in obstacles)
    with open(vars.output_path, 'w') as file:
        json.dump(obstacles, file)
else:
    pass
