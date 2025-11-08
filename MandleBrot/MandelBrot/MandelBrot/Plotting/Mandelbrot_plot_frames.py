#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image
import pandas as pd
import sys

###########################################################################
def apply_colormap(c_array,cmap):

	cmap = plt.get_cmap('inferno')

	rgba = cmap(c_array)
	
	return rgba[:,:,:3]


##########################################################################
def read_data(datafile,x_dim,y_dim,min_c,max_c):
	
	print("\tStart reading the data file")
	df = pd.read_csv(datafile,header=None)
	print("\tFinished reading the data file")

	if df.shape[0] != y_dim or df.shape[1] != x_dim :
		raise ValueError(f"Unexpected dimensions: got ({df.shape[0]}, {df.shape[1]}) but expected ({y_dim}, {x_dim})")
	
	data_array = df.values
	data = np.zeros((y_dim,x_dim,3))

	norm = (data_array - min_c)/(max_c - min_c)
	norm = np.clip(norm,0,1)
	
	
	cmap = plt.get_cmap("rainbow")

	data = apply_colormap(norm,cmap)


	return data

##########################################################################

datafile = sys.argv[1]
frame_init, frame_stride, frame_last = int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])
x_dim, y_dim = int(sys.argv[5]), int(sys.argv[6])
c_min, c_max = 0 , 1
outputwidth , outputheight = 16, 9

##########################################################################
for frame_i in range(frame_init,frame_last+1,frame_stride):
	datafile_i = datafile + "-" + str(frame_i) + ".dat"
	outputfile_i = datafile_i.replace(".dat","")

##########################################################################
	print(f"Processing the data file {datafile_i}")
	data = read_data(datafile_i,x_dim,y_dim,c_min,c_max)
	print("\tFinished processing the data file")
##########################################################################
	print("\tPlotting the data")
	fig, ax = plt.subplots()
	ax.imshow(data)
	plt.xticks([])
	plt.yticks([])
##########################################################################
	plt.subplots_adjust(left=0.,
                    bottom=0., 
                    right=1., 
                    top=1., 
                    wspace=0, 
                    hspace=0)
##########################################################################
	print("\tSaving the plot")
	fig.set_size_inches(outputwidth,outputheight)
	fig.savefig(outputfile_i+".svg",dpi=100)
##########################################################################
	plt.close()
##########################################################################
print("All done!")
