clear all
%% Przygotowanie danych
% Obiekt regulacji i jego modele
KS = [1,.1;.02,.9]; % Macierz wzmocnie� statycznych
Gs  = [... % Model w postaci transmitancji ci�g�ej
    tf(KS(1,1),[1,1]), tf(KS(1,2),[1,1]);...
    tf(KS(2,1),[1,1]), tf(KS(2,2),[1,1]);...
];
Gz = c2d(Gs,1); % Dyskretyzacja transmitancji ci�g�ej
for m=1:2 % Wyznaczenie parametr�w modelu ARX
    tmpa = conv(Gz.Denominator{m,1},Gz.Denominator{m,2});
    GPC_a(m,:) = tmpa(2:end);
    tmpb = conv(Gz.Numerator{m,1},Gz.Denominator{m,2});
    GPC_b(m,1,:) = tmpb(1:end);
    tmpb = conv(Gz.Numerator{m,2},Gz.Denominator{m,1});
    GPC_b(m,2,:) = tmpb(1:end);
end

% Parametry algorytmu regulacji
N  = 10; % Horyzont predykcji
Nu = 10; % Horyzont sterowania
lambda = 1.0; % Wsp�czynnik kary za przyrosty sterowa�
psi = 1.0; % Wsp�czynnik kary za uchyb regulacji
dumin = -0.10; % Ograniczenie na minimalny przyrost sterowania
dumax =  0.10; % Ograniczenie na maksymalny przyrost sterowania
umin  = -4.00; % Ograniczenie na minimaln� warto�� sterowania
umax  =  4.00; % Ograniczenie na maksymaln� warto�� sterowania

codea = AutoMATiC_Generate('AutoMATiC_GPC_Analytic_Algorithm','controllera',1); % 1 -- nadpisz
coden = AutoMATiC_Generate('AutoMATiC_GPC_Numeric_Algorithm','controllern',0); % 0 -- dopisz