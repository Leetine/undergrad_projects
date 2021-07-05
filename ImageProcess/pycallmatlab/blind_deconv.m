function [kernel, interim_latent] = blind_deconv(y,lambda_dark, lambda_grad, inopts)
% 
% Do multi-scale blind deconvolution
%
%% Input: 
% @y : input blurred image (grayscale); 
% @lambda_dark: the weight for the L0 regularization on intensity
% @lambda_grad: the weight for the L0 regularization on gradient
% @opts: see the description in the file "demo_text_deblurring.m"
%% Output:
% @kernel: the estimated blur kernel
% @interim_latent: intermediate latent image
%
% The Code is created based on the method described in the following paper 
%   [1] Jinshan Pan, Deqing Sun, Hanspteter Pfister, and Ming-Hsuan Yang,
%        Blind Image Deblurring Using Dark Channel Prior, CVPR, 2016. 
%   [2] Jinshan Pan, Zhe Hu, Zhixun Su, and Ming-Hsuan Yang,
%        Deblurring Text Images via L0-Regularized Intensity and Gradient
%        Prior, CVPR, 2014. 
%
%   Author: Jinshan Pan (sdluran@gmail.com)
%   Date  : 03/22/2016

opts=struct(inopts)

% gamma correct
if opts.gamma_correct~=1 
    y = y.^opts.gamma_correct; %灰度拉伸
end
b = zeros(opts.kernel_size);

 
%计算金字塔层数num_scales 
ret = sqrt(0.5);
maxitr=max(floor(log(5/min(opts.kernel_size))/log(ret)),0);
num_scales = maxitr + 1;
fprintf('Maximum iteration level is %d\n', num_scales);

 
%计算每一层的模糊核k1*k2，分别存在k1list和k2list中
retv=ret.^[0:maxitr];%retv表示缩放倍数，第一个值为1，第二个值为1/sqrt(2)倍
k1list=ceil(opts.kernel_size*retv);%每次迭代估计的模糊核大小
k1list=k1list+(mod(k1list,2)==0);%保证大小为奇数
k2list=ceil(opts.kernel_size*retv);
k2list=k2list+(mod(k2list,2)==0);%k1*k2大小

 
% derivative filters
dx = [-1 1; 0 0];
dy = [-1 0; 1 0]; 
% blind deconvolution - multiscale processing - from top to bottom
for s = num_scales:-1:1
  if (s == num_scales)
      % at coarsest level, initialize kernel
      ks = init_kernel(k1list(s));
      k1 = k1list(s);
      k2 = k1; % always square kernel assumed
  else 
    % upsample kernel from previous level to next finer level
    k1 = k1list(s);
    k2 = k1; % always square kernel assumed
    
    % resize kernel from previous level 
    ks = resizeKer(ks,1/ret,k1list(s),k2list(s));    
  end;
   
  cret=retv(s);
  ys=downSmpImC(y,cret);  
  fprintf('Processing scale %d/%d; kernel size %dx%d; image size %dx%d\n', ...
            s, num_scales, k1, k2, size(ys,1), size(ys,2));
            

  %% Useless operation
  if (s == num_scales)%若是第一次迭代，输入当前层图像和模糊核，输出阈值
    [~, ~, threshold]= threshold_pxpy_v1(ys,max(size(ks)));
  
  end
  
  %输入当前层图像、模糊核，输出新的模糊核、潜在图像 
  [ks, lambda_dark, lambda_grad, interim_latent] = blind_deconv_main(ys, ks, lambda_dark,...
      lambda_grad, threshold, opts);
   %% center the kernel
   ks = adjust_psf_center(ks);
   ks(ks(:)<0) = 0;%去除负值并归一化 
   sumk = sum(ks(:));
   ks = ks./sumk;
  %% if we meet the last iteration, set elements below threshold to 0
  if (s == 1)
    kernel = ks;
    if opts.k_thresh>0
        %置小于最大值除以阈值opts.k_thresh的值置为0
        kernel(kernel(:) < max(kernel(:))/opts.k_thresh) = 0;
    else
        %置小于0的值为0
        kernel(kernel(:) < 0) = 0;
    end
    %归一化
    kernel = kernel / sum(kernel(:));
  end
