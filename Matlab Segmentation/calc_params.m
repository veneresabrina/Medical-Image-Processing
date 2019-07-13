function sigma = calc_params(I)

    N = (size(I,1)*size(I,2)); %total number of pixel of the image
    mu = sum(sum(I))/N; %avarage gray value of the image
    p = 0;
    for i1=1:size(I,1)
        for i2=1:size(I,2)
            p = p + (I(i1,i2)-mu)^2;
        end
    end
    sigma = sqrt(p/N); %deviazione standard
    
end