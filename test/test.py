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

    RK4 = rk_ext.RKIntegrator(func, bt, 10000, lowerBound, upperBound, initialValue)
    while (not RK4.isFinished()):
        RK4.step()

    xVec = RK4.get_xVec()
    dxVec = RK4.get_dxVec()
    tVec = RK4.get_tVec()


def func(t, x):
    return x/(1 + t)

main()
