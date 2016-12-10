function [T2,T3] = classificationFeatureGray(tab, length)

%% Initialisation

T2 = tab; % R?sultat de tab apr?s seuillage ? 2 classes.
T3 = tab; % R?sultat de tab apr?s seuillage ? 3 classes.

Min = min(tab);
Max = max(tab);
Mid = (Max + Min) / 2;
t1 = Min + (Max - Min) / 3;
t2 = Min + (Max - Min) * 2 / 3;

%% Seuillage

for i=1:length
   value = tab(i);
   
   %% 2 classes
   if value < Mid
       T2(i) = Min;
   else
       T2(i) = Max;
   end
   
   %% 3 classes
   if value < t1
       T3(i) = Min;
   else
       if value < t2
           T3(i) = Mid;
       else
           T3(i) = Max;
       end
   end
   
end