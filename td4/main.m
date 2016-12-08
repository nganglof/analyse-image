clear all
close all
clc


%MEX compilation
mex -O CFLAGS="\$CFLAGS -Wall -Wextra -W -std=c99" Mex/Superpixel_fct.c -outdir Mex/
addpath('Mex');


%% Image loading

img = imread('Img/cow.JPG');  %cow.JPG   bird.JPG
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
% 
% figure, 
% subplot 121
% imagesc(img.*repmat(~borders,[1 1 3]))
% subplot 122
% imagesc(label_map)
% drawnow;

%% 2)- Features extraction

feature_size = 10;
SP_nbr = max(label_map(:)); %number of labels = number of superpixels
SP_feat = zeros(SP_nbr, feature_size);

img_red = img(:,:,1); % Red channel
img_green = img(:,:,2); % Green channel
img_blue = img(:,:,3); % Blue channel
img_g = rgb2gray(img);
img_moy = zeros(size(img,1), size(img,2));
img_moyR = zeros(size(img,1), size(img,2));
img_moyG = zeros(size(img,1), size(img,2));
img_moyB = zeros(size(img,1), size(img,2));

for i=1:SP_nbr
    sp_pos = label_map == i;
    
    SP_feat(i,1)= mean(img_g(sp_pos));
    SP_feat(i,2)= min(img_g(sp_pos));
    SP_feat(i,3)= max(img_g(sp_pos));
    SP_feat(i,4)= mean(img_red(sp_pos));
    SP_feat(i,5)= mean(img_green(sp_pos));
    SP_feat(i,6)= mean(img_blue(sp_pos));
    
    tmphist = cumsum(hist(img_g(sp_pos)));
    SP_feat(i,7)= tmphist(length(tmphist));
    tmphist = cumsum(hist(img_red(sp_pos)));
    SP_feat(i,8)= tmphist(length(tmphist));
    tmphist = cumsum(hist(img_green(sp_pos)));
    SP_feat(i,9)= tmphist(length(tmphist));
    tmphist = cumsum(hist(img_blue(sp_pos)));
    SP_feat(i,10)= tmphist(length(tmphist));
    
    %img_moy(sp_pos) = SP_feat(i,10);
    
    
end


img2 = zeros(size(img,1), size(img,2), size(img,3));
img2(:,:,1) = img_moyR;
img2(:,:,2) = img_moyG;
img2(:,:,3) = img_moyB;



%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%%%feature extraction (mean levels of gray, mean color, normalized color
%%%histogram, etc.)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% 3)- Classification

N = 2; %number of classes

%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%%%Classification function (OTSU, global thresholding)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

img_class = zeros(size(img,1), size(img,2));

for j = 1:10

    for i=1:SP_nbr
        sp_pos = label_map == i;

        img_class(sp_pos) = SP_feat(i,j);
    end

    IDX = otsu(img_class,N);

    figure, imagesc(IDX), axis image off
    title(['n = ' int2str(N)])
    colormap(gray)
end

%% 4)- Patch-based classification

ps = 4;  %patch_size

%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%Regular grid sampling.
%Same pipeline.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%











