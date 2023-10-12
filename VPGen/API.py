from ctypes import (
    Structure,
    c_longdouble,
    c_bool,
    c_short,
    c_uint,
    CDLL,
    POINTER,
    c_int,
    CFUNCTYPE,
    c_void_p,
)
from json import load
from os.path import abspath, dirname
from matplotlib import pyplot, patches
from time import perf_counter


class Point(Structure):
    _fields_ = [
        ('x', c_longdouble),
        ('y', c_longdouble),
        ('z', c_longdouble),
    ]

    def __repr__(self):
        return f'({self.x}, {self.y}, {self.z})'


class Periodicity(Structure):
    _fields_ = [
        ('x', c_bool),
        ('y', c_bool),
        ('z', c_bool),
    ]


class Radius(Structure):
    _fields_ = [
        ('min', c_longdouble),
        ('max', c_longdouble),
    ]


class Counter(Structure):
    _fields_ = [
        ('type', c_short),
        ('number', c_uint),
        ('porosity', c_longdouble),
    ]


class Domain(Structure):
    _fields_ = [
        ('dimension', c_int),
        ('origin', Point),
        ('size', Point),
        ('indent', Point),
        ('periodicity', Periodicity),
        ('radius', Radius),
        ('counter', Counter),
        ('minimum_distance', c_longdouble),
        ('iterations', c_uint),
    ]


class Obstacle(Structure):
    _fields_ = [
        ('center', Point),
        ('radius', c_longdouble),
    ]

    def __repr__(self):
        return f'{self.center} ({self.radius})'


def loadLibrary(filename):
    lib = CDLL(filename)
    lib.generate.argtypes = (Domain, CALLBACK_TYPE)
    lib.generate.restype = POINTER(c_longdouble * 2)
    return lib


def createDomain(data):
    return Domain(
            c_int(data['dimension']),
            Point(*data['origin']),
            Point(*data['size']),
            Point(*data['indent']),
            Periodicity(*data['periodicity']),
            Radius(
                c_longdouble(min(data['radius_range'])),
                c_longdouble(max(data['radius_range']))
            ),
            Counter(
                c_short(data['type']),
                c_uint(data['number']),
                c_longdouble(data['porosity'])
            ),
            c_longdouble(data['minimum_distance']),
            c_uint(data['iterations']),
        )


def loadGeometry(filename):
    with open(filename) as file:
        data = load(file)
    return data


def generateFromFile(filename):
    data = loadGeometry(filename)
    generateFromDict(data)


def generateFromDict(data):
    domain = createDomain(data)
    generate(domain)


def generate(domain):
    library_path = dirname(abspath(__file__))
    lib = loadLibrary(f'{library_path}/VPGen.so')
    callback = CALLBACK_TYPE(printer)

    start = perf_counter()
    obstacles_number, porosity = lib.generate(domain, callback).contents
    end = perf_counter()
    obstacles_number = int(obstacles_number)
    print('Obstacles:', obstacles_number, 'Porosity:', round(porosity, 3), 'Time:', round(end - start, 1))
    lib.getObstacles.restype = POINTER(Obstacle * obstacles_number)
    obstacles = lib.getObstacles().contents
    # print(*obstacles, sep='\n')
    plot2d(domain, obstacles, savefig='domain', indent=False)


def plot2d(domain, obstacles, savefig=None, indent=True):
    ax, fig = pyplot.subplots()

    for obstacle in obstacles:
        fig.add_patch(
            patches.Circle(
                (obstacle.center.x, obstacle.center.y), obstacle.radius
            )
        )
    fig.set_xlim(
        domain.origin.x + (domain.indent.x if indent else 0),
        domain.origin.x + domain.indent.x + (domain.indent.x if not indent else 0) + domain.size.x
    )
    fig.set_ylim(
        domain.origin.y + (domain.indent.y if indent else 0),
        domain.origin.y + domain.indent.y + (domain.indent.y if not indent else 0) + domain.size.y
    )

    if savefig is None:
        pyplot.show()
    else:
        ax.savefig(savefig + '.png')


def plot3d(domain, obstacles):
    ...  # TODO make using vtk


def printer(obstacles, porosity):
    # print(f'Obstacles: {obstacles}, porosity: {porosity}')
    ...


CALLBACK_TYPE = CFUNCTYPE(c_void_p, c_int, c_longdouble)
