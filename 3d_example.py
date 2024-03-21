from VPGen import *
import gmsh

def main():
    '''
    type: 6 - ellipsoid,
    type: 9 - dodecahedron
    '''
    data = loadGeometry('./data3d.json')
    domain = createDomain(data)
    generate3d(domain)
    # obstacles, number, porosity, time = generate(domain)
    # print('Obstacles:', number, 'Porosity:', round(porosity, 3), 'Time:', round(time, 1))
    # # plot2d(domain, obstacles, savefig='domain', indent=False)
    # plot3d(domain, obstacles)
    # saveObstacles('./obstacles.json', obstacles)


if __name__ == '__main__':
    main()
