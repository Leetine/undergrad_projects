function [px, py, threshold]= threshold_pxpy_v1(latent,psf_size,threshold)

% mask indicates region with gradients. outside of mask shud be smooth...

if ~exist('threshold', 'var')
    threshold = 0;
    b_estimate_threshold = true;
else
    b_estimate_threshold = false;
end
%denoised表示当前图像，并求水平和垂直梯度，平方和即图像总梯度大小
denoised=latent;

dx = [-1 1; 0 0];
dy = [-1 0; 1 0];

px = conv2(denoised, dx, 'valid');
py = conv2(denoised, dy, 'valid');
pm = px.^2 + py.^2;

% if this is the first prediction, then we need to find an appropriate
% threshold value by building a histogram of gradient magnitudes
if b_estimate_threshold
    pd = atan(py./px);%梯度角度
    pm_steps = 0:0.00006:2;
    H1 = cumsum(flipud(histc(pm(pd >= 0 & pd < pi/4), pm_steps)));
    H2 = cumsum(flipud(histc(pm(pd >= pi/4 & pd < pi/2), pm_steps)));
    H3 = cumsum(flipud(histc(pm(pd >= -pi/4 & pd < 0), pm_steps)));
    H4 = cumsum(flipud(histc(pm(pd >= -pi/2 & pd < -pi/4), pm_steps)));

    th = max([max(psf_size)*20, 10]);
    
    for t=1:numel(pm_steps) %numel 数组元素的数目 一维度等于length
        min_h = min([H1(t) H2(t) H3(t) H4(t)]);
        if min_h >= th
            threshold = pm_steps(end-t+1);
            break
        end
    end
end

% thresholding
m = pm < threshold;
while all(m(:)==1)
    threshold = threshold * 0.81;
    m = pm < threshold;
end
px(m) = 0;
py(m) = 0;


% % update prediction parameters
% threshold = threshold * 0.9;
%% my modification
if b_estimate_threshold
    threshold = threshold;
else
    threshold = threshold./1.1;
end

end

