function [exectime, data] = gateway_fnctn(segment, data)
    % Inizializzazione dei contatori e tempi nella struttura data
    if ~isfield(data, 'lostP'), data.lostP = 0; end
    if ~isfield(data, 'recVel'), data.recVel = 0; end
    if ~isfield(data, 'recDist'), data.recDist = 0; end
    if ~isfield(data, 'recAng'), data.recAng = 0; end
    if ~isfield(data, 'time_vel'), data.time_vel = 0; end
    if ~isfield(data, 'time_dist'), data.time_dist = 0; end
    if ~isfield(data, 'time_ang'), data.time_ang = 0; end

    switch segment
        case 1
            velocita_msg = ttTryFetch('velocita_signal');
            if ~isempty(velocita_msg)
                data.velocita = velocita_msg;
                data.velocita_time = ttCurrentTime;
                data.recVel = data.recVel + 1;
                disp(['t=' num2str(data.velocita_time) ' GATEWAY: valore di velocità ricevuto: ' num2str(getMsgValue(data.velocita))]);
            else
                disp(['t=' num2str(ttCurrentTime) ' GATEWAY: non era messaggio di velocità']);
            end
            exectime = 0.1;

        case 2
            distanza_msg = ttTryFetch('distanza_signal');
            if ~isempty(distanza_msg)
                data.distanza = distanza_msg;
                data.distanza_time = ttCurrentTime;
                data.recDist = data.recDist + 1;
                disp(['t=' num2str(data.distanza_time) ' GATEWAY: valore di distanza ricevuto: ' num2str(getMsgValue(data.distanza))]);
            else
                disp(['t=' num2str(ttCurrentTime) ' GATEWAY: non era messaggio di distanza']);
            end
            exectime = 0.1;

        case 3
            angolo_msg = ttTryFetch('angolo_signal');
            if ~isempty(angolo_msg)
                data.angolo = angolo_msg;
                data.angolo_time = ttCurrentTime;
                data.recAng = data.recAng + 1;
                disp(['t=' num2str(data.angolo_time) ' GATEWAY: valore di angolo ricevuto: ' num2str(getMsgValue(data.angolo))]);
            else
                disp(['t=' num2str(ttCurrentTime) ' GATEWAY: non era messaggio di angolo']);
            end
            exectime = 0.12;

        case 4
            time_now = ttCurrentTime;

            % Inoltro messaggio velocità
            if isfield(data, 'velocita') && getMsgValue(data.velocita) ~= 0
                msg.msg = getMsgValue(data.velocita);
                msg.type = 'velocita_signal';
                ttSendMsg([2, 1], msg, 80);
                if isfield(data, 'velocita_time')
                    data.time_vel = time_now - data.velocita_time;
                else
                    data.time_vel = 0;
                end
                ttAnalogOut(5, data.time_vel);
            else
                disp(['t=' num2str(time_now) ' GATEWAY: non inoltro messaggio velocità']);
            end

            % Inoltro messaggio distanza
            if isfield(data, 'distanza') && getMsgValue(data.distanza) ~= 0
                msg.msg = getMsgValue(data.distanza);
                msg.type = 'distanza_signal';
                ttSendMsg([2, 1], msg, 80);
                if isfield(data, 'distanza_time')
                    data.time_dist = time_now - data.distanza_time;
                else
                    data.time_dist = 0;
                end
                ttAnalogOut(6, data.time_dist);
            else
                disp(['t=' num2str(time_now) ' GATEWAY: non inoltro messaggio distanza']);
            end

            % Inoltro messaggio angolo
            if isfield(data, 'angolo') && getMsgValue(data.angolo) ~= 0
                msg.msg = getMsgValue(data.angolo);
                msg.type = 'angolo_signal';
                ttSendMsg([2, 1], msg, 80);
                if isfield(data, 'angolo_time')
                    data.time_ang = time_now - data.angolo_time;
                else
                    data.time_ang = 0;
                end
                ttAnalogOut(7, data.time_ang);
            else
                disp(['t=' num2str(time_now) ' GATEWAY: non inoltro messaggio angolo']);
            end

            % Pacchetti persi
            if ~isfield(data, 'velocita') && ~isfield(data, 'distanza') && ~isfield(data, 'angolo')
                data.lostP = data.lostP + 1;
            end

            % Output delle metriche
            ttAnalogOut(1, data.lostP);
            ttAnalogOut(2, data.recVel);
            ttAnalogOut(3, data.recDist);
            ttAnalogOut(4, data.recAng);

            exectime = -1;
    end
end

% Funzione per estrarre il valore dal messaggio
function val = getMsgValue(signal)
    if isstruct(signal) && isfield(signal, 'msg')
        val = signal.msg;
    else
        val = signal;
    end
end