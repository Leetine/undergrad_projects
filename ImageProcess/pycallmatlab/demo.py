# -*- coding: utf-8 -*-

"""
Created on Thu Apr 15 16:58:25 2021

@author: LPC
"""
"""
User specified parameter:
Kernel estimation part:
'gamma_correct': gamma correction for the input image (typically set as 1 and 2.2)
'kernel_size':   the size of blur kernel
'lambda_dark':   the weight for the L0 regularization on dark channel prior (typically set as 4e-3)
'lambda_grad':   the weight for the L0 regularization on gradient (typically set as 4e-3)
Non-blind deconvolution part:
Our algorithm is mainly designed for blur kernel esitmation part. 
For the non-blind deblurring, we use existing methods to genetate the final results for fair comparisons.
"""

import matlab.engine
from datetime import datetime
import numpy as np
import matlab
import cv2 as cv

eng = matlab.engine.start_matlab()
#eng.demo_deblurring(nargout=0)
#eng.quit()

#是否过饱和
saturation=0
#暗通道权值 
lambda_dark=0.004
#梯度权值
lambda_grad=0.004
#tv正则化参数
lambda_tv=0.001
#l0正则化参数
lambda_l0=0.0005
#是否去除振铃效应
weight_ring=1.0

#filename:文件名
#prescale： 
#xt_iter:迭代次数
#gamma_correct：1代表不进行灰度拉伸，<1更亮，>1更暗
#k_thresh：阈值
#kernel_size：模糊核尺寸

#和纯MATLAB参数一样，但是结果有一些差异
#filename='image/1_pattt_use.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":2.2,"k_thresh":20,"kernel_size":55.0}
#lambda_tv=0.01 #混编需要重新调整参数以估计清晰的模糊核
#lambda_l0=0.002 #可以减少反卷积阶段的振铃效应，视觉效果尚可

#
#filename='image/IMG_4548_small.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":1.0,"k_thresh":20,"kernel_size":55.0} 
#saturation=1

filename='image/IMG_1240_blur.png'
opts={"prescale":1,"xk_iter":5,"gamma_correct":1.0,"k_thresh":20,"kernel_size":45.0} 

#filename='image/real_img2.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":2.2,"k_thresh":20,"kernel_size":25.0} 


#filename='image/Blurry1_6.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":1.0,"k_thresh":20,"kernel_size":41.0} 

#filename='image/Blurry1_9.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":1.0,"k_thresh":20,"kernel_size":159.0} 

#filename='image/Blurry3_8.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":1.0,"k_thresh":20,"kernel_size":189.0} 

#filename='image/256_256.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":2.2,"k_thresh":20,"kernel_size":55.0} 
#lambda_tv = 0.001
#lambda_l0 =0.002
#
#filename='image/512_512.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":2.2,"k_thresh":20,"kernel_size":55.0} 
#lambda_tv = 0.001
#lambda_l0 =0.002

#filename='image/1024_1024.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":2.2,"k_thresh":20,"kernel_size":55.0} 
#lambda_tv = 0.001
#lambda_l0 =0.002

#filename='image/7_patch_use.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":1.0,"k_thresh":20,"kernel_size":85.0}
#lambda_tv = 0.01; lambda_l0 = 0.002;

#filename='image/wall.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":1.0,"k_thresh":20,"kernel_size":65.0}
#lambda_tv=0.0001
#lambda_l0=0.002

#filename='image/Blurred img.png'
#opts={"prescale":1,"xk_iter":5,"gamma_correct":1.0,"k_thresh":20,"kernel_size":25.0}

##================Estimate the blur kernel(based on the dark channel priority)===================
def im2double(im):
  min_val = np.min(im.ravel())
  max_val = np.max(im.ravel())
  out = (im.astype('float') - min_val) / (max_val - min_val)
  return out

# 读入图像
y=cv.imread(filename,cv.IMREAD_COLOR)
print("Successfully load the Image")

# 如果是彩色图像(存在channel)，则先转为灰度图像
# 将图像转为double格式（之前为uint8格式）并且值缩放到0~1,储存为yg 
if(3==len(y.shape)):
    yg=im2double(cv.cvtColor(y, cv.COLOR_BGR2GRAY))
else:
    yg=im2double(y)  
# type(y),type(yg):numpy.ndarray
# np.size(y) is the same as np.size(yg)

# matlab中读入的图像数据是uint8数据类型的矩阵，而python中使用的图像数据格式为ndarray数组。
# How to pass numpy.ndarray from Python to Matlab?
# 尝试将数组/矩阵转列表yg=yg.tolist(),不对,传入的应该是矩阵而不是列表
# 尝试savemat("yg.mat", {"array": yg}, do_compression=False),不行
yg=matlab.double(yg.tolist())
# type(yg): mlarray.double

#开始计时
tic=datetime.now()
print("Processing...")

#输入灰度模糊图像，输出潜在中间图像和模糊核
#将 MATLAB 数组作为输入参数传递给由用于 Python 的 MATLAB 引擎 API 调用的函数。
#当 MATLAB 函数将数值数组作为输出参数返回时，引擎会将该数组返回到 Python。
#kernel,interim_latent=eng.blind_deconv(yg,lambda_dark,lambda_grad,opts)
kernel = matlab.double(eng.blind_deconv(yg,lambda_dark,lambda_grad,opts))
#type(kernel):mlarray.double
print("Successfully Get the Kernel")

#结束计时
toc=datetime.now()
## Algorithm is done!
print('Elapsed time: %f seconds' % (toc-tic).total_seconds())



## ============Non-blind deconvolution ((Just use text deblurring methods))====================##

#将y由int8转为0-1的double类型
y=im2double(y)
# type(y):numpy.ndarray

#传给下面的函数时需要转换类型为mlarray.double
y=matlab.double(y.tolist())

print("Final Deblur...")
# Final Deblur:
#输入模糊图像和模糊核，输出最终的清晰图像
if (0==saturation):
        ## 1. TV-L2 denoising method
        Latent=eng.ringing_artifacts_removal(y,kernel,lambda_tv,lambda_l0,weight_ring)
        #type(Latent):mlarray.single
else:
        ## 2. Whyte's deconvolution method (For saturated images)
        Latent=eng.whyte_deconv(y,kernel)

#convert mlarray to numpy array
Latent=np.array(Latent._data).reshape(Latent.size[::-1]).T #加速效果不太明显，下一行也可以
#Latent=np.array(Latent)
#type(Latent):numpy.ndarray

#对模糊核归一化
kernel=np.array(kernel) #numpy.ndarray
k=kernel -kernel.min()
k=k / k.max() #numpy.ndarray

#将结果写入创建好的指定位置，还原数值为0-255
#cv.imwrite('./results/'+filename[6:-4]+'_kernel.png',k*255)
#cv.imwrite('./results/'+filename[6:-4]+'_result.png',Latent*255)
#cv.imwrite('results/',filename[6:-4]'+'_interim_result.png',interim_latent)
print("Successfully save the kernel and Latent image.")

#OR write to the directory without changing your working directory：
#import os
#cv.imwrite(os.path.join('./', filename[6:-4]+'_kernel.png'), k*255)
#cv.imwrite(os.path.join('./', filename[6:-4]+'_result.png'), Latent*255)

cv.imshow("Latent Image",Latent)
cv.waitKey(0)
cv.destroyAllWindows()

eng.quit()
