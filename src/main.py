import numpy as np
from ctypes import *

test_dll = "build/test.dll"
test = CDLL(test_dll)

array1 = np.array([[1, 2], [3, 4], [5, 6]])
test.test(array1.ctypes.data_as(POINTER(c_int)), array1.shape[1], array1.shape[0])

array2 = [2, 3, 4, 5]
test.test((c_int * len(array2))(*array2), 2, 2)
