clear all
close all
clc

%MEX compilation
mex -O CFLAGS="\$CFLAGS -Wall -Wextra -W -std=c99" Mex/Superpixel_fct.c -outdir Mex/
addpath('Mex');


%% Image loading

img = imread('Img/bird.JPG');  %cow.JPG   bird.JPG
img = double(img)/255;


%% 1)- Superpixel decomposition

%Parameters
SP_nbr = 500;     %to modify
compactness  = 10;   %to modify


%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%Superpixel decomposition
tic;
label_map = double(Superpixel_fct(single(img), SP_nbr, compactness));
toc;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%Display borders
[gx,gy] = gradient(label_map);
borders = ((gx>0)+(gy>0))>0;

figure, 
subplot 121
imagesc(img.*repmat(~borders,[1 1 3]))
subplot 122
imagesc(label_map)
drawnow;

%% 2)- Features extraction

feature_size = 10;
SP_nbr = max(label_map(:)); %number of labels = number of superpixels
SP_feat = zeros(SP_nbr, feature_size);

img_g = rgb2gray(img);
img_moy = zeros(size(img,1), size(img,2));

for i=1:SP_nbr
    sp_pos = label_map == i;
    r = min(img_g(sp_pos));
    img_moy(sp_pos) = r;
end

figure,
imagesc(img_moy)
colormap(gray)


%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%%%feature extraction (mean levels of gray, mean color, normalized color
%%%histogram, etc.)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% 3)- Classification

N = 2; %number of classes

%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%%%Classification function (OTSU, global thresholding)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%% 4)- Patch-based classification

ps = 4;  %patch_size

%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%Regular grid sampling.
%Same pipeline.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%











