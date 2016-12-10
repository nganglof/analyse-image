clear all
close all
clc


%MEX compilation
mex -O CFLAGS="\$CFLAGS -Wall -Wextra -W -std=c99" Mex/Superpixel_fct.c -outdir Mex/
addpath('Mex');


%% Image loading

img = imread('Img/bench.JPG');  %cow.JPG   bird.JPG
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

% Initialisation

feature_size = 10;
SP_nbr = max(label_map(:)); %number of labels = number of superpixels
SP_feat = zeros(SP_nbr, feature_size);
img_red = img(:,:,1); % Red channel
img_green = img(:,:,2); % Green channel
img_blue = img(:,:,3); % Blue channel
img_g = rgb2gray(img);
img_mean = zeros(size(img,1), size(img,2));
img_meanR = zeros(size(img,1), size(img,2));
img_meanG = zeros(size(img,1), size(img,2));
img_meanB = zeros(size(img,1), size(img,2));
img_hist_gray = zeros(size(img,1), size(img,2));
img_hist_red = zeros(size(img,1), size(img,2));
img_hist_green = zeros(size(img,1), size(img,2));
img_hist_blue = zeros(size(img,1), size(img,2));

% Extraction des features

for i=1:SP_nbr+1
    sp_pos = label_map == (i-1);
    
    % Features
    SP_feat(i,1) = mean(img_g(sp_pos));
    SP_feat(i,2) = mean(img_red(sp_pos));
    SP_feat(i,3) = mean(img_green(sp_pos));
    SP_feat(i,4) = mean(img_blue(sp_pos));
end



% Histogrammes

imgHist_gray = cumsum(imhist(img_g) / (size(img_g,1) * size(img_g,2)));
imgHist_red = cumsum(imhist(img_red) / (size(img_red,1) * size(img_red,2)));
imgHist_green = cumsum(imhist(img_green) / (size(img_green,1) * size(img_green,2)));
imgHist_blue = cumsum(imhist(img_blue) / (size(img_blue,1) * size(img_blue,2)));
for i=1:SP_nbr+1
    sp_pos = label_map == (i-1);
    
    % Results
    img_mean(sp_pos) = SP_feat(i,1);
    img_meanR(sp_pos) = SP_feat(i,2);
    img_meanG(sp_pos) = SP_feat(i,3);
    img_meanB(sp_pos) = SP_feat(i,4);
    img_hist_gray(sp_pos) = imgHist_gray(uint8(SP_feat(i,1) * 255) + 1);
    img_hist_red(sp_pos) = imgHist_red(uint8(SP_feat(i,2) * 255) + 1);
    img_hist_green(sp_pos) = imgHist_green(uint8(SP_feat(i,3) * 255) + 1);
    img_hist_blue(sp_pos) = imgHist_blue(uint8(SP_feat(i,4) * 255) + 1);
end

img_mean_color = zeros(size(img,1), size(img,2), size(img,3));
img_mean_color(:,:,1) = img_meanR;
img_mean_color(:,:,2) = img_meanG;
img_mean_color(:,:,3) = img_meanB;
img_hist_color = zeros(size(img,1), size(img,2), size(img,3));
img_hist_color(:,:,1) = img_hist_red;
img_hist_color(:,:,2) = img_hist_green;
img_hist_color(:,:,3) = img_hist_blue;

% Affichage

figure, imagesc(img); title('Image originale');
figure, imagesc(img_mean); colormap(gray); title('Image Superpixels moyenne niveau de gris');
figure, imagesc(img_mean_color); title('Image Superpixels moyenne couleur');
figure, imagesc(img_hist_gray); colormap(gray); title('Image Superpixels histogramme niveau gris');
figure, imagesc(img_hist_color); title('Image Superpixels histogramme couleur');



%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%%%feature extraction (mean levels of gray, mean color, normalized color
%%%histogram, etc.)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% 3)- Classification

N = 2; %number of classes

%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%%%Classification function (OTSU, global thresholding)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Initialisation
img_mean_class2 = zeros(size(img,1), size(img,2));
img_mean_class3 = zeros(size(img,1), size(img,2));
img_R_class2 = zeros(size(img,1), size(img,2));
img_G_class2 = zeros(size(img,1), size(img,2));
img_B_class2 = zeros(size(img,1), size(img,2));
img_R_class3 = zeros(size(img,1), size(img,2));
img_G_class3 = zeros(size(img,1), size(img,2));
img_B_class3 = zeros(size(img,1), size(img,2));
img_class2 = zeros(size(img,1), size(img,2), size(img,3));
img_class3 = zeros(size(img,1), size(img,2), size(img,3));

% Seuillage global

[T2_mean,T3_mean] = classificationFeatureGray(SP_feat(:,1), SP_nbr+1);
[T2_R,T3_R] = classificationFeatureGray(SP_feat(:,2), SP_nbr+1);
[T2_G,T3_G] = classificationFeatureGray(SP_feat(:,3), SP_nbr+1);
[T2_B,T3_B] = classificationFeatureGray(SP_feat(:,4), SP_nbr+1);

for i=1:SP_nbr+1
    sp_pos = label_map == (i-1);
    img_mean_class2(sp_pos) = T2_mean(i);
    img_mean_class3(sp_pos) = T3_mean(i);
    img_R_class2(sp_pos) = T2_R(i);
    img_G_class2(sp_pos) = T2_G(i);
    img_B_class2(sp_pos) = T2_B(i);
    img_R_class3(sp_pos) = T3_R(i);
    img_G_class3(sp_pos) = T3_G(i);
    img_B_class3(sp_pos) = T3_B(i);
end

img_class2(:,:,1) = img_R_class2;
img_class2(:,:,2) = img_G_class2;
img_class2(:,:,3) = img_B_class2;
img_class3(:,:,1) = img_R_class3;
img_class3(:,:,2) = img_G_class3;
img_class3(:,:,3) = img_B_class3;

% Otsu

img_gray_otsu2 = otsu(img_mean,2);
img_gray_otsu3 = otsu(img_mean,3);
img_otsu2 = otsu(img_mean_color,2);
img_otsu3 = otsu(img_mean_color,3);

% Affichage

figure, imagesc(img_mean_class2); colormap(gray); title('Seuillage global niveau de gris 2 classes');
figure, imagesc(img_mean_class3); colormap(gray); title('Seuillage global niveau de gris 3 classes');
figure, imagesc(img_class2); title('Seuillage global couleur 2 classes');
figure, imagesc(img_class3); title('Seuillage global couleur 3 classes');
figure, imagesc(img_gray_otsu2); colormap(gray); title('Otsu niveau de gris 2 classes');
figure, imagesc(img_gray_otsu3); colormap(gray); title('Otsu niveau de gris 3 classes');
figure, imagesc(img_otsu2); title('Otsu couleur 2 classes');
figure, imagesc(img_otsu3); title('Otsu couleur 3 classes');

%% 4)- Patch-based classification

ps = 4;  %patch_size

%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%Regular grid sampling.
%Same pipeline.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%











