%A partire dalle immagini TC il fegato è segmentato utilizzando la tecnica
%graph-cut sulle fette 60-70-80...199 e il resto delle maschere è ottenuto
%a partire da queste utilizzando un controllo sulle soglie di intensità.
%Le immagini sono infine salvate nella cartella contenente il programma per
%fare il 3D rendering in VTK

%scegliere la cartella ITK/Projects all'interno del disco C del proprio computer
selpath_ITK = uigetdir;
path_addome = strcat(selpath_ITK,'\VolumeRendering\bin\Release'); %percorso per il rendering dell'addome completo
path_fegato = strcat(selpath_ITK,'\VolumeRenderingSmoothing\bin\Release'); %percorso per il rendering del fegato
path_MPR = strcat(selpath_ITK,'\MPR\bin\Release');%percorso per la ricostruzione multiplanare
%scelta cartella immagini DICOM
selpath_DICOM=uigetdir;

allI = loadI(selpath_DICOM); %caricamento e lettura delle immagini 
allJ = zeros(size(allI,1),size(allI,2),size(allI,3)); %inizializzazione della matrice contenente le maschere

semi = xlsread('semi.xlsx'); %caricamento delle informazioni per la segmentazione ottimale del fegato
for j = 1:length(semi) %dall'immagine 60 alla 199
    allJ = segmentation(semi(j,:),allI,allJ,1:9);
end
allJ = propagation(allI,allJ,semi(1,1)-1:-1:semi(1,1)-59,0,-1);    
implay(allJ)
save("Automatic_Segmentation.mat", "allJ");
save("Immagini.mat", "allI");
saveI(allI,allJ,selpath_DICOM,path_fegato) %salva le immagini nella cartella ITK per il volume rendering del fegato