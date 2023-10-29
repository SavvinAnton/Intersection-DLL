from ctypes import (
    Structure,
    c_longdouble,
    c_bool,
    c_short,
    c_uint,
    c_int,
    c_bool,
)


class Point(Structure):
    _fields_ = [
        ('x', c_longdouble),
        ('y', c_longdouble),
        ('z', c_longdouble),
    ]

    def __repr__(self) -> str:
        return f'({self.x}, {self.y}, {self.z})'
    
    def __call__(self) -> tuple:
        return self.x, self.y, self.z


class Periodicity(Structure):
    _fields_ = [
        ('x', c_bool),
        ('y', c_bool),
        ('z', c_bool),
    ]
    
    def __call__(self) -> tuple:
        return self.x, self.y, self.z


class Radius(Structure):
    _fields_ = [
        ('min', c_longdouble),
        ('max', c_longdouble),
    ]
    
    def __call__(self) -> tuple:
        return self.min, self.max


class Counter(Structure):
    _fields_ = [
        ('type', c_short),
        ('number', c_uint),
        ('porosity', c_longdouble),
    ]
    
    def __call__(self) -> tuple:
        return self.type, self.number, self.porosity


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
        ('exact_count', c_bool),
        ('order', c_short),
    ]


class Obstacle(Structure):
    _fields_ = [
        ('center', Point),
        ('radius', c_longdouble),
    ]

    def __repr__(self) -> str:
        return f'{self.center} ({self.radius})'
    
    def __call__(self) -> tuple:
        return self.center, self.radius
