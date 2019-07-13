%In questa funzione le maschere ottenute vengono salvate nella cartella
%indicata per permettere all'algoritmo di VTK di fare il rendering delle
%nuove maschere. Le informazioni originali delle immagini di partenza sono
%mantenute inalterate e le maschere sono salvate, con i giusti valori di
%intensità di grigio, in formato DICOM.
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% Input:
%       • allI = matrice tridimensionale contenente tutte le immagini TC
%       • allJ = matrice tridimensionale contenente tutte le maschere
%       • selpath_DICOM = percorso file selezionato dall'utente dove sono contenute le
%         immagini DICOM
%       • path_fegato = percorso file selezionato dall'utente dove è contenuto l'eseguibile del rendering del fegato
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


function saveI(allI,allJ,selpath_DICOM,path_fegato)
    
    c = 0;
    MyFolderInfo = dir(selpath_DICOM);
    cd(selpath_DICOM)
    
    for i = 1:length(MyFolderInfo)
        k(i)=contains(MyFolderInfo(i).name,".dcm");
        if k(i)==1
            c=c+1;    
            info = dicominfo(MyFolderInfo(i).name);
            name = sprintf('fegato%d.dcm',c);
            cd(strcat(path_fegato,'\immagini-automatiche'));          
            dicomwrite(allJ(:,:,c).*allI(:,:,c),name,info);
            cd(selpath_DICOM);
        end
    end
cd ../

end