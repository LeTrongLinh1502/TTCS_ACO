%**************************************************************************
%       UNG DUNG THUAT TOAN DAN KIEN CHO BAI TOAN TIM DUONG NGAN NHAT
%                            TSP-ACO
% -------------------------------------------------------------------------
clear
clc

% Tham so dau vao
miter = 30; % So lan lap
m =200; % So con kien
% Thong so
p = .2;            % Tham so bay hoi.
alpha = 1;          % He so dieu chinh anh huong nong do vet mui.
beta = 2;           % He so dieu chinh anh huong heuristic.

% Toa do
file = fopen('E:\Bao cao TTCS\Test\test1.txt', 'r');
s = fscanf(file, '%d');
% nhap vao toa do
for i = 1:(length(s) / 2)
x(i) = [s(i*2 - 1)];
y(i) = [s(i*2)];
end;
% ------------------------------------
n = length(x); % So thanh pho
t = 0.0001*ones(n); % Khoi tao ma tran mui
% --------------------------In thành pho ra-----------------------------------------------
subplot(2,1,1);% chia khung hien thi thanh ma tran 2*1 
plot(x,y,'o', 'MarkerEdgeColor','m', 'MarkerFaceColor','g','MarkerSize',10)% to mau cho diem hien thi TP
xlabel('truc hoanh do x');ylabel('truc tung do y');
for i=1:n
    text(x(i)-.4,y(i)+2,['TP',num2str(i)]);% tao hien thi TP
end
for i=1:n
    text(x(i)-.4,y(i)-2,['(',num2str(x(i)),',',num2str(y(i)),')']);%tao hien thi toa do
end
title('Toa do cac thanh pho');
xlabel('truc hoanh do x');
ylabel('truc tung do y');
grid on;% them duong luoi vao do thi (là cac vach ke ngang)
%-------------------------------------------------------------------------------------------------------
% Khoang cach giua cac thanh pho.
for i=1:n
    for j=1:n
        d(i,j)=sqrt((x(i)-x(j))^2+(y(i)-y(j))^2);
    end
end
%--------------------------------------------------------------------------
% Tao ma tran heuristic
for i=1:n
    for j=1:n
        if d(i,j)==0
            h(i,j)=0;
        else
            h(i,j)=1/d(i,j);
        end
    end
end
tic
%--------------------------------------------------------------------------
% -------------------------------------------------------------------------
%                          THUAT TOAN DAN KIEN
% -------------------------------------------------------------------------
for i = 1:miter
% B1: Khoi tao thanh pho xuat phat cho moi con kien 
    for j = 1:m
        ThanhPho(j,1)=fix(1+rand*(n-1));
    end
% B2: Tim hanh trinh cua moi con kien
    [HanhTrinh] = HanhTrinhKien(ThanhPho, m, n, h, t, alpha, beta);
    HanhTrinh=horzcat(HanhTrinh,HanhTrinh(:,1));%noi mang theo chieu ngang
    for j2 = 1:m
        [HanhTrinh(j2,:)] = kopt_aco(d, HanhTrinh(j2,:));
    end
% Step 3: Tinh toan quang duong di
    [KhoangCach] = TinhKhoangCach(m, n, d, HanhTrinh);
    [DuongDiNganNhat(i),best_index]=min(KhoangCach);
    KhoangCach(best_index) = DuongDiNganNhat(i);
    [t]= CapNhatMui(m, n, t, HanhTrinh, KhoangCach, p);
    
% Step 4: Tim duong ngan nhat
    
    besttour(i,:)=HanhTrinh(best_index,:);
    KhoangCachTrungBinh(i)=mean(KhoangCach);
    SoLanLap(i)=i;
    %-----------------------in ra qua trinh chay-------------------------
    [k,l]=min(DuongDiNganNhat);
    K(i) = k;
    %fprintf('\nLan thu: %d  -- Duong ngan nhat (ACO) =  %f\n', i, k);
    fprintf('\nLan thu: %d  \n', i);
    %---------------------------------------------------------------------
end
%-------------in ra time chay----------------------------------------------
t = toc;
fprintf('\nTime run: %f s\n', t);
% -------------------------------------------------------------------------
% Tim duong ngan nhat
[k,l]=min(DuongDiNganNhat);
for i=1:n+1
    X(i)=x(besttour(l,i));
    Y(i)=y(besttour(l,i));
end
%-------------------hien thi ket qua---------------------------------------
subplot(2,1,2);
plot(X,Y,'--ro', 'MarkerEdgeColor','m', 'MarkerFaceColor','g','MarkerSize',10)
xlabel('truc hoanh do x');ylabel('truc tung do y');
for i=1:n
    text(X(i)-.4,Y(i)+2,['TP',num2str(besttour(l,i))]);
end
title(['Duong ngan nhat (ACO) =  ',num2str(k)]);
grid on;

fclose(file);