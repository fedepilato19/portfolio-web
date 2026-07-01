BlogFeedback - Analisi Predittiva e Machine Learning
Questo repository contiene un progetto di Data Science e Machine Learning focalizzato sulla predizione del numero di commenti che un post di un blog riceverà nelle ore successive alla sua pubblicazione, basandosi su caratteristiche storiche e testuali.

Struttura del Codice
Il codice segue una pipeline strutturata tipica dei progetti di Intelligenza Artificiale:
Caricamento e Preprocessing dei Dati
Importazione dei dati del dataset BlogFeedback e gestione delle librerie principali come NumPy, Pandas e TensorFlow/Keras.
Ingegneria delle Caratteristiche (Feature Selection)
Utilizzo di diverse tecniche avanzate per selezionare le variabili più significative e ridurre la dimensionalità del dataset, tra cui SelectKBest con metriche di regressione, SelectFromModel e la tecnica di eliminazione ricorsiva delle feature (RFE).
Addestramento dei Modelli di Regressione
Esplorazione e ottimizzazione di diversi algoritmi predittivi per confrontarne le performance:
DecisionTreeRegressor per un approccio basato su alberi di decisione.
RandomForestRegressor ed ExtraTreesRegressor per modelli ensemble robusti.
GradientBoostingRegressor e AdaBoost per algoritmi di boosting sequenziale.
Reti Neurali artificiali implementate tramite TensorFlow e Keras.
Validazione e Ottimizzazione
Utilizzo di strategie di ottimizzazione degli iperparametri (RandomizedSearchCV) e tecniche di validazione incrociata (KFold) per garantire la stabilità dei modelli ed evitare l'overfitting.
Valutazione delle Metriche
I modelli vengono valutati e confrontati quantitativamente sfruttando metriche standard di regressione tra cui l'Errore Medio Assoluto (MAE), l'Errore Quadratico Medio (MSE) e il coefficiente di determinazione R2.