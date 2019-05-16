clearvars -except MPC_CROSSCOMPILER
warning off

if(~exist('MPC_CROSSCOMPILER','var'))
    NO_CROSSCOMPILER = 1;
    addpath('..\\Libs\\MATLAB\\');
else
    NO_CROSSCOMPILER = 0;
end

alg = 'AutoMATiC_DMC_Numeric_Algorithm';
%% Przygotowanie danych
control_value = zeros(2,1); % tutaj przechowywane s� nowe przyrosty sterowa�

INIT = 1;
kp = 100; kk = 1000;    % Czas startu i ko�ca eksperymentu
nu = 2; ny = 2; nx = 2; % Wymiarowo�� zadania
AD_Y    = zeros(kk,ny); % Macierze i wektory
AD_U    = zeros(kk,nu); % do przechowywania
AD_X    = zeros(kk,nx); % danych na temat
AD_DU   = zeros(kk,nu); % procesu regulacji
AD_Z    = zeros(1,2);   % i samej regulacji

%% Obiekt regulacji i jego modele
KS = [1,.1;.02,.9]; % Macierz wzmocnie� statycznych
Gs  = [... % Model w postaci transmitancji ci�g�ej
    tf(KS(1,1),[1,1]), tf(KS(1,2),[1,1]);...
    tf(KS(2,1),[1,1]), tf(KS(2,2),[1,1]);...
];
SSs = ss([-1,0;0,-1],KS,[1,0;0,1],[0,0;0,0]); % Model w przestrzeni stan�w
Gz = c2d(Gs,1); % Dyskretyzacja transmitancji ci�g�ej
SSz = c2d(SSs,1); % Dyskretyzacja modelu w przestrzeni stan�w
Stmp = step(Gz);      % Odpowied� skokowa
S = shiftdim(Stmp,1); % Przekszta�cenie do wygodniejszej postaci
DMC_S=S(:,:,2:end);   % Pozbycie si� pierwszego elementu
for m=1:ny % Wyznaczenie parametr�w modelu ARX
    tmpa = conv(Gz.Denominator{m,1},Gz.Denominator{m,2});
    GPC_a(m,:) = tmpa(2:end);
    tmpb = conv(Gz.Numerator{m,1},Gz.Denominator{m,2});
    GPC_b(m,1,:) = tmpb(1:end);
    tmpb = conv(Gz.Numerator{m,2},Gz.Denominator{m,1});
    GPC_b(m,2,:) = tmpb(1:end);
end

%% Parametry algorytmu regulacji
N  = 10; % Horyzont predykcji
Nu = 10; % Horyzont sterowania
lambda = 1.0; % Wsp�czynnik kary za przyrosty sterowa�
psi = 1.0; % Wsp�czynnik kary za uchyb regulacji
dumin = -0.10; % Ograniczenie na minimalny przyrost sterowania
dumax =  0.10; % Ograniczenie na maksymalny przyrost sterowania
umin  = -4.00; % Ograniczenie na minimaln� warto�� sterowania
umax  =  4.00; % Ograniczenie na maksymaln� warto�� sterowania

run(alg); % Uruchomienie inicjalizuj�ce algorytm

%% Inicjalizacja procesu regulacji
Zpp = [0;0]; % Domy�lna warto�� zadana
Xpp = [0;0]; % Stan w punkcie pracy (nie wyznaczony)
Upp = [1;1]; % Sterowanie w punkcie pracy
for AD_K = 1:kk % Wyznaczenie stanu w punkcie pracy
    Xpp = SSz.A*Xpp+SSz.B*Upp;
end
Ypp = SSz.C*Xpp; % Wyznaczenie sterowania w punkcie pracy
simT = (1:kk)'; % Czas dyskretny regulacji
simU = ones(kk,1)*Upp'; % Trajektoria sygna�u steruj�cego
simY = ones(kk,1)*Ypp'; % Trajektoria sygna�u wyj�ciowego
simZ = ones(kk,1)*Zpp'; % Trajektoria sygna�u zadanego
for AD_K = 1:(kp-1) % Inicjalizacja pocz�tkowych warto�ci trajektorii
    AD_Y(AD_K,:) = Ypp;
    AD_U(AD_K,:) = Upp;
    AD_X(AD_K,:) = Xpp;
