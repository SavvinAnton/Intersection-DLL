from ctypes import (
    Structure,
    c_longdouble,
    c_bool,
    c_short,
    c_uint,
    c_int,
)


class Point(Structure):
    _fields_ = [
        ('x', c_longdouble),
        ('y', c_longdouble),
        ('z', c_longdouble),
    ]

    def __repr__(self) -> str:
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

    def __repr__(self) -> str:
        return f'{self.center} ({self.radius})'
