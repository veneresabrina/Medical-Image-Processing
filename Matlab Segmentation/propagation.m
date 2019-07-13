%In questa funzione si propaga il risultato ottenuto dalla tecnica del
%graph cut al numero di fette indicato in input. La propagazione può
%seguire due versi cioè crescente o decrescente.
%Per ogni fetta successiva a quella corrente si assegna alla variabile J la
%maschera sulla quale verranno fatte le considerazioni (quindi quella di
%partenza) e per ogni punto della maschera J (quindi per ogni punto del fegato)
%si controlla che l'intensità del pixel corrispondente nella slice successiva sia coerente con
%l'intensità dello stesso pixel nella slice corrente.
% La maschera così ottenuta viene perfezionata dai passaggi di chiusura,
% riempimento ed eliminazione di elementi di area molto piccola.
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% Input: 
%       • allI = matrice tridimensionale contenente tutte le immagini TC
%       • allJ = matrice tridimensionale contenente tutte le maschere
%       • fette = vettore contenente il numero di fette successive da
%                 segmentare
%       • num = l'indice della fetta di partenza considerata
%       • verso = +1 o -1 rispettivamente quando il verso della propagazione 
%                 è crescente o decrescente
% Output:
%       • allJ = matrice tridimensionale contenente tutte le maschere
%               aggiornate
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


function allJ = propagation(allI,allJ,fette,num,verso)


    for k = fette % ~ Per le successive fette

    J = allJ(:,:,num+k-verso); 
    J_successiva = zeros(size(J,1)); % ~ Inizializzazione maschera

    %~ Per ogni punto della maschera J
        for i=1:size(J,1)
            for j=1:size(J,2)
                if J(i,j) == 1
                intens_prima = allI(i,j,num+k-verso);
                intens_dopo = allI(i,j,num+k); 
                diff = abs(intens_prima-intens_dopo);
                    if diff < 0.03 * intens_prima % ~ Se è diminuita al massimo del 3% allora è ancora fegato
                        J_successiva(i,j) = 1;       
                    end
                end
            end
        end

   J_successiva = imclose(J_successiva,strel('disk',2));

   J_successiva = imfill(J_successiva);

   J_successiva = bwareaopen(J_successiva,50);

   allJ(:,:,num+k) = J_successiva;    

    end


end