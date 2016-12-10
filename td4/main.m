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

feature_size = 10;
SP_nbr = max(label_map(:)); %number of labels = number of superpixels
SP_feat = zeros(SP_nbr, feature_size);

img_red = img(:,:,1); % Red channel
img_green = img(:,:,2); % Green channel
img_blue = img(:,:,3); % Blue channel
img_g = rgb2gray(img);
img_mean = zeros(size(img,1), size(img,2));
img_moyR = zeros(size(img,1), size(img,2));
img_moyG = zeros(size(img,1), size(img,2));
img_moyB = zeros(size(img,1), size(img,2));

for i=1:SP_nbr+1
    sp_pos = label_map == (i-1);
    
    % Features
    SP_feat(i,1) = mean(img_g(sp_pos));
    SP_feat(i,2) = mean(img_red(sp_pos));
    SP_feat(i,3) = mean(img_green(sp_pos));
    SP_feat(i,4) = mean(img_blue(sp_pos));
    
    % Results
    img_mean(sp_pos) = SP_feat(i,1);
    img_moyR(sp_pos) = SP_feat(i,2);
    img_moyG(sp_pos) = SP_feat(i,3);
    img_moyB(sp_pos) = SP_feat(i,4);
end


img2 = zeros(size(img,1), size(img,2), size(img,3));
img2(:,:,1) = img_moyR;
img2(:,:,2) = img_moyG;
img2(:,:,3) = img_moyB;

figure, imagesc(img); title('Image originale');
figure, imagesc(img_mean); colormap(gray()); title('Image Superpixels moyenne niveau de gris');
figure, imagesc(img2); title('Image Superpixels moyenne couleur');


% Histograms : 
% Construire les histos (gray, R, G, B)
% Construire les histos cumul?s
% Normaliser ? 255
% Pour chaque superpixel, taper ? l'indice de la moyenne du superpixel


%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%%%feature extraction (mean levels of gray, mean color, normalized color
%%%histogram, etc.)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% 3)- Classification

N = 2; %number of classes

%%%%%%%%%%%%%%%%%%%%%%%%%%% TO MODIFY %%%%%%%%%%%%%%%%%%%%%%%%%
%%%Classification function (OTSU, global thresholding)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% D?claration
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

%% Seuillage global

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

%% Otsu

img_gray_otsu2 = otsu(img_mean,2);
img_gray_otsu3 = otsu(img_mean,3);
img_otsu2 = otsu(img2,2);
img_otsu3 = otsu(img2,3);

%% Affichage

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











