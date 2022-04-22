function [t]=CapNhatMui(m,n,t,HanhTrinh, KhoangCach, p)
t = (1-p)*t;
for i=1:m
    for j=1:n
        dt = 1 / KhoangCach(i);
        t(HanhTrinh(i,j),HanhTrinh(i,j+1))=t(HanhTrinh(i,j),HanhTrinh(i,j+1))+dt;
    end
end
