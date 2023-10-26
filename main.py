from VPGen import *

def main():
    data = loadGeometry('./data.json')
    domain = createDomain(data)
    obstacles, number, porosity, time = generate(domain)
    print('Obstacles:', number, 'Porosity:', round(porosity, 3), 'Time:', round(time, 1))
    plot2d(domain, obstacles, savefig='domain', indent=False)
    saveObstacles('./obstacles.json', obstacles)


if __name__ == '__main__':
    main()
