from ctypes import Structure, c_longdouble, CDLL, POINTER, c_int, c_bool, c_short, c_uint32, c_uint
import matplotlib.pyplot as plt
import matplotlib.patches as ptc

import vars


class Point(Structure):
    _fields_ = [
        ('x', c_longdouble),
        ('y', c_longdouble),
        ('z', c_longdouble)
    ]

    def __str__(self) -> str:
        return f'({self.x}, {self.y}, {self.z})'


class Obstacle(Structure):
    _fields_ = [
        ('point', Point),
        ('r', c_longdouble)
    ]

    def __str__(self) -> str:
        return f'{self.point} ({self.r})'


class Rect(Structure):
    _fields_ = [
        ('origin', Point),
        ('size', Point)
    ]

    def __str__(self) -> str:
        return f'{self.origin} {self.size}'


class Radius(Structure):
    _fields_ = [
        ('min', c_longdouble),
        ('max', c_longdouble)
    ]

    def __str__(self) -> str:
        return f'({self.min}, {self.max})'


class Period(Structure):
    _fields_ = [
        ('x', c_bool),
        ('y', c_bool),
        ('z', c_bool)
    ]

    def __str__(self) -> str:
        return f'({self.x}, {self.y}, {self.z})'


class Count(Structure):
    _fields_ = [
        ('type', c_short),
        ('obstacles', c_uint32),
        ('porosity', c_longdouble)
    ]

    def __str__(self) -> str:
        return f'({self.type}, {self.obstacles}, {self.porosity})'


class Indent(Structure):
    _fields_ = [
        ('x', c_longdouble),
        ('y', c_longdouble),
        ('z', c_longdouble)
    ]

    def __str__(self) -> str:
        return f'({self.x}, {self.y}, {self.z})'


def createDomain(geometry_data: dict) -> tuple:
    return (
        Rect(Point(*geometry_data['origin']), Point(*geometry_data['size'])),
        Count(geometry_data['numberPorosityMax'], geometry_data['number'], geometry_data['porosity']),
        Radius(min(geometry_data['radius_range']), max(geometry_data['radius_range'])),
        Period(*geometry_data['periodicity']),
        Indent(*geometry_data['indent']),
        c_longdouble(geometry_data['minimum_distance']),
        c_uint(geometry_data['maximum_iterations'])
    )


def generate(geometry_data, lib=None, domain=None) -> tuple:
    if lib is None:
        lib = loadLibrary()
        lib.updateSrand()

    if domain is None:
        domain = createDomain(geometry_data)
    lib.generate(*domain)

    lib.getObstacles.restype = POINTER(Obstacle * lib.length())
    obstacles: list = lib.getObstacles().contents

    return obstacles


def loadLibrary(lib_path: str=vars.lib_path) -> CDLL:
    lib = CDLL(lib_path)

    lib.length.restype = c_int
    lib.generate.argtypes = Rect, Count, Radius, Period, Indent, c_longdouble, c_uint

    return lib


def plot(obstacles, domain: Rect=None, indent: Indent=None) -> None:
    current_axis = plt.gca()

    for obstacle in obstacles:
        current_axis.add_patch(ptc.Circle((obstacle.point.x, obstacle.point.y), obstacle.r, color = "red"))

    fig = plt.gcf()
    fig.set_size_inches((9, 3), forward=True)
    plt.xlim(0, 3)
    plt.show()