end

%% Symulacja
if(NO_CROSSCOMPILER)
    for AD_K = kp:kk
        % Symulacja z wykorzystaniem dyskretnych r�wna� stanu
        AD_X(AD_K,:) = (SSz.A*AD_X(AD_K-1,:)'+SSz.B*AD_U(AD_K-1,:)')';
        AD_Y(AD_K,:) = (SSz.C*AD_X(AD_K-1,:)'+SSz.D*AD_U(AD_K-1,:)')';

        % Okre�lenie warto�ci zadanej
        if    (AD_K<= 200); AD_Z = [0.0, 0.0];
        elseif(AD_K<= 400); AD_Z = [0.0, 0.4];
        elseif(AD_K<= 600); AD_Z = [0.1, 0.4];
        elseif(AD_K<= 800); AD_Z = [0.1, 0.2];
        elseif(AD_K<=1000); AD_Z = [0.5, 0.2];
        end

        % Algorytm regulacji
        run(alg);

        % Zapis nowej warto�ci przyrosti sterowania (control_value)
        AD_DU(AD_K,:) = control_value';              
        % Zapis nowej warto�ci sterowania
        AD_U (AD_K,:) = AD_U(AD_K-1,:)+AD_DU(AD_K,:);

        % Zapis danych do p�niejszych prezentacji
        simT(AD_K) = AD_K;
        simU(AD_K,:) = AD_U(AD_K,:);
        simY(AD_K,:) = AD_Y(AD_K,:);
        simZ(AD_K,:) = AD_Z(:);
    end
end

% Wy�wietlenie przebiegu eksperymentu
if(NO_CROSSCOMPILER)
    plot_result(simT,simY,simZ,simU)
end

% Wygenerowanie algorytmu regulacji w j�zyku C (dodatkowo kod zapisany jest
% do zmiennej ,,code'' na potrzeby jego skopiowania w dodatkowe miejsce
if(~NO_CROSSCOMPILER)
    funname = 'controller';
    code = AutoMATiC_Generate(alg,funname);
    fprintf('[MATLAB] Generated algorithm ''%s'', to a function called ''%s''\n',alg,funname);
else
    fprintf('[MATLAB] Did not generate anything\n');
end

% Kompilacja programu testuj�cego wygenerowany algorytm regulacji
if(NO_CROSSCOMPILER)
    %fd=fopen('./controller.cinclude','w');
    %fprintf(fd,code);
    %fclose(fd);
    %!mex -v -client engine ./*.c -output test.exe
end

% Uruchomienie programu testuj�cego wygenerowany algorytm regulacji
if(NO_CROSSCOMPILER)
    %!test.exe
end

% Wy�wietlenie danych z przebiegu regulacji -- 
%     -- (program w C zapisuje dane do data.csv)
if(NO_CROSSCOMPILER)
    data = csvread('data.csv',1);
    simT = data(:,1);
    simZ = data(:,2:3);
    simY = data(:,4:5);
    simU = data(:,6:7);
    Empty = data(:,8);
    plot_result(simT,simY,simZ,simU);
end

function plot_result(simT,simY,simZ,simU)
%% Funkcja wy�wietlaj�ca przebieg regulacji na jednej figurze podzielonej 
%% w poziomie na g�rny wykres (wyj�cie Y i warto�� zadana Z w czasie T), 
%% oraz na dolny wykres (warto�� sterowania U w czasie T)
    figure;
    subplot(2,1,1);
    plot(simT,simY);
    hold on;
    stairs(simT,simZ,'--');
    grid;
    title('Wyj�cie');

    subplot(2,1,2);
    stairs(simT,simU);
    grid;
    ylim([-2 2]);
    title('Sterowanie');
end


