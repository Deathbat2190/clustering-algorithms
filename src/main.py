import numpy as np
from ctypes import *
import math
from numpy.random import RandomState, MT19937
import time
from argparse import ArgumentParser
import sys

# import os
# print(os.getenv("APPDATA"))
# print(sys.path.append(os.getenv("APPDATA") + "\\Python\\Lib\\site-packages"))
# print(sys.path)

# from sklearnex import patch_sklearn
# patch_sklearn()
from sklearn.datasets import make_blobs
from sklearn.cluster import KMeans, AgglomerativeClustering, DBSCAN

argument_parser = ArgumentParser()
argument_parser.add_argument("-c", "--custom", action='store_true')
argument_parser.add_argument("algorithm")
args = argument_parser.parse_args()
run_custom = args.custom
algorithm = args.algorithm
algorithm = algorithm.lower()

if algorithm not in ['kmeans', 'hierarchical', 'dbscan', 'none']:
    print(f'Algorithm {algorithm} not available.')
    exit()

clustering_dll = "build/clustering.dll"
clustering = CDLL(clustering_dll)

clustering.Init.argtypes = c_int, c_int, c_int, c_char_p
clustering.Visualize.argtypes = POINTER(c_float), c_int, POINTER(c_int)

clustering.InitKMeans.argtypes = c_int, c_int, c_int
clustering.InitKMeans.restype = c_void_p

clustering.Fit.argtypes = c_void_p, POINTER(c_float), c_int, c_int

clustering.Predict.argtypes = c_void_p, POINTER(c_float), c_int, c_int
clustering.Predict.restype = POINTER(c_int)

clustering.KMeansCleanup.argtypes = (c_void_p, )

samples = 10000
iterations = 300
center_count = 3
center_coords = [[-5, 0], [5, 0], [0, 5]]
x, y = make_blobs(n_samples = samples, centers = center_coords, random_state = 0)

class K_Means():

    def __init__(self, cluster_count, random_state, max_iterations):
        self.state = clustering.InitKMeans(cluster_count, random_state, max_iterations);

    def fit(self, data):
        if data.dtype == np.float64:
            data = np.array(data, dtype = np.float32)
        clustering.Fit(self.state, data.flatten().ctypes.data_as(POINTER(c_float)), data.shape[0], data.shape[1])
        return self

    def predict(self, data):
        if data.dtype == np.float64:
            data = np.array(data, dtype = np.float32)
        predicted_pointer = clustering.Predict(self.state, data.flatten().ctypes.data_as(POINTER(c_float)), data.shape[0], data.shape[1])
        return np.ctypeslib.as_array(predicted_pointer, shape = (data.shape[0], ))

    def __del__(self):
        clustering.KMeansCleanup(self.state);

if run_custom:
    start = 0
    end = 0
    if algorithm == 'kmeans':
        print("Running custom K-Means")
        start = time.perf_counter()
        k = K_Means(cluster_count = center_count, random_state = 0, max_iterations = iterations).fit(x)
        y = k.predict(x)
        end = time.perf_counter()
    
    if algorithm == 'hierarchical':
        print("Running custom hierarchical")
        start = time.perf_counter()
        end = time.perf_counter()

    if algorithm == 'dbscan':
        print("Running custom DBSCAN")
        start = time.perf_counter()
        end = time.perf_counter()

    print(f"Custom {algorithm} took  {end - start} seconds")
else:
    start = 0
    end = 0
    if algorithm == 'kmeans':
        print("Running K-Means from sklearn")
        start = time.perf_counter()
        kmeans = KMeans(n_clusters = center_count, random_state = 0, max_iter = iterations, init = 'random').fit(x)
        y = kmeans.predict(x)
        end = time.perf_counter()

    if algorithm == 'hierarchical':
        print("Running hierarchical from sklearn")
        start = time.perf_counter()
        hierarchical = AgglomerativeClustering(n_clusters = center_count, linkage = 'average').fit(x)
        y = hierarchical.labels_
        end = time.perf_counter()

    if algorithm == 'dbscan':
        print("Running DBSCAN from sklearn")
        start = time.perf_counter()
        dbscan = DBSCAN().fit(x)
        y = dbscan.labels_
        end = time.perf_counter()

    print(f"Sklearn {algorithm} took {end - start} seconds")

windowWidth = 1920
windowHeight = 1920

x = x.flatten()
x = np.array(x, dtype = np.float32)
x_pointer = x.ctypes.data_as(POINTER(c_float))

y = np.array(y, dtype = np.int32)
y_pointer = y.ctypes.data_as(POINTER(c_int))

window_title = ""
if run_custom:
    window_title = "Praca Magisterska - Custom"
else:
    window_title = "Praca Magisterska - Sklearn"

clustering.Init(windowWidth, windowHeight, 60, create_string_buffer(window_title.encode("UTF-8")))
clustering.Visualize(x_pointer, samples, y_pointer)
