

function neighbors8 = conneigh8(Nrows,Ncols,ind)

    neighbors8 = zeros(9,1);
    [row_ind,col_ind] = ind2sub([Nrows Ncols],ind);
    
    matr = [-1 0 1];
    k=1;
    for i = 1:3:9
        row(i:i+2) = row_ind + matr(k);
        col(i:i+2) = col_ind + matr;
        k = k+1;
    end
    for j = 1:9
        neighbors8(j) = sub2ind([Nrows Ncols],row(j),col(j));
    end

    
    
end