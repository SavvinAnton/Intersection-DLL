from VPGen import *

def main():
    domain = loadGeometry('./test_data.json')
    obstacles, number, porosity, time = generate(domain)
    print('Obstacles:', number, 'Porosity:', round(porosity, 3), 'Time:', round(time, 1))
    # plot2d(domain, obstacles, savefig='domain', indent=False)


if __name__ == '__main__':
    main()
