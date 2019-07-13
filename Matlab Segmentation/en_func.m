%energy function from the article
function e = en_func(sigma,xi,xj)

    e = 1/(sqrt(2*pi)*sigma)*exp(-(abs(xi-xj)^2)/(2*sigma^2));
    
end