import rk_ext

def main():
    bt = [[   0.,   0.,   0.,   0.,   0.],
          [ 1/2., 1/2.,   0.,   0.,   0.],
          [ 1/2.,   0., 1/2.,   0.,   0.],
          [   1.,   0.,   0.,   1.,   0.],
          [   0., 1/6., 1/3., 1/3., 1/6.]]

    lowerBound = 0
    upperBound = 2
    initialValue = 1

    RK4 = rk_ext.RKIntegrator(func, bt, 1000, lowerBound, upperBound, initialValue)
    RK4.run()


def func(t, x):
    return x/(1 + t)

main()
