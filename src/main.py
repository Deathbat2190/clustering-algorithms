import numpy as np
from ctypes import *
import math
from sklearn.datasets import make_blobs

test_dll = "build/test.dll"
test = CDLL(test_dll)

# array1 = np.array([[1, 2], [3, 4], [5, 6]])
# test.test(array1.ctypes.data_as(POINTER(c_int)), array1.shape[1], array1.shape[0])

# array2 = [2, 3, 4, 5]
# test.test((c_int * len(array2))(*array2), 2, 2)
# test.createIntArray.argtypes = c_int, c_int;
# test.createIntArray.restype = POINTER(c_int);
# array = test.createIntArray(5, 8);
# print(type(array))
# nparray = np.ctypeslib.as_array(array, shape=(5, 8));
# for i in range(5):
#     for j in range(8):
#         nparray[i][j] = i + j;
# test.test(nparray.ctypes.data_as(POINTER(c_int)), 5, 8);
# test.test(array, 5, 8);
# test.freeIntArray(array);

test_data = []
for x in np.arange(-10, 10, 0.01):
    if x == 0:
        continue
    test_data.append([float(x), float(x * math.sin(1.0 / x))])
test_data = np.array(test_data, dtype=np.float32)
test_data_pointer = test_data.ctypes.data_as(POINTER(c_float))

samples = 1000
center_count = 8
x, y = make_blobs(n_samples = samples, centers = center_count, random_state = 0)
x = x.flatten()
x = np.array(x, dtype=np.float32)

x_pointer = x.ctypes.data_as(POINTER(c_float))
y_pointer = y.ctypes.data_as(POINTER(c_int))
test.visualize.argtypes = POINTER(c_float), c_int
test.visualize(x_pointer, samples, y_pointer)

