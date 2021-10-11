# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

dir_result = './'
info = np.loadtxt(dir_result + 'info.dat')
with open(dir_result + 'waveform.dat') as f:
    waveform_lines = f.readlines()
n_batch = len(waveform_lines)

#sample_rate = 4 # single-channel
sample_rate = 1 # ABCD mode

info = info[0:6]
waveform_lines = waveform_lines[0:6]

idx_card = info[:,0] # 0-3 for the ADC input channel, 4 for the TDC, 5 for the time stamp
idx_channel = info[:,1]
# flags = info[:,2]
pkt_length = info[:,3] # number of 64-bit elements, each containing 4 samples
# therefore, 4*pkt_length = No. of data each line in "waveform.dat"
time_stamp = info[:,4] # timestamp of the last word in the packet in ps
start_time = time_stamp - 3200. * (pkt_length - 1) / sample_rate
end_time = time_stamp + 3200. / sample_rate

# the row of data from card 0
idx_card_0 = np.nonzero(idx_card==0)[0]
# the row of data from card 1
idx_card_1 = np.nonzero(idx_card==1)[0]

def plot_signals(idx):
    data = np.fromstring(waveform_lines[idx], dtype=float, sep=' ')
    time = np.arange(start_time[idx], end_time[idx], 3200 / 4. / sample_rate) 
    # 4-channel: 3200 / 4.; 1-channel: 3200 / 4. / 4
    # NOTE: each interval is fixed to 3200 ps. For different modes, this interval 
    # contains different numbers of samples: 
    # For the single-channel mode, the 3200 ps interval contains 16 samples; 
    # for ABCD mode, the 3200 ps interval contains 4 samples. 
    # See Fig. 2.12-2.14 of manual for detailed description.

    plt.plot((time)/1000., data, '.') # ps to ns
    #plt.plot((time-start_time[idx])/1000/4., data, '.') # note the channel, and ps to ns
    
def plot_time_stamps(dir_result):
    plt.plot((time_stamp[25:65]-time_stamp[0])/1e6, '.-') # ps to us
    plt.show()
    
    

# plot_time_stamps(dir_result)

#for i in range(3): # n_batch

for i in idx_card_0:
    plot_signals(i)
    
for i in idx_card_1:
    plot_signals(i)
    
#for i in [idx_card_1[0]]:
#    plot_signals(i)
    
plt.grid()
plt.xlabel('Time (ns)')
plt.ylabel('Signal (Arb. units)')
#plt.savefig('burst-sin-nc3.png', dpi=300)
plt.show()
