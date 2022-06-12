import os

os.environ['OMP_NUM_THREADS'] = '1'

import numpy as np
from ctypes import *
import math
from numpy.random import RandomState, MT19937
import time
from argparse import ArgumentParser
import sys
from matplotlib import pyplot as plt
from scipy.cluster.hierarchy import dendrogram
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

# ================ Drawing =====================
clustering.Init.argtypes = c_int, c_int, c_int, c_char_p
clustering.Visualize.argtypes = POINTER(c_float), c_int, POINTER(c_int)

# ================ KMeans  =====================
clustering.KMeansInit.argtypes = c_int, c_int, c_int
clustering.KMeansInit.restype = c_void_p

clustering.KMeansFit.argtypes = c_void_p, POINTER(c_float), c_int, c_int

clustering.KMeansPredict.argtypes = c_void_p, POINTER(c_float), c_int, c_int
clustering.KMeansPredict.restype = POINTER(c_int)

clustering.KMeansCleanup.argtypes = (c_void_p, )

# ================ Hierarchical ================
clustering.HierarchicalInit.argtypes = (c_int, )
clustering.HierarchicalInit.restype = c_void_p

clustering.HierarchicalFit.argtypes = c_void_p, POINTER(c_float), c_int, c_int
clustering.HierarchicalFit.restype = POINTER(c_int)

clustering.HierarchicalCleanup.argtypes = (c_void_p, )

# ================ DBSCAN  =====================
clustering.DBSCANInit.argtypes = c_float, c_int
clustering.DBSCANInit.restype = c_void_p

clustering.DBSCANFit.argtypes = c_void_p, POINTER(c_float), c_int, c_int
clustering.DBSCANFit.restype = POINTER(c_int)

clustering.DBSCANCleanup.argtypes = (c_void_p, )

samples = 25000
iterations = 300
center_count = 5
center_coords = [[-5, 0], [5, 0], [0, 5]]
x, y = make_blobs(n_samples = samples, centers = center_count, random_state = 0)
# x, y = make_blobs(n_samples = samples, centers = center_coords, random_state = 0)

class Custom_KMeans():

    def __init__(self, cluster_count, random_state, max_iterations):
        self.state = clustering.KMeansInit(cluster_count, random_state, max_iterations)

    def fit(self, data):
        if data.dtype == np.float64:
            data = np.array(data, dtype = np.float32)
        clustering.KMeansFit(self.state, data.flatten().ctypes.data_as(POINTER(c_float)), data.shape[0], data.shape[1])
        return self

    def predict(self, data):
        if data.dtype == np.float64:
            data = np.array(data, dtype = np.float32)
        predicted_pointer = clustering.KMeansPredict(self.state, data.flatten().ctypes.data_as(POINTER(c_float)), data.shape[0], data.shape[1])
        return np.ctypeslib.as_array(predicted_pointer, shape = (data.shape[0], ))

    def __del__(self):
        clustering.KMeansCleanup(self.state);

class Custom_Hierarchical():

    def __init__(self, cluster_count):
        self.state = clustering.HierarchicalInit(cluster_count)
        self.labels = 0

    def fit(self, data):
        if data.dtype == np.float64:
            data = np.array(data, dtype = np.float32)
        labels = clustering.HierarchicalFit(self.state, data.T.flatten().ctypes.data_as(POINTER(c_float)), data.shape[0], data.shape[1])
        self.labels = np.ctypeslib.as_array(labels, shape = (data.shape[0], ))
        return self
    
    def __del__(self):
        clustering.HierarchicalCleanup(self.state)

class Custom_DBSCAN():

    def __init__(self, eps, min_points):
        self.state = clustering.DBSCANInit(eps, min_points)
        self.labels = 0

    def fit(self, data):
        if data.dtype == np.float64:
            data = np.array(data, dtype = np.float32)
        labels = clustering.DBSCANFit(self.state, data.T.flatten().ctypes.data_as(POINTER(c_float)), data.shape[0], data.shape[1])
        self.labels = np.ctypeslib.as_array(labels, shape = (data.shape[0], ))
        return self

    def __del__(self):
        clustering.DBSCANCleanup(self.state);

random_seed = 0

if run_custom:
    start = 0
    end = 0
    if algorithm == 'kmeans':
        print("Running custom K-Means")
        start = time.perf_counter()
        k = Custom_KMeans(cluster_count = center_count, random_state = random_seed, max_iterations = iterations).fit(x)
        y = k.predict(x)
        end = time.perf_counter()
    
    if algorithm == 'hierarchical':
        print("Running custom hierarchical")
        start = time.perf_counter()
        hierarchical = Custom_Hierarchical(cluster_count = center_count).fit(x)
        y = hierarchical.labels
        end = time.perf_counter()

    if algorithm == 'dbscan':
        print("Running custom DBSCAN")
        start = time.perf_counter()
        dbscan = Custom_DBSCAN(eps = 0.5, min_points = 5).fit(x)
        y = dbscan.labels
        end = time.perf_counter()

    print(f"Custom {algorithm} took  {end - start} seconds")
else:
    start = 0
    end = 0
    if algorithm == 'kmeans':
        print("Running K-Means from sklearn")
        start = time.perf_counter()
        kmeans = KMeans(n_clusters = center_count, random_state = random_seed, max_iter = iterations, tol = 0.0, init = 'random', n_init = 1).fit(x)
        y = kmeans.predict(x)
        end = time.perf_counter()
        print("Finished after", kmeans.n_iter_, "iterations")
        print("Used", kmeans._n_threads, "threads")

    if algorithm == 'hierarchical':
        print("Running hierarchical from sklearn")
        start = time.perf_counter()
        hierarchical = AgglomerativeClustering(n_clusters = center_count, linkage = 'average', compute_full_tree = False).fit(x)
        y = hierarchical.labels_
        end = time.perf_counter()
        # For dendrogram only
        # hierarchical = AgglomerativeClustering(distance_threshold = 0, n_clusters = None, linkage = 'average').fit(x)
        # plt.figure(figsize = (5, 10))
        # plot_dendrogram(hierarchical)
        # plt.show()

    if algorithm == 'dbscan':
        print("Running DBSCAN from sklearn")
        start = time.perf_counter()
        dbscan = DBSCAN(algorithm = 'brute', n_jobs = 8).fit(x)
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

def plot_dendrogram(model, **kwargs):
    # Create linkage matrix and then plot the dendrogram

    # create the counts of samples under each node
    counts = np.zeros(model.children_.shape[0])
    n_samples = len(model.labels_)
    for i, merge in enumerate(model.children_):
        current_count = 0
        for child_idx in merge:
            if child_idx < n_samples:
                current_count += 1  # leaf node
            else:
                current_count += counts[child_idx - n_samples]
        counts[i] = current_count

    linkage_matrix = np.column_stack(
        [model.children_, model.distances_, counts]
    ).astype(float)

    # Plot the corresponding dendrogram
    dendrogram(linkage_matrix, **kwargs)
