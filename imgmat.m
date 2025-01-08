
% Fonction image Matlab 
addpath('C:\Users\utilisateur\Downloads\BigProjet\PROJETC_LMLTM\Bibliotheque\Gratuites'); 

img= 'Banc02_gris.pgm';


if exist(img, 'file')
    img = imread(img); % Lecture de l'image
    disp(['Type de l''image : ', class(img)]); % Afficher le type d'image pour vérifier sa lecture

    % Exemple de traitement simple après lecture
    figure;
    imshow(img);
    title('Image chargée');

    % 1. Calcul de l'histogramme
    if size(img, 3) == 3 % Cas d'une image couleur
        figure;
        subplot(3, 1, 1);
        imhist(img(:, :, 1));
        title('Histogramme du canal rouge');

        subplot(3, 1, 2);
        imhist(img(:, :, 2));
        title('Histogramme du canal vert');

        subplot(3, 1, 3);
        imhist(img(:, :, 3));
        title('Histogramme du canal bleu');
    else % Cas d'une image en niveaux de gris
        img = im2uint8(img); % Conversion en uint8 si nécessaire
        figure;
        imhist(img);
        title('Histogramme de l''image');
    end

    % 2. Filtrage par un filtre de convolution (matrice 3x3)

    noisyImg = imnoise(img, 'gaussian', 0, 0.01); % Bruit gaussien avec variance 0.01
    
    % Affichage de l'image originale et bruitée
    figure;
    subplot(1, 2, 1);
    imshow(img);
    title('Image originale');
    
    subplot(1, 2, 2);
    imshow(noisyImg);
    title('Image bruitée');

    h = fspecial('average', [3 3]); % Exemple : filtre moyenneur
    filteredImg = imfilter(noisyImg, h);
    figure;
    imshow(filteredImg);
    title('Image filtrée');

    % 3. Détection de contours (gradient ou Laplacien)
    if size(img, 3) == 3
        imgGray = rgb2gray(img); % Convertir en niveaux de gris si nécessaire
    else
        imgGray = img;
    end
    edgeImg = edge(imgGray, 'sobel'); % Exemple avec Sobel
    figure;
    imshow(edgeImg);
    title('Contours détectés (Sobel)');

    % 4. Détection de droites par la transformée de Hough
    [H, T, R] = hough(edgeImg);
    figure;
    imshow(imadjust(rescale(H)), [], 'XData', T, 'YData', R, 'InitialMagnification', 'fit');
    xlabel('\theta (degrés)');
    ylabel('\rho (pixels)');
    title('Transformée de Hough');
    axis on;
    axis normal;
    hold on;

    % Trouver les pics dans l'espace de Hough
    peaks = houghpeaks(H, 5);
    x = T(peaks(:, 2));
    y = R(peaks(:, 1));
    plot(x, y, 's', 'color', 'red');
    hold off;

    % Tracer les lignes détectées
    lines = houghlines(edgeImg, T, R, peaks, 'FillGap', 20, 'MinLength', 30);
    figure;
    imshow(img);
    hold on;
    for k = 1:length(lines)
        xy = [lines(k).point1; lines(k).point2];
        plot(xy(:, 1), xy(:, 2), 'LineWidth', 2, 'Color', 'green');
    end
    title('Lignes détectées');
    hold off;

    % 5. Rehaussement de contours (par addition avec le Laplacien)
    laplacianFilter = fspecial('laplacian');
    laplacianImg = imfilter(double(imgGray), laplacianFilter);
    sharpenedImg = uint8(double(imgGray) + laplacianImg);
    figure;
    imshow(sharpenedImg);
    title('Image avec contours rehaussés');

    % 6. Segmentation par sélection de couleurs (pour les images couleur)
    if size(img, 3) == 3
        % Exemple : sélectionner une plage de rouges
        redChannel = img(:, :, 1);
        greenChannel = img(:, :, 2);
        blueChannel = img(:, :, 3);

        % Intervalle de sélection (à adapter)
        redMask = (redChannel > 100) & (redChannel < 200);
        greenMask = (greenChannel < 100);
        blueMask = (blueChannel < 100);

        colorMask = redMask & greenMask & blueMask;
        segmentedImg = img;
        segmentedImg(repmat(~colorMask, [1 1 3])) = 0; % Masquer les pixels hors sélection
        figure;
        imshow(segmentedImg);
        title('Image segmentée par sélection de couleurs');
    end
else
    error('Le fichier spécifié n''existe pas : %s', imgPath);
end
