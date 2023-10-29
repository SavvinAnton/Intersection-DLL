from math import floor, sqrt
from numpy import cbrt

from . import Inline, Chess
from .Classes import Domain


def __calculate_settings(dimension, size, obstacles, periodicity):
    dimension = int(dimension)
    if dimension not in (2, 3):
        raise ValueError('Wrong dimension')
    size = [float(axis) for axis in size[:dimension]]
    obstacles = int(obstacles)
    periodicity = [bool(axis) for axis in periodicity]

    if dimension == 2:
        lines = [
            floor(sqrt(obstacles * size[0] / size[1])),
            floor(sqrt(obstacles * size[1] / size[0])),
            1
        ]
    else:
        lines = [
            floor(cbrt(obstacles * size[0] / sqrt(size[1] * size[2]))),
            floor(cbrt(obstacles * size[1] / sqrt(size[0] * size[2]))),
            floor(cbrt(obstacles * size[2] / sqrt(size[0] * size[1])))
        ]

    while lines[0] * lines[1] * lines[2] < obstacles:
        minimum = max(lines)
        axis = lines.index(minimum)
        lines[axis] += 1

    indent = [size[i] / (lines[i] + (1 if not periodicity[i] else -1)) for i in range(dimension)]
    if dimension == 2:
        lines.pop()

    return lines, indent


def generateChess(domain: Domain, callback_function):
    lines, indents = __calculate_settings(domain.dimension, domain.size(), 2 * domain.counter.number, domain.periodicity())
    return Chess.generate(domain, (lines, indents), callback_function)


def generateInline(domain: Domain, callback_function):
    lines, indents = __calculate_settings(domain.dimension, domain.size(), domain.counter.number, domain.periodicity())
    return Inline.generate(domain, (lines, indents), callback_function)


def generateNotRandom(domain: Domain, callback_function):
    if domain.order == 1:
        return generateChess(domain, callback_function)
    elif domain.order == 2:
        return generateInline(domain, callback_function)
    else:
        raise ValueError('Uncorrect order')
