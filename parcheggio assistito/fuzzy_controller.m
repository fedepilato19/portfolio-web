function fit = fuzzy_controller(velocita, distanza, angolo)
    % Carica il sistema fuzzy (il file .fis)
    fis = readfis('parcheggioAssistito.fis');  % Assicurati che il file .fis sia nel tuo percorso di lavoro

    % Normalizza i dati di ingresso se necessario
    % Se hai bisogno di normalizzare i tuoi dati prima di passarli al controller fuzzy
    % (dipende dal range definito nel tuo file .fis)
    v = velocita;    % Velocità
    d = distanza;    % Distanza
    a = angolo;      % Angolo

    % Passa i dati al sistema Mamdani (valori di ingresso come struttura)
    inputs = [v, d, a];
    
    % Esegui l'inferenza fuzzy
    fit = evalfis(fis, inputs);

    % Puoi anche limitare l'output se necessario
    fit = max(0, min(fit, 1));  % Se la FIT deve rimanere tra 0 e 1
end
