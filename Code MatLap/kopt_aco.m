
function [X] = kopt_aco(d, X)
Maxit3 = 20;            % number of iterations

n = length(X);         % number of nodes

fX = Distance(d,n,X);
for iter = 1:Maxit3
    r1 = fix(1+rand*(n-1));
    while true
        r2 = fix(1+rand*(n-1));
        if (r2 ~= r1)
            break;
        end
    end
    while true
        r3 = fix(1+rand*(n-1));
        if (r3 ~= r1 && r3 ~= r2)
            break;
        end
    end
    rtour(1,:) = Reverse_sub_tour(X, r1, n);
    rtour(2,:) = Reverse_sub_tour(X, r2, n);
    rtour(3,:) = Reverse_sub_tour(X, r3, n);
    rtour(4,:) = Reverse_sub_tour(X, r1, n);
    rtour(4,:) = Reverse_sub_tour(rtour(4,:), r2, n);
    rtour(5,:) = Reverse_sub_tour(X, r2, n);
    rtour(5,:) = Reverse_sub_tour(rtour(5,:), r3, n);
    rtour(6,:) = Reverse_sub_tour(X, r1, n);
    rtour(6,:) = Reverse_sub_tour(rtour(6,:), r3, n);
    rtour(7,:) = Reverse_sub_tour(X, r1, n);
    rtour(7,:) = Reverse_sub_tour(rtour(7,:), r2, n);
    rtour(7,:) = Reverse_sub_tour(rtour(7,:), r3, n);
    for i = 1:7
        fXtemp = Distance(d,n,rtour(i,:));
        if (fX > fXtemp)
            fX = fXtemp;
            X = rtour(i,:);
        end
    end
end
end



%% Reverse_sub_tour
function [tour] = Reverse_sub_tour(sub_tour,r,n);
sub_tour([r r+1]) = sub_tour([r+1 r]);
if (r == 1)
    sub_tour(n) = sub_tour(1);
else
    sub_tour(1) = sub_tour(n);
end
tour = sub_tour;
end

%% TinhKhoangCach
function [distance]=Distance(d,n,tour)
    distance=0;
    for i=1:(n-1)
        distance=distance+d(tour(i),tour(i+1));
    end
end