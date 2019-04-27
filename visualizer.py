# -*- coding: utf-8 -*-
"""Visualize CSV results of testbench.
"""

import matplotlib as mp
import numpy as np
import matplotlib.pyplot as plt

import numpy as np

def load_data(file: str):
    return np.genfromtxt(file,
                         delimiter='\t', names=['p', 'trips_min',
                                                'trips_max', 'trips_mean',
                                                'trips_med', 'trips_sd',
                                                'succ_mean', 'succ_med',
                                                'diag_mean', 'diag_med'])

simulations = [
    ("Cont-NoSubErr", "./simulations/Continuous.csv"),
    ("Cont-SubErr", "./simulations/Continuous_with_subsequent_corruptions.csv"),
    ("Ev-NoSubErr", "./simulations/Event-Based.csv"),
    ("Ev-SubErr", "./simulations/Event-Based_with_subsequent_corruptions.csv"),
]

def plot(name: str,data):
    print("Plotting " + name)
    plt.scatter(data["p"], data["trips_sd"])
    plt.scatter(data["p"], data["succ_mean"] * 75)
    plt.show()


    
def compare(data):
    fig, ax1 = plt.subplots()

    ax1.set_xlabel("Error chance per bit")
    ax1.set_ylabel("% Successful Transmissions Mean")
    ax1.scatter(data["Ev-SubErr"]["p"], data["Ev-SubErr"]["trips_sd"] * 100)
    #ax1.scatter(data["Cont-SubErr"]["p"], data["Cont-SubErr"]["succ_mean"] * 100)
    #ax1.scatter(data["Ev-SubErr"]["p"], data["Ev-SubErr"]["succ_mean"] * 100)

    ax2 = ax1.twinx()
    ax2.set_ylabel("Byte-Trips Mean (Event-based resending)", color='g')
    #ax2.scatter(data["Ev-SubErr"]["p"], data["Ev-SubErr"]["trips_mean"], color='g')
    ax2.scatter(data["Ev-SubErr"]["p"], data["Ev-SubErr"]["succ_mean"], color='g')

    plt.show()

data = {}

for sim in simulations:
    data[sim[0]] = load_data(sim[1])

compare(data)