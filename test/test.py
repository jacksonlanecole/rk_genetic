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

    RK4_1D = rk_ext.RKIntegrator(func, bt, 10000, lowerBound, upperBound, initialValue)
    #while (not RK4_1D.isFinished()):
    #    RK4_1D.step()

    xVec =  RK4_1D.get_xVec()
    dxVec = RK4_1D.get_dxVec()
    tVec =  RK4_1D.get_tVec()

    #print(RK4_1D.get_last(2))

    lowerBound = 0.
    upperBound = 2.
    initialValue = [1., 1., 1., 1., 1.]
    RK4_2D = rk_ext.RKIntegrator(func, bt, 10000., lowerBound, upperBound)
    x = initialValue

    while (not RK4_2D.isFinished()):
        x = RK4_2D.step(x)

    print(x)

def func(t, x):
    return x/(1 + t)

main()
