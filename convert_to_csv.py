#-*-coding:gb2312-*-
import numpy as np
import pandas as pd
import glob
import uproot
import argparse
import random
import math
import awkward as ak
import sys

def tracks_from_file(tr_file, tr_ttree='event',
                     step_vals=None):
    tr_columns = step_vals
    print(f'uproot.open : {tr_file} . . . ')
    T = uproot.open(tr_file)
    if ('event;1' in T.keys()) == False:
        return 0
    ttree = uproot.open(tr_file)[tr_ttree]
    events = ttree.arrays(tr_columns)

    run_data = []
    for i in range(0,len(events)):
        temp = []
        for column in step_vals:
            temp.append(events[i][column])
        run_data.append(temp)
    dt = pd.DataFrame(run_data, columns=step_vals, dtype=object)
    dt = dt.dropna(axis=0) #删除含有缺失值的行

    return dt

def main():
    parser = argparse.ArgumentParser(description="Script to read tracks from MC output.")
    parser.add_argument('--InputFile', dest='input_file',
                            action='store', required=True,
                            help='Input file for processing')
    parser.add_argument('--OutputFile', dest='output_file',
                            action='store', required=True,
                            help='Output file path')

    step_vals = ['Energy','prex', 'prey', 'prez','postx',    #要读取的信息。
            'posty', 'postz', 'ptype', 'eventID',
            'trackID','parentID', 'dE','creatprosName','endprosName','tag','copyNo','time']
    df = pd.DataFrame()

    args = parser.parse_args(sys.argv[1:])
    output_file = args.output_file

    df = tracks_from_file(tr_file=args.input_file,
                           tr_ttree='event',
                           step_vals=step_vals)
    df.loc[:, 'step'] = 1
    for i in range(1,len(df)):
        if (df.loc[i].ptype == df.loc[i-1].ptype) & (df.loc[i].eventID == df.loc[i-1].eventID) &(df.loc[i].trackID == df.loc[i-1].trackID) &(df.loc[i].tag == df.loc[i-1].tag):
            df.loc[i,'step'] = df.loc[i-1,'step'] + 1
    """
    all_emit = 2000
    #duty_cicle_array = np.logspace(2,3.8,50)
    time_sample_array = []
    i = 0
    pulse_weight = 10
    pulse_count = 0
    sample_number = 1000
    expectation = 6.5
    while i < all_emit:
        for j in range(0,sample_number): 
            temp = random.uniform(0, sample_number-1) #each pulse
            if (temp < expectation):
                time_sample_array.append((pulse_count*4400 + j/sample_number*pulse_weight)*1e-6)
                falg = 1
                i = i+1
                if i == all_emit:
                    break
        pulse_count = pulse_count + 1
    
    for i in range(len(df)):
        df.loc[i,'time'] = df.loc[i,'time'] + time_sample_array[df.loc[i,'eventid']]

    df.time = df.time*1e9
    df.time = df.time + 1e9
    """
    df.to_csv(output_file, index=False)

if __name__ == '__main__':
    main()