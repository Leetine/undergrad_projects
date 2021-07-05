# -*- coding: utf-8 -*-
"""
Created on Thu May  6 17:13:49 2021

@author: LPC
"""

import cv2 as cv
import numpy as np


def zmMinFilterGray(src, r=7):
    '''最小值滤波，r是滤波器半径'''
    return cv.erode(src, np.ones((2 * r + 1, 2 * r + 1)))


filename='results/iter_cp/5wall_interim_result.png'

m=cv.imread(filename,cv.IMREAD_COLOR)

V1 = np.min(m, 2)                     
      
Dark_Channel = zmMinFilterGray(V1, 7)       # 得到暗通道图像

cv.imwrite('./results/iter_cp/'+filename[16:-4]+'_Dark_Channel.png',Dark_Channel)

