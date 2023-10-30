from random import random

from .Classes import Domain

from matplotlib import pyplot, patches

def generate(domain: Domain, settings: tuple, callback_function):
    obstacles = []
    lines, indents = settings
    # print(lines)
    if lines[0] % 2 == 0:
        domain.periodicity.x = False
    if lines[1] % 2 == 0:
        domain.periodicity.y = False
    if lines[2] % 2 == 0:
        domain.periodicity.z = False
    # indents = [domain.size[i] / (lines[i] + (1 if not domain.periodicity[i] else -1)) for i in range(domain.dimension)]
    # if domain.dimension == 2:
    #     indents.append(0)
    adder = [
        0 if domain.periodicity.x else 1,
        0 if domain.periodicity.y else 1,
        0 if domain.periodicity.z else 1
    ]
    print(adder)
    for i in range(lines[0]):
        for j in range(lines[1]):
            for k in range(lines[2]):
                if len(obstacles) == domain.counter.number:
                    break
                index = (i + j + k) % 2
                if index == 0:
                    radius = random() * (domain.radius.max - domain.radius.min) + domain.radius.min
                    obstacles.append(
                        (
                            (i + adder[0]) * indents[0],
                            (j + adder[1]) * indents[1],
                            (k + adder[2]) * indents[2],
                            radius
                        )
                    )
    ax, fig = pyplot.subplots()
    for obstacle in obstacles:
        fig.add_patch(
            patches.Circle(
                (obstacle[0], obstacle[1]), obstacle[3]
            )
        )
    pyplot.show()
