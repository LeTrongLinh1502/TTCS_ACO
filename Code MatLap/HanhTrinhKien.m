function [HanhTrinh] = HanhTrinhKien(ThanhPho,m,n,h,t,alpha,beta);
for i=1:m
    mh=h;
    for j=1:n-1
        c=ThanhPho(i,j);
        mh(:,c)=0;
        temp=(t(c,:).^alpha).*(mh(c,:).^beta);
        s=(sum(temp));
        p=(1/s).*temp;
        r=rand;
        s=0;
        for k=1:n
            s=s+p(k);
            if r<=s
                ThanhPho(i,j+1)=k;
                break
            end
        end
    end
end
HanhTrinh=ThanhPho;

