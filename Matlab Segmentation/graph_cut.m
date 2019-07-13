% Funzione che implementa la teoria di graph cut al fine di segmentare
% l'immagine data sulla base di una soglia di energia definita con l'ausilio
% del pixel di riferimento, tutti i pixel aventi energia superiore alla 
% soglia sono possibili candidati per costruire la maschera del fegato.
% La definizione della maschera viene perfezionata utilizzando
% una soglia di intensità per scartare i pixel che pur superando la soglia di
% energia non sono di intensità sufficientemente simile a quella del
% pixel di riferimento (la tolleranza è di +-3%)
% Il riferimento è costruito dal pixel selezionato (certamente appartenente
% al fegato) e dal suo insieme 8-connesso, il controllo di superamento delle soglie è
% effettuato per tutti i 9 pixel e se almeno 5 pixel su 9 superano le condizioni
% allora quel pixel è etichettato come fegato (valore 1 nella maschera
% finale).
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% Input: 
%       • I = l'immagine sulla quale si applica la tecnica
%       • dati = riga che contiene i dati utili all'applicazione di graph
%               cut: l'indice della fetta di riferimento, le coordinate del
%               pixel di partenza e il valore di area massimo eliminabile
% Output:
%       • J = maschera dell'immagine I
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

function J = graph_cut(I,dati)
    riga = dati(2);
    colonna = dati(3);
    area_piccola = dati(4);

    ind_x_in = sub2ind(size(I),riga,colonna); % ~ Indice del pixel appartenente al fegato
    neighbors8=conneigh8(size(I,1),size(I,2),ind_x_in); % ~ Indici dei pixel vicini

    sigma = calc_params(I); % ~ Deviazione standard
    e = zeros(size(I,1)*size(I,2),1); % ~ Inizializzazione vettore dei valori di energia

    voting = zeros(size(I,1)*size(I,2),2+length(neighbors8));
    j0 = 1;
    for j1 = 1:size(I,1)
        for j2 = 1:size(I,2)
            voting(j0,1) = j1; % ~ Riga
            voting(j0,2) = j2; % ~ Colonna
            j0 = j0+1;
        end
    end

    for i=1:length(neighbors8)
            [riga,colonna] = ind2sub([size(I,1),size(I,2)],neighbors8(i));
            xi = I(riga,colonna); % ~ Intensità del pixel attuale della ROI considerata
            
            % ~ Calcolo di tutte le energie e le posizioni di riferimento rispetto al pixel iniziale xi
            for i1 = 1:size(I,1)*size(I,2)
                xj = I(voting(i1,1),voting(i1,2)); % ~ Tutti gli altri pixel
                e(i1,1) = en_func(sigma,xi,xj); % ~ Valore di energia
            end

        soglia_energia = e((voting(:,1)==riga&voting(:,2)==colonna),1); % ~ Il valore di energia del fegato
        soglia_energia = soglia_energia-0.003*soglia_energia;
        soglia_intens1 = xi-0.03*xi; % ~ Intensità del fegato - 3%
        soglia_intens2 = xi+0.03*xi; % ~ Intensità del fegato + 3%
        J = zeros(512);

        for k = 1:length(voting) 
            if e(k,1) >= soglia_energia && I(voting(k,1),voting(k,2)) > soglia_intens1 && I(voting(k,1),voting(k,2)) < soglia_intens2
               voting(k,i+2) = 1; % ~ Segnala il voto fegato true secondo il pixel corrente
            end
        end
    end
    for h1=1:size(I,1)*size(I,2)
        if sum(voting(h1,3:11))>4 % ~ Se 4 volte su 9 è stato riconosciuto il fegato
           J(voting(h1,1),voting(h1,2))=1; % ~ Maschera del fegato 
        end
    end

    J = imfill(J,'holes');

    se = strel('disk',3); % ~ Elemento strutturale per l'erosione
    J = imerode(J,se);

    J = bwareaopen(J, area_piccola);


end