end
%np_interim_latent = py.numpy.array(interim_latent(:).');
%np_kernel = py.numpy.array(kernel(:).'); 
%返回kernel、interim_latent
%% end kernel estimation
end
  

%%%%%%%%%%%%%%% Sub-function %%%%%%%%%%%%%%%%%
%--------------------------------------------%
%如果是第一次迭代，则初始化为一个方形矩阵ks
%第(minisize-1)/2行的最中间两个值为0.5，其他为0
function [k] = init_kernel(minsize)
  k = zeros(minsize, minsize);
  k((minsize - 1)/2, (minsize - 1)/2:(minsize - 1)/2+1) = 1/2;
end


%--------------------------------------------%
function sI=downSmpImC(I,ret)
%% refer to Levin's code
if (ret==1)
    sI=I;
    return
end

sig=1/pi*ret;
g0=[-50:50]*2*pi;
sf=exp(-0.5*g0.^2*sig^2);
sf=sf/sum(sf);
csf=cumsum(sf);
csf=min(csf,csf(end:-1:1));
ii=find(csf>0.05);
sf=sf(ii);
sum(sf);

I=conv2(sf,sf',I,'valid');

%首先求I的各列与向量 sf 的卷积，然后求每行结果与向量 sf' 的卷积
%valid：舍去需要补零的部分：裁剪为（ma-mb+1）*（na-nb+1） mb*nb为卷积核大小

[gx,gy]=meshgrid([1:1/ret:size(I,2)],[1:1/ret:size(I,1)]);

sI=interp2(I,gx,gy,'bilinear');%重插值
end

%--------------------------------------------%
%每一步的模糊核初始为上一步模糊核的值进行重插值
function k=resizeKer(k,ret,k1,k2)
%%
% levin's code
k=imresize(k,ret);%放大根号2倍，大小向上取整
k=max(k,0);%去掉小于0的值
k=fixsize(k,k1,k2);
if max(k(:))>0
    k=k/sum(k(:));%去掉小于0的值并归一化
end
end

%--------------------------------------------%
function nf=fixsize(f,nk1,nk2)
[k1,k2]=size(f);

while((k1~=nk1)|(k2~=nk2))%循环增改模糊核的大小直至k1=nk1 k2=nk2
    
    if (k1>nk1)%如果resize的k的行数比预期多
        s=sum(f,2);%每行的sum求和
        if (s(1)<s(end))
            f=f(2:end,:);%如果第一行的和比最后一行小，那么模糊核删除第一行
        else
            f=f(1:end-1,:);%否则删除最后一行
        end
    end
    
    if (k1<nk1)%如果小于的话则需要增补一行
        s=sum(f,2);
        if (s(1)<s(end))
            tf=zeros(k1+1,size(f,2));
            tf(1:k1,:)=f;%在最后一行后补一行0
            f=tf;
        else
            tf=zeros(k1+1,size(f,2));
            tf(2:k1+1,:)=f;%在第一行前补一行0
            f=tf;
        end
    end
    
    if (k2>nk2)
        s=sum(f,1);
        if (s(1)<s(end))
            f=f(:,2:end);
        else
            f=f(:,1:end-1);
        end
    end
    
    if (k2<nk2)
        s=sum(f,1);
        if (s(1)<s(end))
            tf=zeros(size(f,1),k2+1);
            tf(:,1:k2)=f;
            f=tf;
        else
            tf=zeros(size(f,1),k2+1);
            tf(:,2:k2+1)=f;
            f=tf;
        end
    end
    
    
    
    [k1,k2]=size(f);
    
end

nf=f;
end
%%