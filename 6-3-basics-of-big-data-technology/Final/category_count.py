import sys
import time
from impala.dbapi import connect
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def getArgs():
    argv = sys.argv[1:]
    return argv[0], argv[1], argv[2]

def load_file(filename):
    dataSet = pd.read_csv(filename)
    return dataSet

def hive_connect(host="mzh-2019211397-0001", port=10000):
    return connect(host=host, port=port, database='default')

if __name__ == '__main__':
    host, port, file_path = getArgs()
    conn = hive_connect(host, port)
    df = load_file(file_path)
    movieId2Category = dict()
    count_result = dict()
    for i in df.columns[4:]:
        count_result[i] = 0
    for row in df.iterrows():
        movieId2Category[row[1]['movieId']] = row[1][4:].to_list()
    plt.ion()
    ax = df.columns[4:].to_list()
    while True:
        count_result = [0] * 19
        time.sleep(1800)
        with conn.cursor() as cursor:
            cursor.execute('SELECT movieId, count(*) FROM movie_rating_details GROUP BY movieId')
            for row in cursor:
                for i, value in enumerate(movieId2Category[row[0]]):
                    count_result[i] += value * row[1]
        print(count_result)
        plt.clf()
        plt.bar(ax,count_result) 
        plt.xticks(rotation=90)
        plt.pause(0.001)
