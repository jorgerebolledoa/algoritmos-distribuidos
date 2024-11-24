import numpy as np
a = np.array([[1, 0,0,0,0,0,0], [0,1,0,0,0,0,0], [0,0,1,0,0,0,0],[0,0,0,1,0,0,0]])
b = np.array([1, 2])
x = np.linalg.solve(a, b)
x
array([-1.,  1.])