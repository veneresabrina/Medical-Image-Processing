% Dati i valori ottimali per la segmentazione del fegato contenuti nel file
% excel, questa funzione aggiorna la matrice tridimensionale allJ che
% conterrà le maschere del fegato.
% Nella variabile I si salva l'immagine TC da segmentare, successivamente
% nella variabile J si salva la maschera ottenuta dall'algoritmo graph cut.
% La maschera trovata viene salvata nella corretta posizione della matrice
% tridimensionale e le sue informazioni vengono utilizzate per determinare
% la posizione del fegato nelle fette successive.
%  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% Input: 
%       • dati = vettore che contiene i 4 valori ottimali per la
%               segmentazione della slice corrente.
%               dati(1) è il numero di immagine che si sta analizzando
%               dati(2) è la riga del pixel che sarà preso come riferimento per
%               individuare il fegato
%               dati(3) è la colonna del pixel di riferimento
%               dati(4) è l'area di soglia, tutti gli elementi di area minore di questa
%               saranno scartati
%       • allI = matrice tridimensionale contenente tutte le immagini TC
%       • allJ = matrice tridimensionale contenente tutte le maschere
%       • fette = vettore contenente il numero di fette successive da
%                 segmentare (in questo caso le successive 9) con
%                 l'algoritmo di propagazione.
% Output:
%       • allJ = matrice tridimensionale aggiornata con le nuove maschere
%                trovate
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

function allJ = segmentation(dati,allI,allJ,fette)

    I = allI(:,:,dati(1));
    J = graph_cut(I,dati);
    allJ(:,:,dati(1)) = J;
    allJ = propagation(allI,allJ,fette,dati(1),1);
 
end
