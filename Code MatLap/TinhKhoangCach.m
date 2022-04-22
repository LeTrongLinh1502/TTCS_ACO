function [KhoangCach]=TinhKhoangCach(m,n,d,HanhTrinh)
for i=1:m
    s=0;
    for j=1:n
        s=s+d(HanhTrinh(i,j),HanhTrinh(i,j+1));
    end
    KhoangCach(i)=s;
end