% In questa funzione avviene il caricamento delle immagini.
% Si seleziona manualmente la cartella contenente le immagini TC da
% segmentare (questa cartella deve trovarsi all'interno della cartella di
% lavoro di Matlab).
% Se la cartella contiene le immagini nel corretto formato, la funzione
% legge le immagini Dicom e le salva in una matrice tridimensionale. La
% matrice allI avrà dimensioni NxMxK dove NxM è la dimensione dell'immagine
% e K è il numero di immagini Dicom contenute nella cartella.
% La funzione mat2gray salva i corretti valori di intensità nella nuova
% matrice convertendo l'immagine in scala di grigio.
% Infine la funzione esce dalla cartella delle immagini tornando nella
% corretta cartella di lavoro di Matlab.
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% 
% Input:
% • selpath= percorso file selezionato dall'utente dove sono contenute le
%            immagini DICOM
% Output: 
% • allI = matrice di dimensioni NxMxK (NxM dimensioni dell'immagine,
%          K numero di immagini salvate)
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

function allI = loadI(selpath)

    cd(selpath); 
    MyFolderInfo = dir(selpath);

    allI=[]; 
    c=0;

    for i = 1:length(MyFolderInfo)
        k(i)=contains(MyFolderInfo(i).name,".dcm");
        if k(i)==1
            c=c+1;    
            file = dicomread(MyFolderInfo(i).name);
            allI(:,:,c) = mat2gray(file);
        end
    end
    cd ../
end